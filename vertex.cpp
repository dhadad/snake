#include "vertex.h"

bool Vertex::rangeCheck(int x, int y){
    return x >= width || y >= height || x < 0 || y < 0;
}

direction Vertex::getDirectionFromChar(char c) {
    switch (c) {
        case 'w': return UP;
        case 's': return DOWN;
        case 'd': return RIGHT;
        case 'a': return LEFT;
        default: return NONE;
    }
}

bool Vertex::oppositeDirections(direction first, direction second) {
    return (first == UP && second == DOWN) || \
        (first == DOWN && second == UP) || \
        (first == RIGHT && second == LEFT) || \
        (first == LEFT && second == RIGHT);
}

Vertex& Vertex::operator+=(const Vertex& other) {
    int new_x = x + other.x, new_y = y + other.y;
    if (rangeCheck(new_x ,new_y)) throw OutOfRange();
    x = new_x, y = new_y; 
    return *this;
}

int Vertex::getX() const {
    return this->x;
}

int Vertex::getY() const {
    return this->y;
}

Vertex& Vertex::step() {
    switch (dir) {
        case DOWN:
            y = (y + jump < height) ? y + jump : throw OutOfRange();
            break;
        case UP:
            y = y - jump > 0 ? y - jump : throw OutOfRange();
            break;
        case RIGHT:
            x = (x + jump < width) ? x + jump : throw OutOfRange();
            break;
        case LEFT:
            x = x - jump > 0 ? x - jump : throw OutOfRange();
            break;
        default: //nothing
            break;
    }
    return *this;
}

Vertex Vertex::peekStep() {
    Vertex next = Vertex(*this);
    return next.step();
}