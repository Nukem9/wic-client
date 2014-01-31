#pragma once

// Defines a pointer typedef
#define MAKEPTR(type, addr, name)	typedef Ptr<type, addr> _##name;

// Defines a pointer typedef and class variable
#define CLASSPTR(type, addr, name)	MAKEPTR(type, addr, name); static _##name name;

// Defines a pointer (in a C++ file) for a class variable
#define DPTR(c, n)					c::_##n c::n

// Hacked together
template<typename T, sizeptr_t Address>
class Ptr
{
	static_assert(Address > 0, "Address bound to a zero value");

public:
	inline void operator = (const T& Value)
	{
		*(T *)Address = Value;
	}

	inline T operator -> () const 
	{
		return *(T *)Address;
	}

	inline operator T& () const
	{
		return *(T *)Address;
	}
};