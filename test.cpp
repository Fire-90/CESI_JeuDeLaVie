#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <chrono>
#include <SFML/Graphics.hpp>

using namespace std;

class Cell {
public:
    bool isAlive;
    Cell() : isAlive(false) {}
};

class Grid {
private:
    vector<vector<Cell>> cells;
    int rows, cols;

public:
    Grid(int r, int c) : rows(r), cols(c) {
        cells.resize(rows, vector<Cell>(cols));
    }

    void loadFromFile(const string& fileName) {

    }

    void saveToFile(const string& fileName) {

    }

    int countAliveNeighbors(int x, int y) const {

    }

    void update() {

    }

    void displayConsole() const {

    }

    void displayGraphical() const {

    }
};

int main() {

}
