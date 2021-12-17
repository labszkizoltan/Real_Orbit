
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
	// clear the buffers of the renderer
	Renderer::Refresh();

	std::shared_ptr<Shader> shader = nullptr;

	// set light
	Renderer::SetLightPosition(m_Scene->GetLight().location);

	// render into the shadow map
	//... create a view of all TransformComponents and loop over them, regardless of their mesh type

	// render into the shadow map
	Renderer::s_DepthBuffer->Bind();
	glCullFace(GL_FRONT);

	std::shared_ptr<Shader> shadow_shader = Renderer::s_ShaderLibrary.BindShader(MeshType::SHADOW_MAP);
	for (int i = 0; i < m_Scene->m_MeshLibrary.size(); i++)
	{
		m_Scene->m_MeshLibrary.m_Meshes[i]->SetInstances(m_Scene->m_MeshLibrary.m_MeshTransforms[i]);
		m_Scene->m_MeshLibrary.m_Meshes[i]->Draw();
	}

	Renderer::s_DepthBuffer->Unbind();
	glCullFace(GL_BACK);

	// draw into the framebuffer
	Renderer::s_FrameBuffer->Bind();

	// set camera
	Renderer::SetCamera(m_Scene->GetCamera());

	// draw skybox and meshes:
	Renderer::BindShader(m_Scene->m_Skybox->GetMeshType());
	m_Scene->m_Skybox->Draw();

	for (int i = 0; i < m_Scene->m_MeshLibrary.size(); i++)
	{
		Renderer::BindShader(m_Scene->m_MeshLibrary.m_Meshes[i]->GetMeshType());
		m_Scene->m_MeshLibrary.m_Meshes[i]->Draw(); // no need to set transform data again, it was already uploaded at shadow mapping
	}

	Renderer::s_FrameBuffer->Unbind();
}
