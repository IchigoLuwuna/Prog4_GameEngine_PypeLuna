#ifndef SDBMHASH_H
#define SDBMHASH_H
#include <cstddef>

namespace dae
{
template <size_t length>
class SdbmHash
{
public:
	static consteval size_t Calculate( const char* text )
	{
		size_t value{};
		const size_t nextChar{ CalculateNext( text, value ) };
		return nextChar + ( value << 6 ) + ( value << 16 ) - value;
	}

	static consteval size_t CalculateNext( const char* text, size_t& value )
	{
		const size_t nextChar{ SdbmHash<length - 1>::CalculateNext( text, value ) };
		value = nextChar + ( value << 6 ) + ( value << 16 ) - value;
		return text[length - 1];
	}
};

template <>
class SdbmHash<1>
{
public:
	static consteval size_t CalculateNext( const char* text, size_t& )
	{
		return text[0];
	}
};

template <size_t length>
consteval size_t Hash( const char ( &text )[length] )
{
	return SdbmHash<length - 1>::Calculate( text );
}
} // namespace dae
#endif
