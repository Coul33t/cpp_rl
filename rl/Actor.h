#ifndef __ACTOR__
#define __ACTOR__

#include <string>
#include <utility> 
#include <iostream>

class Actor {
	// Not gonna lie, having accessors that does nothing else than returning references
	// to the variables or modifying them without any form of check is pointless to me. 
	// My code isn't to be used by someone else than me, and I (kinda) know what I'm doing
	// (and what I can do) with my objects. Bloating my code with player->getX() instead of 
	// player->x is boring and tedious.
	public:
		int x, y;
		std::string name;
		int ch;
		TCODColor col;
		bool blocks;
		bool is_dead;

		Destructible* destructible;

	public:
		Actor(int x, int y, std::string name, int ch, TCODColor col);

		std::pair<int, int> getCoordinates();
		void setCoordinates(std::pair<int, int>);

		void move(int dx, int dy);
		void attack(Actor* target);

		void render() const;
		void update();
		

};

#endif // __ACTOR__
