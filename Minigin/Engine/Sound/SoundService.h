#ifndef SOUNDSERVICE_H
#define SOUNDSERVICE_H
namespace dae
{
class SoundService
{
public:
	virtual ~SoundService() = default;

	virtual void Play( const char* path, float volume ) = 0;
};
} // namespace dae
#endif
