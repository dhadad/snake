#include "board.h"

int Board::getRows() const {
    return rows;
}

int Board::getCols() const {
    return cols;
}

vector<placeholder>& Board::operator[](int index) {
    if (index < 0 || index >= cols) 
        throw OutOfRange();
    return vec[index];
}

vector<placeholder> Board::operator[](int index) const {
    if (index < 0 || index >= cols) 
        throw OutOfRange();
    return vec[index];
}

void Board::update(const Vertex& point, const placeholder& value) {
    int x = point.getX() / jump, y = point.getY() / jump;
    vec[x][y] = value;
}

vector<int> Board::generateNewFruit() {
    int x = rand() % cols, y = rand() % rows;
    while (vec[x][y] != placeholder::EMPTY) {
        x = rand() % cols, y = rand() % rows;
    }
    vec[x][y] = FRUIT;
    vector<int> coardinates {x, y};
    return coardinates;
}

bool Board::checkRowsRange(int index) {
    return index < rows && index >= 0;
}