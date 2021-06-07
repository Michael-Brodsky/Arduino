/*
 *	This file defines servo type traits for specific hardware.
 *
 *	***************************************************************************
 *
 *	File: servos.h
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

#if !defined SERVOS_H__ 
# define SERVOS_H__ 20210531L

# include "servo_traits.h"

struct hiwonder_20 {};

template<>
struct servo_traits<hiwonder_20> : public servo_types
{
	using type = servo_traits<hiwonder_20>;			// These type traits type alias.

	static const step_t	min_pulse_width = 544U;		// Absolute minimum pulse width in microseconds.
	static const step_t	max_pulse_width = 2574U;	// Absolute maximum pulse width in microseconds.
	static const step_t	max_step_size = 160U;		// Maximum pulse width change per step in microseconds.
	static const usecs_t min_step_interval = 600U;	// Minimum time interval between steps in microseconds.
	static const msecs_t min_sweep_delay = 250U;	// Minimum time interval between sweeps in milliseconds.
	static const angle_t max_control_angle = 180U;	// Absolute maximum rotation angle in degrees.
};

// Computes the step interval for a particular servo and step size.
template<class T>
static usecs_t step_interval(usecs_t stepSize)
{
	return stepSize * T::min_step_interval + (stepSize > 1U ? 2U : 0);
}

#endif // !defined SERVOS_H__