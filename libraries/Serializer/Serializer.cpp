#include "Serializer.h"

size_t Serializer::addr_ = 0;

size_t Serializer::address() const
{
    return addr_;
}

void Serializer::reset()
{
    addr_ = 0;
}
