/*
 *	This file declares an abstract interface class for asynchronously 
 *	sweepable servo controller types.
 *
 *	***************************************************************************
 *
 *	File: IServo.h
 *	Date: May 31, 2021
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

#if !defined ISERVO_H__ 
# define ISERVO_H__ 20210531L

#include "IClockable.h"
#include "servos.h"

struct IServo : public servo_types, public IClockable
{
	virtual ~IServo() = default;

	virtual size_t	sweep(angle_t) = 0;
	virtual angle_t	sweep() const = 0;
};

#endif // !defined ISERVO_H__ 