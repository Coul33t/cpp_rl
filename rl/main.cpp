#include <iostream>

#include "Main.h"

// Not gonna lie, having accessors that does nothing else than returning references
// to the variables is pointless to me. My code isn't to be used by someone else
// than me, and I (kinda) know what I'm doing (and what I can do) with my objects.
// Bloating my code with player->getX() instead of player->x is boring and tedious.

int main() {	
	Engine engine;

	while (!TCODConsole::isWindowClosed()) {
		engine.update();
		engine.render();
		TCODConsole::flush();
	}
	return 0;
}
