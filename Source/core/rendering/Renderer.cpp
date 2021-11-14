
#include "Renderer.h"
#include <core/rendering/shader_source_files/ShaderSourceCollection.h>
//#include <core/rendering/shader_source_files/basic_3D_shaders.h>
#include <core/scene/Components.h>
#include <core/GlobalConstants.h>

float Renderer::s_AspectRatio = 1.0f;
std::shared_ptr<Depthbuffer> Renderer::s_DepthBuffer = nullptr;
ShaderLibrary Renderer::s_ShaderLibrary;

Renderer::~Renderer()
{
}

int Renderer::Init()
{
	// initialize GLAD
	int result = gladLoadGL();
	if (!result)
		std::cout << "Failed to initialize OpenGL context\n";

	// Create the framebuffer for the shadow map:
	FrameBufferSpecification fbspec;
	fbspec.Width = 4096;
	fbspec.Height = 4096;
	s_DepthBuffer = std::shared_ptr<Depthbuffer>(new Depthbuffer(fbspec));
	s_DepthBuffer->GetDepthAttachment()->SetSlot(1);
	s_DepthBuffer->GetDepthAttachment()->Bind();

	// Add the shaders to the shader library

	// MeshType::COLOURED_MESH
	s_ShaderLibrary.AddShader(
		std::string(perspective_3d_shader_vertexSrc),
		std::string(perspective_3d_shader_fragmentSrc)
	);

	// MeshType::TEXTURED_MESH
	s_ShaderLibrary.AddShader(
		std::string(textured_3d_shader_vertexSrc),
		std::string(textured_3d_shader_fragmentSrc)
	);

	// MeshType::NORMAL_MESH
	s_ShaderLibrary.AddShader(
		std::string(normal_shader_vertexSrc),
		std::string(normal_shader_fragmentSrc)
	);

	// MeshType::SKYBOX
	s_ShaderLibrary.AddShader(
		std::string(skybox_shader_vertexSrc),
		std::string(skybox_shader_fragmentSrc)
	);


	// Shadow mapper
	s_ShaderLibrary.AddShader(
		std::string(shadow_mapper_vertexSrc),
		std::string(shadow_mapper_fragmentSrc)
	);


	
//	// Add subsequent shader here:
//	// MeshType::OTHER_MESH_TYPE
//	s_ShaderLibrary.AddShader(
//		ParseShader("Source/core/rendering/shader_source_files/other_3D_vertex_shd.glsl"),
//		ParseShader("Source/core/rendering/shader_source_files/other_3D_fragment_shd.glsl")
//	);


	SetAspectRatio(s_AspectRatio);

	TransformComponent camera_trf;
	camera_trf.location = Vec3D({ 0,0,-1 });
	camera_trf.orientation = Identity(1.0f);
	SetCamera(camera_trf);
	SetZoomLevel(g_InitialZoomLevel);
	SetLightPosition(Vec3D(0.0f, 0.0f, -3.0f));
	SetMinMaxRange(0.05f, 200.0f);
	s_ShaderLibrary.SetTextureSlots();

	return result;
}

void Renderer::Draw(Mesh* mesh)
{
	if (mesh == nullptr)
		return;

	s_ShaderLibrary.BindShader(mesh->GetMeshType());
	// shader uniforms should be uploaded here later on
	mesh->Draw();
}

void Renderer::Draw(Entity entity)
{
//	Framebuffer::UnbindAll();

	// check if the required components are there, otherwise skip the function body
	if (entity.HasComponent<TransformComponent>() && entity.HasComponent<MeshComponent>())
	{
		TransformComponent& trf = entity.GetComponent<TransformComponent>();
		MeshComponent& mesh = entity.GetComponent<MeshComponent>();
		//	LOG_CORE_INFO("Transform component = {0}, {1}, {2}; mesh type = {3}", trf.x, trf.y, trf.rotation, (int)mesh.meshPtr->GetMeshType());

		std::shared_ptr<Shader> shader = s_ShaderLibrary.BindShader(mesh.meshPtr->GetMeshType()); //	MeshType::COLOURED_MESH
		shader->UploadUniformFloat3("body_location", trf.location.Glm());
		shader->UploadUniformMat3("body_orientation", trf.orientation.Glm());
		shader->UploadUniformFloat("body_scale", trf.scale);

		mesh.meshPtr->Draw();
	}
}

void Renderer::DrawToShadowMap(Entity entity)
{
	s_DepthBuffer->Bind();

	// check if the required components are there, otherwise skip the function body
	if (entity.HasComponent<TransformComponent>() && entity.HasComponent<MeshComponent>())
	{
		TransformComponent& trf = entity.GetComponent<TransformComponent>();
		MeshComponent& mesh = entity.GetComponent<MeshComponent>();

		std::shared_ptr<Shader> shader = s_ShaderLibrary.BindShader(MeshType::SHADOW_MAP);
		shader->UploadUniformFloat3("body_location", trf.location.Glm());
		shader->UploadUniformMat3("body_orientation", trf.orientation.Glm());
		shader->UploadUniformFloat("body_scale", trf.scale);

		mesh.meshPtr->Draw();
	}

	s_DepthBuffer->Unbind();
}

void Renderer::SetAspectRatio(float aspect_ratio)
{
	s_AspectRatio = aspect_ratio;
	s_ShaderLibrary.SetAspectRatio(aspect_ratio);
}

void Renderer::SetCamera(TransformComponent camera_transform)
{
	s_ShaderLibrary.SetCamera(camera_transform);
}

void Renderer::SetZoomLevel(float zoom_level)
{
	s_ShaderLibrary.SetZoomLevel(zoom_level);
}

void Renderer::SetMinMaxRange(float min, float max)
{
	s_ShaderLibrary.SetMinMaxRange(min, max);
}

void Renderer::SetLightPosition(Vec3D light_pos)
{
	s_ShaderLibrary.SetLightPosition(light_pos);
}

void Renderer::Refresh()
{

	glClearColor(0.0f, 0.05f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	s_DepthBuffer->Bind();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_DEPTH_BUFFER_BIT);
	s_DepthBuffer->Unbind();
}

