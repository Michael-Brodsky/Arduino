/*
 *	This file defines an abstract interface class for character display 
 *	managment components.
 * 
 *	***************************************************************************
 *
 *	File: IDisplay.h
 *	Date: April 26, 2021
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
 *	***************************************************************************
 *
 *	Description:
 *
 *		`IDisplay' is an abstract interface class that provides a common
 *		interface for components that manage character displays. 
 *
 *	***************************************************************************/

#if !defined IDISPLAY_H__
# define IDISPLAY_H__ 20210426L

struct IDisplay 
{
	virtual ~IDisplay() = default;

	virtual void refresh() = 0; // Refreshes the display.
};

#endif // !defined IDISPLAY_H__