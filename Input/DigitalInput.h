/*
 * This file defines a digital GPIO input polling type.
 */
#if !defined DIGITALINPUT_H__
#define DIGITALINPUT_H__ 

#include <IInput.h>		// IInput interface class.

class DigitalInput : public IInput // Digital GPIO input polling type.
{
public:		/**** Member Types and Constants ****/
	enum class Trigger	// DigitalInput trigger type.
	{ 
		None,	// Input cannot be triggered.
		Edge,	// Input is "edge" triggered.
		Level	// Input is "level" triggered.
	};

	enum class State : int 
	{ 
		Low=0,	// Input is triggered "low" or on a falling edge.
		High=1	// Input is triggered "high" or on a rising edge.
	};

	enum class Mode : int 
	{ 
		Input=0,		// Internal pullup resistor is disabled.
		InputPullup=2	// Internal pullup resistor is enabled.
	};

public:		/**** Ctors ****/
	DigitalInput(pin_t pin = 0, Mode mode = Mode::InputPullup);			// Attach constructor.

public:		/**** Member Functions ****/
	void			attach(pin_t, const Mode&);							// Attaches a GPIO input pin.
	void			trigger(const Trigger&, const State&, ICommand*);	// Sets the input trigger type and command.
	State			operator()();										// Returns the attached pin's current state.
	bool			operator()(const State&);							// Checks whether the attached pin is in a given state.
	bool			operator==(const State&) const;						// Checks whether the attached pin state is equal to another pin.
	bool			operator!=(const State&) const;						// Checks whether the attached pin state is not equal to another pin.
	bool			triggered(ICommand*) override;						// Checks whether the attached pin is currently triggered.

private:	/**** Member Objects ****/
	pin_t			pin_;					// The attached GPIO input pin.
	State			input_state_;			// The attached pin's current state.
	Trigger			trigger_type_;			// The attached pin's trigger type.
	State			trigger_state_;			// The attached pin's trigger state.
	ICommand*		trigger_command_;		// The command to execute when the attached pin is triggered.
};

#endif // !defined DIGITALINPUT_H__