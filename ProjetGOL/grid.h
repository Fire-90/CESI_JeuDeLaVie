#ifdef Grid_H
#define Grid_H

#include <iostream>
#include <fstream>

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
        ofstream file(fileName);
        for (const auto& row : cells) {
            for (const auto& cell : row) {
                file << (cell.isAlive ? 1 : 0) << " ";
            }
            file << "\n";
        }
        file.close();
    }

    int countAliveNeighbors(int x, int y) const {
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

    void update() {
        vector<vector<Cell>> nextState = cells;

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

    void displayConsole() const {
        for (const auto& row : cells) {
            for (const auto& cell : row) {
                cout << (cell.isAlive ? "O" : ".") << " ";
            }
            cout << "\n";
        }
    }

    void displayGraphical(sf::RenderWindow& window, int cellSize) const {
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
};

#endif