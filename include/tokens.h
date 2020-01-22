/*	File: tokens.h
 *	Date: September 27, 2015
 *	Version: 0.99
 *	Author: Michael Brodsky
 *	Email: superhotmuffin@hotmail.com
 *	Copyright (c) 2012-2016 Michael Brodsky 
 */

/*	**************************************************************************
 *
 *  This file (tokens.h) is free software: you can redistribute it and/or 
 *	modify it under the terms of the GNU General Public License as 
 *	published  by the Free Software Foundation, either version 3 of the 
 *	License, or (at your option) any later version.
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
 *	This file defines three macros for manipulating preprocessor tokens.
 *
 *	***************************************************************************
 *
 *	Usage:
 *
 *	#include "tokens.h"
 *
 *	***************************************************************************
 *
 *	Overview:
 *
 *	This file defines the following macros:
 *
 *		STRINGIZE(a):		Converts the token `a' into a string literal, 
 *		CONCAT(a, b):		Concatenates tokens `a' and `b' into `ab'.
 *		CONCAT3(a, b, c):	Concatenates tokens `a', `b' and `c' into `abc'.
 *		
 *	This file also defines two `helper' macros, STRINGIZE__(a) and 
 *	CONCAT__(a, b) that are used by the other macros so they work properly. 
 *	The helper macros should not be called directly.
 *
 *	***************************************************************************/

#ifndef TOKENS_H__
#define TOKENS_H__	20150927L

#if !(defined(STRINGIZE) || defined(STRINGIZE__))
# define STRINGIZE__(a)		#a
/* Converts a token into a string literal: token -> "token". */
# define STRINGIZE(a)		STRINGIZE__(a)
#endif /*!defined(STRINGIZE) ... */

#if !(defined(CONCAT3) || defined(CONCAT) || defined(CONCAT__))
# define CONCAT__(a, b)		a##b
/* Concatenates tokens a and b into ab. */
# define CONCAT(a, b)		CONCAT__(a, b)
/* Concatenates tokens a, b and c into abc. */
# define CONCAT3(a, b, c)	CONCAT__(CONCAT__(a, b), c)
#endif /* !defined(CONCAT3) ... */

#endif /* !TOKENS_H__ */