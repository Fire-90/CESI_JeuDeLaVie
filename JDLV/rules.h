#ifndef Rules_H
#define Rules_H

// Classe Rules pour encapsuler les règles du jeu
class Rules {
public:
    static int determineNextState(Cell& cell, int aliveNeighbors) {                         //Determine le prochaine etat en fonction de la cellule et des voisins
        if (cell.GetState() == 2) {                                                         //Si c'est une cellule obstacle
            return 2;                                                                       //aucun changement
        } else if (cell.GetState() == 1 && aliveNeighbors == 2 || aliveNeighbors == 3){     //Si c'est une cellule vivante avec 2 ou 3 voisins vivants 
            return 1;                                                                       //La cellule reste vivante
        } else if (cell.GetState() == 0 && aliveNeighbors == 3){                            //Si c'est une cellule morte avec exactement 3 voisins vivants
            return 1;                                                                       //La cellule nait
        } else {                                                                            //Sinon
            return 0;                                                                       //La cellule reste morte ou meurt de surpopulation
        }
    }
};

#endif