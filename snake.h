#ifndef SNAKE_H_
#define SNAKE_H_

#include "vertex.h"
#include <vector>
using std::vector;

class Snake {
    int score;
    vector<Vertex> chain;
public: 
    Snake(): score(0), chain(1, Vertex(height/2, width/2, RIGHT)) {}
    void advance();
    void changeDirection(direction new_direction);
    direction getDirection();
    Snake& operator+=(const Vertex& vertex);
    Snake& operator--();
    Vertex& operator[](int index);
    int getScore() const;
    int length() const;
    vector<Vertex>::iterator begin();
    vector<Vertex>::iterator end();
};

#endif //SNAKE_H_