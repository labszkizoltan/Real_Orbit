

//https://skypjack.github.io/entt/md_docs_md_entity.html


#include "SceneRenderer.h"


SceneRenderer::SceneRenderer(std::shared_ptr<Scene> scene)
	: m_Scene(scene)
{
}

void SceneRenderer::SetScene(std::shared_ptr<Scene> scene)
{
	m_Scene = scene;
}

//void SceneRenderer::RenderScene(std::shared_ptr<Scene> scene)
void SceneRenderer::RenderScene()
{
	std::shared_ptr<Shader> shader = nullptr;

	// set light
	Renderer::SetLightPosition(m_Scene->GetLight().light_transform.location);

//	auto light_trf_view = m_Scene->m_Registry.view<LightComponent>();
//	for (auto entity : light_trf_view)
//	{
//		auto& trf = light_trf_view.get<LightComponent>(entity);
//		Renderer::SetLightPosition(trf.light_transform.location);
//	}

	// render into the shadow map
	//... create a view of all TransformComponents and loop over them, regardless of their mesh type
	// std::shared_ptr<Shader> shader = Renderer::s_ShaderLibrary.BindShader(MeshType::SHADOW_MAP);

	Renderer::s_DepthBuffer->Bind();
	glCullFace(GL_FRONT);

	shader = Renderer::s_ShaderLibrary.BindShader(MeshType::SHADOW_MAP);
	ShadowMapMeshType<MeshType::COLOURED_MESH>(shader);
	ShadowMapMeshType<MeshType::NORMAL_MESH>(shader);

	Renderer::s_DepthBuffer->Unbind();
	glCullFace(GL_BACK);

	// set camera
	Renderer::SetCamera(m_Scene->GetCamera().camera_transform);

	// there should be only one camera in a scene
//	auto camera_trf_view = m_Scene->m_Registry.view<CameraComponent>();
//	for (auto entity : camera_trf_view)
//	{
//		auto& trf = camera_trf_view.get<CameraComponent>(entity);
//		Renderer::SetCamera(trf.camera_transform);
//	}

	// draw the skybox
	shader = Renderer::s_ShaderLibrary.BindShader(MeshType::SKYBOX);
	auto skybox_view = m_Scene->m_Registry.view<TypedMeshComponent<MeshType::SKYBOX>>();
	for (auto entity : skybox_view)
	{
		auto& skyb_mesh = skybox_view.get<TypedMeshComponent<MeshType::SKYBOX>>(entity);
//		skyb_mesh.meshPtr->Draw(); // the draw function of the skybox will internally turn off, then on the depth testing
	}

	DrawMeshType<MeshType::COLOURED_MESH>();
	DrawMeshType<MeshType::NORMAL_MESH>();
}
