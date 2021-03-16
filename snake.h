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
    /** advance: move all the snake vertices one step further.     */
    void advance();
    /** changeDirection: edit the movement direction of the snake's front vertex.
     * @param new_direction
    */
    void changeDirection(direction new_direction);
    /** getDirection: get the direction of the front vertex.
     * @return direction of the snake's movement.
    */
    direction getDirection() const;
    /** operator+=: adds a vertex in the end of the snake, updates score.
     * @param vertex A reference to the vertex to be added.
     * @return A reference to the updated snake.
    */
    Snake& operator+=(const Vertex& vertex);
    /** operator-=: updates the number of lives of the snake, as a result of a self cross.
     * @param disqualifications An integer, number of lives to be deduced. Asserted to be
     * non negative.
     * @return A reference to the updated snake.
    */
    Snake& operator-=(int);
    /** operator--: deduce one life, as a result of a self cross.
     * @return A reference to the updated snake.
    */
    Snake& operator--();
    /** operator-[]: access a specific vertex in the snake.
     * @param index An integer, must be non negative but smaller the the snake's size.
     * @return A reference to the wanted vertex, if exists. Otherwise, throws excepetion.
    */
    Vertex& operator[](int index);
    /** getScore: returns the current score.
     * @return Integer represting current score.
     */
    int getScore() const;
    /** getLives: returns how many lives left.
     * @return Integer represting current nubmer of lives.
     */
    int getLives() const;
    /** length: returns the snake's length.
     * @return Integer represting current nubmer of vertices.
     */
    int length() const;
    /** reset: makes the snake one vertex long, the move it to the default location.
     * @return A reference to the updated snake.
    */
    Snake& reset();
    /** begin: @return iterator, points to the fron to the snake.    */
    vector<Vertex>::iterator begin();
    /** end: @return iterator, points to the a cell following the snake's end.    */
    vector<Vertex>::iterator end();
    /** begin: @return iterator, points to the fron to the snake.    */
    vector<Vertex>::const_iterator begin() const;
    /** end: @return iterator, points to the a cell following the snake's end.    */
    vector<Vertex>::const_iterator end() const;
};

#endif //SNAKE_H_