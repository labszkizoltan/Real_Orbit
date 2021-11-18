

//https://skypjack.github.io/entt/md_docs_md_entity.html


#include "SceneRenderer.h"
#include "Renderer.h"

#include <core/scene/Components.h>

SceneRenderer::SceneRenderer(std::shared_ptr<Scene> scene)
	: m_Scene(scene)
{
}

void SceneRenderer::RenderScene()
{
	// the scene class could have a built in camera entity or a reference to a transform component, that describes the camera
	// same goes for light

	// Also the stages of this function could go into smaller private functions later on

	// set light
	//...

	// render into the shadow map
	//... create a view of all TransformComponents and loop over them, regardless of their mesh type
	// std::shared_ptr<Shader> shader = Renderer::s_ShaderLibrary.BindShader(MeshType::SHADOW_MAP);

	// set camera
	// somehow the camera needs to be obtained
	// maybe this will do:
	// auto group = m_Scene->m_Registry.group<TransformComponent, MeshComponent>(entt::exclude<MeshComponent>);
	// auto group = m_Scene->m_Registry.group<>(entt::get<TransformComponent, MeshComponent>, entt::exclude<MeshComponent>);

	auto camera_trf_view = m_Scene->m_Registry.view<CameraComponent>();
	for (auto entity : camera_trf_view)
	{
		auto& trf = camera_trf_view.get<CameraComponent>(entity);
		Renderer::SetCamera(trf.camera_transform);
	}

	// draw the skybox
	auto skybox_view = m_Scene->m_Registry.view<TypedMeshComponent<MeshType::SKYBOX>>();
	// auto something = skybox_view.begin();
	for (auto entity : skybox_view)
	{
		// Renderer::s_ShaderLibrary.BindShader(TypedMeshComponent<MeshType::SKYBOX>::GetStaticType()); // this should work, but looks awfully overcomplicated
		std::shared_ptr<Shader> shader = Renderer::s_ShaderLibrary.BindShader(MeshType::SKYBOX);
		auto& skyb_mesh = skybox_view.get<TypedMeshComponent<MeshType::SKYBOX>>(entity);
		skyb_mesh.meshPtr->Draw(); // the draw function of the skybox will internally turn off, then on the depth testing
	}

	// draw all other entities by MeshType
	//----- COLOURED_MESH -----//
	std::shared_ptr<Shader> shader = Renderer::s_ShaderLibrary.BindShader(MeshType::COLOURED_MESH);
	auto coloured_mesh_group = m_Scene->m_Registry.group<TransformComponent, TypedMeshComponent<MeshType::COLOURED_MESH>>();
	for (auto entity : coloured_mesh_group)
	{
		TransformComponent& trf = coloured_mesh_group.get<TransformComponent>(entity);
		TypedMeshComponent<MeshType::COLOURED_MESH>& mesh = coloured_mesh_group.get<TypedMeshComponent<MeshType::COLOURED_MESH>>(entity);

		shader->UploadUniformFloat3("body_location", trf.location.Glm());
		shader->UploadUniformMat3("body_orientation", trf.orientation.Glm());
		shader->UploadUniformFloat("body_scale", trf.scale);

		mesh.meshPtr->Draw();
	}

	//----- TEXTURED_MESH -----//
	shader = Renderer::s_ShaderLibrary.BindShader(MeshType::TEXTURED_MESH);
	auto textured_mesh_group = m_Scene->m_Registry.group<TransformComponent, TypedMeshComponent<MeshType::TEXTURED_MESH>>();
	for (auto entity : textured_mesh_group)
	{
		TransformComponent& trf = textured_mesh_group.get<TransformComponent>(entity);
		TypedMeshComponent<MeshType::TEXTURED_MESH>& mesh = textured_mesh_group.get<TypedMeshComponent<MeshType::TEXTURED_MESH>>(entity);

		shader->UploadUniformFloat3("body_location", trf.location.Glm());
		shader->UploadUniformMat3("body_orientation", trf.orientation.Glm());
		shader->UploadUniformFloat("body_scale", trf.scale);

		mesh.meshPtr->Draw();
	}

	//----- NORMAL_MESH -----//
	shader = Renderer::s_ShaderLibrary.BindShader(MeshType::NORMAL_MESH);
	auto normal_mesh_group = m_Scene->m_Registry.group<TransformComponent, TypedMeshComponent<MeshType::NORMAL_MESH>>();
	for (auto entity : normal_mesh_group)
	{
		TransformComponent& trf = normal_mesh_group.get<TransformComponent>(entity);
		TypedMeshComponent<MeshType::NORMAL_MESH>& mesh = normal_mesh_group.get<TypedMeshComponent<MeshType::NORMAL_MESH>>(entity);

		shader->UploadUniformFloat3("body_location", trf.location.Glm());
		shader->UploadUniformMat3("body_orientation", trf.orientation.Glm());
		shader->UploadUniformFloat("body_scale", trf.scale);

		mesh.meshPtr->Draw();
	}
}
