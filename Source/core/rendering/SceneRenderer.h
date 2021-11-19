
#ifndef SCENE_RENDERER_H
#define SCENE_RENDERER_H

#include <core/scene/Scene.h>

class SceneRenderer
{
public:

	SceneRenderer() = default;
//	SceneRenderer(std::shared_ptr<Scene> scene);
	~SceneRenderer() = default;

	static void RenderScene(std::shared_ptr<Scene> scene);

public:
//	std::shared_ptr<Scene> m_Scene = nullptr;

};


#endif // SCENE_RENDERER_H
