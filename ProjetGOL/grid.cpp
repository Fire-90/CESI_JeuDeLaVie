#include "Grid.h"
#include <fstream>
#include <iostream>
#include <thread>
#include <chrono>

Grid::Grid(int r, int c) : rows(r), cols(c) {
    cells.resize(rows, std::vector<Cell>(cols));
}

void Grid::loadFromFile(const std::string& fileName) {
    std::ifstream file(fileName);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + fileName);
    }

    file >> rows >> cols;
    cells.resize(rows, std::vector<Cell>(cols));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int state;
            file >> state;
            cells[i][j].isAlive = (state == 1);
        }
    }
    file.close();
}

void Grid::saveToFile(const std::string& fileName) {
    std::ofstream file(fileName);
    for (const auto& row : cells) {
        for (const auto& cell : row) {
            file << (cell.isAlive ? 1 : 0) << " ";
        }
        file << "\n";
    }
    file.close();
}

int Grid::countAliveNeighbors(int x, int y) const {
    int count = 0;
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) continue;

            int nx = (x + dx + rows) % rows;  // Grille torique
            int ny = (y + dy + cols) % cols;

            if (cells[nx][ny].isAlive) {
                count++;
            }
        }
    }
    return count;
}

void Grid::update() {
    std::vector<std::vector<Cell>> nextState = cells;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int aliveNeighbors = countAliveNeighbors(i, j);

            if (cells[i][j].isAlive) {
                nextState[i][j].isAlive = aliveNeighbors == 2 || aliveNeighbors == 3;
            } else {
                nextState[i][j].isAlive = aliveNeighbors == 3;
            }
        }
    }
    cells = nextState;
}

void Grid::displayConsole() const {
    for (const auto& row : cells) {
        for (const auto& cell : row) {
            std::cout << (cell.isAlive ? "O" : ".") << " ";
        }
        std::cout << "\n";
    }
}

void Grid::displayGraphical(sf::RenderWindow& window, int cellSize) const {
    window.clear();
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            sf::RectangleShape rectangle(sf::Vector2f(cellSize - 1, cellSize - 1));
            rectangle.setPosition(j * cellSize, i * cellSize);
            rectangle.setFillColor(cells[i][j].isAlive ? sf::Color::Green : sf::Color::Black);
            window.draw(rectangle);
        }
    }
    window.display();
}
