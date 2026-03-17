#pragma once
#include <vector>
#include <memory>
#include "Scene.h"
#include "Engine/Patterns/Singleton.h"

namespace dae
{
class Scene;
class SceneManager final : public Singleton<SceneManager>
{
public:
	Scene& CreateScene();
	void Destroy();

	void Update();
	void Render();
	void CleanUpRemovableObjects();

private:
	friend class Singleton<SceneManager>;
	SceneManager() = default;
	std::vector<std::unique_ptr<Scene>> m_Scenes{};
};
} // namespace dae
