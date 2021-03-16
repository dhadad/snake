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
    /** removeSnake: given a snake, remove its location from the board.
     * @param s A reference to a snake object.
     */
    void removeSnake(const Snake& s);
    /** getRows: how many rows the board consists of.
     * @return Integer represting number of rows.
     */
    int getRows() const;
    /** getCols: how many coloumns the board consists of.
     * @return Integer represting number of coloumns.
     */
    int getCols() const;
    /** operator[]: used in order to access a specific coloumn.
     * @param this mutable board.
     * @param index integer (must be in the valid range for a coloumn in the board).
     * @return A reference to the vector used to store the information regarding 
     * the wanted coloumn in the board.
     */
    vector<placeholder>& operator[](int index);
    /** operator[]: used in order to access a specific coloumn.
     * @param this constant board.
     * @param index integer (must be in the valid range for a coloumn in the board).
     * @return A copy of the vector used to store the information regarding 
     * the wanted coloumn in the board.
     */
    vector<placeholder> operator[](int index) const;
    /** update: change the value in a specific cell
     * @param this mutable board.
     * @param point a reference to a vertex, which stores the relevant coardinates.
     * @param value the value to replace the current one.
     */
    void update(const Vertex& point, const placeholder& value);
    /** generateNewFruit: randomly place a new fruit in the board.
     * @return vector of 2 integers, the x, y coardinates of the new fruit.
     */
    vector<int> generateNewFruit();
    /** checkRowsRange: checks if a given integer is a valid row number.
     * @param index integer to be used to access a specific row
     * @return true if it is indeed a valid row, false otherwise.
     */
    bool checkRowsRange(int index);
};

#endif //BOARD_H_