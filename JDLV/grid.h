#ifndef Grid_H
#define Grid_H

#include <iostream>
#include <fstream>
#include "rules.h"

using namespace std;

// Classe de base pour une grille générique
class GridBase {
protected:
    vector<vector<Cell>> cells;
    int rows, cols;

    int countAliveNeighbors(int x, int y) const {  // compte les cellules voisines vivantes
        int count = 0;
        for (int dx = -1; dx <= 1; ++dx) {         // permet de se reperer et faire le tour des voisines
            for (int dy = -1; dy <= 1; ++dy) {
                if (dx == 0 && dy == 0) continue;  // Ignore la cellule du milieu

                int nx = (x + dx + rows) % rows;   // Gestion de la grille torique
                int ny = (y + dy + cols) % cols; 

                if (cells[nx][ny].alive) {
                    count++;
                }
            }
        }
        return count; //retourne le nombre de cellule vivante autour
    }

public:
    // Getter pour accéder à une cellule spécifique
    Cell& getCell(int x, int y) {
        if (x < 0 || x >= rows || y < 0 || y >= cols) {
            throw out_of_range("Cell indices are out of range.");
        }
        return cells[x][y];
    }

    //Constructeur de la grille
    GridBase(int r, int c) : rows(r), cols(c) {
        cells.resize(rows, vector<Cell>(cols));
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                cells[i][j] = Cell(i, j);
            }
        }
    }

    //Destructeur de la grille
    virtual ~GridBase() {}

    //Méthode pour utiliser le fichier txt d'entrer
    virtual void loadFromFile(const string& fileName) {
        ifstream file(fileName);
        if (!file.is_open()) { //si le fichier ne peux pas etre ouvert
            throw runtime_error("Unable to open file");
        }

        file >> rows >> cols; // lire dans le fichier les colonnes et lignes
        cells.resize(rows, vector<Cell>(cols));
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                int state;
                file >> state; //lire l'etat
                cells[i][j] = Cell(i, j, state == 1); //ecris l'etat de la cellule depuis le fichier
            }
        }
        file.close();
    }

    //Méthode qui pourrait générer un nom de fichier
    string generateFolderName (const string& baseFolderName) const {
        return baseFolderName + "_out";  // Ajoute un suffixe "_out"
    }

    // Méthode pour sauvegarder une itération dans un fichier
    void saveToFile(const string& baseFileName, int iteration) const {
        // Générer le nom du dossier
        string folderName = generateFolderName(baseFileName);

        // Créer le dossier s'il non existant
        if (!filesystem::exists(folderName)) {
            filesystem::create_directory(folderName);
        }

        // Générer le nom du fichier en fonction de l'itération
        string fileName = folderName + "/iteration_" + to_string(iteration) + ".txt";

        // Créer et ouvrir un fichier pour l'écriture
        ofstream file(fileName);
        if (!file) {
            cerr << "Erreur lors de l'ouverture du fichier : " << fileName << "\n";
            return;
        }

        // Écrire les données dans le fichier
        for (const auto& row : cells) {
            for (const auto& cell : row) {
                file << (cell.alive ? 1 : 0) << " ";  // 1 si vivante, 0 si morte
            }
            file << "\n";
        }

        // Fermer le fichier après l'écriture
        file.close();
        cout << "Données sauvegardées dans le fichier : " << fileName << "\n";
    }

    virtual void update() = 0;
    virtual void display() = 0;

};

// Classe pour le mode console
class ConsoleGrid : public GridBase {
public:
    ConsoleGrid(int r, int c) : GridBase(r, c) {}

    void update() override {
        vector<vector<Cell>> nextState = cells;
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                int aliveNeighbors = countAliveNeighbors(i, j);
                nextState[i][j].alive = Rules::determineNextState(cells[i][j], aliveNeighbors); //Nouvelle etat de la cellule en fonction des regles
            }
        }
        cells = nextState; //Change l'etat de la cellule avec le nouvelle état
    }

    void display() override {
        for (const auto& row : cells) {
            for (const auto& cell : row) {
                cout << (cell.alive ? "O" : ".") << " "; //affichage des cellules
            }
            cout << "\n";
        }
    }
};

// Classe pour le mode graphique
class GraphicalGrid : public GridBase {
private:
    int cellSize;
    sf::RenderWindow window;

public:
    GraphicalGrid(int r, int c, int size): 
        GridBase(r, c), cellSize(size), window(sf::VideoMode(c * size, r * size), "Game of Life") {}

    void update() override {
        vector<vector<Cell>> nextState = cells;
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                int aliveNeighbors = countAliveNeighbors(i, j);
                nextState[i][j].alive = Rules::determineNextState(cells[i][j], aliveNeighbors);
            }
        }
        cells = nextState;
    }

    void display() override {
        window.clear();
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                sf::RectangleShape rectangle(sf::Vector2f(cellSize - 1, cellSize - 1));
                rectangle.setPosition(j * cellSize, i * cellSize);
                rectangle.setFillColor(cells[i][j].alive ? sf::Color::Green : sf::Color::Black);
                window.draw(rectangle);
            }
        }
        window.display();
    }

    void run(int time) {
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
            }
            display();
            update();
            this_thread::sleep_for(chrono::milliseconds(time));
        }
    }
};

#endif
