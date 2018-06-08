#pragma once

#include "Ai.h"

class BasicAi: public Ai {
	public:
		BasicAi();
		~BasicAi();
		void update(Actor* owner);
};

