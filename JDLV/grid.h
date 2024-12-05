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

    int countAliveNeighbors(int x, int y) {  // Compte les cellules voisines vivantes
        int count = 0;
        for (int dx = -1; dx <= 1; ++dx) {         // Permet de se reperer et faire le tour des voisines
            for (int dy = -1; dy <= 1; ++dy) {
                if (dx == 0 && dy == 0) continue;  // Ignorer la cellule du milieu

                int nx = (x + dx + rows) % rows;   // Grille torique
                int ny = (y + dy + cols) % cols; 

                if (cells[nx][ny].GetState() == 1) { //Compter si la cellule est vivante
                    count++;
                }
            }
        }
        return count; //retourne le nombre de cellule vivante autour
    }

public:
    // Getter pour accéder à une cellule spécifique
    Cell& getCell(int x, int y) {
        if (x < 0 || x >= rows || y < 0 || y >= cols) { //retrouve la cellule choisi
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
        ifstream file(fileName); // Ouvre le fichier
        if (!file.is_open()) { // Si le fichier ne peux pas etre ouvert
            throw runtime_error("Unable to open file");
        }

        file >> rows >> cols; // Lire dans le fichier les colonnes et lignes
        cells.resize(rows, vector<Cell>(cols));
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                int state;
                file >> state; // Lire l'état depuis le fichier
                cells[i][j] = Cell(i, j, state); // Écris l'etat de la cellule depuis le fichier
            }
        }
        file.close();
    }

    //Méthode qui pourrait générer un nom de fichier
    string generateFolderName (const string& baseFolderName) const {
        return baseFolderName + "_out";  // Ajoute un suffixe "_out"
    }

    // Méthode pour sauvegarder une itération dans un fichier
    void saveToFile(const string& baseFileName, int iteration) {
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
        for (auto& row : cells) {
            for (auto& cell : row) {
                file << cell.GetState() << " ";  // 1 si vivante, 0 si morte
            }
            file << "\n";
        }

        // Fermer le fichier après l'écriture
        file.close();
        cout << "Données sauvegardées dans le fichier : " << fileName << "\n";
    }

    // Méthodes virtuelles pures
    virtual void update() = 0;
    virtual void display() = 0;

};

// Classe pour le mode console
class ConsoleGrid : public GridBase {
public:
    ConsoleGrid(int r, int c) : GridBase(r, c) {}

    void update() override {
        vector<vector<Cell>> nextState = cells;
        for (int i = 0; i < rows; ++i) { // Calculer pour chaque cellule de la grille
            for (int j = 0; j < cols; ++j) {
                int aliveNeighbors = countAliveNeighbors(i, j);
                nextState[i][j].SetState(Rules::determineNextState(cells[i][j], aliveNeighbors)); // Nouvelle etat de la cellule en fonction des regles
            }
        }
        cells = nextState; // Change l'etat de la cellule avec le nouvelle état
    }

    void display() override {
        for (auto& row : cells) {
            for (auto& cell : row) {
                cout << ((cell.GetState()==1) ? "O" : (cell.GetState()==2) ? "□" : ".") << " ";  // Affichage des cellules dans la console
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
    bool isActive;
public:
    //Constructeur de la grille graphique avec la librairie STML
    GraphicalGrid(int r, int c, int size): 
        GridBase(r, c), cellSize(size), window(sf::VideoMode(c * size, r * size), "Game of Life") {}

    void update() override {
        vector<vector<Cell>> nextState = cells;
        for (int i = 0; i < rows; ++i) { // Calculer l'état pour chaque cellule de la grille
            for (int j = 0; j < cols; ++j) {
                int aliveNeighbors = countAliveNeighbors(i, j);
                nextState[i][j].SetState(Rules::determineNextState(cells[i][j], aliveNeighbors)); // Regarde autour de la cellule et determine son état en fonction des voisins
            }
        }
        cells = nextState; // Met a jour l'état de la cellule
    }

    void display() override {
        window.clear(sf::Color(40,40,40)); // Clear la fenetre
        for (int i = 0; i < rows; ++i) { //Pour chaque cellule
            for (int j = 0; j < cols; ++j) {
                sf::RectangleShape rectangle(sf::Vector2f(cellSize - 1, cellSize - 1)); // Créer un rectangle pour chaque cellule
                rectangle.setPosition(j * cellSize, i * cellSize); // Positionnement des cellules
                rectangle.setFillColor((cells[i][j].GetState()==1) ? sf::Color(255,255,255) : (cells[i][j].GetState()==2) ? sf::Color(255,0,0) : sf::Color(0,0,0)); // Paramètre les cellules et leurs couleurs
                window.draw(rectangle); // Créer les rectangles
            }
        }
        window.display();
    }

    void run(int time) {
        while (window.isOpen()) {
            sf::Event event;

            // Gestion des événements du clavier
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) { //Ferme la fenêtre
                    window.close();
                }

                if (event.type == sf::Event::LostFocus) { //Met en pause le jeu quand la fenêtre n'est pas focus
                    isActive = false;
                }

                if (event.type == sf::Event::GainedFocus) { //Enlève la pause le jeu quand la fenêtre est focus
                    isActive = true;
                }

                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Space) { // Basculer l'état pause ON/OFF avec la barre espace
                        isActive = !isActive;
                    }
                    
                    if (event.key.code == sf::Keyboard::Left) { // Réduction du délai avec la flèche gauche
                        time = max(0, time - 5); 
                        cout << "Changed : " << time << " ms" << endl;
                    }

                    if (event.key.code == sf::Keyboard::Right) { // Augmentation du délai avec la flèche droite
                        time = min(1000, time + 5);
                        cout << "Changed refresh rate: " << time << " ms" << endl;
                    }
                }
                
                // Gestion des événements de la souris
                if (event.type == sf::Event::MouseButtonPressed) { // Si la souris est pressé 
                    int mouseX = event.mouseButton.x;              // Récupere la position en x de la souris
                    int mouseY = event.mouseButton.y;              // Récupere la position en y de la souris

                    int cellX = mouseY / cellSize;                 // Trouve les coordonnées de la cellule où pointe la souris
                    int cellY = mouseX / cellSize;                 // CellX et CellY sont les coordonnées de la cellule
 
                    // Vérifier si la cellule existe
                    if (cellX >= 0 && cellX < rows && cellY >= 0 && cellY < cols) {
                        // Clic gauche changer l'état de la cellule entre vivante ou morte (obstacle est ingoré)
                        if (event.mouseButton.button == sf::Mouse::Left) { 
                            int currentState = cells[cellX][cellY].GetState(); //Savoir quelle type de cellule est cliqué
                            cells[cellX][cellY].SetState((currentState == 2) ? 2 : (currentState == 1) ? 0 : 1);
                        }   

                        // Clic droit change l'état de la cellule entre morte et obstacle (vivant est ingnoré)
                        if (event.mouseButton.button == sf::Mouse::Right) {
                            int currentState = cells[cellX][cellY].GetState(); //Savoir quelle type de cellule est cliqué
                            cells[cellX][cellY].SetState((currentState == 1) ? 1 : (currentState == 2) ? 0 : 2);
                        }
                    }
                }
            }

            // Mise à jour en fonction de isActive
            if (isActive) {
                update();
                
            this_thread::sleep_for(chrono::milliseconds(time)); //Temps entre chaque itérations
            }
            display(); // Mettre à jour la fenêtre
        }
    }
};

#endif