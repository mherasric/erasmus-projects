#include <iostream>
#include <vector>
#include <memory>  // For smart pointers
using namespace std;

// Abstract base class for all vehicles becasue it doesn't have a concrete implementation of disfplay()
class Vehicle {
protected:
    string registrationNumber;  // Common attribute for all vehicles

public:
    Vehicle(const string& regNum) : registrationNumber(regNum) {}
    virtual ~Vehicle() {}

    // Pure virtual function to display vehicle information
    virtual void display() const = 0;
};

// Concrete class representing a Car
class Car : public Vehicle {
private:
    string bodyStyle;  // Specific attribute for Car

public:
    Car(const string& regNum, const string& style)
        : Vehicle(regNum), bodyStyle(style) {}

    // Getter for body style
    string getBodyStyle() const {
        return bodyStyle;
    }

    // Setter for body style
    void setBodyStyle(const string& style) {
        bodyStyle = style;
    }

    // Override display() to show car-specific info
    void display() const override {
        cout << "Car - Registration: " << registrationNumber
             << ", Body Style: " << bodyStyle << endl;
    }
};

// Concrete class representing a Truck
class Truck : public Vehicle {
private:
    double payloadCapacity;  // Specific attribute for Truck

public:
    Truck(const string& regNum, double capacity)
        : Vehicle(regNum), payloadCapacity(capacity) {}

    // Getter for payload capacity
    double getPayloadCapacity() const {
        return payloadCapacity;
    }

    // Setter for payload capacity
    void setPayloadCapacity(double capacity) {
        payloadCapacity = capacity;
    }

    // Override display() to show truck-specific info
    void display() const override {
        cout << "Truck - Registration: " << registrationNumber
             << ", Payload Capacity: " << payloadCapacity << " tons" << endl;
    }
};

// Class representing the Garage, holding multiple vehicles
class Garage {
private:
    // Vector of smart pointers to base class Vehicle
    vector<shared_ptr<Vehicle>> vehicles;

public:
    // Adds a new vehicle to the garage
    void addVehicle(const shared_ptr<Vehicle>& vehicle) {
        vehicles.push_back(vehicle);
    }

    // Displays all vehicles currently in the garage
    void display() const {
        cout << "--- Garage Vehicles ---" << endl;
        for (const auto& v : vehicles) {
            v->display();  // Polymorphic call
        }
    }
};

// Main function to demonstrate functionality
int main() {
    Garage garage;

    // Create a Car and a Truck
    shared_ptr<Car> car1 = make_shared<Car>("ABC123", "Sedan");
    shared_ptr<Truck> truck1 = make_shared<Truck>("XYZ789", 15.5);

    // Add them to the garage
    garage.addVehicle(car1);
    garage.addVehicle(truck1);

    // Display all vehicles in the garage
    garage.display();

    return 0;
}
