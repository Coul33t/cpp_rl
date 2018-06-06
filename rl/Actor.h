#ifndef __ACTOR__
#define __ACTOR__

#include <string>
#include <utility> 
#include <iostream>
#include "libtcod.hpp"

class Actor {
	private:
		int x, y;
		std::string name;
		int ch;
		TCODColor col;

	public:
		Actor(int x, int y, std::string name, int ch, TCODColor col);

		const int getX() const;
		const int getY() const;
		const std::string& getName() const;

		void setX(int x);
		void setY(int y);

		std::pair<int, int> getCoordinates();
		void setCoordinates(std::pair<int, int>);

		void move(int dx, int dy);
		void attack(Actor* target);

		void render() const;
		void update();
		

};

#endif // __ACTOR__
