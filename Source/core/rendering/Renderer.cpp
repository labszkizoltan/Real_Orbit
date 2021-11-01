
#include "Renderer.h"
#include <core/rendering/shader_source_files/ShaderSourceCollection.h>
//#include <core/rendering/shader_source_files/basic_3D_shaders.h>
#include <core/scene/Components.h>

float Renderer::s_AspectRatio = 1.0f;
ShaderLibrary Renderer::s_ShaderLibrary;

Renderer::~Renderer()
{
}

int Renderer::Init()
{
	int result = gladLoadGL();
	if (!result)
		std::cout << "Failed to initialize OpenGL context\n";

	// MeshType::COLOURED_MESH
	s_ShaderLibrary.AddShader(
//		ParseShader("Source/core/rendering/shader_source_files/basic_3D_vertex_shd.glsl"),
//		ParseShader("Source/core/rendering/shader_source_files/basic_3D_fragment_shd.glsl")

//		std::string(basic_3d_shader_vertexSrc),
//		std::string(basic_3d_shader_fragmentSrc)

		std::string(perspective_3d_shader_vertexSrc),
		std::string(perspective_3d_shader_fragmentSrc)
	);

	// MeshType::TEXTURED_MESH
	s_ShaderLibrary.AddShader(
		std::string(textured_3d_shader_vertexSrc),
		std::string(textured_3d_shader_fragmentSrc)
	);

//	// MeshType::SKYBOX
//	s_ShaderLibrary.AddShader(
//		ParseShader("Source/core/rendering/shader_source_files/other_3D_vertex_shd.glsl"),
//		ParseShader("Source/core/rendering/shader_source_files/other_3D_fragment_shd.glsl")
//	);

	
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
	SetZoomLevel(1.0f);

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
