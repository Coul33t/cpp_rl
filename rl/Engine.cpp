#include "Engine.h"

Engine::Engine(): game_status(STARTUP) {
	TCODConsole::setCustomFont("Anikki_square_16x16.png", TCOD_FONT_LAYOUT_ASCII_INROW);
	TCODConsole::initRoot(80, 50, "libtcod C++ tutorial", false);
	
	map = new Map(70, 45);

	player = new Actor(0, 0, "Player", '@', TCODColor::white);
	player->setCoordinates(map->getRoomCenter(0));
	actors.push_back(player);

	fov_radius = 20;

	populateDungeon();
}

Engine::~Engine() {
	for (auto it = actors.begin(); it != actors.end(); it++)
		delete *it;
	delete map;
}

Actor* Engine::getPlayer() const {
	return player;
}

void Engine::addMonster(int x, int y) {
	actors.push_back(new Actor(x, y, "Vermin", 'v', TCODColor::brass));
}

void Engine::populateDungeon() {
	srand((unsigned int)time(NULL));

	for (auto it = map->getRooms().begin(); it != map->getRooms().end(); it++) {
		int nb_monster = rand() % (((*it)->y2 - (*it)->y1) * ((*it)->x2 - (*it)->x1)) / 32;

		int cluster_chance = rand() % 100;

		// Here comes the HORDE
		if (cluster_chance < 5)
			nb_monster *= 3;

		for (int i = 0; i < nb_monster; i++) {
			int x = (*it)->x1 + rand() % ((*it)->x2 - (*it)->x1);
			int y = (*it)->y1 + rand() % ((*it)->y2 - (*it)->y1);
			while (!canWalk(x, y)) {
				x = (*it)->x1 + rand() % ((*it)->x2 - (*it)->x1);
				y = (*it)->y1 + rand() % ((*it)->y2 - (*it)->y1);
			}

			addMonster(x, y);
		}
	}

	if (actors.size() < CLUSTER_THRESHOLD) {
		int rn = rand() % map->getRooms().size();

		for (int i = 0; i < 20; i++) {
			int x = map->getRooms()[rn]->x1 + rand() % (map->getRooms()[rn]->x2 - map->getRooms()[rn]->x1);
			int y = map->getRooms()[rn]->y1 + rand() % (map->getRooms()[rn]->y2 - map->getRooms()[rn]->y1);
			while (!canWalk(x, y)) {
				x = map->getRooms()[rn]->x1 + rand() % (map->getRooms()[rn]->x2 - map->getRooms()[rn]->x1);
				y = map->getRooms()[rn]->y1 + rand() % (map->getRooms()[rn]->y2 - map->getRooms()[rn]->y1);
			}

			addMonster(x, y);
		}
	}
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
	if (canWalk(actor->getX() + dx, actor->getY() + dy)) {
		actor->move(dx, dy);
		return true;
	}

	else {
		for (auto it = actors.begin(); it != actors.end(); it++) {
			if (((*it) != player) && ((*it)->getX() == (actor->getX() + dx) || (*it)->getY() == (actor->getY() + dy))) {
				actor->attack((*it));
				return true;
			}
		}
	}

	return false;
}

void Engine::update() {

	if (game_status == STARTUP)
		map->computeFov(player->getX(), player->getY(), fov_radius);

	game_status = IDLE;

	TCOD_key_t key;
	TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL);

	int dx = 0, dy = 0;

	switch (key.vk) {
		case TCODK_KP8:
			dy = -1;
			break;

		case TCODK_KP9:
			dx = 1;
			dy = -1;
			break;

		case TCODK_KP6:
			dx = 1;
			break;

		case TCODK_KP3:
			dx = 1;
			dy = 1;
			break;
		
		case TCODK_KP2:
			dy = 1;
			break;

		case TCODK_KP1:
			dx = -1;
			dy = 1;
			break;
		
		case TCODK_KP4:
			dx = -1;
			break;

		case TCODK_KP7:
			dx = -1;
			dy = -1;
			break;

		case TCODK_F3:
			delete map;
			map = new Map(70, 45);
			player->setCoordinates(map->getRoomCenter(0));
			break;

		default:
			break;
	}

	if (dx != 0 || dy != 0) {
		game_status = NEW_TURN;
		if (move(player, dx, dy))
			map->computeFov(player->getX(), player->getY(), fov_radius);
	}

	if (game_status == NEW_TURN) {
		for (auto it = actors.begin(); it != actors.end(); it++)
			if ((*it) != player)
				(*it)->update();
	}
}

void Engine::render() {
	TCODConsole::root->clear();
	map->render();

	for (auto it = actors.begin(); it != actors.end(); it++)
		if (map->isInFov((*it)->getX(), (*it)->getY()))
			(*it)->render();
}
