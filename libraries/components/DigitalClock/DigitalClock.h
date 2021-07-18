/*
 *	This files declares a class that encapsulates the behavior of a simple 
 *	digital clock.
 *
 *	***************************************************************************
 *
 *	File: DigitalClock.h
 *	Date: July 18, 2021
 *	Version: 0.99
 *	Author: Michael Brodsky
 *	Email: mbrodskiis@gmail.com
 *	Copyright (c) 2012-2021 Michael Brodsky
 *
 *	***************************************************************************
 *
 *  This file is part of "Pretty Good" (Pg). "Pg" is free software:
 *	you can redistribute it and/or modify it under the terms of the
 *	GNU General Public License as published by the Free Software Foundation,
 *	either version 3 of the License, or (at your option) any later version.
 *
 *  This file is distributed in the hope that it will be useful, but
 *	WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *	along with this file. If not, see <http://www.gnu.org/licenses/>.
 *
 *	**************************************************************************
 * 
 *	Description:
 * 
 *		The `DigitalClock' class encapsulates the functional logic for a 
 *		simple digital clock with an alarm function. It also provides EEPROM 
 *		support for saving the alarm date, time and status.
 * 
 *		The class has three operating modes which clients can set using the 
 *		`mode()' method. `Run' is the default mode and is used to display the 
 *		current date and time. `SetTime' and `SetAlarm' modes are used to 
 *		adjust the current and alarm time respectively.
 * 
 *		Adjustable values are assigned to fields, enumerated by the nested 
 *		`Field' type. Clients can scroll through the fields, forward or 
 *		backward, with the `next()' and `prev()' methods respectively. The 
 *		field values are adjusted up and down by the `inc()' and `and dec()'
 *		methods. Clients use the `alarmEnabled()' and `alarmActive()' methods 
 *		to set or query the current alarm status. The `status()' method 
 *		returns the current clock "sync" status and the `time()' method 
 *		returns the display time, which can be either the current or adjusted 
 *		time, depending on the operating mode.
 * 
 *		The implementation also contains logic to prevent clients from setting 
 *		invalid values, such as setting the alarm to a time before the current 
 *		time or setting the date and time before the minimum setting of Jan 1, 
 *		1970.
 * 
 *	**************************************************************************/

#if !defined DIGITALCLOCK_H__
# define DIGITALCLOCK_H__ 20210718L

#include "TimeLib.h"		// Arduino time lib.
#include "EEPROMStream.h"	// Eeprom support.
#include "array.h"			// `std_array' type.
#include "IClockable.h"		// `IClockable' interface (for client alarm checking).
#include "IComponent.h"		// `IComponent' interface.

// Type that encapsulates the behavior of a digital clock.
class DigitalClock : public IClockable, public IComponent, public ISerializeable  
{
public:
	// Enumerates the valid operating modes.
	enum class Mode 
	{
		Run,
		SetAlarm,
		SetTime
	};

	// Enumerates the valid display fields.
	enum class Field
	{
		Day, 
		Month, 
		Year, 
		Hour, 
		Minute, 
		Second, 
		Alarm
	};

	// Container for iterating through display field enums.
	static const std_array<Field, 7U> fields_;
	// Display field iterator type.
	using iterator = std_array<Field, 7U>::const_iterator;
	// timeStatus_t type alias.
	using TimeStatus = timeStatus_t;	
	// Display callback type.
	using Callback = void(*)();			

public:
	DigitalClock(EEPROMStream&, Callback);

public:
	// Returns the current time sync status.
	TimeStatus	status() const;
	// Returns the current/adjusted time,
	time_t		time() const;
	// Sets the operating mode.
	void		mode(Mode mode);
	// Returns the current operating mode.
	Mode		mode() const;
	// Enables/disables the alarm function.
	void		alarmEnabled(bool);
	// Checks whether the alarm function is enabled. 
	bool		alarmEnabled() const;
	// Checks whether the alarm is currently tripped.
	bool		alarmActive() const;
	// Advances to the previous clock field.
	void		prev();
	// Advances to the next clock field.
	void		next();
	// Increments the value of the current clock field.
	void		inc();
	// Decrements the value of the current clock field.
	void		dec();
	// Saves current time & alarm to EEPROM.
	void		serialize(EEPROMStream&) const override;
	// Loads saved time & alarm from EEPROM.
	void		deserialize(EEPROMStream&) override;

private:
	// Updates the adjusted time. 
	void		adjustTime(int);
	// Saves the current alarm time to the onboard EEPROM.
	//void		saveAlarm();
	// Checks whether to trip the alarm.
	void		clock() override;

private:
	TimeStatus	status_;		// The current time sync status.
	Mode		mode_;			// The current operating mode.
	time_t		adjusted_time_;	// Holds the adjusted time in `SetAlarm' and `SetTime' modes.
	time_t		alarm_time_;	// Holds the current alarm time.
	time_t		temp_time_;		// Stores a temporary copy of the the time when entering one of the "set" modes.
	Callback	callback_;		// Client callback.
	bool		alarm_enabled_;	// Flag indicating whether the alarm is currently enabled.
	bool		alarm_active_;	// Flag indicating whether the alarm is currently tripped.
	iterator	current_;		// Points to the currently active clock field being adjusted.
	EEPROMStream& eeprom_;
};

#endif // !defined DIGITALCLOCK_H__