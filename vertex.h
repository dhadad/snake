#ifndef VERTEX_H_
#define VERTEX_H_

#include "assert.h"
#include "exceptions.h"

enum direction: int {NONE = 0, UP, DOWN, LEFT, RIGHT, EXIT};

/** CONSATANTS: */
const int height = 321;
const int width = 481;
const int jump = 8;

class Vertex {
    int x, y;
    static bool rangeCheck(int x, int y);
public:
    direction dir;
    Vertex(int x, int y, direction dir) try: x(x), y(y), dir(dir) {
        if (rangeCheck(x ,y)) throw OutOfRange();
    }
    catch(...) {
        throw;
    };
    Vertex(const Vertex& other): Vertex(other.x, other.y, other.dir) {};
    /** operator+=: adds the coardinates of a given vertex into those of the current
     *  vertex.
     * @param this a mutable Vertex.
     * @param other A reference to the vertex to be used in the addition.
     * @return A reference to the updated vertex.
    */
    Vertex& operator+=(const Vertex& other);
    /** getX: returns the value of the X coardinate.
     * @return Integer represting the X coardinate.
     */
    int getX() const;
    /** getY: returns the value of the Y coardinate.
     * @return Integer represting the Y coardinate.
     */
    int getY() const;
    /** step: change the coardinates by one step, in the direction mentioned in the
     *  'dir' member.
     * @param this a mutable Vertex.
     * @return A reference to the updated vertex.
    */
    Vertex& step();
    /** peekStep: returns a copy of the vertex to be recieved if 'step' method was to
     *  be used.
     * @return A copy of the wanted vertex.
    */
    Vertex peekStep() const;
    /** getDirectionFromChar: static function, useful for translating key press to a
     *  known direction.
     * @param c The character pressed by the user.
     * @return The direction defined for the given character.
    */
    static direction getDirectionFromChar(char c);
    /** oppositeDirections: static function, checks if 2 directions are exactly oppoiste
     *  to each other.
     * @param first direction.
     * @param second direction.
     * @return true if the given directions are indeed opposite, false otherwise.
    */
    static bool oppositeDirections(direction first, direction second);
};

#endif //VERTEX_H_