#include "BasicAi.h"

BasicAi::BasicAi() {

}


BasicAi::~BasicAi() {

}


void BasicAi::update(Actor* owner) {
	if (owner->destructible && owner->destructible->isDead())
		return;

	else {

	}


}
