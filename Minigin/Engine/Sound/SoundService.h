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
	virtual void StopAll()
	{
		assert( false && "Attempted to stop all on null service" );
	}
	virtual void Mute()
	{
		assert( false && "Attempted to mute on null service" );
	}
	virtual void UnMute()
	{
		assert( false && "Attempted to unmute on null service" );
	}
};
} // namespace dae
#endif
