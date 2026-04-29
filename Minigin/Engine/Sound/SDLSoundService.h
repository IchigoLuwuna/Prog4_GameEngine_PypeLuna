#ifndef SDLSOUNDSERVICE_H
#define SDLSOUNDSERVICE_H
#include "SoundService.h"
#include <memory>

namespace dae
{
class SDLSoundService final : public SoundService
{
public:
	SDLSoundService();
	~SDLSoundService();

	virtual void Play( const char* path, float volume ) override;

private:
	class Impl;

	std::unique_ptr<Impl> m_pImpl{};
};
} // namespace dae
#endif
