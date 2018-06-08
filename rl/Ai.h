#pragma once

#include "Actor.h"

class Ai {
	public:
		Ai();
		~Ai();
		virtual void update(Actor* owner) = 0;
};

