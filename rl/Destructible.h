#ifndef __DESTRUCTIBLE__
#define __DESTRUCTIBLE__

#include <string>

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

#endif
