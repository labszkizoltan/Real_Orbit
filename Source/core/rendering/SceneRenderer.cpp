

//https://skypjack.github.io/entt/md_docs_md_entity.html


#include "SceneRenderer.h"
#include "Renderer.h"

#include <core/scene/Components.h>

// SceneRenderer::SceneRenderer(std::shared_ptr<Scene> scene)
// 	: m_Scene(scene)
// {
// }

void SceneRenderer::RenderScene(std::shared_ptr<Scene> scene)
{
	// the scene class could have a built in camera entity or a reference to a transform component, that describes the camera
	// same goes for light

	// Also the stages of this function could go into smaller private functions later on

	std::shared_ptr<Shader> shader = nullptr;

	// set light
	//...
	auto light_trf_view = scene->m_Registry.view<LightComponent>();
	for (auto entity : light_trf_view)
	{
		auto& trf = light_trf_view.get<LightComponent>(entity);
		Renderer::SetLightPosition(trf.light_transform.location);
	}

	// render into the shadow map
	//... create a view of all TransformComponents and loop over them, regardless of their mesh type
	// std::shared_ptr<Shader> shader = Renderer::s_ShaderLibrary.BindShader(MeshType::SHADOW_MAP);

	Renderer::s_DepthBuffer->Bind();
	glCullFace(GL_FRONT);

	shader = Renderer::s_ShaderLibrary.BindShader(MeshType::SHADOW_MAP);
	auto shadow_mesh_group_1 = scene->m_Registry.view<TransformComponent, TypedMeshComponent<MeshType::COLOURED_MESH>>();
	for (auto entity : shadow_mesh_group_1)
	{
		TransformComponent& trf = shadow_mesh_group_1.get<TransformComponent>(entity);
		TypedMeshComponent<MeshType::COLOURED_MESH>& mesh = shadow_mesh_group_1.get<TypedMeshComponent<MeshType::COLOURED_MESH>>(entity);

		shader->UploadUniformFloat3("body_location", trf.location.Glm());
		shader->UploadUniformMat3("body_orientation", trf.orientation.Glm());
		shader->UploadUniformFloat("body_scale", trf.scale);

		mesh.meshPtr->Draw();
	}
	auto shadow_mesh_group_2 = scene->m_Registry.view<TransformComponent, TypedMeshComponent<MeshType::TEXTURED_MESH>>();
	for (auto entity : shadow_mesh_group_2)
	{
		TransformComponent& trf = shadow_mesh_group_2.get<TransformComponent>(entity);
		TypedMeshComponent<MeshType::TEXTURED_MESH>& mesh = shadow_mesh_group_2.get<TypedMeshComponent<MeshType::TEXTURED_MESH>>(entity);

		shader->UploadUniformFloat3("body_location", trf.location.Glm());
		shader->UploadUniformMat3("body_orientation", trf.orientation.Glm());
		shader->UploadUniformFloat("body_scale", trf.scale);

		mesh.meshPtr->Draw();
	}
	auto shadow_mesh_group_3 = scene->m_Registry.group<TransformComponent, TypedMeshComponent<MeshType::NORMAL_MESH>>();
	for (auto entity : shadow_mesh_group_3)
	{
		TransformComponent& trf = shadow_mesh_group_3.get<TransformComponent>(entity);
		TypedMeshComponent<MeshType::NORMAL_MESH>& mesh = shadow_mesh_group_3.get<TypedMeshComponent<MeshType::NORMAL_MESH>>(entity);

		shader->UploadUniformFloat3("body_location", trf.location.Glm());
		shader->UploadUniformMat3("body_orientation", trf.orientation.Glm());
		shader->UploadUniformFloat("body_scale", trf.scale);

		mesh.meshPtr->Draw();
	}

	Renderer::s_DepthBuffer->Unbind();
	glCullFace(GL_BACK);

	// set camera
	// somehow the camera needs to be obtained
	// maybe this will do:
	// auto group = scene->m_Registry.group<TransformComponent, MeshComponent>(entt::exclude<MeshComponent>);
	// auto group = scene->m_Registry.group<>(entt::get<TransformComponent, MeshComponent>, entt::exclude<MeshComponent>);

	auto camera_trf_view = scene->m_Registry.view<CameraComponent>();
	for (auto entity : camera_trf_view)
	{
		auto& trf = camera_trf_view.get<CameraComponent>(entity);
		Renderer::SetCamera(trf.camera_transform);
	}

	// draw the skybox
	shader = Renderer::s_ShaderLibrary.BindShader(MeshType::SKYBOX);
	auto skybox_view = scene->m_Registry.view<TypedMeshComponent<MeshType::SKYBOX>>();
	// auto something = skybox_view.begin();
	for (auto entity : skybox_view)
	{
		// Renderer::s_ShaderLibrary.BindShader(TypedMeshComponent<MeshType::SKYBOX>::GetStaticType()); // this should work, but looks awfully overcomplicated
		auto& skyb_mesh = skybox_view.get<TypedMeshComponent<MeshType::SKYBOX>>(entity);
		skyb_mesh.meshPtr->Draw(); // the draw function of the skybox will internally turn off, then on the depth testing
	}

	// draw all other entities by MeshType
	// only ione of these can be a group, the rest should be view
	//----- COLOURED_MESH -----//
	shader = Renderer::s_ShaderLibrary.BindShader(MeshType::COLOURED_MESH);
	auto coloured_mesh_group = scene->m_Registry.view<TransformComponent, TypedMeshComponent<MeshType::COLOURED_MESH>>();
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
	auto textured_mesh_group = scene->m_Registry.view<TransformComponent, TypedMeshComponent<MeshType::TEXTURED_MESH>>();
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
	auto normal_mesh_group = scene->m_Registry.group<TransformComponent, TypedMeshComponent<MeshType::NORMAL_MESH>>();
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
