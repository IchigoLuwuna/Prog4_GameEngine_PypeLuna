#ifndef DEBUGSOUNDSERVICE_H
#define DEBUGSOUNDSERVICE_H
#include <memory>
#include "Engine/Sound/SoundService.h"

namespace dae
{
class DebugSoundService : public SoundService
{
public:
	DebugSoundService( std::unique_ptr<SoundService>&& pSoundService );

	virtual void Play( const char* path, float volume ) override;

private:
	std::unique_ptr<SoundService> m_pUnderlyingService{};
};
} // namespace dae
#endif
