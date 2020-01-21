/*
 * This file defines an abstract interface class for asynchrounous, event-
 * driven objects.
 */
#if !defined ICLOCKABLE_H__
#define ICLOCKABLE_H__ 20191031L

struct IClockable							// Asynhronous object abstract interface class.
{
	virtual ~IClockable() {}

	virtual void clock() = 0;
};

template <class T>
static IClockable* iclockable(T& object)	// Converts a pointer of a derived type to a pointer of type IClockable.
{
	return static_cast<IClockable*>(&object);
}

template <class T>
static void iclock(T& object)				// Calls a derived type's `clock()' implementation.
{
	iclockable(object)->clock();
}

#endif // !defined ICLOCKABLE_H__
