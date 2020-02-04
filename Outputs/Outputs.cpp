#include "Outputs.h"

#pragma region digital_output

DigitalOutput::DigitalOutput(pin_t pin) :
	pin_(pin)
{
	pinMode(pin_, OUTPUT);
}

DigitalOutput::State DigitalOutput::operator()() const
{
	return digitalRead(pin_);
}

void DigitalOutput::operator()(State state)
{
	digitalWrite(pin_, state);
}

#pragma endregion

#pragma region output_toggle_command

OutputToggleCommand::OutputToggleCommand(DigitalOutput& output) :
	ICommand(), output_(output)
{
}

OutputToggleCommand::~OutputToggleCommand()
{
}

void OutputToggleCommand::execute() 
{
	output_(!output_());
}

#pragma endregion

#pragma region output_state_command

OutputStateCommand::OutputStateCommand(DigitalOutput& output, DigitalOutput::State state) :
	OutputToggleCommand(output), state_(state)
{
}

void OutputStateCommand::execute() 
{
	output_(state_);
}

#pragma endregion