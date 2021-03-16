#ifndef BOARD_H_
#define BOARD_H_

#include <vector>
#include <stdlib.h>
#include <ctime>
#include "vertex.h"
#include "snake.h"

using std::vector;

const int fruitsNum = 10;
enum placeholder: int8_t {EMPTY = 0, SNAKE, FRUIT};

class Board {
    int rows, cols;
    vector<vector<placeholder>> vec; // used as the game map
public:
    Board(const Snake& s): rows((height -1) / jump), cols((width -1) / jump), vec(cols, vector<placeholder>(rows, EMPTY)) {
        for (auto i = s.begin(); i != s.end(); ++i) {
            vec[i->getY()/rows][i->getX()/cols] = SNAKE;
	    }
        // Randomnly generate fruits
        srand((unsigned int)time(NULL));
        for (int i = 0; i < fruitsNum; ++i) {
            int x = rand() % cols;
            int y = rand() % rows;
            if (vec[x][y] != EMPTY) i--;
            else vec[x][y] = FRUIT;
        }
    }
    void removeSnake(const Snake &);
    int getRows() const;
    int getCols() const;
    vector<placeholder>& operator[](int index);
    vector<placeholder> operator[](int index) const;
    void update(const Vertex& point, const placeholder& value);
    vector<int> generateNewFruit();
    bool checkRowsRange(int index);
};

#endif //BOARD_H_