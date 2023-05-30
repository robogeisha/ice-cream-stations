#define RAYGUI_IMPLEMENTATION
#define RAYGUI_SUPPORT_ICONS
#include "raylib.h"
#include "raygui.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX_STATIONS 20
#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 600

typedef struct {
    char name[50];
    float target;
    float actual;
    float variance;
    int radius;
    float x;
    float y;
    bool selected;
    bool actualSelected;
    char inputActual[10];
    char inputVariance[10];
    bool inputBottomSelected;
    char inputBottom[9];
    bool inputActualVisible;
    bool showError;
} Station;

Station stations[MAX_STATIONS];
int currentStations = 0;
int lastSelectedStation = -1;
bool keyState[512];


float generateNormalRandom() {
    float u = (float)GetRandomValue(0, 10000) / 10000.0;
    float v = (float)GetRandomValue(0, 10000) / 10000.0;
    return sqrtf(-2.0 * logf(u)) * cosf(2.0 * PI * v);
}


bool stationsOverlap(const Station* a, const Station* b) {
    float distance = sqrtf(powf(b->x - a->x, 2) + powf(b->y - a->y, 2));
    return distance <= (a->radius + b->radius);
}

bool isValidDate(const char* dateStr) {
    int len = strlen(dateStr);
    if (len != 8) return false;
    if (!(dateStr[0] < '3')) return false;
    if (dateStr[0] == '3' && !(dateStr[1] == '0' || dateStr[1] == '1')) return false;
    if (!(dateStr[2] < '2')) return false;
    if (dateStr[2] == '1' && !(dateStr[3] < '3')) return false;
    for (int i = 4; i < 8; i++) {
        if (!isdigit(dateStr[i])) return false;
    }
    return true;
}

void initializeStation(Station* station, const char* dateStr) {
    station->name[0] = '\0';
    station->target = 50.0f + generateNormalRandom() * 10.0f;  // Assuming a mean of 50 and a standard deviation of 10

     // Adjust target value based on the date (month)
    int month = (dateStr[2] - '0') * 10 + (dateStr[3] - '0');
    if (month >= 11 || month <= 2) {  // Winter months
        station->target *= 0.5;  // Reduce target value by 50%
    } else if (month >= 5 && month <= 8) {  // Summer months
        station->target *= 1.5;  // Increase target value by 50%
    }
    station->actual = 40.0f;
    station->variance = (station->actual - station->target) / station->target * 100.0f;
    station->radius = 60;
  station->x = GetRandomValue(station->radius * 2, SCREEN_WIDTH - station->radius * 2);
station->y = GetRandomValue(station->radius * 2, SCREEN_HEIGHT - station->radius * 2);

    station->selected = false;
    station->actualSelected = false;
    station->inputActual[0] = '\0';
    station->inputVariance[0] = '\0';
    station->inputBottomSelected = false;
    station->inputBottom[0] = '\0';
    station->inputActualVisible = false;
    station->showError = false;
    strcpy(station->inputBottom, "11122022");
    


}

void initStations() {
    for (int i = 0; i < MAX_STATIONS; i++) {
        initializeStation(&stations[i], "01232023"); 


        bool intersect = false;
        for (int j = 0; j < i; j++) {
            if (stationsOverlap(&stations[i], &stations[j])) {
                intersect = true;
                break;
            }
        }

        if (intersect) {
            break;
        }

        currentStations++;
    }
}

void addNewStation(Vector2 mousePoint) {
    if (currentStations < MAX_STATIONS) {
        Station newStation;
       initializeStation(&newStation, "01232023");

        newStation.x = mousePoint.x;
        newStation.y = mousePoint.y;

        bool overlap = false;
        for (int i = 0; i < currentStations; i++) {
            if (stationsOverlap(&newStation, &stations[i])) {
                overlap = true;
                break;
            }
        }

        if (!overlap) {
            stations[currentStations++] = newStation;
        }
    }
}

void updateStation(Station* station, Vector2 mousePoint) {
    float distance = sqrtf(powf(mousePoint.x - station->x, 2) + powf(mousePoint.y - station->y, 2));

    if (distance < station->radius) {
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            if (station->selected) {
                station->selected = false;
                station->actualSelected = false;
                station->inputBottomSelected = false;
            } else {
                station->selected = true;
                for (int i = 0; i < MAX_STATIONS; i++) {
                    if (&stations[i] != station) {
                        stations[i].selected = false;
                    }
                }
                lastSelectedStation = station - stations;
            }
        }
    }

    if (station->selected) {
        if (CheckCollisionPointRec(mousePoint, (Rectangle){station->x, station->y - 30, 80, 30})) {
            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                station->actualSelected = !station->actualSelected;
                station->inputBottomSelected = false;
                station->inputActual[0] = '\0';
            }
        }

        if (CheckCollisionPointRec(mousePoint, (Rectangle){station->x, station->y + station->radius, 80, 30})) {
            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                station->inputBottomSelected = !station->inputBottomSelected;
                station->actualSelected = false;
                station->inputBottom[0] = '\0';
            }
        }
    }

    if (station->actualSelected) {
        if (IsKeyPressed(KEY_ENTER)) {
            station->actualSelected = false;
            station->actual = strtof(station->inputActual, NULL);
            station->variance = (station->actual - station->target) / station->target * 100.0f;
            snprintf(station->inputVariance, 10, "%.2f", station->variance);
            station->inputActual[0] = '\0';
        } else {
            for (int i = KEY_ZERO; i <= KEY_NINE; i++) {
                if (IsKeyPressed(i) && !keyState[i]) {
                    int lengthActual = strlen(station->inputActual);
                    if (lengthActual < 10) {
                        station->inputActual[lengthActual] = (char)('0' + (i - KEY_ZERO));
                        station->inputActual[lengthActual + 1] = '\0';
                    }
                    break;
                }
            }
        }
    }

    bool showError = false;

 if (station->inputBottomSelected) {
        if (IsKeyPressed(KEY_ENTER)) {
            if (isValidDate(station->inputBottom)) {
                station->inputBottomSelected = false;
                station->showError = false; // Reset error message when date is valid
                int month = (station->inputBottom[2] - '0') * 10 + (station->inputBottom[3] - '0');
                if (month >= 11 || month <= 2) {  // Winter months
                    station->target *= 0.5;  // Reduce target value by 50%
                } else if (month >= 5 && month <= 8) {  // Summer months
                    station->target *= 1.5;  // Increase target value by 50%
                }
            } else {
                station->showError = true;  // Keep error message displayed when date is invalid
            }
        } else {

            for (int i = KEY_ZERO; i <= KEY_NINE; i++) {
                if (IsKeyPressed(i) && !keyState[i]) {
                    int lengthBottom = strlen(station->inputBottom);
                    if (lengthBottom < 8) {
                        station->inputBottom[lengthBottom] = (char)('0' + (i - KEY_ZERO));
                        station->inputBottom[lengthBottom + 1] = '\0';
                    }
                    break;
                }
            }
        }
    }


}

void drawStation(Station* station) {
    Color stationColor = DARKBLUE;
    if (station->variance <= -10.0f) {
        stationColor = RED; // Change to red if 10% or more below target
    } else if (station->variance >= 5.0f) {
        stationColor = LIME; // Change to green if 5% or more above target
    }

    DrawCircle(station->x, station->y, station->radius, stationColor);

    DrawText(station->name, station->x - MeasureText(station->name, 10) / 2, station->y - 40, 10, WHITE);

    char bufferActual[50];
    char bufferTarget[50];
    char bufferVariance[50];

    sprintf(bufferActual, "Actual: %.2f", station->actual);
    sprintf(bufferTarget, "Target: %.2f", station->target);
    sprintf(bufferVariance, "Variance: %.2f%%", station->variance);

    DrawText(bufferActual, station->x - MeasureText(bufferActual, 10) / 2, station->y - 20, 10, WHITE);
    DrawText(bufferTarget, station->x - MeasureText(bufferTarget, 10) / 2, station->y, 10, WHITE);
    DrawText(bufferVariance, station->x - MeasureText(bufferVariance, 10) / 2, station->y + 20, 10, WHITE);

    if (station->selected && station->actualSelected) {
        GuiTextBox((Rectangle){station->x, station->y - 30, 80, 30}, station->inputActual, 10, true);
    }

    if (station->selected && station->inputBottomSelected) {
        GuiTextBox((Rectangle){station->x, station->y + station->radius, 80, 30}, station->inputBottom, 9, true);
    } else {
        DrawText(station->inputBottom, station->x + station->radius - MeasureText(station->inputBottom, 10), station->y + station->radius, 15, BLACK);
    }

    if (station->showError) {
        DrawText("Invalid date!", station->x, station->y + station->radius + 40, 10, RED);
    }
}


int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Stations");
    SetTargetFPS(60);

    initStations();

    while (!WindowShouldClose()) {
        Vector2 mousePoint = GetMousePosition();

        for (int i = 0; i < 512; i++) {
            keyState[i] = IsKeyDown(i);
        }

        for (int i = 0; i < currentStations; i++) {
            updateStation(&stations[i], mousePoint);
        }

        bool clickOnEmpty = true;
        for (int i = 0; i < currentStations; i++) {
            if (CheckCollisionPointCircle(mousePoint, (Vector2){stations[i].x, stations[i].y}, stations[i].radius)) {
                clickOnEmpty = false;
                break;
            }
        }

        if (IsKeyPressed(KEY_G)) {
            currentStations = 0;
            initStations();
        }

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && clickOnEmpty) {
            addNewStation(mousePoint);
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (int i = 0; i < currentStations; i++) {
            drawStation(&stations[i]);
        }

        if (currentStations == MAX_STATIONS) {
            DrawText("Max station number reached", 10, 10, 20, RED);
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
