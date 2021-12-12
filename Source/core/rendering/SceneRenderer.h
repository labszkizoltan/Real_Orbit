
#ifndef SCENE_RENDERER_H
#define SCENE_RENDERER_H

#include <core/scene/Scene.h>
#include <core/scene/Components.h>
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

	template <MeshType Type>
	void DrawMeshType();

	template <MeshType Type>
	void ShadowMapMeshType(std::shared_ptr<Shader> shader);

private:
	std::shared_ptr<Scene> m_Scene = nullptr;

};


template<MeshType Type>
inline void SceneRenderer::DrawMeshType()
{
	std::shared_ptr<Shader> shader = Renderer::s_ShaderLibrary.BindShader(Type);
	auto mesh_group = m_Scene->m_Registry.view<TransformComponent, TypedMeshComponent<Type>>();
	for (auto entity : mesh_group)
	{
		TransformComponent& trf = mesh_group.get<TransformComponent>(entity);
		TypedMeshComponent<Type>& mesh = mesh_group.get<TypedMeshComponent<Type>>(entity);

		shader->UploadUniformFloat3("body_location", trf.location.Glm());
		shader->UploadUniformMat3("body_orientation", trf.orientation.Glm());
		shader->UploadUniformFloat("body_scale", trf.scale);

//		mesh.meshPtr->Draw();
	}
}

template<MeshType Type>
inline void SceneRenderer::ShadowMapMeshType(std::shared_ptr<Shader> shader)
{
	auto mesh_group = m_Scene->m_Registry.view<TransformComponent, TypedMeshComponent<Type>>();
	for (auto entity : mesh_group)
	{
		TransformComponent& trf = mesh_group.get<TransformComponent>(entity);
		TypedMeshComponent<Type>& mesh = mesh_group.get<TypedMeshComponent<Type>>(entity);

		shader->UploadUniformFloat3("body_location", trf.location.Glm());
		shader->UploadUniformMat3("body_orientation", trf.orientation.Glm());
		shader->UploadUniformFloat("body_scale", trf.scale);

//		mesh.meshPtr->Draw();
	}
}


#endif // SCENE_RENDERER_H
