#include <iostream>
using namespace std;

// Base class Animal
class Animal {
protected:
    string commonName;
    string scientificName;

public:
    // Parameterized constructor
    Animal(string cName, string sName) : commonName(cName), scientificName(sName) {}

    // Virtual method to display animal information (Overridden in derived classes)
    virtual void displayInfo() {
        cout << "Common Name: " << commonName << endl;
        cout << "Scientific Name: " << scientificName << endl;
    }
};

// Derived class Mammal inheriting from Animal
class Mammal : public Animal {
private:
    bool isAquatic;
    unsigned char gestationPeriod;

public:
    // Parameterized constructor
    Mammal(string cName, string sName, bool aquatic, unsigned char gPeriod)
        : Animal(cName, sName), isAquatic(aquatic), gestationPeriod(gPeriod) {}

    // Overriding displayInfo method to include Mammal-specific attributes
    void displayInfo() override {
        Animal::displayInfo();
        cout << "Is Aquatic: " << (isAquatic ? "Yes" : "No") << endl;
        cout << "Gestation Period: " << static_cast<int>(gestationPeriod) << " months" << endl;
    }
};

// Derived class Bird inheriting from Animal
class Bird : public Animal {
private:
    unsigned int wingSpan;

public:
    // Parameterized constructor
    Bird(string cName, string sName, unsigned int wSpan)
        : Animal(cName, sName), wingSpan(wSpan) {}

    // Overriding displayInfo method to include Bird-specific attributes
    void displayInfo() override {
        Animal::displayInfo();
        cout << "Wing Span: " << wingSpan << " cm" << endl;
    }
};

// Derived class Reptile inheriting from Animal
class Reptile : public Animal {
private:
    bool isVenomous;

public:
    // Parameterized constructor
    Reptile(string cName, string sName, bool venomous)
        : Animal(cName, sName), isVenomous(venomous) {}

    // Overriding displayInfo method to include Reptile-specific attributes
    void displayInfo() override {
        Animal::displayInfo();
        cout << "Is Venomous: " << (isVenomous ? "Yes" : "No") << endl;
    }
};

int main() {
    // Creating objects of derived classes
    Mammal elephant("Elephant", "Loxodonta africana", false, 22);
    Bird eagle("Eagle", "Aquila chrysaetos", 220);
    Reptile komodoDragon("Komodo Dragon", "Varanus komodoensis", true);

    // Displaying information of each object
    cout << "Mammal Information:\n";
    elephant.displayInfo();
    cout << "\nBird Information:\n";
    eagle.displayInfo();
    cout << "\nReptile Information:\n";
    komodoDragon.displayInfo();

    return 0;
}
