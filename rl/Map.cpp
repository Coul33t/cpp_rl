#include "Map.h"

class BspListener : public ITCODBspCallback {
	private:
		Map & map; // a map to dig
		int roomNum; // room number
		int lastx, lasty; // center of the last room
	public:
		BspListener(Map &map) : map(map), roomNum(0) {}
		bool visitNode(TCODBsp *node, void *userData) {
			if (node->isLeaf()) {
				int x, y, w, h;

				w = node->w;
				h = node->h;
				x = node->x + 1;
				y = node->y + 1;
				if (x + w >= map.width)
					w -= 1;
				if (y + h >= map.height)
					h -= 1;

				map.createRoom(x, y, x + w - 1, y + h - 1);
			}
			return true;
		}
};

Map::Map() : width(50), height(50) {
	for (auto i = 0; i < width*height; i++)
		tiles.push_back(new Tile());
	TCODBsp bsp(0, 0, 50, 50);
	bsp.splitRecursive(NULL, 5, ROOM_MIN_SIZE, ROOM_MAX_SIZE, 1.5f, 1.5f);
	BspListener listener(*this);
	bsp.traverseInvertedLevelOrder(&listener, NULL);
	createDoors();
}

Map::Map(int w, int h) : width(w), height(h) {
	for (auto i = 0; i < width*height; i++)
		tiles.push_back(new Tile());
	TCODBsp bsp(0, 0, width, height);
	bsp.splitRecursive(NULL, 5, ROOM_MIN_SIZE, ROOM_MAX_SIZE, 1.5f, 1.5f);
	BspListener listener(*this);
	bsp.traverseInvertedLevelOrder(&listener, NULL);
	createDoors();
}
Map::~Map() {

}

bool Map::isWalkable(int x, int y) const {
	return !tiles[x + y * width]->can_walk;
}

void Map::setWall(int x, int y) {
	tiles[x + y * width]->can_walk = false;
	tiles[x + y * width]->ch = '#';
}

void Map::setFloor(int x, int y) {
	tiles[x + y * width]->can_walk = true;
	tiles[x + y * width]->ch = '.';
}

void Map::dig(int x1, int y1, int x2, int y2) {
	if (x1 > x2) {
		int tmp = x1;
		x1 = x2;
		x2 = tmp;
	}

	if (y1 > y2) {
		int tmp = y1;
		y1 = y2;
		y2 = tmp;
	}

	for (int x = x1; x < x2; x++)
		for (int y = y1; y < y2; y++)
			setFloor(x, y);
}

void Map::createRoom(int x1, int y1, int x2, int y2) {
	dig(x1, y1, x2, y2);
	rooms.push_back(new Room{ x1, y1, x2, y2 });
}

bool Map::roomIsClosed(Room* room) {
	for (auto x = room->x1; x < room->x2; x++)
		if (tiles[x + (room->y1 - 1) * width]->ch == '.' || tiles[x + (room->y2) * width]->ch == '.')
			return false;
	for (auto y = room->y1; y < room->y2; y++)
		if (tiles[(room->x1 - 1) + y * width]->ch == '.' || tiles[(room->x2) + y * width]->ch == '.')
			return false;
	return true;
}

std::pair<int, int> Map::getRoomCenter(int room_idx) {
	return std::pair<int, int>((rooms[room_idx]->x1 + rooms[room_idx]->x2) / 2,
							   (rooms[room_idx]->y1 + rooms[room_idx]->y2) / 2);
}

void Map::createDoors() {
	srand(time(NULL));

	float chance;
	int pos;

	for (auto it = rooms.begin(); it != rooms.end(); it++) {
		if (roomIsClosed(*it)) {
			for (auto i = 0; i < 4; i++) {
				int chance = rand() % 100 + 1;

				if (chance <= (1 / (i + 1)) * 100) {
					
					if (i % 2 == 0)
						pos = (*it)->y1 + rand() % (((*it)->y2 + 1) - (*it)->y1);
					else
						pos = (*it)->x1 + rand() % (((*it)->x2 + 1) - (*it)->x1);

					switch (i) {
						case 0:
							if ((*it)->y1 > 0) {
								tiles[pos + (*it)->y1 * width]->can_walk = true;
								tiles[pos + (*it)->y1 * width]->ch = '.';
							}	
						case 1:
							if ((*it)->x2 < width - 1) {
								tiles[(*it)->x2 + pos * width]->can_walk = true;
								tiles[(*it)->x2 + pos * width]->ch = '.';
							}
							
						case 2:
							if ((*it)->y2 > height - 1) {
								tiles[pos + (*it)->y2 * width]->can_walk = true;
								tiles[pos + (*it)->y2 * width]->ch = '.';
							}
						case 3:
							if ((*it)->x1 > 0) {
								tiles[(*it)->x1 + pos * width]->can_walk = true;
								tiles[(*it)->x1 + pos * width]->ch = '.';
							}
					}
				}
			}
		}
	}
}

void Map::render() const {
	static const TCODColor darkWall(150, 150, 200);
	static const TCODColor darkGround(50, 50, 150);

	for (unsigned int x = 0; x < width; x++) {
		for (unsigned int y = 0; y < height; y++) {
			TCODConsole::root->setChar(x, y, tiles[x + y * width]->ch);
			if (tiles[x + y * width]->ch == '#')
				TCODConsole::root->setCharForeground(x, y, darkWall);
			else
				TCODConsole::root->setCharForeground(x, y, darkGround);
		}
	}	
}