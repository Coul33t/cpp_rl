#ifndef __MAP__
#define __MAP__

#include <iostream>

#include <vector>

#include <stdlib.h>
#include <time.h>

#include "libtcod.hpp"

enum direction {NORTH = 0, EAST = 1, SOUTH = 2, WEST = 3};

struct Tile {
	int ch;
	bool can_walk;
	Tile() : can_walk(false), ch('#') {}
};

struct Room {
	int x1, y1, x2, y2;
};

class Map {
	public:
		Map();
		Map(int w, int h);
		~Map();

		std::pair<int, int> getRoomCenter(int room_idx);
		bool isWalkable(int x, int y) const;
		void render() const;

	protected:
		int width, height;
		std::vector<Tile*> tiles;
		std::vector<Room*> rooms;

		void setWall(int x, int y);
		void setFloor(int x, int y);
		bool roomIsClosed(Room*);
		bool wallHasDoor(Room*, direction direction);
		bool isIntersection(int x, int y, direction direcrion);
		void createDoorsRandom();
		void createDoorsAll();

		friend class BspListener;

		void dig(int x1, int x2, int y1, int y2);
		void createRoom(int x1, int x2, int y1, int y2);

		static const int ROOM_MAX_SIZE = 15;
		static const int ROOM_MIN_SIZE = 8;
};

#endif // __MAP__