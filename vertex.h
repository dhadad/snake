#ifndef VERTEX_H_
#define VERTEX_H_

#include "assert.h"
#include "exceptions.h"

const int height = 321;
const int width = 481;
const int jump = 8;
enum direction: int {NONE = 0, UP, DOWN, LEFT, RIGHT, EXIT};

class Vertex {
    int x, y;
    static bool rangeCheck(int x, int y);
public:
    direction dir;
    Vertex(int x, int y, direction dir) try: x(x), y(y), dir(dir) {
        if (rangeCheck(x ,y)) throw OutOfRange();
    }
    catch(...) {
        throw;
    };
    Vertex(const Vertex& other): Vertex(other.x, other.y, other.dir) {};
    Vertex& operator+=(const Vertex& other);
    int getX() const;
    int getY() const;
    Vertex& step();
    Vertex peekStep();
    static direction getDirectionFromChar(char c);
    static bool oppositeDirections(direction first, direction second);
};

#endif //VERTEX_H_