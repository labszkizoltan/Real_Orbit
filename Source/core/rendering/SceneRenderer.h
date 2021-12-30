
#ifndef SCENE_RENDERER_H
#define SCENE_RENDERER_H

#include <core/scene/Scene.h>
#include <core/scene/CoreComponents.h>
#include <core/rendering/Renderer.h>
#include <core/rendering/Shader.h>


class SceneRenderer
{
public:

	SceneRenderer() = default;
	SceneRenderer(std::shared_ptr<Scene> scene);
	~SceneRenderer() = default;

	void SetScene(std::shared_ptr<Scene> scene);

	void RenderScene();

private:
	std::shared_ptr<Scene> m_Scene = nullptr;

};


#endif // SCENE_RENDERER_H
