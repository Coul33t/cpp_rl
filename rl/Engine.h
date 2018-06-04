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
		std::vector<Actor*> actors;
		Actor* player;
		Map* map;

		int fov_radius;
		bool compute_fov;
};

#endif // __ENGINE__