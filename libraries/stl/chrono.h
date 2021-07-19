/*
 *	This file defines a library of date and time utilities.
 *
 *	***************************************************************************
 *
 *	File: chrono.h
 *	Date: June 30, 2021
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
 *	**************************************************************************/

#if !defined __CHRONO_H
# define __CHRONO_H 20210630L

# include <inttypes.h>
# include "types.h"
# include "numeric_limits.h"

static constexpr time_t MillisPerSecond = 1000;
static constexpr time_t SecondsPerMinute = 60;
static constexpr time_t MinutesPerHour = 60;
static constexpr time_t HoursPerDay = 24;
static constexpr time_t DaysPerWeek = 7;
static constexpr time_t MinDaysPerMonth = 28;
static constexpr time_t MaxDaysPerMonth = 31;
static constexpr time_t SecondsPerHour = SecondsPerMinute * MinutesPerHour;
static constexpr time_t SecondsPerDay = SecondsPerHour * HoursPerDay;
static constexpr time_t MillisPerMinute = SecondsPerMinute * MillisPerSecond;
static constexpr time_t MillisPerHour = SecondsPerHour * MillisPerSecond;
static constexpr time_t MillisPerDay = SecondsPerDay * MillisPerSecond;
static constexpr bool IsLeapYear(const time_t year)
{
	return (year % 4) || ((year % 100 == 0) && (year % 400)) 
		? false 
		: true;
}
static constexpr time_t DaysPerMonth(const time_t month, const time_t year)
{
	return (month == 2) 
		? (MinDaysPerMonth + IsLeapYear(year)) 
		: MaxDaysPerMonth - (month - 1) % DaysPerWeek % 2;
}

class std_chrono_millis
{
public:
	std_chrono_millis() = default;
	std_chrono_millis(time_t count) : count_(count), seconds_(count_ / MillisPerSecond) {}

public:
	time_t count() const { return count_; }
	uint8_t seconds() const { return static_cast<uint8_t>(seconds_ % SecondsPerMinute); }
	uint8_t minutes() const { return static_cast<uint8_t>((seconds_ / SecondsPerMinute) % SecondsPerMinute); }
	uint8_t hours() const { return static_cast<uint8_t>(seconds_ / SecondsPerHour); }

private:
	time_t count_;
	time_t seconds_;
};

template<intmax_t Num, intmax_t Denom = 1>
class std_ratio 
{
public:
	constexpr intmax_t num() { return Num; }
	constexpr intmax_t den() { return Denom; }

public:
	using type = std_ratio<num, den>;
};

template <class Rep>
struct std_chrono_duration_values
{
	static constexpr Rep zero() { return Rep(0); }
	static constexpr Rep min() { return std_numeric_limits<Rep>::min(); }
	static constexpr Rep max() { return std_numeric_limits<Rep>::max(); }
};

template<class Rep, class Period = std_ratio<1>>
class std_chrono_duration
{
public:
	using rep = Rep;
	using period = Period;

public:
	constexpr std_chrono_duration() = default;
	std_chrono_duration(const std_chrono_duration& duration) = default;
	template<class Rep2>
	constexpr explicit std_chrono_duration(const Rep2& r);
	template<class Rep2, class Period2>
	constexpr std_chrono_duration(const std_chrono_duration<Rep2, Period2>& d);

	std_chrono_duration& operator=(const std_chrono_duration& other) = default;

public:
	static constexpr std_chrono_duration zero();
	static constexpr std_chrono_duration min();
	static constexpr std_chrono_duration max();
	constexpr rep count() const { return r_; };

private:
	Rep r_;
	Period p_;
};

#endif //