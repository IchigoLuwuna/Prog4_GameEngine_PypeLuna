#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H
#include <vector>
#include <memory>
#include "Scene.h"
#include "Patterns/Singleton.h"

namespace dae
{
class Scene;
class SceneManager final : public Singleton<SceneManager>
{
public:
	Scene& CreateScene();
	void Destroy();

	void Update();
	void Render() const;
	void CleanUpRemovableObjects();

	Scene& GetScene( size_t idx ) const;

	void EnableScene( size_t idx );
	void DisableScene( size_t idx );

private:
	friend class Singleton<SceneManager>;
	SceneManager() = default;
	std::vector<std::unique_ptr<std::pair<Scene, bool>>> m_Scenes{};
};
} // namespace dae
#endif
