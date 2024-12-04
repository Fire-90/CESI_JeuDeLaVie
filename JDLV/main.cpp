//Importation des libraries
#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <chrono>
#include <filesystem>
#include <SFML/Graphics.hpp>
using namespace std;

//Importation des classes en fichier .h
#include "cells.h"
#include "grid.h"
#include "gameoflife.h"
#include "rules.h"
#include "unittest.h"

//Fonction main
int main(int argc, char* argv[]) { //Prendre en argument le nom du fichier .txt
    //Vérifie que l'utilisateur rentre bien un nom de fichier
    if (argc < 2) {     
        cerr << "Usage: " << argv[0] << " <input_file> OR 'test <test_file>' to run unit tests.\n"; //Donne une erreur et des expliquations
        return 1;
    }

    string arg = argv[1]; //Découpe les arguments en deux ('test' + 'nom_de_fichier.txt')

    if (arg == "test") {
        // Vérifier si un fichier de test est fourni
        if (argc < 3) {
            cerr << "Error: Please provide a test file name when running tests.\n"; //Donne une erreur et des expliquations
            return 1;
        }
        string testFileName = argv[2];
        // Exécution des tests unitaires
        UnitTest::runTest(testFileName);
    } else {
        // Exécution du jeu avec le fichier d'entrée fourni
        GameOfLife game; //Créer l'instance de jeu
        game.run(arg); //Exécuter le jeu
    }

    return 0;
}