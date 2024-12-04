#ifndef Cell_H
#define Cell_H

// Class Cell
class Cell {
protected:
    int state;
    int x, y;

public:
    //Constructeur de la classe
    Cell(int x = 0, int y = 0, int state = false):
        x(x), y(y), state(state){}
    
    //Getteur de l'état de la cellule
    int GetState() {
        return state;
    }

    //Setteur de l'état de la cellule
    void SetState(int newstate) {
        state=newstate;
    }
};

#endif
