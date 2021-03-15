#include <stdio.h>
#include <stdbool.h>
#include <string>
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
typedef enum {GREEN, BLANK, WHITE} color;
Snake s = Snake();
Board b = Board(s);


void setColor(color c) {
	switch (c) {
		case GREEN: gfx_color(0,200,0);
			break;
		case WHITE: gfx_color(200,200,200);
			break;
		default: gfx_color(0,0,0);
			break;
	}
}

void updatePrintedScore(color c) {
	setColor(c);
	string score = std::to_string(s.getScore());
	gfx_text(score.c_str(), jump*11, height+1.5*jump);
	gfx_flush();
}

void initBoard() {
	gfx_open(width,height+jump*2,"Snake");
	setColor(WHITE);
	gfx_line(0, height, width, height);
	gfx_text("SCORE:", jump*4, height+1.5*jump);
	updatePrintedScore(WHITE);
	for (int i = 0; i < b.getRows(); ++i)
		for(int j = 0; j < b.getCols(); ++j)
			if (b[i][j] == FRUIT)
				gfx_text("P", i*jump, j*jump);
}



void printSnake(color c) {
	setColor(c);
	for (vector<Vertex>::iterator i = s.begin(); i != s.end(); ++i) {
		gfx_text("@", i->getX(), i->getY());
	}
	gfx_flush();
}

void advanceEverySec() {
	while(!done.load()) {
		m.lock(); //???
		try {
			Vertex old_tail = s[s.length()-1]; //Uses copy c'tor
			Vertex new_head = s[0].peekStep();
			printSnake(BLANK);
			if (b[new_head.getX() / jump][new_head.getY() / jump] == FRUIT) {//Next step has a fruit
				gfx_text("P", new_head.getX(), new_head.getY()); //Removes the fruit from the map
				s.advance();
				b.update(new_head, SNAKE);
				updatePrintedScore(BLANK);
				s+=old_tail; //Increases by one
				updatePrintedScore(WHITE);
				vector<int> fruit_coardinates = b.generateNewFruit();
				setColor(WHITE);
				gfx_text("P", fruit_coardinates[0]*jump, fruit_coardinates[1]*jump);
			} else {
				b.update(old_tail, EMPTY);
				s.advance();
				b.update(new_head, SNAKE);
			}
			printSnake(GREEN);
		} catch(OutOfRange& e) {
			throw;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(350));
		m.unlock();
	}
}

void waitForInput() {
	char c = ' ';
	while(Vertex::getDirectionFromChar(c) == NONE || Vertex::getDirectionFromChar(c) == s.getDirection() || \
			Vertex::oppositeDirections(Vertex::getDirectionFromChar(c), s.getDirection())) {
		c = gfx_wait();
	}
	s.changeDirection(Vertex::getDirectionFromChar(c));
	done = true;
}

int main() {
	initBoard();
	thread advancing(advanceEverySec);
	advancing.detach();
	while (1) {
		thread get_input(waitForInput);
		if (get_input.joinable())
			get_input.join();
		done = false;
	}
	return 0;
}
