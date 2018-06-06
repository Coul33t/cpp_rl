#include "Main.h"

Destructible::Destructible(int max_hp, int hp, int defense, std::string c_name):
	max_hp(max_hp), hp(hp), defense(defense), corpse_name(c_name){
}

Destructible::~Destructible() {

}

bool Destructible::isDead() {
	return (hp >= 0);
}

int Destructible::takeDamage(Actor* owner, int dmg) {
	dmg -= defense;

	if (dmg > 0) {
		hp -= dmg;
		if (hp <= 0)
			die(owner);
	}

	else {
		dmg = 0;
	}

	return dmg;
}

void Destructible::die(Actor* owner) {
	owner->ch = '%';
	owner->col = TCODColor::darkRed;
	owner->name = corpse_name;
	owner->blocks = false;
	owner->is_dead = true;
}