#include "Player.h"

extern "C" void setHooqLogFile(const char* path)
{
	Hooq::Player::setLogFile(QString::fromLocal8Bit(path));
}

extern "C" void startHooqPlayback()
{
	Hooq::Player::run();
}
