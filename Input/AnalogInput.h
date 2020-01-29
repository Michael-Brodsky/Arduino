/*
 * This file defines an asynchronous, analog GPIO input polling type.
 */
#if !defined ANALOGINPUT_H__
#define ANALOGINPUT_H__ 20200119L
#include <assert.h>			// assert() macro.
#include <IInput.h>			// IInput interface class.
#include <ClockCommand.h>	// ClockCommand interface class.

class AnalogInput : public IInput, public IClockable	// Asynchronous analog GPIO input polling type.
{
public:		/**** Member Types and Constants ****/
	struct Trigger	// AnalogInput trigger type.
	{
		analog_t	level_;		// Input trigger level.
		ICommand*	command_;	// Command to execute when input is triggered.
	};

	typedef Trigger** iterator;	// AnalogInput trigger iterator type.

public:		/**** Ctors ****/ 
	explicit AnalogInput(pin_t);				// Attach constructor.

public:		/**** Member Functions ****/
	void		attach(pin_t);					// Attaches a GPIO input pin.
	analog_t	operator()();					// Returns the attached pin's current input level.
	bool		operator<(analog_t);			// Checks whether the attached pin's current input level is less than a given level.
	void		trigger(iterator, iterator);	// Assigns a set of input trigger objects.
	bool		triggered(ICommand*) override;	// Checks whether the attached pin is currently triggered.

private:
	void		clock() override;				// IClockable `clock()' function implementation.

private:	/**** Member Objects ****/
	pin_t		pin_;							// The attached GPIO input pin.
	analog_t	input_level_;					// The attached pin's current input level.
	bool		is_triggered_;					// Flag indicating whether the attached input is currently triggered.
	iterator	begin_;							// Iterator pointing to the beginning of the current set of triggers.
	iterator	end_;							// Iterator pointing to the end of the current set of triggers.
};

#endif // !defined ANALOGINPUT_H__