#include "Actor.h"

Actor::Actor(int x, int y, std::string name, int ch, TCODColor colour) :
	x(x), y(y), name(name), ch(ch), col(col) {

}

void Actor::render() const {
	TCODConsole::root->setChar(x, y, ch);
	TCODConsole::root->setCharForeground(x, y, col);
}

void Actor::move(int dx, int dy) {
	this->x += dx;
	this->y += dy;
}

int Actor::getX() const {
	return x;
}

int Actor::getY() const {
	return y;
}

void Actor::setX(int x) {
	this->x = x;
}

void Actor::setY(int y) {
	this->y = y;
}

std::pair<int, int> Actor::getCoordinates() {
	return std::pair<int, int>(x, y);
}
void Actor::setCoordinates(std::pair<int, int> new_coord) {
	x = new_coord.first;
	y = new_coord.second;
}

void Actor::attack(Actor* target) {

}

void Actor::update() {

}