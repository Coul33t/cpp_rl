#ifndef __MAP__
#define __MAP__

#include <iostream>

#include <vector>

#include <stdlib.h>
#include <time.h>

struct Tile {
	int ch;
	bool can_walk;
	bool explored;
	Tile() : can_walk(false), ch('#'), explored(false) {}
};

struct Room {
	int x1, y1, x2, y2;
};

class Map {
	public:
		Map(int w, int h);
		~Map();

		std::vector<Room*>& getRooms();
		std::pair<int, int> getRoomCenter(int room_idx);
		void computeFov(int p_x, int p_y, int radius);
		bool isInFov(int x, int y) const;
		bool isWalkable(int x, int y) const;
		void render() const;

	protected:
		enum direction { NORTH = 0, EAST = 1, SOUTH = 2, WEST = 3 };

		int width, height;
		std::vector<Tile*> tiles;
		std::vector<Room*> rooms;
		TCODMap *t_map;

		void setWall(int x, int y);
		void setFloor(int x, int y);

		bool roomIsClosed(Room*) const;
		bool wallHasDoor(Room*, direction direction) const;
		bool isIntersection(int x, int y, direction direction) const;
	
		bool isExplored(int x, int y) const;

		friend class BspListener;

		void dig(int x1, int x2, int y1, int y2);
		void createRoom(int x1, int x2, int y1, int y2);

		void createDoorsRandom();
		void createDoorsAll();

		static const int ROOM_MAX_SIZE = 12;
		static const int ROOM_MIN_SIZE = 7;
};

#endif // __MAP__