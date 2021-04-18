#include "DigitalClock.h"

const size_t EepromAlarmEnabled = 0;			// alarm_enabled_ EEPROM address.
const size_t EepromAlarmTime = sizeof(bool);	// alarm_time_ EEPROM address.

const std_array<DigitalClock::Field, 7U> DigitalClock::fields_ =
{
	DigitalClock::Field::Day,
	DigitalClock::Field::Month,
	DigitalClock::Field::Year,
	DigitalClock::Field::Hour,
	DigitalClock::Field::Minute,
	DigitalClock::Field::Second,
	DigitalClock::Field::Alarm
};


DigitalClock::DigitalClock(Callback callback) :
	status_(TimeStatus::timeNotSet), mode_(), adjusted_time_(), 
	alarm_enabled_(eepromGet<bool>(EepromAlarmEnabled)),
	alarm_time_(eepromGet<time_t>(EepromAlarmTime)),
	alarm_active_(), temp_time_(), current_(fields_.cbegin()), callback_(callback) 
{

}

DigitalClock::TimeStatus DigitalClock::status() const
{
	return timeStatus();
}

time_t DigitalClock::time() const
{
	switch (mode_)
	{
	case DigitalClock::Mode::Run:
		return now();
		break;
	case DigitalClock::Mode::SetAlarm:
	case DigitalClock::Mode::SetTime:
		return adjusted_time_;
		break;
	default:
		break;
	}
}
DigitalClock::Mode DigitalClock::mode() const
{
	return mode_;
}

void DigitalClock::mode(Mode mode)
{
	switch (mode)
	{
	case DigitalClock::Mode::Run:
		switch (mode_)
		{
		case DigitalClock::Mode::SetAlarm:
			if (temp_time_ != adjusted_time_)	// Only update the alarm time if it was adjusted.
				alarm_time_ = (adjusted_time_ < now())
					? adjusted_time_ + SECS_PER_DAY + (now() - adjusted_time_) // if before now(), bump alarm to tommorow.
					: adjusted_time_; 
			saveAlarm();
			break;
		case DigitalClock::Mode::SetTime:
			if (temp_time_ != adjusted_time_)	// Only update the current time if it was adjusted.
				setTime(adjusted_time_);
			break;
		default:
			break;
		}
		break;
	case DigitalClock::Mode::SetAlarm:
		temp_time_ = adjusted_time_ = alarm_time_ < now() // Never init adjusted_time_ to before now().
			? now() 
			: alarm_time_;
		alarm_active_ = false; // This just sets the flag, may need another callback to signal alarm hardware.
		break;
	case DigitalClock::Mode::SetTime:
		temp_time_ = adjusted_time_ = now();
		break;
	default:
		break;
	}
	current_ = fields_.cbegin();
	mode_ = mode;
}

void DigitalClock::prev()
{
	auto last = mode_ == DigitalClock::Mode::SetAlarm // SetAlarm mode has one extra field.
		? fields_.cend() 
		: fields_.cend() - 1U; 

	if (current_ == fields_.cbegin())
		current_ = last;
	--current_;
}

void DigitalClock::next()
{
	auto last = mode_ == DigitalClock::Mode::SetAlarm // SetAlarm mode has one extra field.
		? fields_.cend() 
		: fields_.cend() - 1U; 

	if (++current_ == last)
		current_ = fields_.cbegin();
}

void DigitalClock::inc()
{
	adjustTime(+1);
}

void DigitalClock::dec()
{
	adjustTime(-1);
}

void DigitalClock::adjustTime(int adjustment)
{
	time_t previous = adjusted_time_;
	tmElements_t tm;
	uint8_t* p = nullptr;

	breakTime(adjusted_time_, tm);
	switch (*current_)
	{
	case DigitalClock::Field::Day:
		p = &tm.Day;
		break;
	case DigitalClock::Field::Month:
		p = &tm.Month;
		break;
	case DigitalClock::Field::Year:
		p = &tm.Year;
		break;
	case DigitalClock::Field::Hour:
		p = &tm.Hour;
		break;
	case DigitalClock::Field::Minute:
		p = &tm.Minute;
		break;
	case DigitalClock::Field::Second:
		p = &tm.Second;
		break;
	case DigitalClock::Field::Alarm:
		alarm_enabled_ = !alarm_enabled_;
		break;
	default:
		break;
	}
	if (p)	// Only do this if we're not setting the alarm enable/disable field.
	{
		*p += adjustment;
		if (tm.Month == 0)	// Bug in TimeLib: if Month = 1 (Jan) && adjustment = -1 (down) then
		{					// Month = 0 but displayed date doesn't change.
			tm.Month = 12;	// Fix: if Month == 0 after adjustment, then set Month = 12 and  decrement Year.
			--tm.Year;
		}
		adjusted_time_ = makeTime(tm);
		if (adjustment < 0 && adjusted_time_ > previous) // Prevent underflow if adjusted time is before Jan 1, 1970.
			adjusted_time_ = previous;
	}
}

void DigitalClock::alarmEnabled(bool value)
{
	alarm_enabled_ = value;
}

bool DigitalClock::alarmEnabled() const
{
	return alarm_enabled_;
}

bool DigitalClock::alarmActive() const
{
	return alarm_active_;
}

void DigitalClock::saveAlarm()
{
	eepromUpdate(EepromAlarmEnabled, alarm_enabled_);
	eepromUpdate(EepromAlarmTime, alarm_time_);
}

void DigitalClock::clock()
{
	if (alarm_enabled_ && !(now() < alarm_time_))
	{
		(*callback_)();
		alarm_active_ = true;
		alarm_time_ += SECS_PER_DAY; // Advance alarm to same time tomorrow.
		saveAlarm();
	}
}