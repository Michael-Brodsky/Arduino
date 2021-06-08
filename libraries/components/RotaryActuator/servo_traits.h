/*
 *	This file declares servo traits types.
 *
 *	***************************************************************************
 *
 *	File: servo_traits.h
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

#if !defined SERVO_TRAITS_H__ 
# define SERVO_TRAITS_H__ 20210531L

# include <types.h> // `Arduino and `stdint' types.

// Common servo signaling, and control data types and constants.
struct servo_types 
{
	using angle_t = uint16_t;						// Type that can hold any angle in degrees.
	using anglediff_t = int16_t;					// Type that can hold the difference of any angles in degrees.
	using step_t = usecs_t;							// Type that can hold any pulse width (step) in microseconds.
	using stepdir_t = long;							// Type that can hold the difference of any pulse widths (direction) in microseconds.
	static const angle_t InvalidAngle = UINT16_MAX;	// Constant indicating an invalid angle.
	static const step_t InvalidStep = UINT32_MAX;	// Constant indicating an invalid pulse width (step).
};

// Default servo type traits and constants.
template<class T>
struct servo_traits : public servo_types  
{
	using type = servo_traits<T>;									// Type traits type alias.

	static const step_t	min_pulse_width = T::min_pulse_width;		// Absolute minimum pulse width in microseconds.
	static const step_t	max_pulse_width = T::max_pulse_width;		// Absolute maximum pulse width in microseconds.
	static const step_t	max_step_size = T::max_step_size;			// Maximum pulse width change per step in microseconds.
	static const usecs_t min_step_interval = T::min_step_interval;	// Minimum time interval between steps in microseconds.
	static const msecs_t min_sweep_delay = T::min_sweep_delay;		// Minimum time interval between sweeps in milliseconds.
	static const angle_t max_control_angle = T::max_control_angle;	// Absolute maximum rotation angle in degrees.
};

#endif // !defined SERVO_TRAITS_H__ 