#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

// Enum representing types of entities on the grid
enum class EntityType { EMPTY, FOX, GOPHER, PLANT };

// Forward declaration of SimulationGrid used in Entity class
class SimulationGrid;

// Abstract base class for all entities on the grid
class Entity {
protected:
    int row, col;  // Position of the entity in the grid
public:
    Entity(int r = 0, int c = 0) : row(r), col(c) {}
    virtual ~Entity() = default;

    // Return the entity type (EMPTY, PLANT, FOX, GOPHER)
    virtual EntityType what() const = 0;

    // Return string representation of the entity (e.g., "E", "P", "G1")
    virtual string toString() const = 0;

    // Compute next state of this entity based on neighbors in grid
    virtual Entity* next(const SimulationGrid& g) = 0;

    // Clone the current entity (used in copying)
    virtual Entity* clone() const = 0;

    // Count neighboring entities and fill the population array
    virtual void demographics(unsigned int population[], const SimulationGrid& g) const;

    int getRow() const { return row; }
    int getCol() const { return col; }
};

// Represents an empty space in the grid
class Empty : public Entity {
public:
    Empty(int r = 0, int c = 0) : Entity(r, c) {}
    EntityType what() const override { return EntityType::EMPTY; }
    string toString() const override { return "E"; }
    Entity* next(const SimulationGrid& g) override;
    Entity* clone() const override { return new Empty(*this); }
};

// Represents a plant entity on the grid
class Plant : public Entity {
public:
    Plant(int r = 0, int c = 0) : Entity(r, c) {}
    EntityType what() const override { return EntityType::PLANT; }
    string toString() const override { return "P"; }
    Entity* next(const SimulationGrid& g) override;
    Entity* clone() const override { return new Plant(*this); }
};

// Abstract class representing an animal (either a Fox or a Gopher)
class Animal : public Entity {
protected:
    int age;  // Age of the animal
public:
    Animal(int r, int c, int age = 0) : Entity(r, c), age(age) {}
    int getAge() const { return age; }
    void setAge(int a) { age = a; }
};

// Gopher class derived from Animal
class Gopher : public Animal {
public:
    Gopher(int r = 0, int c = 0, int age = 0) : Animal(r, c, age) {}
    EntityType what() const override { return EntityType::GOPHER; }
    string toString() const override { return "G" + to_string(age); }
    Entity* next(const SimulationGrid& g) override;
    Entity* clone() const override { return new Gopher(*this); }
};

// Fox class derived from Animal
class Fox : public Animal {
public:
    Fox(int r = 0, int c = 0, int age = 0) : Animal(r, c, age) {}
    EntityType what() const override { return EntityType::FOX; }
    string toString() const override { return "F" + to_string(age); }
    Entity* next(const SimulationGrid& g) override;
    Entity* clone() const override { return new Fox(*this); }
};

// Factory function to generate entity from a string token (used in file reading)
Entity* createEntity(const string& token, int row, int col) {
    if (token == "E") return new Empty(row, col);
    else if (token == "P") return new Plant(row, col);
    else if (token[0] == 'G') return new Gopher(row, col, token[1] - '0');
    else if (token[0] == 'F') return new Fox(row, col, token[1] - '0');
    throw invalid_argument("Invalid entity token: " + token);
}

// SimulationGrid represents the 2D grid and handles all entity interactions
class SimulationGrid {
public:
    int rows, cols;  // Dimensions of the grid
    vector<vector<Entity*>> grid;  // 2D grid storing pointers to entities

    // Default constructor
    SimulationGrid() : rows(0), cols(0) {}

    // Constructor for random grid initialization
    SimulationGrid(int r, int c);

    // Constructor for reading grid from file
    SimulationGrid(const string& filepath);

    // Destructor: cleans up all dynamic entities
    ~SimulationGrid();

    // Copy constructor and assignment disabled
    SimulationGrid(const SimulationGrid& other) = delete;
    SimulationGrid& operator=(const SimulationGrid& other) = delete;

    // Move constructor
    SimulationGrid(SimulationGrid&& other) noexcept;

    // Move assignment operator
    SimulationGrid& operator=(SimulationGrid&& other) noexcept;

    // Prints the current state of the grid to console
    void print() const;

    // Returns pointer to entity at (r, c); nullptr if out of bounds
    Entity* get(int r, int c) const;

    // Sets a new entity at position (r, c) and deletes previous one
    void set(int r, int c, Entity* e);

    // Output stream operator to write grid to file
    friend ostream& operator<<(ostream& os, const SimulationGrid& g);

    // Input stream operator to read grid from file
    friend istream& operator>>(istream& is, SimulationGrid& g);
};

// Implementation of the demographics method
// Counts how many of each entity type are present in the 8 neighboring cells
void Entity::demographics(unsigned int population[], const SimulationGrid& g) const {
    fill(population, population + 4, 0);  // Initialize counts to 0
    int dx[]{ 0, 0, 1, 1, 1, -1, -1, -1 }; // X-direction offsets
    int dy[]{ 1, -1, -1, 0, 1, -1, 0, 1 }; // Y-direction offsets

    for (int i = 0; i < 8; ++i) {
        int r = row + dy[i];
        int c = col + dx[i];
        Entity* neighbor = g.get(r, c);
        if (neighbor)
            population[(int)neighbor->what()]++; // Increment count for the entity type
    }
}

// Rules for an Empty cell: it can spawn a gopher, fox, or plant based on neighboring counts
Entity* Empty::next(const SimulationGrid& g) {
    unsigned int pop[4];
    demographics(pop, g);
    if (pop[2] > 1) return new Gopher(row, col, 0);
    else if (pop[1] > 1) return new Fox(row, col, 0);
    else if (pop[3] >= 1) return new Plant(row, col);
    else return new Empty(row, col);
}

// Rules for a Plant: can be eaten or spawn gophers
Entity* Plant::next(const SimulationGrid& g) {
    unsigned int pop[4];
    demographics(pop, g);
    if (pop[2] >= pop[3] * 2) return new Empty(row, col); // Overgrazed
    else if (pop[2] >= 3) return new Gopher(row, col, 0); // Gophers take over
    else return new Plant(row, col);
}

// Rules for a Gopher: dies by age, starvation, or predation
Entity* Gopher::next(const SimulationGrid& g) {
    unsigned int pop[4];
    demographics(pop, g);
    if (age >= 4 || pop[3] == 0 || pop[1] >= pop[2]) return new Empty(row, col); // Dies
    else return new Gopher(row, col, age + 1); // Ages
}

// Rules for a Fox: dies by age or starvation, otherwise survives
Entity* Fox::next(const SimulationGrid& g) {
    unsigned int pop[4];
    demographics(pop, g);
    if (age >= 5 || pop[2] == 0) return new Empty(row, col); // Dies
    else return new Fox(row, col, age + 1); // Ages
}

// Constructor for random initialization of grid
SimulationGrid::SimulationGrid(int r, int c) : rows(r), cols(c) {
    grid.resize(r, vector<Entity*>(c, nullptr));
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 3);

    for (int i = 0; i < r; ++i) {
        for (int j = 0; j < c; ++j) {
            int r = dis(gen);
            switch (r) {
                case 0: grid[i][j] = new Empty(i, j); break;
                case 1: grid[i][j] = new Plant(i, j); break;
                case 2: grid[i][j] = new Gopher(i, j, 0); break;
                case 3: grid[i][j] = new Fox(i, j, 0); break;
            }
        }
    }
}

// Constructor for loading grid from file
SimulationGrid::SimulationGrid(const string& filepath) {
    ifstream in(filepath);
    if (!in.is_open()) throw runtime_error("Cannot open file");
    in >> rows >> cols;
    grid.resize(rows, vector<Entity*>(cols, nullptr));
    string token;
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j) {
            in >> token;
            grid[i][j] = createEntity(token, i, j);
        }
}

// Destructor: deletes all dynamically allocated entities
SimulationGrid::~SimulationGrid() {
    for (auto& row : grid)
        for (auto& cell : row)
            delete cell;
}

// Move constructor: takes ownership of another grid
SimulationGrid::SimulationGrid(SimulationGrid&& other) noexcept {
    rows = other.rows;
    cols = other.cols;
    grid = move(other.grid);
    other.rows = other.cols = 0;
}

// Move assignment: transfers ownership of another grid
SimulationGrid& SimulationGrid::operator=(SimulationGrid&& other) noexcept {
    if (this != &other) {
        for (auto& row : grid)
            for (auto& cell : row)
                delete cell;
        rows = other.rows;
        cols = other.cols;
        grid = move(other.grid);
        other.rows = other.cols = 0;
    }
    return *this;
}

// Prints the entire grid to standard output
void SimulationGrid::print() const {
    for (auto& row : grid) {
        for (auto& cell : row)
            cout << cell->toString() << " ";
        cout << "\n";
    }
}

// Returns the entity at (r, c) if within bounds, otherwise nullptr
Entity* SimulationGrid::get(int r, int c) const {
    if (r < 0 || r >= rows || c < 0 || c >= cols) return nullptr;
    return grid[r][c];
}

// Sets a new entity at (r, c), deleting the old one first
void SimulationGrid::set(int r, int c, Entity* e) {
    if (grid[r][c]) delete grid[r][c];
    grid[r][c] = e;
}

// Output operator to save grid to file
ostream& operator<<(ostream& os, const SimulationGrid& g) {
    os << g.rows << " " << g.cols << "\n";
    for (int i = 0; i < g.rows; ++i) {
        for (int j = 0; j < g.cols; ++j)
            os << g.grid[i][j]->toString() << " ";
        os << "\n";
    }
    return os;
}

// Input operator to load grid from file
istream& operator>>(istream& is, SimulationGrid& g) {
    is >> g.rows >> g.cols;
    g.grid.resize(g.rows, vector<Entity*>(g.cols, nullptr));
    string token;
    for (int i = 0; i < g.rows; ++i)
        for (int j = 0; j < g.cols; ++j) {
            is >> token;
            g.grid[i][j] = createEntity(token, i, j);
        }
    return is;
}

// Simulation class handles the lifecycle of the simulation
class Simulation {
public:
    void run();  // Runs the simulation
};

// Asks the user to choose input method, runs N cycles, prints and saves final state
void Simulation::run() {
    SimulationGrid grid;
    int choice;
    cout << "Choose initialization method:\n1. Random\n2. From file\n> ";
    cin >> choice;

    if (choice == 1) {
        int r, c;
        cout << "Enter rows and cols: ";
        cin >> r >> c;
        grid = SimulationGrid(r, c);
    } else if (choice == 2) {
        string path;
        cout << "Enter file path: ";
        cin >> path;
        grid = SimulationGrid(path);
    }

    int cycles;
    cout << "Enter number of cycles: ";
    cin >> cycles;

    cout << "\nInitial Grid:\n";
    grid.print();

    // Run simulation for specified number of cycles
    for (int c = 0; c < cycles; ++c) {
        SimulationGrid next(grid.rows, grid.cols);
        for (int i = 0; i < grid.rows; ++i)
            for (int j = 0; j < grid.cols; ++j)
                next.set(i, j, grid.get(i, j)->next(grid));
        grid = move(next);
    }

    cout << "\nFinal Grid:\n";
    grid.print();

    // Save final grid to file
    ofstream out("final_grid.txt");
    if (out.is_open()) {
        out << grid;
        cout << "\nFinal grid written to final_grid.txt\n";
    }
}

// Main function starts the simulation
int main() {
    Simulation sim;
    sim.run();
    return 0;
}
