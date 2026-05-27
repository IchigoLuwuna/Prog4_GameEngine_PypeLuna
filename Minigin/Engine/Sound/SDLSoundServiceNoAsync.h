#ifndef SDLSOUNDSERVICENOASYNC_H
#define SDLSOUNDSERVICENOASYNC_H
#include <memory>
#include "Sound/SoundService.h"

namespace dae
{
class SDLSoundServiceNoAsync : public SoundService
{
public:
	SDLSoundServiceNoAsync();
	~SDLSoundServiceNoAsync();

	virtual void Play( const char* path, float volume ) override;

private:
	class Impl;

	std::unique_ptr<Impl> m_pImpl{};
};
} // namespace dae
#endif
