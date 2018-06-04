#ifndef __ACTOR__
#define __ACTOR__
#include <utility> 
#include "libtcod.hpp"

class Actor {
	private:
		int x, y;
		int ch;
		TCODColor col;

	public:
		Actor(int x, int y, int ch, TCODColor col);
		void render() const;

		void move(int dx, int dy);

		int getX() const;
		int getY() const;

		void setX(int x);
		void setY(int y);

		std::pair<int, int> getCoordinates();
		void setCoordinates(std::pair<int, int>);

};

#endif // __ACTOR__