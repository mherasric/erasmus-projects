#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <memory>
#include <algorithm>

using namespace std;

// --- ENUM CLASS FOR ENTITY TYPES ---
enum class EntityType { EMPTY, FOX, GOPHER, PLANT };

// --- FORWARD DECLARATION ---
class SimulationGrid;

// --- BASE CLASS FOR ALL ENTITIES ---
// Abstract base class for all grid elements: Fox, Gopher, Plant, Empty.
// It stores position (row, col) and defines interface for child classes.
class Entity {
protected:
    int row, col;
public:
    Entity(int r, int c) : row(r), col(c) {}
    virtual ~Entity() = default;

    virtual EntityType what() const = 0; // Returns the type of entity (enum class)
    virtual string toString() const = 0; // String representation to print on the grid
    virtual Entity* next(const SimulationGrid& g) = 0; // Returns the next state of the entity, depending on the current grid

    virtual void demographics(unsigned int population[], const SimulationGrid& g) const; // Calculates the number of neighboring entities by type
};

// --- DERIVED CLASS: EMPTY CELL ---
class Empty : public Entity {
public:
    Empty(int r, int c) : Entity(r, c) {}
    EntityType what() const override { return EntityType::EMPTY; }
    string toString() const override { return "E"; }

    Entity* next(const SimulationGrid& g) override;
};

// --- DERIVED CLASS: PLANT ---
class Plant : public Entity {
public:
    Plant(int r, int c) : Entity(r, c) {}
    EntityType what() const override { return EntityType::PLANT; }
    string toString() const override { return "P"; }

    Entity* next(const SimulationGrid& g) override;
};

// --- ABSTRACT CLASS FOR ANIMALS ---
class Animal : public Entity {
protected:
    int age;
public:
    Animal(int r, int c, int age) : Entity(r, c), age(age) {}
    int getAge() const { return age; }
    void setAge(int a) { age = a; }
};

// --- DERIVED CLASS: GOPHER ---
class Gopher : public Animal {
public:
    Gopher(int r, int c, int age) : Animal(r, c, age) {}
    EntityType what() const override { return EntityType::GOPHER; }
    string toString() const override { return "G" + to_string(age); }

    Entity* next(const SimulationGrid& g) override;
};

// --- DERIVED CLASS: FOX ---
class Fox : public Animal {
public:
    Fox(int r, int c, int age) : Animal(r, c, age) {}
    EntityType what() const override { return EntityType::FOX; }
    string toString() const override { return "F" + to_string(age); }

    Entity* next(const SimulationGrid& g) override;
};

// --- SIMULATION GRID ---
class SimulationGrid {
public:
    int rows, cols;
    vector<vector<Entity*>> grid;

    SimulationGrid(int r, int c) : rows(r), cols(c) { //constructor to allocate grid
        grid.resize(r, vector<Entity*>(c, nullptr));
    }

    ~SimulationGrid() { //destructor deletes all dynamically allocated emtities
        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < cols; ++j)
                delete grid[i][j];
    }

    // Delete copy constructor and copy assignment
    SimulationGrid(const SimulationGrid& other) = delete;
    SimulationGrid& operator=(const SimulationGrid& other) = delete;

    // Move constructor
    SimulationGrid(SimulationGrid&& other) noexcept {
        rows = other.rows;
        cols = other.cols;
        grid = std::move(other.grid);
        other.rows = 0;
        other.cols = 0;
    }

    // Move assignment operator
    SimulationGrid& operator=(SimulationGrid&& other) noexcept {
        if (this != &other) {
            // Delete current entities
            for (int i = 0; i < rows; ++i)
                for (int j = 0; j < cols; ++j)
                    delete grid[i][j];

            // Move data
            rows = other.rows;
            cols = other.cols;
            grid = std::move(other.grid);

            other.rows = 0;
            other.cols = 0;
        }
        return *this;
    }

    void print() const {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j)
                cout << grid[i][j]->toString() << " ";
            cout << "\n";
        }
    }

    Entity* get(int r, int c) const {
        if (r < 0 || r >= rows || c < 0 || c >= cols)
            return nullptr;
        return grid[r][c];
    }

    void set(int r, int c, Entity* e) {
        if (grid[r][c]) delete grid[r][c];
        grid[r][c] = e;
    }
};

// --- ENTITY::DEMOGRAPHICS IMPLEMENTATION ---
void Entity::demographics(unsigned int population[], const SimulationGrid& g) const {
    fill(population, population + 4, 0);
    int dx[]{ 0, 0, 1, 1, 1, -1, -1, -1 };
    int dy[]{ 1, -1, -1, 0, 1,  -1, 0, 1 };

    for (int i = 0; i < 8; ++i) {
        int r = row + dy[i];
        int c = col + dx[i];
        Entity* neighbor = g.get(r, c);
        if (neighbor)
            population[(int)neighbor->what()]++;
    }
}

// --- ENTITY UPDATES ---
Entity* Empty::next(const SimulationGrid& g) {
    unsigned int pop[4];
    demographics(pop, g);
    if (pop[2] > 1) return new Gopher(row, col, 0);
    else if (pop[1] > 1) return new Fox(row, col, 0);
    else if (pop[3] >= 1) return new Plant(row, col);
    else return new Empty(row, col);
}

Entity* Plant::next(const SimulationGrid& g) {
    unsigned int pop[4];
    demographics(pop, g);
    if (pop[2] >= pop[3] * 2) return new Empty(row, col);
    else if (pop[2] >= 3) return new Gopher(row, col, 0);
    else return new Plant(row, col);
}

Entity* Gopher::next(const SimulationGrid& g) {
    unsigned int pop[4];
    demographics(pop, g); //Count neighboring entities
    if (age >= 4) return new Empty(row, col); //gopher dies after 4 cycles
    else if (pop[3] == 0) return new Empty(row, col); //if there are no plants, it starves
    else if (pop[1] >= pop[2]) return new Fox(row, col, 0); //if foxes outnumber gophers, it gets eaten
    else return new Gopher(row, col, age + 1);//otherwise, it survives and grows older
}

Entity* Fox::next(const SimulationGrid& g) {
    unsigned int pop[4];
    demographics(pop, g);
    if (age >= 5) return new Empty(row, col);
    else if (pop[2] == 0) return new Empty(row, col);
    else return new Fox(row, col, age + 1);
}

// --- MAIN ---
int main() {
    int rows = 5, cols = 5, cycles; //grid size and number of simulation

    cout << "Enter number of simulation cycles: ";
    cin >> cycles;

    // Create random grid
    SimulationGrid grid(rows, cols);

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 3);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int r = dis(gen);
            switch (r) {
            case 0: grid.set(i, j, new Empty(i, j)); break;
            case 1: grid.set(i, j, new Plant(i, j)); break;
            case 2: grid.set(i, j, new Gopher(i, j, 0)); break;
            case 3: grid.set(i, j, new Fox(i, j, 0)); break;
            }
        }
    }

    cout << "\nInitial Grid:\n";
    grid.print();

    // Simulation loop
    for (int c = 0; c < cycles; ++c) {
        SimulationGrid nextGrid(rows, cols);
        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < cols; ++j)
                nextGrid.set(i, j, grid.get(i, j)->next(grid));

        grid = std::move(nextGrid);  
    }

    cout << "\nFinal Grid after " << cycles << " cycles:\n";
    grid.print();

    return 0;
}

