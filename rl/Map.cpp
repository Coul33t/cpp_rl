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
					w = map.width - x;
				if (y + h >= map.height)
					h = map.height - y;

				map.createRoom(x, y, x + w - 1, y + h - 1);
			}
			return true;
		}
};

Map::Map(int w, int h) : width(w), height(h) {

	for (auto i = 0; i < width*height; i++)
		tiles.push_back(new Tile());

	t_map = new TCODMap(width, height);

	TCODBsp bsp(0, 0, width, height);
	bsp.splitRecursive(NULL, 5, ROOM_MIN_SIZE, ROOM_MAX_SIZE, 1.1f, 1.1f);
	BspListener listener(*this);
	bsp.traverseInvertedLevelOrder(&listener, NULL);

	createDoorsAll();
}

Map::~Map() {
	for (auto it = tiles.begin(); it != tiles.end(); it++)
		delete (*it);

	for (auto it = rooms.begin(); it != rooms.end(); it++)
		delete (*it);

	delete t_map;
}

std::vector<Room*>& Map::getRooms() {
	return rooms;
}

bool Map::isWalkable(int x, int y) const {
	return tiles[x + y * width]->can_walk;
}

bool Map::isExplored(int x, int y) const {
	return tiles[x + y * width]->explored;
}

bool Map::isInFov(int x, int y) const {
	if (t_map->isInFov(x, y)) {
		tiles[x + y * width]->explored = true;
		return true;
	}
	return false;
}

void Map::computeFov(int p_x, int p_y, int radius) {
	t_map->computeFov(p_x, p_y, radius);
}

void Map::setWall(int x, int y) {
	tiles[x + y * width]->can_walk = false;
	tiles[x + y * width]->ch = '#';
	t_map->setProperties(x, y, false, false);
}

void Map::setFloor(int x, int y) {
	tiles[x + y * width]->can_walk = true;
	tiles[x + y * width]->ch = '.';
	t_map->setProperties(x, y, true, true);
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
		for (int y = y1; y < y2; y++) {
			setFloor(x, y);
			t_map->setProperties(x, y, true, true);
		}
			
}

void Map::createRoom(int x1, int y1, int x2, int y2) {
	dig(x1, y1, x2, y2);
	//TODO: figure out why the -1 is necessary here
	// EDIT: Maybe (probably) because BSP splits rectangles,
	// so each rectangle is adjacent (e.g. (1,10), (11,20))
	// but each room has ONE more tile between them
	rooms.push_back(new Room{ x1, y1, x2-1, y2-1 });
}

bool Map::roomIsClosed(Room* room) const {
	// North and South
	for (auto x = room->x1; x < room->x2; x++)
		if (tiles[x + (room->y1 - 1) * width]->ch == '.' || tiles[x + (room->y2) * width]->ch == '.')
			return false;
	// West and East
	for (auto y = room->y1; y < room->y2; y++)
		if (tiles[(room->x1 - 1) + y * width]->ch == '.' || tiles[(room->x2) + y * width]->ch == '.')
			return false;
	return true;
}

bool Map::wallHasDoor(Room* room, direction direction) const {

	switch (direction) {
		case NORTH:
			for (auto x = room->x1; x < room->x2; x++)
				if (tiles[x + (room->y1 - 1) * width]->ch == '.')
					return true;
			return false;
			break;

		case EAST:
			for (auto y = room->y1; y < room->y2; y++)
				if (tiles[room->x2 + y * width]->ch == '.')
					return true;
			return false;
			break;

		case SOUTH:
			for (auto x = room->x1; x < room->x2; x++)
				if (tiles[x + room->y2 * width]->ch == '.')
					return true;
			return false;
			break;

		case WEST:
			for (auto y = room->y1; y < room->y2; y++)
				if (tiles[(room->x1 - 1) + y * width]->ch == '.')
					return true;
			return false;
			break;

		default:
			return true;
			break;
	}
}

bool Map::isIntersection(int x, int y, direction dir) const {
	if (dir == NORTH || dir == SOUTH)
		if (tiles[x + (y - 1) * width]->ch == '#' || tiles[x + (y + 1) * width]->ch == '#')
			return true;
		
	if (dir == WEST || dir == EAST)
		if (tiles[(x + 1) + y * width]->ch == '#' || tiles[(x - 1) + y * width]->ch == '#')
			return true;

	return false;
}

std::pair<int, int> Map::getRoomCenter(int room_idx) {
	return std::pair<int, int>((rooms[room_idx]->x1 + rooms[room_idx]->x2) / 2,
							   (rooms[room_idx]->y1 + rooms[room_idx]->y2) / 2);
}

void Map::createDoorsRandom() {
	srand((unsigned int)time(NULL));

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
								setFloor(pos, (*it)->y1);
							}	
						case 1:
							if ((*it)->x2 < width - 1) {
								setFloor((*it)->x2, pos);
							}
							
						case 2:
							if ((*it)->y2 > height - 1) {
								setFloor(pos, (*it)->y2);
							}
						case 3:
							if ((*it)->x1 > 0) {
								setFloor((*it)->x1, pos);
							}
					}
				}
			}
		}
	}
}

void Map::createDoorsAll() {
	srand((unsigned int)time(NULL));

	int pos;

	for (auto it = rooms.begin(); it != rooms.end(); it++) {
		//wallHasDoor
		for (auto i = 0; i < 4; i++) {
			if (!wallHasDoor((*it), direction(i))) {

				if (i % 2 == 0)
					pos = (*it)->x1 + rand() % ((*it)->x2 + 1 - (*it)->x1);
				else
					pos = (*it)->y1 + rand() % ((*it)->y2 + 1 - (*it)->y1);

				switch (i) {
					
					// North
					case 0:
						if ((*it)->y1 > 1) {
							while (isIntersection(pos, (*it)->y1 - 1, direction(i)))
								pos = (*it)->x1 + rand() % ((*it)->x2 + 1 - (*it)->x1);
							
							setFloor(pos, (*it)->y1 - 1);
						}
						break;

					// East
					case 1:
						if ((*it)->x2 < width - 2) {
							while (isIntersection((*it)->x2 + 1, pos, direction(i)))
								pos = (*it)->y1 + rand() % ((*it)->y2 + 1 - (*it)->y1);

							setFloor((*it)->x2 + 1, pos);
						}
						break;

					// South
					case 2:
						if ((*it)->y2 < height - 2) {
							while (isIntersection(pos, (*it)->y2 + 1, direction(i)))
								pos = (*it)->x1 + rand() % ((*it)->x2 + 1 - (*it)->x1);
								
							setFloor(pos, (*it)->y2 + 1);
						}
						break;

					// West
					case 3:
						if ((*it)->x1 > 1) {
							while (isIntersection((*it)->x1 - 1, pos, direction(i)))
								pos = (*it)->y1 + rand() % ((*it)->y2 + 1 - (*it)->y1);
								
							setFloor((*it)->x1 - 1, pos);
							
						}
						break;
				}
			}
		}
	}
}

void Map::render() const {
	static const TCODColor darkWall(75, 75, 50);
	static const TCODColor darkGround(50, 50, 25);
	static const TCODColor lightWall(150, 150, 200);
	static const TCODColor lightGround(50, 50, 150);

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			if (isInFov(x, y)) {		
				TCODConsole::root->setChar(x, y, tiles[x + y * width]->ch);
				if (tiles[x + y * width]->ch == '#')
					TCODConsole::root->setCharForeground(x, y, lightWall);
				else if (tiles[x + y * width]->ch == '.')
					TCODConsole::root->setCharForeground(x, y, lightGround);
			}

			else if (isExplored(x, y)) {
				TCODConsole::root->setChar(x, y, tiles[x + y * width]->ch);
				if (tiles[x + y * width]->ch == '#')
					TCODConsole::root->setCharForeground(x, y, darkWall);
					
				else if (tiles[x + y * width]->ch == '.')
					TCODConsole::root->setCharForeground(x, y, darkGround);
			}
		}
	}	
}