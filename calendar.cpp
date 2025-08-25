#include <iostream>
using namespace std;

// Array holding the names of the 13 Ethiopian months
const char* ethiopian_months[13] = {
    "Meskerem", "Tikimt", "Hidar", "Tahsas", "Tir", "Yekatit",
    "Megabit", "Miyazya", "Ginbot", "Sene", "Hamle", "Nehase", "Pagume"
};

// Array holding the names of the days of the week, starting from Sunday
const char* week_days[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// Checks if a given Gregorian year is a leap year
// Returns 1 (true) if leap year, 0 (false) otherwise
int isLeapYear(int year) {
    return (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
}

// Returns the number of days in a given Gregorian month and year
int getNoOfDays(int year, int month) {
    switch (month) {
        case 2: // February
            return isLeapYear(year) ? 29 : 28;
        case 4: case 6: case 9: case 11: // April, June, September, November
            return 30;
        default: // All other months
            return 31;
    }
}

// Displays the name of a Gregorian month and a separator line
void displayMonthName(int month) {
    const char* names[12] = {"January","February","March","April","May","June","July","August","September","October","November","December"};
    cout << endl << endl << names[month-1] << endl;
    cout << "=====================" << endl;
}

// Calculates the day of the week for a given Gregorian date
// Uses Zeller's Congruence algorithm
// Returns 0 for Sunday, 1 for Monday, ..., 6 for Saturday
int getDayOfTheWeek(int day, int month, int year) {
    // If month is January or February, treat it as month 13 or 14 of the previous year
    if (month < 3) {
        month += 12;
        year--;
    }
    int K = year % 100; // Year of the century
    int J = year / 100; // Zero-based century
    int h = (day + 13*(month+1)/5 + K + K/4 + J/4 + 5*J) % 7;
    // Adjust result so that 0 = Sunday, 1 = Monday, ..., 6 = Saturday
    return (h + 6) % 7;
}

// Displays the calendar for a single Gregorian month
void displayCalendarOfTheMonth(int year, int month) {
    int n_days = getNoOfDays(year, month); // Get number of days in the month
    int k = getDayOfTheWeek(1, month, year); // Get the weekday of the 1st day (0=Sunday)
    displayMonthName(month); // Print the month name
    cout << "Sun Mon Tue Wed Thu Fri Sat\n";
    // Print spaces for days before the 1st of the month
    for (int i = 0; i < k; i++) cout << "    ";
    // Print all days of the month
    for (int j = 1; j <= n_days; j++) {
        printf("%3d ", j);
        // Start a new line after Saturday
        if ((k + j) % 7 == 0) cout << endl;
    }
    cout << endl;
}

// Displays the calendar for an entire Gregorian year (all 12 months)
void displayCalendarOfTheYear(int year) {
    for (int month = 1; month <= 12; month++) {
        displayCalendarOfTheMonth(year, month);
    }
}

// Checks if a given Ethiopian year is a leap year
// Returns true if leap year, false otherwise
bool isEthiopianLeap(int year) {
    // In the Ethiopian calendar, a year is a leap year if (year + 1) is divisible by 4
    return (year + 1) % 4 == 0;
}

// Returns the number of days in a given Ethiopian month and year
int getEthiopianMonthDays(int year, int month) {
    if (month >= 1 && month <= 12)
        return 30; // First 12 months always have 30 days
    else if (month == 13)
        return isEthiopianLeap(year) ? 6 : 5; // Pagume has 5 or 6 days depending on leap year
    else
        return -1; // Invalid month
}

// Converts an Ethiopian date to a Gregorian date
// eth_day, eth_month, eth_year: Ethiopian date to convert
// g_day, g_month, g_year: variables to store the Gregorian result (by reference)
void convertToGregorian(int eth_day, int eth_month, int eth_year, int &g_day, int &g_month, int &g_year) {
    // Ethiopian New Year is on September 11 (Gregorian) or September 12 (Gregorian leap year)
    int new_year_gregorian = eth_year + 7; // Gregorian year for Ethiopian New Year
    int new_year_month = 9; // September
    int new_year_day = 11; // Default to September 11

    // If the next Gregorian year is a leap year, Ethiopian New Year is on Sep 12
    if (isLeapYear(new_year_gregorian + 1)) {
        new_year_day = 12;
    }

    // Calculate how many days have passed since Meskerem 1 (Ethiopian New Year)
    int days_since_new_year = (eth_month - 1) * 30 + (eth_day - 1);
    if (eth_month == 13) {
        days_since_new_year = 12 * 30 + (eth_day - 1);
    }

    // Start from Ethiopian New Year in Gregorian calendar
    g_day = new_year_day;
    g_month = new_year_month;
    g_year = new_year_gregorian;

    // Add days_since_new_year to Gregorian date, rolling over months and years as needed
    int days_in_greg_month[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    for (int i = 0; i < days_since_new_year; ++i) {
        g_day++;
        days_in_greg_month[1] = isLeapYear(g_year) ? 29 : 28; // Adjust February for leap years
        if (g_day > days_in_greg_month[g_month - 1]) {
            g_day = 1;
            g_month++;
            if (g_month > 12) {
                g_month = 1;
                g_year++;
            }
        }
    }
}

// Converts and prints an Ethiopian date to Gregorian, including the day of the week
void convertToGregorian(int eth_day, int eth_month, int eth_year) {
    int g_day, g_month, g_year;
    // Check for valid Ethiopian date
    if (eth_month < 1 || eth_month > 13 || eth_day < 1 || eth_day > getEthiopianMonthDays(eth_year, eth_month)) {
        cout << "Invalid Ethiopian date.\n";
        return;
    }
    // Convert to Gregorian
    convertToGregorian(eth_day, eth_month, eth_year, g_day, g_month, g_year);
    cout << "Gregorian Date: " << g_day << "/" << g_month << "/" << g_year << endl;
    cout << "Day of the week: " << week_days[getDayOfTheWeek(g_day, g_month, g_year)] << endl;
}

// Gets the weekday for an Ethiopian date (0=Sunday, ..., 6=Saturday)
int getEthiopianWeekday(int eth_day, int eth_month, int eth_year) {
    int g_day, g_month, g_year;
    // Convert Ethiopian date to Gregorian date
    convertToGregorian(eth_day, eth_month, eth_year, g_day, g_month, g_year);
    // Get the weekday for the Gregorian date
    return getDayOfTheWeek(g_day, g_month, g_year);
}

// Structure to represent an Ethiopian holiday
struct EthiopianHoliday {
    int month;      // Ethiopian month (1-13)
    int day;        // Day in the month
    const char* name;    // Name of the holiday
};

// List of major Ethiopian holidays (add more as needed)
EthiopianHoliday holidays[] = {
    {1, 1, "Enkutatash (New Year)"},
    {1, 17, "Meskel (Finding of the True Cross)"},
    {4, 29, "Genna (Christmas)"},
    {5, 11, "Timket (Epiphany)"},
    {7, 23, "Adwa Victory Day"},
    {8, 23, "Siklet (Good Friday)"},
    {9, 27, "Fasika (Easter)"},
    {10, 1, "Patriots' Victory Day"},
    {12, 26, "Buhe"},
    {13, 5, "Pagume Holiday"}
    // Add more holidays as needed
};

// Function to check if a given Ethiopian date is a holiday
const char* getEthiopianHoliday(int month, int day) {
    int n = sizeof(holidays) / sizeof(holidays[0]);
    for (int i = 0; i < n; ++i) {
        if (holidays[i].month == month && holidays[i].day == day) {
            return holidays[i].name;
        }
    }
    return "";
}

// Displays a single Ethiopian month with correct weekdays and holidays
void displayEthiopianMonth(int year, int month) {
    if (month < 1 || month > 13) {
        cout << "Invalid Ethiopian month.\n";
        return;
    }
    int days = getEthiopianMonthDays(year, month); // Number of days in the Ethiopian month
    cout << "\n" << ethiopian_months[month - 1] << " " << year << "\n";
    cout << "----------------------------\n";
    cout << "Su Mo Tu We Th Fr Sa\n";
    int start_weekday = getEthiopianWeekday(1, month, year); // Find the weekday for the 1st day
    for (int i = 0; i < start_weekday; ++i) cout << "   ";
    for (int i = 1; i <= days; i++) {
        const char* holiday = getEthiopianHoliday(month, i);
        if (holiday != "") {
            // Mark holidays with an asterisk and show the day in brackets
            printf("[%2d]", i);
        } else {
            printf("%2d ", i);
        }
        // Start a new line after Saturday
        if ((start_weekday + i) % 7 == 0) cout << endl;
    }
    cout << endl;
    // After the calendar, list the holidays for this month
    bool found = false;
    for (int i = 1; i <= days; ++i) {
        const char* holiday = getEthiopianHoliday(month, i);
        if (holiday != "") {
            if (!found) {
                cout << "Holidays this month:\n";
                found = true;
            }
            cout << "  " << i << " " << ethiopian_months[month - 1] << ": " << holiday << endl;
        }
    }
}

// Displays the entire Ethiopian calendar for a year (all 13 months)
void displayEthiopianYear(int year) {
    for (int month = 1; month <= 13; ++month) {
        displayEthiopianMonth(year, month);
        cout << endl;
    }
}

// Asks the user for a Gregorian date and prints the day of the week
void showDayOfWeekForGregorian() {
    int d, m, y;
    cout << "Enter Gregorian date (day month year): ";
    cin >> d >> m >> y;
    // Check for valid Gregorian date
    if (y < 1600 || y > 2400 || m < 1 || m > 12 || d < 1 || d > getNoOfDays(y, m)) {
        cout << "Invalid Gregorian date.\n";
        return;
    }
    int dow = getDayOfTheWeek(d, m, y);
    cout << "Day of the week: " << week_days[dow] << endl;
}

// Main menu for the calendar converter program
// Displays options and handles user input
void menu() {
    int choice;
    while (true) {
        cout << "\n--- Calendar Converter ---\n";
        cout << "1. Display Ethiopian Calendar of a Year\n";
        cout << "2. Display Ethiopian Month\n";
        cout << "3. Convert Ethiopian Date to Gregorian\n";
        cout << "4. Check if Ethiopian Year is Leap\n";
        cout << "5. Display Gregorian Calendar of a Year\n";
        cout << "6. Show Day of the Week for a Gregorian Date\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice) {
            case 1: {
                int y;
                cout << "Enter Ethiopian year: ";
                cin >> y;
                displayEthiopianYear(y); // Show all 13 Ethiopian months for the year
                break;
            }
            case 2: {
                int y, m;
                cout << "Enter Ethiopian year and month (year month): ";
                cin >> y >> m;
                displayEthiopianMonth(y, m); // Show a single Ethiopian month
                break;
            }
            case 3: {
                int d, m, y;
                cout << "Enter Ethiopian date (day month year): ";
                cin >> d >> m >> y;
                convertToGregorian(d, m, y); // Convert and show Gregorian equivalent
                break;
            }
            case 4: {
                int y;
                cout << "Enter Ethiopian year: ";
                cin >> y;
                if (isEthiopianLeap(y)) cout << y << " is a leap year.\n";
                else cout << y << " is not a leap year.\n";
                break;
            }
            case 5: {
                int year;
                cout << "Enter Gregorian year (1600-2400): ";
                cin >> year;
                if (year < 1600 || year > 2400) {
                    cout << "Invalid year.\n";
                    break;
                }
                displayCalendarOfTheYear(year); // Show all 12 Gregorian months for the year
                break;
            }
            case 6:
                showDayOfWeekForGregorian(); // Ask for a Gregorian date and show its weekday
                break;
            case 7:
                cout << "Exiting. Goodbye!\n";
                return;
            default:
                cout << "Invalid choice.\n";
        }
    }
}

// The main function starts the program by calling the menu
int main() {
    menu();
    return 0;
}