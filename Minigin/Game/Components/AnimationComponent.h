#ifndef ANIMATIONCOMPONENT_H
#define ANIMATIONCOMPONENT_H
#include <unordered_map>
#include <Patterns.h>
#include "SpriteSheetComponent.h"

namespace dae
{
class AnimationComponent final : public Component
{
public:
	enum class LoopingMode : uint8_t
	{
		repeat,
		single,
		singleAndTerminate
	};

	struct AnimationInfo
	{
		uint32_t startIdx{};
		uint32_t endIdx{};
		float timePerFrame{};
		LoopingMode loopingMode{};
	};

	AnimationComponent( GameObject* pParent );

	virtual void Update() override;

	bool IsAtEnd() const;

	AnimationComponent& AddAnimation( size_t animationHash, const AnimationInfo& info );
	AnimationComponent& SetAnimation( size_t animationHash );

private:
	ReferencePtr<SpriteSheetComponent> m_SpriteSheetRef;
	std::unordered_map<size_t, AnimationInfo> m_AnimationInfos{};
	size_t m_CurrentAnimation{};
	float m_AnimationTime{};
};
} // namespace dae
#endif
