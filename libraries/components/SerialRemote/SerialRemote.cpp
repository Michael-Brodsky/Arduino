#include "library.h"
#include "SerialRemote.h"

SerialRemote::SerialRemote(char* buf, size_t size_buf, const Command commands[], size_t size_cmds) :
	commands_(commands, size_cmds), current_(std_end(commands_)), buf_(buf, size_buf), data_(buf_.begin()) 
{
	
}

SerialRemote::SerialRemote(char* buf_first, char* buf_last, const Command* cmd_first, const Command* cmd_last) :
	commands_(cmd_first, cmd_last), current_(std_end(commands_)), buf_(buf_first, buf_last), data_(buf_.begin()) 
{
	
}

void SerialRemote::poll()
{
	if (Serial.available())
	{
		data_ += Serial.readBytes(data_, std_distance(data_, buf_.end()));
		if (*(data_ - 1U) == EndOfTextChar || data_ == buf_.end())
		{
			*(--data_) = '\0';
			current_ = std_find(std_begin(commands_), std_end(commands_), buf());
			if (current_ != std_end(commands_))
			{
				current_->program()->execute();
				if (echo())
					Serial.print(buf());
			}
			data_ = buf_.begin();
		}
	}
}

char* SerialRemote::buf()
{
	return buf_.data();
}

const char* SerialRemote::buf() const
{
	return buf_.data();
}

bool& SerialRemote::echo()
{
	return echo_;
}

const bool& SerialRemote::echo() const
{
	return echo_;
}

void SerialRemote::clock()
{
	poll();
}