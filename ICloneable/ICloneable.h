/*
 *  This file defines an abstract interface class for creating/cloning 
 *  objects using the C++ "Virtual Constructor Idiom".
 */

#if !defined ICLONEABLE_H__
#define ICLONEABLE_H__ 20200124L

#endif // !defined ICLONEABLE_H__

struct ICloneable 
{
    virtual ~ICloneable() { }
    
    virtual ICloneable* clone() const = 0;  // Uses the copy constructor
    virtual ICloneable* create() const = 0; // Uses the default constructor
};
