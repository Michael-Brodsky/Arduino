/*
 * This file defines an abstract interface class for checking the state of 
 * GPIO inputs.
 */
#if !defined INPUT_H__
#define INPUT_H__ 
#include <types.h>		// "arduino" & stdint types.
#include <ICommand.h>	// ICommand interface class.

struct Input // GPIO input abstract interface class.
{
	virtual ~Input() { }

	virtual bool triggered(ICommand* command = nullptr) = 0; 
};

#endif //!defined INPUT_H__