#ifndef VERTEX_H_
#define VERTEX_H_

#include "exceptions.h"

const int height = 321;
const int width = 481;
const int jump = 8;
typedef enum {UP, DOWN, LEFT, RIGHT, NONE} direction;

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
/*
Vertex operator+(const Vertex& left, const Vertex& right) {
    Vertex result = Vertex(left);
    result += right;
    return result;
}*/

#endif //VERTEX_H_