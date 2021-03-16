#ifndef SNAKE_H_
#define SNAKE_H_

#include "vertex.h"
#include <vector>
using std::vector;

static const int default_lives = 3;
static const int default_x = width/2;
static const int default_y = height/2;
static const direction default_dir = RIGHT;

class Snake {
    int score;
    int lives;
    vector<Vertex> chain;
public: 
    Snake(): score(0), lives(default_lives), chain(1, Vertex(default_x, default_y, default_dir)) {}
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
    Snake& reset();
    vector<Vertex>::iterator begin();
    vector<Vertex>::iterator end();
    vector<Vertex>::const_iterator begin() const;
    vector<Vertex>::const_iterator end() const;
};

#endif //SNAKE_H_