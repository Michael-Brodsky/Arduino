/*
 *	Hardware configuration and application constants for Sequencer sketch.
 *
 *	***************************************************************************
 *
 *	File: config.h
 *	Date: May 30, 2021
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
# define CONFIG_H__ 20210530L

# include <utility.h>				// `std_pair' type.
# include <utils.h>					// `resetFunc', `Print', `PrintLn', `charcat' functions.
# include <TimeLib.h>				// Arduino time lib.
# include <AnalogKeypad.h>			// `Keypad' type.
# include <Display.h>				// `Display' type. 
# include <TaskScheduler.h>			// `TaskScheduler' type.
# include <RotaryActuator.h>			// `RotaryActuator' and `SweepServo' types
# include <Sequencer.h>				// `Sequencer' type.
# include <SerialRemote.h>			// `SerialRemote' type.

 /*
  * LCD display hardware constants
  */

const pin_t LcdRs = 8;
const pin_t LcdEnable = 9;
const pin_t LcdD4 = 4;
const pin_t LcdD5 = 5;
const pin_t LcdD6 = 6;
const pin_t LcdD7 = 7;

const uint8_t LcdCols = 16;
const uint8_t LcdRows = 2;

/*
 * Display chars, strings & fmt spec constants.
 */

const char* EventPrintFmt[] = { "%4s:%02u %8s", "%c  %3u%c %02u:%02u:%02u" };
const char* ConfigPrintFmt[] = { "%4s:   Step:%3u", "Wrap:%c Intvl:%3u" };
const char* MenuPrintFmt[] = { "%4s: %4s  %4s", "%4s  %4s  %4s" };
const char* CommPrintFmt[] = { "%4s: %6lu %3s", "" };
const char* AutoLabel = "Auto";
const char* ManLabel = " Man";
const char* PgmLabel = " Pgm";
const char* CfgLabel = " Cfg";
const char* CommLabel = "Comm";
const char* MenuLabel = "Menu";
const int DecimalRadix = 10;
const char SpinnerChars[] = { '|', '/', '-', '/' };
const char DegreesSymbol = 0xDF;	// Degrees symbol.
const char WrapChar = 'Y';
const char NoWrapChar = 'N';
const char StringDelimiterChar = '"';
const char GroupSeparatorChar = ',';
const char RecordSeparatorChar = ';';
const char* SerialStartString = "srt";	// Sequencer start cmd.
const char* SerialStopString = "stp";	// Sequencer stop cmd.
const char* SerialResumeString = "res";	// Sequencer resume cmd.
const char* SerialResetString = "rst";	// Sequencer reset cmd.
const char* SerialListString = "lst";	// Sequencer list events cmd.
const char* SerialStoreString = "sto";	// Sequencer store events cmd.

/*
 * Display row/col coordinates.
 */

const uint8_t TopRow = 0;
const uint8_t BottomRow = 1;
const uint8_t StatusCol = 0;
const uint8_t IndexCol = 6;
const uint8_t NameCol = 10;
const uint8_t ModeCol = 0;
const uint8_t AngleCol = 5;
const uint8_t HourCol = 9;
const uint8_t MinuteCol = 12;
const uint8_t SecondCol = 15;
const uint8_t StepCol = 15;
const uint8_t IntervalCol = 15;
const uint8_t WrapCol = 5;
const uint8_t BaudCol = 11;
const uint8_t ProtoCol = 15;
const uint8_t AutoCol = 9;
const uint8_t ManCol = 15;
const uint8_t PgmCol = 3;
const uint8_t CfgCol = 9;
const uint8_t CommCol = 15;
const uint8_t MsgCol = 0;
const uint8_t SpinnerDivisor = 1;

/*
 * Setting adjustment constants.
 */

const int8_t Increment = +1;
const int8_t Decrement = -1;

/*
 * Keypad hardware and button constants.
 */

const pin_t KeypadInputPin = 0;

const analog_t RightButtonTriggerLevel = 60;
const analog_t UpButtonTriggerLevel = 200;
const analog_t DownButtonTriggerLevel = 400;
const analog_t LeftButtonTriggerLevel = 600;
const analog_t SelectButtonTriggerLevel = 800;

/*
 * Comms types and constants.
 */

using baud_type = unsigned long;
using protocol_type = char;
using serial_type = std_pair<const char*, const protocol_type>;
const baud_type SupportedBaudRates[] = { 300, 600, 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200 };
const serial_type SupportedSerialProtocols[] = // Commented out stuff to free up memory on Uno.
{
	//serial_type("5N1", SERIAL_5N1),
	//serial_type("6N1", SERIAL_6N1),
	//serial_type("7N1", SERIAL_7N1),
	serial_type("8N1", SERIAL_8N1),
	//serial_type("5N2", SERIAL_5N2),
	//serial_type("6N2", SERIAL_6N2),
	//serial_type("7N2", SERIAL_7N2),
	//serial_type("8N2", SERIAL_8N2),
	//serial_type("5E1", SERIAL_5E1),
	//serial_type("6E1", SERIAL_6E1),
	serial_type("7E1", SERIAL_7E1),
	//serial_type("8E1", SERIAL_8E1),
	//serial_type("5E2", SERIAL_5E2),
	//serial_type("6E2", SERIAL_6E2),
	//serial_type("7E2", SERIAL_7E2),
	//serial_type("8E2", SERIAL_8E2),
	//serial_type("5O1", SERIAL_5O1),
	//serial_type("6O1", SERIAL_6O1),
	//serial_type("7O1", SERIAL_7O1),
	//serial_type("8O1", SERIAL_8O1),
	//serial_type("5O2", SERIAL_5O2),
	//serial_type("6O2", SERIAL_6O2),
	//serial_type("7O2", SERIAL_7O2),
	//serial_type("8O2", SERIAL_8O2)
};
const baud_type DefaultBaudRate = 9600;
const protocol_type DefaultSerialProtocol = SERIAL_8N1;

class SerialComms : public ISerializeable
{
public:
	using baud_container_type = ArrayWrapper<const baud_type>;
	using serial_container_type = ArrayWrapper<const serial_type>;
	using baud_iterator_type = baud_container_type::const_iterator;
	using serial_iterator_type = serial_container_type::const_iterator;
	enum class Select { Baud = 0, Protocol };

public:
	template<size_t Size1, size_t Size2>
	SerialComms(const baud_type(&supported_baud_rates)[Size1], const serial_type(&supported_protocols)[Size2]) :
		supported_baud_rates_(supported_baud_rates), baud_rate_(match(DefaultBaudRate)), baud_rate_copy_(baud_rate_),
		supported_protocols_(supported_protocols), protocol_(match(DefaultSerialProtocol)), protocol_copy_(protocol_),
		selection_(Select::Baud)
	{}
	SerialComms(const baud_type supported_baud_rates[], size_t size1, const serial_type supported_protocols[], size_t size2) :
		supported_baud_rates_(supported_baud_rates, size1), baud_rate_(match(DefaultBaudRate)), baud_rate_copy_(baud_rate_),
		supported_protocols_(supported_protocols, size2), protocol_(match(DefaultSerialProtocol)), protocol_copy_(protocol_),
		selection_(Select::Baud)
	{}
	SerialComms(const baud_type* first1, const baud_type* last1, const serial_type* first2, const serial_type* last2) :
		supported_baud_rates_(first1, last1), baud_rate_(match(DefaultBaudRate)), baud_rate_copy_(baud_rate_),
		supported_protocols_(first2, last2), protocol_(match(DefaultSerialProtocol)), protocol_copy_(protocol_),
		selection_(Select::Baud)
	{}

public:
	void protocol(const protocol_type& value)
	{
		serial_iterator_type it = match(value);

		protocol_ = it != std_end(supported_protocols_) ? it : match(DefaultSerialProtocol);
	}
	const serial_type& protocol() const { return *protocol_; }
	void baud(const baud_type& value)
	{
		baud_iterator_type it = match(value);

		baud_rate_ = it != std_end(supported_baud_rates_) ? it : match(DefaultBaudRate);
	}
	const baud_type& baud() const { return *baud_rate_; }
	void select(Select selection) { selection_ = selection; }
	void next()
	{
		switch (selection_)
		{
		case SerialComms::Select::Baud:
			if (++baud_rate_ == std_end(supported_baud_rates_))
				baud_rate_ = std_begin(supported_baud_rates_);
			break;
		case SerialComms::Select::Protocol:
			if (++protocol_ == std_end(supported_protocols_))
				protocol_ = std_begin(supported_protocols_);
			break;
		default:
			break;
		}
	}
	void prev()
	{
		switch (selection_)
		{
		case SerialComms::Select::Baud:
			if (baud_rate_ == std_begin(supported_baud_rates_))
				baud_rate_ = std_end(supported_baud_rates_);
			--baud_rate_;
			break;
		case SerialComms::Select::Protocol:
			if (protocol_ == std_begin(supported_protocols_))
				protocol_ = std_end(supported_protocols_);
			--protocol_;
			break;
		default:
			break;
		}
	}
	void serialize(EEPROMStream& s) const override { s << baud(); s << protocol().second; }
	void deserialize(EEPROMStream& s) override
	{
		baud_type b;
		protocol_type p;

		s >> b;
		s >> p;
		baud(b);
		protocol(p);
	}
	void copy() { baud_rate_copy_ = baud_rate_; protocol_copy_ = protocol_; }
	void restore() { baud_rate_ = baud_rate_copy_; protocol_ = protocol_copy_; }

private:
	baud_iterator_type match(const baud_type baud) const
	{
		return std_find(std_begin(supported_baud_rates_), std_end(supported_baud_rates_), baud);
	}
	serial_iterator_type match(const protocol_type protocol) const
	{
		for (auto& it : supported_protocols_)
		{
			if (it.second == protocol)
				return &it;
		}

		return std_end(supported_protocols_);
	}

private:
	baud_container_type supported_baud_rates_;
	serial_container_type supported_protocols_;
	baud_iterator_type baud_rate_;
	serial_iterator_type protocol_;
	baud_iterator_type baud_rate_copy_;
	serial_iterator_type protocol_copy_;
	Select selection_;
};

// `ButtonTag' type required by `AnalogKeypad'.
enum class ButtonTag
{
	Right = 0,
	Up,
	Down,
	Left,
	Select
};

// `CommandTag' type required by `SerialComms'.
enum class CommandTag
{
	Start = 0,
	Stop,
	Resume,
	Reset,
	List,
	Store
};

/*
 * Servo types and constants.
 */

using angle_t = servo_types::angle_t;
using step_t = servo_types::step_t;
using servo_hardware = hiwonder_20;
const pin_t ServoControlPin = 3;
const step_t ServoDfltStepSize = 40;
const step_t ServoMinStepSize = SweepServo<servo_hardware>::DefaultStepSize;
const step_t ServoMaxStepSize = servo_traits<servo_hardware>::max_step_size;
const msecs_t ServoDfltStepInterval = step_interval<typename servo_traits<servo_hardware>::type>(ServoDfltStepSize) / 1000UL;
const msecs_t ServoMinStepInterval = step_interval<typename servo_traits<servo_hardware>::type>(ServoMinStepSize) / 1000UL;
const msecs_t ServoMaxStepInterval = step_interval<typename servo_traits<servo_hardware>::type>(ServoMaxStepSize) / 1000UL;
const angle_t ServoMaxAngle = servo_traits<servo_hardware>::max_control_angle;
const angle_t ServoMinAngle = 0;
const msecs_t RemainingTimeOffset = 900; // Offset in milliseconds to make sequence "time remaining" display prettier.

/*
 * Timing constants.
 */

const msecs_t KeypadLongPressInterval = 1000;
const msecs_t KeypadPollingInterval = 100;
const msecs_t DisplayRefreshInterval = 100;
const msecs_t SequencerClockingInterval = 500;
const msecs_t SerialPollingInterval = 500;

/*
 * Operation types.
 */

enum class Mode
{
	Auto = 0,
	Man,
	Pgm,
	Cfg,
	Menu,
	Comms
};

enum class Scroll
{
	Previous = 0,
	Next
};

enum class Action
{
	Start = 0,
	Stop,
	Resume
};

/*
 * Event record types and constants.
 */

enum class EventGroup
{
	Name = 0,
	Duration,
	Angle
};

#if !defined _DEBUG
const uint8_t MaxEventRecords = 8;
#else
const uint8_t MaxEventRecords = 2;
#endif
const uint8_t MaxCharsPerRecord = 23;
const uint8_t MaxLengthEventName = 8;

/*
 * Application Types
 */

// Serializable `RotaryActuator' position command type.
class ActuatorCommand : public ISerializableCommand
{
public:
	using Method = Callback<void, RotaryActuator, angle_t>::callback_t;

public:
	ActuatorCommand() = default; // Must be default constructable.
	ActuatorCommand(RotaryActuator* object, Method method, angle_t angle) :
		ISerializableCommand(), object_(object), method_(method), angle_(angle)
	{}
	
public:
	void	object(RotaryActuator* object) { object_ = object; }
	void	method(Method method) { method_ = method; }
	angle_t angle() const { return angle_; }
	void	angle(angle_t angle) { angle_ = angle; }
	void	execute() override { (object_->*method_)(angle_); }
	void	serialize(EEPROMStream& s) const override { s << angle_; }
	void	deserialize(EEPROMStream& s) override { s >> angle_; }

private:
	RotaryActuator* object_;	// Command receiver object.
	Method			method_;	// Receiver::Method.
	angle_t			angle_;		// Method parameter.
};

// Serializable servo timing configuration type.
struct config_t : public ISerializeable
{
	step_t	step_size_;
	msecs_t	step_interval_;
	bool	wrap_;

	config_t() = default;
	config_t(step_t	step_size, msecs_t step_interval, bool wrap) :
		ISerializeable(), step_size_(step_size), step_interval_(step_interval), wrap_(wrap) 
	{}
	void serialize(EEPROMStream& s) const override { s << step_size_; s << step_interval_; s << wrap_; }
	void deserialize(EEPROMStream& s) override { s >> step_size_; s >> step_interval_; s >> wrap_; }
};

// Serializable sequence type.
struct sequence_t : public ISerializeable
{
	using size_type = EEPROMStream::size_type;

	Sequencer::container_type events_;

	sequence_t() = default;
	sequence_t(const Sequencer::container_type events) : events_(events) {}

	void serialize(EEPROMStream& s) const override
	{
		for (auto it : events_)
		{
			s << it->name_;
			s << it->duration_;
			if (it->command_)
				static_cast<ActuatorCommand*>(it->command_)->serialize(s);
		}
	}

	void deserialize(EEPROMStream& s) override
	{
		for (auto it : events_)
		{
			char* ptr = const_cast<char*>(it->name_);

			s >> ptr;
			s >> it->duration_;
			if (it->command_)
				static_cast<ActuatorCommand*>(it->command_)->deserialize(s);
		}
	}
};

/*
 * Application type aliases.
 */

using event_type = Sequencer::event_type;
using event_state_type = event_type::State;
using sequence_type = Sequencer::container_type;
using actuator_command_type = ActuatorCommand;

#endif // !defined CONFIG_H__ 
