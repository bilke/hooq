#include "GlobalEventPlayer.h"

extern "C" void setFredhookLogFile(const char* path)
{
	GlobalEventPlayer::setLogFile(QString::fromLocal8Bit(path));
}

extern "C" void startFredhookPlayback()
{
	GlobalEventPlayer::run();
}
