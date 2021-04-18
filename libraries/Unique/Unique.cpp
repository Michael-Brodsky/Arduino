#include "Unique.h"

Unique::uniq_t Unique::next_ = 0;	// Initialize the unique identifier to zero.

Unique::Unique() : 
	id_(next_++) 
{

}

Unique::Unique(const Unique& other) : 
	id_(next_++) 
{

}

Unique& Unique::operator=(const Unique& other) 
{ 
	// `id_' member isn't copied, derived types retain their original unique id.
	return *this; 
}

Unique::uniq_t Unique::id() const
{ 
	return id_; 
};