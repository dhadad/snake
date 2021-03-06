#include <string>
#include <atomic>
#include <mutex>
#include <chrono>
#include <thread> 
#include "vertex.h"
#include "snake.h"
#include "board.h"

extern "C" {
	#include "gfx/gfx.h"
}

using std::thread;
using std::string;

/* GLOBALS */

std::mutex m;
std::atomic<bool> done(false);
static const int init_speed = 350; //milliseconds
static const double speed_increase = 0.95;

/* AUXILIARIES */

enum color: int8_t {BLANK = 0, GREEN, WHITE};

/**
 * setColor: choose the color of the next item to be drawn to the screen.
 */
void setColor(const color& c) {
	switch (c) {
		case GREEN: gfx_color(0,200,0);
			break;
		case WHITE: gfx_color(200,200,200);
			break;
		default: gfx_color(0,0,0); // Black
			break;
	}
}

/**
 * updatePrintedScore: prints the current score.
 * @param s A reference to the snake object.
 * @param c Color to be used in printing the text.
 */
void updatePrintedScore(const Snake& s, const color& c) {
	setColor(c);
	string score = std::to_string(s.getScore());
	gfx_text(score.c_str(), jump*11, height+1.5*jump);
	gfx_flush();
}

/**
 * updatePrintedLives: prints the current number of lives left.
 * @param s A reference to the snake object.
 * @param c Color to be used in printing the text.
 */
void updatePrintedLives(const Snake& s, const color& c) {
	setColor(c);
	string score = std::to_string(s.getLives());
	gfx_text(score.c_str(), jump*25, height+1.5*jump);
	gfx_flush();
}

/**
 * printSnake: prints the location of the vertices the snake consists of.
 * '@' will be used to identify the snake.
 * @param s A reference to the snake object.
 * @param c Color to be used in printing the text.
 */
void printSnake(const Snake& s, const color& c) {
	setColor(c);
	for (auto i = s.begin(); i != s.end(); ++i) {
		gfx_text("@", i->getX(), i->getY());
	}
	gfx_flush();
}

/**
 * initBoard: initialize the game window.
 * @param s A reference to the snake object.
 * @param b A reference to the board object.
 */
void initBoard(const Snake& s, const Board& b) {
	gfx_open(width,height+jump*2,"Snake");
	setColor(WHITE);
	gfx_line(0, height, width, height);
	gfx_text("SCORE:", jump*4, height+1.5*jump);
	gfx_text("LIVES:", jump*18, height+1.5*jump);
	updatePrintedScore(s, WHITE);
	updatePrintedLives(s, WHITE);
	for (int x = 0; x < b.getCols(); ++x)
		for(int y = 0; y < b.getRows(); ++y)
			if (b[x][y] == FRUIT)
				gfx_text("P", x*jump, y*jump);
}

/**
 * closeGame: prints a final message.
 * @param message String to appear when the game ends.
 */
void closeGame(const char* message) {
	setColor(WHITE);
	gfx_text(message, width/3, height/2);
	gfx_text("Press q To Quit", width/3, height/2+jump*1.5);
	gfx_flush();
}

/**
 * snakeEat: the snake steps into a fruit. As a result, the score is updated.
 * @param s A reference to the snake object.
 * @param b A reference to the board object.
 * @param old_tail A reference to the vertex where the snake's old tail is located.
 * @param new_head A refrence to the vertex where the snake's head is going to be
 * after the current step.
 */
void snakeEat(Snake& s, Board& b, const Vertex& old_tail, const Vertex& new_head) {
	/** Remove the fruit from the map */
	gfx_text("P", new_head.getX(), new_head.getY()); 
	/** Move the snake */
	s.advance();
	b.update(new_head, SNAKE);
	/** Update score and the snake length */
	updatePrintedScore(s, BLANK);
	s+=old_tail; 
	updatePrintedScore(s, WHITE);
	/** Genereate a new fruit */
	vector<int> fruit_coardinates = b.generateNewFruit();
	setColor(WHITE);
	gfx_text("P", fruit_coardinates[0]*jump, fruit_coardinates[1]*jump);
}

/**
 * snakeSelfCross: the snake steps into itself. The snake will start over from 
 * the default location, with size 1.
 * @param s A reference to the snake object.
 * @param b A reference to the board object.
 */
void snakeSelfCross(Snake& s, Board& b) {
	updatePrintedLives(s, BLANK);
	b.removeSnake(s);
	s.reset(); // snake of length 1
	--s; // update lives
	updatePrintedLives(s, WHITE);
	int coar_x = s[0].getX() / jump, coar_y = s[0].getY() / jump;
	while (b[coar_x][coar_y] == FRUIT) {
		// if a fruit exists in location, generate a replacement
		setColor(BLANK);
		gfx_text("P", s[0].getX(), s[0].getY()); 
		b.update(s[0], EMPTY);
		vector<int> fruit_coardinates = b.generateNewFruit();
		setColor(WHITE);
		gfx_text("P", fruit_coardinates[0]*jump, fruit_coardinates[1]*jump);
	}
	b.update(s[0], SNAKE);
}

/**
 * snakeStep: the snake steps into a empty place.
 * @param s A reference to the snake object.
 * @param b A reference to the board object.
 * @param old_tail A reference to the vertex where the snake's old tail is located.
 * @param new_head A refrence to the vertex where the snake's head is going to be
 * after the current step.
 */
void snakeStep(Snake& s, Board& b, const Vertex& old_tail, const Vertex& new_head) {
	b.update(old_tail, EMPTY);
	s.advance();
	b.update(new_head, SNAKE);
}

/**
 * advanceEverySec: make a step every contant amount of time.
 * Stops when there are no lives left, or when the snakes exits the screen.
 * @param s A reference to the snake object.
 * @param b A reference to the board object.
 * @param speed Integer represting the number of milliseconds between each step.
 */
void advanceEverySec(Snake& s, Board& b, int speed) {
	while(!done.load()) {
		m.lock();
		try {
			Vertex old_tail = s[s.length()-1]; 
			Vertex new_head = s[0].peekStep();
			int coar_x = new_head.getX() / jump, coar_y = new_head.getY() / jump;
			if (!b.checkRowsRange(coar_y)) 
				throw OutOfRange();
			printSnake(s, BLANK);
			if (b[coar_x][coar_y] == FRUIT) {
				snakeEat(s, b, old_tail, new_head);	// Fruit in the next step
				speed *= speed_increase;
			} else if (b[coar_x][coar_y] == SNAKE) {
				snakeSelfCross(s, b); // The snake crosses itself
				speed = init_speed;
			} else {
				snakeStep(s, b, old_tail, new_head); // A regular step
			}
			printSnake(s, GREEN);
		} catch(const OutOfRange& e ) {
			closeGame("OUT OF RANGE!");
			m.unlock();
			break;
		} catch(const NoLivesLeft& e) {
			closeGame("NO LIVES LEFT!");
			m.unlock();
			break;
		} catch(...) {
			closeGame("ERROR");
			m.unlock();
			break;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(speed));
		m.unlock();
	}
}

/**
 * waitForInput: reads charaters entered by the user.
 * 'w' is used to move up, 's' to move down, 'a' for left and 'd' for right. 
 * 'q' is used in order to exit the program.
 * @param s A reference to the snake object.
 */
void waitForInput(Snake& s) {
	char c = ' ';
	while(Vertex::getDirectionFromChar(c) == NONE || \
			Vertex::getDirectionFromChar(c) == s.getDirection() || \
			Vertex::oppositeDirections(Vertex::getDirectionFromChar(c), s.getDirection())) {
		c = gfx_wait();
		if (Vertex::getDirectionFromChar(c) == EXIT) {
			done = true;
			return;
		}
	}
	s.changeDirection(Vertex::getDirectionFromChar(c));
}

int main() {
	Snake s = Snake();
	Board b = Board(s);
	initBoard(s, b);
	thread advancing(advanceEverySec, std::ref(s), std::ref(b), init_speed);
	advancing.detach();
	while (!done.load()) {
		thread get_input(waitForInput, std::ref(s));
		if (get_input.joinable()) {
			get_input.join();
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(init_speed));
	}
	gfx_close();
	return 0;
}
