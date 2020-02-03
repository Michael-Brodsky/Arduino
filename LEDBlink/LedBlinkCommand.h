#if !defined LEDCOMMAND_H__
#define LEDCOMMAND_H__ 20200204L

class LedCommand : public ICommand 
{
public:
    LedCommand(pin_t led_pin, bool led_on) : 
        ICommand(), led_pin_(led_pin), led_on_(led_on) 
    { 
    }
public:
    void execute() override 
    {
        digitalWrite(led_pin_, led_on_);
    }
protected:
    pin_t   led_pin_;
    bool    led_on_;
};

class LedToggleCommand : public LedCommand 
{
public:
    explicit LedToggleCommand(pin_t led_pin, bool led_on = false) : 
        LedCommand(led_pin, led_on) 
    { 
        pinMode(led_pin_, OUTPUT);
        digitalWrite(led_pin_, led_on_);
    }
public:
    void execute() override 
    {
        led_on_ = !led_on_;
        LedCommand::execute();
    }
};

#endif // !defined LEDCOMMAND_H__
