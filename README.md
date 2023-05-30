# ice-cream-stations

/*******************************************************************************************
*
*   Stations - Documentation
*
*   Description:
*   This program allows the user to interact with a graphical interface that represents
*   stations. Each station has a target value, an actual value, and a variance. The user can
*   select and update the actual value of a station, as well as enter a date to adjust the
*   target value based on the month. The stations are displayed as circles on the screen,
*   and their colors change based on the variance from the target.
*
*   Libraries Used:
*   - raylib.h: A simple and easy-to-use library for game development.
*   - raygui.h: A library for creating graphical user interfaces (GUI) with raylib.
*   - stdbool.h: Defines boolean types and values.
*   - stdlib.h: Defines several general-purpose functions, such as memory allocation.
*   - string.h: Provides functions for manipulating strings.
*   - ctype.h: Provides functions for character classification.
*   - math.h: Provides mathematical functions.
*
*******************************************************************************************/

/*******************************************************************************************
*
*   Structures
*
*   - Station: Represents a station with its properties and user interaction states.
*       - name: The name of the station.
*       - target: The target value for the station.
*       - actual: The actual value of the station.
*       - variance: The variance between the actual and target values.
*       - radius: The radius of the station's circle representation.
*       - x, y: The coordinates of the station on the screen.
*       - selected: Indicates whether the station is currently selected.
*       - actualSelected: Indicates whether the station's actual value is currently selected for editing.
*       - inputActual: The input string for editing the actual value.
*       - inputVariance: The calculated variance value for display.
*       - inputBottomSelected: Indicates whether the station's date input is currently selected for editing.
*       - inputBottom: The input string for editing the date.
*       - inputActualVisible: Indicates whether the input textbox for the actual value is visible.
*       - showError: Indicates whether an error message should be displayed.
*
*******************************************************************************************/

/*******************************************************************************************
*
*   Function Declarations
*
*   - generateNormalRandom: Generates a random number from a normal distribution.
*   - stationsOverlap: Checks if two stations overlap each other.
*   - isValidDate: Checks if a date string is valid.
*   - initializeStation: Initializes the properties of a station.
*   - initStations: Initializes all the stations in the program.
*   - addNewStation: Adds a new station at the specified mouse position.
*   - updateStation: Updates the station based on user interaction.
*   - drawStation: Draws the station on the screen.
*   - main: The main entry point of the program.
*
*******************************************************************************************/

/*******************************************************************************************
*
*   Global Constants
*
*   - MAX_STATIONS: The maximum number of stations that can be created.
*   - SCREEN_WIDTH: The width of the screen in pixels.
*   - SCREEN_HEIGHT: The height of the screen in pixels.
*
*******************************************************************************************/

/*******************************************************************************************
*
*   Global Variables
*
*   - stations: An array of Station structures representing the stations in the program.
*   - currentStations: The current number of stations in the program.
*   - lastSelectedStation: The index of the last selected station.
*   - keyState: An array to track the state of keyboard keys.
*
*******************************************************************************************/

/*******************************************************************************************
*
*   Function Definitions
*
*******************************************************************************************/

/**
 * Generates a random number from a standard normal distribution.
 * @return The generated random number.
 */
float generateNormalRandom();

/**
 * Checks if two stations overlap each other.
 * @param a Pointer to the first station.
 * @param b Pointer to the second station.
 * @return True if the stations overlap, false otherwise.
 */
bool stationsOverlap(const Station* a, const Station* b);

/**
 * Checks if a date string is valid (in the format MMDDYYYY).
 * @param dateStr The date string to check.
 * @return True if the date string is valid, false otherwise.
 */
bool isValidDate(const char* dateStr);

/**
 * Initializes the properties of a station.
 * @param station Pointer to the station structure.
 * @param dateStr The current date string.
 */
void initializeStation(Station* station, const char* dateStr);

/**
 * Initializes all the stations in the program.
 */
void initStations();

/**
 * Adds a new station at the specified mouse position.
 * @param mousePoint The position of the mouse.
 */
void addNewStation(Vector2 mousePoint);

/**
 * Updates the station based on user interaction.
 * @param station Pointer to the station structure.
 * @param mousePoint The position of the mouse.
 */
void updateStation(Station* station, Vector2 mousePoint);

/**
 * Draws the station on the screen.
 * @param station Pointer to the station structure.
 */
void drawStation(Station* station);

/**
 * The main entry point of the program.
 */
int main();

/*******************************************************************************************
*
*   Implementation Details
*
*   - The program uses the raylib and raygui libraries for creating the graphical user
*     interface and drawing shapes on the screen.
*   - The program defines a Station structure to represent each station in the system.
*   - Stations are randomly initialized with target, actual, and variance values. The target
*     value is adjusted based on the date provided.
*   - The program allows the user to select a station and edit its actual value or date.
*   - Stations are drawn as circles on the screen, and their color changes based on the
*     variance from the target value.
*   - The user can add new stations by clicking on an empty area of the screen.
*   - The program supports keyboard input for entering the actual value and date.
*
*******************************************************************************************/
