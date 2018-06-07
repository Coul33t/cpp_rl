#include "Actor.h"

Actor::Actor(int x, int y, std::string name, int ch, TCODColor col) :
	x(x), y(y), name(name), ch(ch), col(col), blocks(true), is_dead(false) {

}

void Actor::render() const {
	TCODConsole::root->setChar(x, y, ch);
	TCODConsole::root->setCharForeground(x, y, col);
}

void Actor::move(int dx, int dy) {
	this->x += dx;
	this->y += dy;
}

std::pair<int, int> Actor::getCoordinates() {
	return std::pair<int, int>(x, y);
}
void Actor::setCoordinates(std::pair<int, int> new_coord) {
	x = new_coord.first;
	y = new_coord.second;
}

void Actor::attack(Actor* target) {
	std::cout << this->name << " attacks the " << target->name << "!" << std::endl;
}

void Actor::update() {
	//std::cout << "Actor " << this->name << " updated." << std::endl;
}