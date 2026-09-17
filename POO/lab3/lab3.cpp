#include <cassert>
#include <iostream>
#include <string>
#include <iomanip> // For std::setw and std::setfill

class Date {
public:
    // Default constructor: Set to UNIX time 1/1/1970
    Date() : year_(1970), month_(1), day_(1) {}

    // Parameterized constructor
    Date(int year, int month, int day) : year_(year), month_(month), day_(day) {
        if (!isValidDate(year, month, day)) {
            year_ = 1970;
            month_ = 1;
            day_ = 1;
        }
    }

    // Convert date to string in YYYY-MM-DD format
    std::string toString() const {
        std::stringstream ss;
        ss << year_ << "-"
           << std::setw(2) << std::setfill('0') << month_ << "-"
           << std::setw(2) << std::setfill('0') << day_;
        return ss.str();
    }

    // Comparison operators
    bool operator==(const Date& other) const {
        return year_ == other.year_ && month_ == other.month_ && day_ == other.day_;
    }

    bool operator<(const Date& other) const {
        if (year_ != other.year_) return year_ < other.year_;
        if (month_ != other.month_) return month_ < other.month_;
        return day_ < other.day_;
    }

    bool operator<=(const Date& other) const {
        return *this < other || *this == other;
    }

    bool operator>(const Date& other) const {
        return !(*this <= other);
    }

    bool operator>=(const Date& other) const {
        return !(*this < other);
    }

    // Add a specified number of days to the date
    void addDays(int days) {
        if (days < 0) return; // Ignore negative days for simplicity

        int d = day_ + days;
        while (d > daysInMonth(year_, month_)) {
            d -= daysInMonth(year_, month_);
            month_++;
            if (month_ > 12) {
                month_ = 1;
                year_++;
            }
        }
        day_ = d;
    }

    // Static method to check if a date is valid
    static bool isValidDate(int year, int month, int day) {
        // Check basic ranges
        if (month < 1 || month > 12 || day < 1 || year < 1) {
            return false;
        }

        // Check if the day is valid for the given month and year
        int maxDays = daysInMonth(year, month);
        return day <= maxDays;
    }

private:
    int year_;
    int month_;
    int day_;

    // Helper method to determine if a year is a leap year
    static bool isLeapYear(int year) {
        return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    }

    // Helper method to get the number of days in a month
    static int daysInMonth(int year, int month) {
        static const int days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if (month == 2 && isLeapYear(year)) {
            return 29;
        }
        return days[month - 1];
    }
};

class DateTest {
public:
    static void testDefaultConstructor() {
        Date d;
        assert(d.toString() == "1970-01-01");
        std::cout << "Default constructor test passed\n";
    }

    static void testParameterizedConstructor() {
        Date d(2023, 12, 31);
        assert(d.toString() == "2023-12-31");
        // Test invalid date (should revert to default)
        Date d2(2023, 4, 31); // April has 30 days
        assert(d2.toString() == "1970-01-01");
        std::cout << "Parameterized constructor test passed\n";
    }

    static void testToString() {
        Date d(2023, 12, 31);
        assert(d.toString() == "2023-12-31");
        Date d2(1970, 1, 1);
        assert(d2.toString() == "1970-01-01");
        std::cout << "toString test passed\n";
    }

    static void testComparisonOperators() {
        Date d1(2023, 12, 31);
        Date d2(2023, 12, 31);
        Date d3(2024, 1, 1);
        Date d4(2023, 11, 30);

        // Test ==
        assert(d1 == d2);
        assert(!(d1 == d3));

        // Test <
        assert(d1 < d3);
        assert(d4 < d1);
        assert(!(d3 < d1));

        // Test <=
        assert(d1 <= d2);
        assert(d1 <= d3);
        assert(!(d3 <= d1));

        // Test >
        assert(d3 > d1);
        assert(d1 > d4);
        assert(!(d1 > d3));

        // Test >=
        assert(d1 >= d2);
        assert(d3 >= d1);
        assert(!(d1 >= d3));

        std::cout << "Comparison operators test passed\n";
    }

    static void testAddDays() {
        // Test 1: Add days within the same month
        Date d1(2023, 12, 1);
        d1.addDays(10);
        assert(d1.toString() == "2023-12-11");

        // Test 2: Add days across month boundary
        Date d2(2023, 12, 30);
        d2.addDays(2);
        assert(d2.toString() == "2024-01-01");

        // Test 3: Add days across year boundary with leap year
        Date d3(2024, 2, 28); // 2024 is a leap year
        d3.addDays(1);
        assert(d3.toString() == "2024-02-29");
        d3.addDays(1);
        assert(d3.toString() == "2024-03-01");

        // Test 4: Add days in a non-leap year
        Date d4(2023, 2, 28); // 2023 is not a leap year
        d4.addDays(1);
        assert(d4.toString() == "2023-03-01");

        std::cout << "addDays test passed\n";
    }

    static void testIsValidDate() {
        // Test 1: Valid dates
        assert(Date::isValidDate(2023, 12, 31));
        assert(Date::isValidDate(2024, 2, 29)); // Leap year
        assert(Date::isValidDate(1970, 1, 1));

        // Test 2: Invalid dates
        assert(!Date::isValidDate(2023, 4, 31)); // April has 30 days
        assert(!Date::isValidDate(2023, 2, 29)); // Not a leap year
        assert(!Date::isValidDate(2023, 13, 1)); // Invalid month
        assert(!Date::isValidDate(2023, 1, 0)); // Invalid day

        std::cout << "isValidDate test passed\n";
    }

    static void runAllTests() {
        testDefaultConstructor();
        testParameterizedConstructor();
        testToString();
        testComparisonOperators();
        testAddDays();
        testIsValidDate();
        std::cout << "All tests passed!\n";
    }
};

int main() {
    DateTest::runAllTests();

    // User interaction
    int year, month, day, daysToAdd;
    std::cout << "\nEnter a date (year month day): ";
    std::cin >> year >> month >> day;

    if (!Date::isValidDate(year, month, day)) {
        std::cout << "Invalid date! Using default date 1970-01-01.\n";
        year = 1970;
        month = 1;
        day = 1;
    }

    Date d(year, month, day);
    std::cout << "Initial date: " << d.toString() << "\n";

    std::cout << "Enter number of days to add: ";
    std::cin >> daysToAdd;
    d.addDays(daysToAdd);
    std::cout << "Date after adding " << daysToAdd << " days: " << d.toString() << "\n";

    return 0;
}

