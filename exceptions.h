#include <exception>

class OutOfRange: public std::exception {};
class EmptySnake: public std::exception {};
class NoLivesLeft: public std::exception {};