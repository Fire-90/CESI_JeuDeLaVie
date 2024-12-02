#ifndef Rules_H
#define Rules_H

// Classe Rules pour encapsuler les règles du jeu
class Rules {
public:
    static bool determineNextState(const Cell& cell, int aliveNeighbors) { //Determine le prochaine etat en fonction de la cellule et des voisins
        if (cell.alive) {                                                  //Si cellule vivante
            return aliveNeighbors == 2 || aliveNeighbors == 3;             //Si elle possede 2 ou 3 voisine vivante, elle rest en vie
        } else {                                                           //Si cellule morte
            return aliveNeighbors == 3;                                    //Si elle possede 3 voisine vivante, elle nait
        }
    }
};

#endif
