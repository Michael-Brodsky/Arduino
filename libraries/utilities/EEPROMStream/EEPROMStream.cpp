#include "EEPROMStream.h"

EEPROMStream::address_type& EEPROMStream::address()
{
	return address_;
}

const EEPROMStream::address_type& EEPROMStream::address() const
{
	return address_;
}

void EEPROMStream::reset()
{
	address_ = 0;
}

EEPROMStream::address_type EEPROMStream::get(address_type address, String& value)
{
	// String objects must be read as individual chars, preceded by the count, 
	// with an upper limit of 256 chars.

	return get(address, (char*)value.c_str());
}

EEPROMStream::address_type EEPROMStream::get(address_type address, char* value)
{
	// C-strings must be read as individual chars, preceded by the count, with an 
	// upper limit of 256 chars.

	uint8_t count = EEPROM.read(address);
	address_type first = address;

	for (uint8_t i = 0U; i < count; i++)
		*value++  = static_cast<char>(EEPROM.read(++address));

	*value = '\0';
	return ++address - first;
}

EEPROMStream::address_type EEPROMStream::put(address_type address, const String& value)
{
	// String objects must be written as individual chars, preceded by the count, 
	// with an upper limit of 256 chars.

	return put(address, value.c_str());
}

EEPROMStream::address_type EEPROMStream::put(address_type address, const char* value)
{
	// C-strings must be written as individual chars, preceded by the count, with an 
	// upper limit of 256 chars.

	uint8_t count = strlen(value);
	address_type first = address;

	EEPROM.write(address, count);
	for (uint8_t i = 0U; i < count; i++)
		EEPROM.write(++address, value[i]);

	return ++address - first;
}
