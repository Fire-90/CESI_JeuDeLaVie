#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <chrono>
#include <SFML/Graphics.hpp>

using namespace std;

#include "cell.h"
#include "grid.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <input_file>\n";
        return 1;
    }

    string inputFile = argv[1];
    Grid grid(0, 0);
    grid.loadFromFile(inputFile);

    cout << "Select mode: \n1. Console \n2. Graphical\n";
    int mode;
    cin >> mode;

    if (mode == 1) {
        int iterations;
        cout << "Enter number of iterations: ";
        cin >> iterations;

        for (int i = 0; i < iterations; ++i) {
            grid.displayConsole();
            grid.update();
            this_thread::sleep_for(chrono::milliseconds(500));
        }
    } else if (mode == 2) {
        sf::RenderWindow window(sf::VideoMode(1000, 800), "Game of Life");
        int cellSize = 10;

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
            }

            grid.displayGraphical(window, cellSize);
            grid.update();
            this_thread::sleep_for(chrono::milliseconds(10));
        }
    } else {
        cerr << "Invalid mode selected.\n";
    }

    return 0;
}
