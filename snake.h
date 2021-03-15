#ifndef SNAKE_H_
#define SNAKE_H_

#include "vertex.h"
#include <vector>
using std::vector;

static const int default_lives = 3;

class Snake {
    int score;
    int lives;
    vector<Vertex> chain;
public: 
    Snake(): score(0), lives(default_lives), chain(1, Vertex(height/2, width/2, RIGHT)) {}
    void advance();
    void changeDirection(direction new_direction);
    direction getDirection();
    Snake& operator+=(const Vertex& vertex);
    Snake& operator-=(int);
    Snake& operator--();
    Vertex& operator[](int index);
    int getScore() const;
    int getLives() const;
    int length() const;
    void damage();
    vector<Vertex>::iterator begin();
    vector<Vertex>::iterator end();
    vector<Vertex>::const_iterator begin() const;
    vector<Vertex>::const_iterator end() const;
};

#endif //SNAKE_H_