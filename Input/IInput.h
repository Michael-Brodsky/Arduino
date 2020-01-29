/*
 * This file defines an abstract interface class for checking the state of 
 * GPIO inputs.

 *	***************************************************************************/

/*	File: IInput.h
 *	Date: January 24, 2020
 *	Version: 0.99
 *	Author: Michael Brodsky
 *	Email: mbrodskiis@gmail.com
 *	Copyright (c) 2012-2020 Michael Brodsky */

/*	***************************************************************************
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

/*
 *	Description:
 *
 */

#if !defined IINPUT_H__
#define IINPUT_H__ 20200124L

#include <ICommand.h>	// ICommand interface class.

struct IInput // GPIO input abstract interface class.
{
	virtual ~IInput() { }

	virtual bool triggered(ICommand* command = nullptr) = 0; 
};

#endif //!defined INPUT_H__
