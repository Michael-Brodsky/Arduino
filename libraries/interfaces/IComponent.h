/*
 *	This files defines a class for implementing the C++ "Mediator" design 
 *  pattern for component types.
 *
 *	***************************************************************************
 *
 *	File: IComponent.h
 *	Date: April 9, 2021
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

#if !defined ICOMPONENT_H__ 
#define ICOMPONENT_H__ 20210408L

#include "IMediator.h"

// Component interface class.
struct IComponent
{
	IComponent(IMediator* mediator = nullptr) : mediator_(mediator) {} 

	~IComponent() = default;
	
	void mediator(IMediator* mediator) { mediator_ = mediator; }

protected:
	IMediator* mediator_;
};

#endif // !defined ICOMPONENT_H__ 