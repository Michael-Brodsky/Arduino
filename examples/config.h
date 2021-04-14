/*
 *	Hardware configuration and application constants for DigitalClock sketch.
 *
 *	***************************************************************************
 *
 *	File: config.h
 *	Date: April 9, 2021
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

#if !defined CONFIG_H__ 
# define CONFIG_H__ 20210409L

const pin_t LcdRs = 8U;
const pin_t LcdEnable = 9U;
const pin_t LcdD4 = 4U;
const pin_t LcdD5 = 5U;
const pin_t LcdD6 = 6U;
const pin_t LcdD7 = 7U;

const uint8_t LcdCols = 16U;
const uint8_t LcdRows = 2U;
const msecs_t DisplayRefreshInterval = 80U;
const msecs_t DisplayBlinkInterval = 1000U;

const uint8_t DateRow = 0;
const uint8_t TimeRow = 1U;
const uint8_t DayCol = 0;
const uint8_t MonthCol = 3U;
const uint8_t YearCol = 7U;
const uint8_t DayOfWeekCol = 0;
const uint8_t HourCol = 4U;
const uint8_t MinuteCol = 7U;
const uint8_t SecondCol = 10U;
const uint8_t AlarmCol = 12U;

const char alarm_set[] = "ALRM", no_alarm_set[] = "    ";
const char alarm_on = '*', alarm_off = ' ';
const char DateFmt[] = "%02d %s %4d %s";
const char TimeFmt[] = "%s %02d:%02d:%02d   %c";

const pin_t KeypadInputPin = 0;
const msecs_t KeypadLongPressInterval = 1000U; 
const msecs_t KeypadPollingInterval = 100U;

const analog_t RightButtonTriggerLevel = 60U;
const analog_t UpButtonTriggerLevel = 200U;
const analog_t DownButtonTriggerLevel = 400U;
const analog_t LeftButtonTriggerLevel = 600U;
const analog_t SelectButtonTriggerLevel = 800U;

enum class ButtonTag
{
	Right, 
	Up, 
	Down, 
	Left, 
	Select  
};

const msecs_t AlarmCheckingInterval = 1000U;

#endif // !defined CONFIG_H__ 

