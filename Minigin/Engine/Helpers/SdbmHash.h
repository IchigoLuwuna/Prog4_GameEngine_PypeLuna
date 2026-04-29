#ifndef SDBMHASH_H
#define SDBMHASH_H
#include <cstddef>

namespace dae
{
consteval size_t Hash( const char* string, size_t length )
{
	size_t hash{};
	for ( size_t index{}; index < length; ++index )
	{
		hash = string[index] + ( hash << 6 ) + ( hash << 16 ) - hash;
	}
	return hash;
}

template <size_t length>
consteval size_t Hash( const char ( &text )[length] )
{
	return Hash( text, length );
}
} // namespace dae

consteval size_t operator""_hash( const char* string, size_t length )
{
	return dae::Hash( string, length );
}
#endif
