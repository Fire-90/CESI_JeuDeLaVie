#ifndef Gameoflife_H
#define Gameoflife_H

//Imporation des librairie
#include <iostream>
#include <fstream>
#include <filesystem>

// Classe principale 'GameOfLife'
class GameOfLife {
public:
    //Méthode pour exécuter le jeu
    void run(const string& inputFile) {
        //Demande pour le choix du mode
        cout << "Select mode: \n1. Console \n2. Graphical\n"; 
        int mode;
        cin >> mode;

        if (mode == 1) { //Choix du mode console
            ConsoleGrid grid(0, 0);
            grid.loadFromFile(inputFile); //Load la grille avec le fichier en entrer

            int iterations;
            cout << "Enter number of iterations: "; //Demande le nombre d'itérations à l'utilisateur
            cin >> iterations;

            grid.display();
            for (int i = 0; i < iterations; ++i) {
                grid.update(); //Met à jour la grille
                cout << "-------------------------------------------------" << endl;
                grid.display(); //Affiche la grille dans la console
    
                this_thread::sleep_for(chrono::milliseconds(10)); //Ajoute du delai pour la lecture
                grid.saveToFile(inputFile, i+1); //Enregistre l'itérations dans un fichier txt
            }

            cout << "Iterations finished" << endl;

        } else if (mode == 2) { //Choix du mode Graphique
            int input;
            int size;
            cout << "Enter time between iterations (in milliseconds): "; //Demande le entre chaque itérations à l'utilisateur
            cin >> input;
            cout << "Enter size of the squared window (in number of cells): "; //Demande la taille de la fenêtre à l'utilisateur
            cin >> size;

            GraphicalGrid grid(size, size, 10);
            grid.loadFromFile(inputFile); //Load la grille avec le fichier en entrer
            grid.run(input); //Exécute le jeu
        } else {
            cerr << "Invalid mode selected.\n";
        }
    }
};

#endif
