#ifndef SOUNDSERVICE_H
#define SOUNDSERVICE_H
#include <cassert>

namespace dae
{
class SoundService
{
public:
	virtual ~SoundService() = default;

	virtual void Play( const char*, float )
	{
		assert( false && "Attempted to play sound on null service" );
	};
};
} // namespace dae
#endif
