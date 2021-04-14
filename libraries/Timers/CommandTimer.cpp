#include "CommandTimer.h"

CommandTimer::CommandTimer(msecs_t interval, ICommand& command, bool repeats) :
	Timer(interval), command_(command), repeats_(repeats)
{

}

void CommandTimer::command(ICommand& command)
{
	command_ = command;
}

ICommand& CommandTimer::command() const
{
	return command_;
}

void CommandTimer::repeats(bool repeats)
{
	repeats_ = repeats;
}

bool CommandTimer::repeats() const
{
	return repeats_;
} 

void CommandTimer::tick()
{
	if (expired())
		execute();
}

void CommandTimer::execute()
{
	command_.execute();
	(repeats_) ? reset() : stop();
}