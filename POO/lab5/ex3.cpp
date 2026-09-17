#include <iostream>
using namespace std;

//Base class Event
class Event {
    protected:
    string eventName;
    string eventDate;
    string location;

    public: 
    //parameterized constructor
    Event(string name, string date, string loc): eventName(name, eventDate(date), location(loc){}

    //overriden: display event details
    virtual void displayEventDetails(){
        cout << "Event Name: " << eventName << endl;
        cout << "Date: " << eventDate << endl;
        cout << "Location: " << location << endl;
    }
};

//derived class Conference inheriting from event
class Conference: public Event {
    private:
    string keynoteSpeaker;

    public:
    // Parameterized constructor to initialize attributes
    Conference(string name, string date, string loc, string speaker)
        : Event(name, date, loc), keynoteSpeaker(speaker) {}

    // Overriding displayEventDetails method to include Conference-specific attributes
    void displayEventDetails() override {
        Event::displayEventDetails();
        cout << "Keynote Speaker: " << keynoteSpeaker << endl;
    }
};

//derived class Concert inheriting from Event
class Concert : public Event {
private:
    string bandName;

public:
    //parameterized constructor
    Concert(string name, string date, string loc, string band)
        : Event(name, date, loc), bandName(band) {}

    // Overriding displayEventDetails method to include Concert-specific attributes
    void displayEventDetails() override {
        Event::displayEventDetails();
        cout << "Performing Band: " << bandName << endl;
    }
};

int main() {
    Conference mathConf("Topology in the Real World", "June 10, 2025", "Murcia", "Dra. Mª Angeles Cifre");
    Concert rapConcert("Rocanrola", "October 8th, 2025", "Alicante", "Delaossa");

    // Displaying information of each event
    cout << "Conference Details:\n";
    mathConf.displayEventDetails();
    cout << "\nConcert Details:\n";
    rapConcert.displayEventDetails();

    return 0;
}

