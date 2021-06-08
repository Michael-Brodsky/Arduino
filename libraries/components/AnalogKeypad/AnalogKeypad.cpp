#include "library.h"
#include "AnalogKeypad.h"

Keypad::Keypad(pin_t pin, Callback callback, LongPress lp_mode, msecs_t lp_interval, const Button buttons[], size_t size) : 
    pin_(pin), callback_(callback), buttons_(buttons, size), current_(std_end(buttons_)),
    lp_timer_(lp_interval), lp_interval_(lp_interval), lp_mode_(lp_mode), repeat_()
{

}

Keypad::Keypad(pin_t pin, Callback callback, LongPress lp_mode, msecs_t lp_interval, const Button* first, const Button* last) : 
    pin_(pin), callback_(callback), buttons_(first, last), current_(last),
    lp_timer_(lp_interval), lp_interval_(lp_interval), lp_mode_(lp_mode), repeat_()
{

}

void Keypad::poll()
{
    auto button = readInput();

    if (button == std_end(buttons_))
    {
        if (current_ != std_end(buttons_))
            releaseEvent(current_);
    }
    else if (current_ == std_end(buttons_))
        pressEvent(button);
    else if (lp_mode_ == LongPress::Hold && lp_timer_.expired())
    {
        callback(button, Event::Longpress);
        lp_timer_.stop();
    }
    else
        (repeat_) ? callback(current_, Event::Press) : ((void)0);
    current_ = button;
}

Keypad::const_iterator Keypad::readInput()
{
    analog_t input_level = analogRead(pin_);
    auto button = std_begin(buttons_);

    for (; button < std_end(buttons_); button++)
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

void Keypad::pressEvent(const_iterator button)
{
    callback(button, Event::Press);
    if (lp_mode_ != LongPress::None)
        lp_timer_.start();
}

void Keypad::releaseEvent(const_iterator button)
{
    if (lp_mode_ == LongPress::Release && lp_timer_.expired())
        callback(button, Event::Longpress);
    else 
        callback(button, Event::Release);
    lp_timer_.stop();
    repeat_ = false;
}

void Keypad::clock()
{
    poll();
}

void Keypad::callback(const_iterator button, Event e)
{
    if (callback_)
        (*callback_)(*button, e);
}
