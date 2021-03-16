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

direction Snake::getDirection() const {
    if (chain.empty()) throw EmptySnake();
    return chain[0].dir;
}

Snake& Snake::operator+=(const Vertex& vertex) {
    chain.push_back(vertex);
    score++;
    return *this;
}

Snake& Snake::operator-=(int disqualifications) {
    assert(disqualifications >= 0);
    for (int i = 0; i < disqualifications; ++i) {
        lives--;
        if (lives < 0)
            throw NoLivesLeft();
    }
    return *this;
}

Snake& Snake::operator--() {
    return this->operator-=(1);
}

Vertex& Snake::operator[](int index){ 
    if (index < 0 || index >= (int)chain.size()) throw OutOfRange();
    return chain[index];
}

int Snake::getScore() const{
    return score;
}

int Snake::getLives() const {
    return lives;
}

int Snake::length() const {
    return chain.size();
}

Snake& Snake::reset() {
    chain = vector<Vertex>(1, Vertex(default_x, default_y, default_dir));
    return *this;
}

vector<Vertex>::iterator Snake::begin() {
    return chain.begin();
}

vector<Vertex>::iterator Snake::end() {
    return chain.end();
}

vector<Vertex>::const_iterator Snake::begin() const {
    return chain.begin();
}

vector<Vertex>::const_iterator Snake::end() const {
    return chain.end();
}