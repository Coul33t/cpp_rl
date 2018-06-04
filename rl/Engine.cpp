#include "Engine.h"

Engine::Engine() {
	TCODConsole::setCustomFont("Anikki_square_16x16.png", TCOD_FONT_LAYOUT_ASCII_INROW);
	TCODConsole::initRoot(80, 50, "libtcod C++ tutorial", false);
	player = new Actor(0, 0, '@', TCODColor::white);
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

void Engine::update() {
	TCOD_key_t key;
	TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL);

	switch (key.vk) {
		case TCODK_KP8:
			if (map->isWalkable(player->getX(), player->getY() - 1)) {
				player->move(0, -1);
				compute_fov = true;
			}
			break;

		case TCODK_KP9:
			if (map->isWalkable(player->getX() + 1, player->getY() - 1)) {
				player->move(1, -1);
				compute_fov = true;
			}
			break;

		case TCODK_KP6:
			if (map->isWalkable(player->getX() + 1, player->getY())) {
				player->move(1, 0);
				compute_fov = true;
			}
			break;

		case TCODK_KP3:
			if (map->isWalkable(player->getX() + 1, player->getY() + 1)) {
				player->move(1, 1);
				compute_fov = true;
			}
			break;
		
		case TCODK_KP2:
			if (map->isWalkable(player->getX(), player->getY() + 1)) {
				player->move(0, 1);
				compute_fov = true;
			}
			break;

		case TCODK_KP1:
			if (map->isWalkable(player->getX() - 1, player->getY() + 1)) {
				player->move(-1, 1);
				compute_fov = true;
			}
			break;
		
		case TCODK_KP4:
			if (map->isWalkable(player->getX() - 1, player->getY())) {
				player->move(-1, 0);
				compute_fov = true;
			}
			break;

		case TCODK_KP7:
			if (map->isWalkable(player->getX() - 1, player->getY() - 1)) {
				player->move(-1, -1);
				compute_fov = true;
			}
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