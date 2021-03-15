#include <stdio.h>
#include <stdbool.h>
#include <string>
#include <stdexcept> 
#include <atomic>
#include <mutex>
#include <iostream>
#include <chrono>
#include <thread> 
#include "vertex.h"
#include "snake.h"
#include "board.h"

using std::thread;
using std::string;

extern "C" {
	#include "gfx/gfx.h"
}

std::mutex m;
std::atomic<bool> done(false);
enum color: int8_t {BLANK = 0, GREEN, WHITE};

void setColor(const color& c) {
	switch (c) {
		case GREEN: gfx_color(0,200,0);
			break;
		case WHITE: gfx_color(200,200,200);
			break;
		default: gfx_color(0,0,0); //BLACK
			break;
	}
}

void updatePrintedScore(const Snake& s, const color& c) {
	setColor(c);
	string score = std::to_string(s.getScore());
	gfx_text(score.c_str(), jump*11, height+1.5*jump);
	gfx_flush();
}

void updatePrintedLives(const Snake& s, const color& c) {
	setColor(c);
	string score = std::to_string(s.getLives());
	gfx_text(score.c_str(), jump*25, height+1.5*jump);
	gfx_flush();
}

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

void printSnake(const Snake& s, const color& c) {
	setColor(c);
	for (auto i = s.begin(); i != s.end(); ++i) {
		gfx_text("@", i->getX(), i->getY());
	}
	gfx_flush();
}

void closeGame(const char* message) {
	setColor(WHITE);
	gfx_text(message, width/3, height/2);
	gfx_text("Press q To Quit", width/3, height/2+jump*1.5);
	gfx_flush();
}

void advanceEverySec(Snake& s, Board& b) {
	while(!done.load()) {
		m.lock(); //???
		try {
			Vertex old_tail = s[s.length()-1]; //Uses copy c'tor
			Vertex new_head = s[0].peekStep();
			printSnake(s, BLANK);
			if (!b.checkRowsRange(new_head.getY() / jump)) throw OutOfRange();
			if (b[new_head.getX() / jump][new_head.getY() / jump] == FRUIT) {//Next step has a fruit
				gfx_text("P", new_head.getX(), new_head.getY()); //Removes the fruit from the map
				s.advance();
				b.update(new_head, SNAKE);
				updatePrintedScore(s, BLANK);
				s+=old_tail; //Increases by one
				updatePrintedScore(s, WHITE);
				vector<int> fruit_coardinates = b.generateNewFruit();
				setColor(WHITE);
				gfx_text("P", fruit_coardinates[0]*jump, fruit_coardinates[1]*jump);
			} else if (b[new_head.getX() / jump][new_head.getY() / jump] == SNAKE) {
				b.update(old_tail, EMPTY);
				s.advance();
				Vertex new_tail = s[s.length()-1];
				updatePrintedLives(s, BLANK);
				--s;
				updatePrintedLives(s, WHITE);
				b.update(new_tail, EMPTY);
				b.update(new_head, SNAKE);
			} else {
				b.update(old_tail, EMPTY);
				s.advance();
				b.update(new_head, SNAKE);
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
		std::this_thread::sleep_for(std::chrono::milliseconds(350));
		m.unlock();
	}
}

void waitForInput(Snake& s) {
	char c = ' ';
	while(Vertex::getDirectionFromChar(c) == NONE || Vertex::getDirectionFromChar(c) == s.getDirection() || \
			Vertex::oppositeDirections(Vertex::getDirectionFromChar(c), s.getDirection())) {
		c = gfx_wait();
		if (Vertex::getDirectionFromChar(c) == EXIT) {
			done=true;
			return;
		}
	}
	s.changeDirection(Vertex::getDirectionFromChar(c));
}

int main() {
	Snake s = Snake();
	Board b = Board(s);
	initBoard(s, b);
	thread advancing(advanceEverySec, std::ref(s), std::ref(b));
	advancing.detach();
	while (!done.load()) {
		thread get_input(waitForInput, std::ref(s));
		if (get_input.joinable())
			get_input.join();
	}
	gfx_close();
	return 0;
}
