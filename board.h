#ifndef BOARD_H_
#define BOARD_H_

#include "vertex.h"
#include "snake.h"
#include <vector>
#include <stdlib.h>
#include <ctime>
using std::vector;

const int fruitsNum = 10;
typedef enum {SNAKE, FRUIT, EMPTY} placeholder;

class Board {
    int rows, cols;
    vector<vector<placeholder>> vec;
public:
    Board(Snake& s): rows((height -1) / jump), cols((width -1) / jump), vec(cols, vector<placeholder>(rows, EMPTY)) {
        for (vector<Vertex>::iterator i = s.begin(); i != s.end(); ++i) {
            vec[i->getY()/rows][i->getX()/cols] = SNAKE;
	    }
        srand((unsigned int)time(NULL));
        for (int i = 0; i < fruitsNum; ++i) {
            int x = rand() % cols;
            int y = rand() % rows;
            if (vec[x][y] != EMPTY) i--;
            else vec[x][y] = FRUIT;
        }
    }
    int getRows() const;
    int getCols() const;
    vector<placeholder>& operator[](int index);
    vector<placeholder> operator[](int index) const;
    void update(Vertex point, placeholder value);
    vector<int> generateNewFruit();
};

#endif //BOARD_H_