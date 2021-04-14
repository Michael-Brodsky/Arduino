#include "library.h"
#include "AnalogKeypad.h"

Keypad::Keypad(pin_t pin, Callback callback, LongPress lp_mode, msecs_t lp_interval, Button buttons[], size_t size) :
    pin_(pin), callback_(callback), begin_(buttons), end_(buttons + size), current_(end_),
    lp_timer_(lp_interval), lp_interval_(lp_interval), lp_mode_(lp_mode), repeat_()
{

}

Keypad::Keypad(pin_t pin, Callback callback, LongPress lp_mode, msecs_t lp_interval, iterator begin, iterator end) :
    pin_(pin), callback_(callback), begin_(begin), end_(end), current_(end_),
    lp_timer_(lp_interval), lp_interval_(lp_interval), lp_mode_(lp_mode), repeat_()
{

}

void Keypad::poll()
{
    auto button = readInput();

    if (button == end_)
    {
        if (current_ != end_)
            releaseEvent(current_);
    }
    else if (current_ == end_)
        pressEvent(button);
    else if (lp_mode_ == LongPress::Hold && lp_timer_.expired())
    {
        (*callback_)(*button, Event::Longpress);
        lp_timer_.stop();
    }
    else
        (repeat_) ? (*callback_)(*current_, Event::Press) : ((void)0);
    current_ = button;
}

Keypad::iterator Keypad::readInput()
{
    analog_t input_level = analogRead(pin_);
    auto button = begin_;

    for (; button < end_; button++)
    {
        if (input_level < (*button).trigger_level_)
            break;
    }

    return button;
}

void Keypad::repeat(bool value)
{
    repeat_ = value;
}

void Keypad::pressEvent(iterator button)
{
    (*callback_)(*button, Event::Press);
    if (lp_mode_ != LongPress::None)
        lp_timer_.start();
}

void Keypad::releaseEvent(iterator button)
{
    if (lp_mode_ == LongPress::Release && lp_timer_.expired())
        (*callback_)(*button, Event::Longpress);
    else
        (*callback_)(*button, Event::Release);
    lp_timer_.stop();
    repeat_ = false;
}

void Keypad::clock()
{
    poll();
}
