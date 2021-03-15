#include "snake.h"

void Snake::advance() {
    for (vector<Vertex>::reverse_iterator i = chain.rbegin(); i != chain.rend(); ++i) {
        i->step();
        if (i + 1 != chain.rend()) 
            i->dir = (i+1)->dir;
    }
}

void Snake::changeDirection(direction new_direction) {
    if (chain.empty()) return;
    chain[0].dir = new_direction;
}

direction Snake::getDirection() {
    if (chain.empty()) throw EmptySnake();
    return chain[0].dir;
}

Snake& Snake::operator+=(const Vertex& vertex) {
    chain.push_back(vertex);
    score++;
    return *this;
}

Snake& Snake::operator--() {
    chain.pop_back();
    score--;
    return *this;
}

Vertex& Snake::operator[](int index){ //Allows editing
    if (index < 0 || index >= (int)chain.size()) throw OutOfRange();
    return chain[index];
}

int Snake::getScore() const{
    return score;
}

int Snake::length() const {
    return chain.size();
}

vector<Vertex>::iterator Snake::begin() {
    return chain.begin();
}

vector<Vertex>::iterator Snake::end() {
    return chain.end();
}