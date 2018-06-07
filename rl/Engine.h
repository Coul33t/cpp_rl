#pragma once

#include <iostream>
#include <algorithm>
#include <vector>

#include "libtcod.hpp"

#include "Actor.h"
#include "Map.h"

class Engine {
	public:
		
		enum GameStatus {
			STARTUP,
			IDLE,
			NEW_TURN,
			VICTORY,
			DEFEAT
		} game_status;

		Engine();
		~Engine();

		Actor* getPlayer() const;

		void update();
		void render();

	private:
		void populateDungeon();
		void addMonster(int x, int y);

		bool canWalk(int x, int y) const;

		std::vector<Actor*> actors;
		Actor* player;
		Map* map;

		int fov_radius;

		bool move(Actor*, int dx, int dy);

		void sendToBack(Actor* actor);

		static const int CLUSTER_THRESHOLD = 20;
};
