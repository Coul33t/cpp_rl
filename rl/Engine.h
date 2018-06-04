#ifndef __ENGINE__
#define __ENGINE__

#include <iostream>
#include "libtcod.hpp"
#include "Actor.h"
#include "Map.h"

class Engine {
	public:
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
		bool compute_fov;

		static const int CLUSTER_THRESHOLD = 20;
};

#endif // __ENGINE__