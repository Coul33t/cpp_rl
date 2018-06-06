#ifndef __ACTOR__
#define __ACTOR__

#include <string>
#include <utility> 
#include "libtcod.hpp"

class Actor {
	private:
		int x, y;
		std::string name;
		int ch;
		TCODColor col;

	public:
		Actor(int x, int y, std::string name, int ch, TCODColor col);

		int getX() const;
		int getY() const;

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
