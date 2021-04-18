/*
 *	This files declares a class that encapsulates behaviors of a keypad 
 *  attached to an analog GPIO input.
 *
 *	***************************************************************************
 *
 *	File: AnalogKeypad.h
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
 *	**************************************************************************
 *
 *	Description:
 *
 *      The `Keypad' class encapsulates the behaviors of a keypad (buttons) 
 *      attached to an analog input pin. It is a generic, reusable type that 
 *      can be parameterized to fit a particular application. It supports any 
 *      number of buttons and the standard button events, such as `press', 
 *      `release', `longpress' and `repeat'. `Keypad' objects can be operated 
 *      asynchronously using the `clock()' method, or synchronously with the 
 *      `poll()' method. Communication with the client is handled via a 
 *      callback that must be implemented by the client. The button and event 
 *      type (press, release or longpress) are passed to the client in the 
 *      callback. 
 * 
 *      Keypad buttons are encapsulated by the nested `Button' class which  
 *      contains a `ButtonTag' field that uniquely identifies each button. 
 *      Since the buttons and their functions are implementation-specific, 
 *      the `ButtonTag' type is only forward declared and must be defined by 
 *      the client, and the definition must be visible to the `Keypad' class. 
 *      Buttons are passed as a collection the the `Keypad' class constructor. 
 * 
 *      Longpress events can occur in one of three ways, when a button is 
 *      held down, after a button is released, or they can be disabled. The 
 *      longpress modes are enumerated by the `Longpress' type and the mode 
 *      is specified by the client at time of construction, as is the long-
 *      press interval which determines how long it takes to trigger the 
 *      event in milliseconds. 
 * 
 *      The `Keypad' class also provides a mechanism to to repeatedly fire 
 *      the button `press' event. Clients enable/disable the repeat mode 
 *      using the `repeat()' method. The `Keypad' class only provides the 
 *      mechanism, the repeat logic is implemented by the client and can be 
 *      enabled or disabled by the client at any time, regardless of the  
 *      internal state of the `Keypad' class. The repeat event is triggered 
 *      by each call to `poll()' and thus fires at the same rate that the 
 *      keypad is polled.
 * 
 *	Examples:
 *
 *      // Client defines the `ButtonTag's, usually to indicate each button's function. 
 *      enum class ButtonTag { Up, Down, Left, Right };
 * 
 *      // Client defines the callback function that handles button events.
 *      void callback(const Keypad::Button&, Keypad::Event) { ... };
 * 
 *      // Client instantiates a button collection. Each button must have a unique 
 *      // analog triggering level (see `analogRead()') and, the buttons MUST be 
 *      // instantiated in increasing order of this level within the collection, from lowest to highest.
 *      Keypad::Button buttons[] = { Keypad::Button(ButtonTag::Up, 0), Keypad::Button(ButtonTag::Up, 42), ... }; 
 * 
 *      // Client instantiates the `Keypad' object specifying the analog input 
 *      // pin to attach, the callback, the longpress mode and interval and 
 *      // the button collection.
 *      Keypad keypad(0, Keypad::Callaback(&callback), Keypad::Longpress::Hold, 1000U, buttons);
 * 
 *      // Client polls the keypad in a loop.
 *      void loop() { 
 *          keypad.poll();
 *      }
 * 
 *      If a button event occurs, the keypad object calls the client callback
 *      function, passing it the button and the type of event.
 * 
 *  Notes:
 * 
 *      It is absolutely essential that button collections appear in increasing 
 *      order of each button's `trigger_level', from lowest to highest. This is 
 *      required because in each iteration through the button collection, each 
 *      button's `trigger_level' is less-than compared to the value returned by 
 *      the `analogRead()' function. If a button with a higher `trigger_level' 
 *      appears in the collection before buttons with lower values, it will 
 *      prevent those buttons from being triggered. 
 * 
 *	**************************************************************************/

#if !defined ANALOGKEYPAD_H__ 
# define ANALOGKEYPAD_H__ 20210409L

#include "IClockable.h"	// `IClockable' interface.
#include "IComponent.h"	// `IComponent' interface.
#include "Timer.h"	    // `Timer' type.

enum class ButtonTag;       // Forward decl, button tags are user-defined.

// Type that encapsulates behaviors of a keypad attached to an analog GPIO input.
class Keypad : public IClockable, public IComponent
{
public:
    // Enumerates valid keypad events.
    enum class Event
    {
        Press,
        Longpress,
        Release
    };

    // Keypad button type.
    struct Button
    {
        ButtonTag tag_;				// Tag uniquely identifying a button.
        analog_t  trigger_level_;	// The analog input triggering level. 
                                    // Button collections MUST be instantiated in increasing order of 
                                    // the `trigger_level_' field, from lowest to highest.

        Button(ButtonTag tag, analog_t trigger_level) : 
            tag_(tag), trigger_level_(trigger_level) 
        {}
    };

    // Enumerates the valid longpress event modes.
    enum class LongPress
    {
        Hold,	    // Event is triggered while button is held down.
        Release,    // Event is triggered after button is released.
        None        // Event is never triggered.
    };

    using Callback = void(*)(const Button&, Event); // Client callback type.
    using iterator = Button*;                       // Button collection iterator type.

public:
    template <size_t Size>
    Keypad(pin_t, Callback, LongPress, msecs_t, Button (&)[Size]);
    Keypad(pin_t, Callback, LongPress, msecs_t, Button[], size_t size);
    Keypad(pin_t, Callback, LongPress, msecs_t, iterator begin, iterator end);

public:
    // Polls the keypad for button events.
    void        poll();
    // Sets the button repeat state.
    void	    repeat(bool);

private:
    // Reads the attached pin's input level and returns the currently pressed button, if any.
    iterator    readInput();
    // Button press event handler.
    void        pressEvent(iterator);
    // Button release event handler.
    void        releaseEvent(iterator);
    // Calls the `poll()' method.
    void        clock() override;

private:
    pin_t       pin_;           // The attached analog GPIO input pin.
    Callback    callback_;      // Client callback.
    iterator    begin_;         // Points to the beginning of the button collection. 
    iterator    end_;           // Points to the end of the button collection. 
    iterator    current_;       // Points to the currently triggered button. 
    Timer       lp_timer_;	    // Keypad longpress event timer.
    msecs_t     lp_interval_;   // Keypad longpress event interval.
    LongPress   lp_mode_;       // Keypad longpress mode.
    bool        repeat_;	    // Flag indicating whether the button press callback is executed on each call to `poll()'.
};

template <size_t Size>
Keypad::Keypad(pin_t pin, Callback callback, LongPress lp_mode, msecs_t lp_interval, Button(&buttons)[Size]) :
    pin_(pin), callback_(callback), begin_(buttons), end_(buttons + Size), current_(end_), 
    lp_timer_(lp_interval), lp_interval_(lp_interval), lp_mode_(lp_mode), repeat_()
{

}

#endif //!defined ANALOGKEYPAD_H__ 
