#include "Engine.h"

Engine::Engine() {
	TCODConsole::setCustomFont("Anikki_square_16x16.png", TCOD_FONT_LAYOUT_ASCII_INROW);
	TCODConsole::initRoot(80, 50, "libtcod C++ tutorial", false);
	player = new Actor(0, 0, "Player", '@', TCODColor::white);
	actors.push_back(player);
	map = new Map(70, 45);
	player->setCoordinates(map->getRoomCenter(0));

	fov_radius = 20;
	compute_fov = true;
	map->computeFov(player->getX(), player->getY(), fov_radius);
}

Engine::~Engine() {
	for (auto it = actors.begin(); it != actors.end(); it++)
		delete *it;
	delete map;
}

Actor* Engine::getPlayer() const {
	return player;
}

bool Engine::canWalk(int x, int y) const {
	if (!map->isWalkable(x, y))
		return false;

	for (auto it = actors.begin(); it != actors.end(); it++)
		if ((*it)->getX() == x && (*it)->getY() == y)
			return false;

	return true;
}

bool Engine::move(Actor* actor, int dx, int dy) {
	if (map->isWalkable(actor->getX() + dx, actor->getY() + dy)) {
		actor->move(dx, dy);
		return true;
	}

	else {
		for (auto it = actors.begin(); it != actors.end(); it++) {
			if ((*it)->getX() == (actor->getX() + dx) || (*it)->getY() == (actor->getY() + dy)) {
				actor->attack((*it));
				return true;
			}
		}
	}

	return false;
}

void Engine::update() {
	TCOD_key_t key;
	TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL);

	switch (key.vk) {
		case TCODK_KP8:
			move(player, 0, -1);
			compute_fov = true;
			break;

		case TCODK_KP9:
			move(player, 1, -1);
			compute_fov = true;
			break;

		case TCODK_KP6:
			move(player, 1, 0);
			compute_fov = true;
			break;

		case TCODK_KP3:
			move(player, 1, 1);
			compute_fov = true;
			break;
		
		case TCODK_KP2:
			move(player, 0, 1);
			compute_fov = true;
			break;

		case TCODK_KP1:
			move(player, -1, 1);
			compute_fov = true;
			break;
		
		case TCODK_KP4:
			move(player, -1, 0);
			compute_fov = true;
			break;

		case TCODK_KP7:
			move(player, -1, -1);
			compute_fov = true;
			break;

		case TCODK_F3:
			delete map;
			map = new Map(70, 45);
			player->setCoordinates(map->getRoomCenter(0));
			break;

		default:
			break;
	}

	if (compute_fov) {
		map->computeFov(player->getX(), player->getY(), fov_radius);
		compute_fov = false;
	}
}

void Engine::render() {
	TCODConsole::root->clear();
	map->render();

	for (auto it = actors.begin(); it != actors.end(); it++)
		if (map->isInFov((*it)->getX(), (*it)->getY()))
			(*it)->render();
}