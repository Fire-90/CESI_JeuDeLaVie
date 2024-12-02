#ifndef Cell_H
#define Cell_H

// Class Cell
class Cell {
public:
    bool alive;
    int x, y;

    Cell(int x = 0, int y = 0, bool state = false)
        : x(x), y(y), alive(state) {}
};

#endif
