#pragma once

#include <string>

#include "libtcod.hpp"

#include "Actor.h"

class Actor;

class Destructible {
	public:
		int max_hp;
		int hp;
		int defense;
		std::string corpse_name;

	
	public:
		Destructible(int max_hp, int hp, int defense, std::string c_name);
		~Destructible();

		bool isDead();
		int takeDamage(Actor* owner, int dmg);
		void die(Actor* owner);

	private:
};
