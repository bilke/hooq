#include "Player.h"

extern "C" void startHooqPlayback()
{
	Hooq::Player::run();
}
