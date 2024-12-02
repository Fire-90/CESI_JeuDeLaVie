#ifndef Gameoflife_H
#define Gameoflife_H

#include <iostream>
#include <fstream>
#include <filesystem>  // Nécessaire pour manipuler le système de fichiers

// Classe principale
class GameOfLife {
public:
    void run(const string& inputFile) {
        cout << "Select mode: \n1. Console \n2. Graphical\n";
        int mode;
        cin >> mode;

        if (mode == 1) {
            ConsoleGrid grid(0, 0);
            grid.loadFromFile(inputFile);

            int iterations;
            cout << "Enter number of iterations: ";
            cin >> iterations;

            grid.display();
            for (int i = 0; i < iterations; ++i) {
                grid.update();
                cout << "-------------------------------------------------" << endl;
                grid.display();
    
                this_thread::sleep_for(chrono::milliseconds(10));
                grid.saveToFile(inputFile, i);
            }

            cout << "Iterations finished" << endl;

        } else if (mode == 2) {
            int input;
            cout << "Enter time between iterations (in milliseconds): ";
            cin >> input;

            GraphicalGrid grid(100, 100, 10);
            grid.loadFromFile(inputFile);
            grid.run(input);
        } else {
            cerr << "Invalid mode selected.\n";
        }
    }
};

#endif
