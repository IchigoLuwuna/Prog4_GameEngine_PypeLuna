#include "AnimationComponent.h"
#include <cmath>
#include <Helpers.h>

dae::AnimationComponent::AnimationComponent( GameObject* pParent )
	: Component( pParent )
{
}

void dae::AnimationComponent::Update()
{
	const AnimationInfo& currentAnimationInfo{ m_AnimationInfos[m_CurrentAnimation] };
	const uint32_t totalFrames{ currentAnimationInfo.endIdx - currentAnimationInfo.startIdx + 1 };
	const float totalAnimationTime{ totalFrames * currentAnimationInfo.timePerFrame };

	m_AnimationTime += Timer::GetInstance().GetElapsed();
	if ( currentAnimationInfo.repeat && m_AnimationTime >= totalAnimationTime )
	{
		m_AnimationTime = std::fmod( m_AnimationTime, totalAnimationTime );
	}

	uint32_t currentIdx{ static_cast<uint32_t>( std::floor( m_AnimationTime / totalAnimationTime * totalFrames ) ) +
						 currentAnimationInfo.startIdx };

	if ( !m_SpriteSheetRef.Validate() )
	{
		m_SpriteSheetRef = GetParent()->GetComponent<SpriteSheetComponent>();
		assert( m_SpriteSheetRef.Validate() && "Parent GameObject does not have a spritesheet" );
	}

	m_SpriteSheetRef->SetIndex( currentIdx );
}

dae::AnimationComponent& dae::AnimationComponent::AddAnimation( size_t animationHash, const AnimationInfo& info )
{
	m_AnimationInfos[animationHash] = info;
	return *this;
}

dae::AnimationComponent& dae::AnimationComponent::SetAnimation( size_t animationHash )
{
	if ( m_CurrentAnimation == animationHash )
	{
		return *this;
	}

	m_CurrentAnimation = animationHash;
	m_AnimationTime = 0.f;
	return *this;
}
