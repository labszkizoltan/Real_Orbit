
#include "Renderer.h"

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
		ParseShader("Source/core/rendering/shader_source_files/basic_3D_vertex_shd.glsl"),
		ParseShader("Source/core/rendering/shader_source_files/basic_3D_fragment_shd.glsl")
	);


//	// Add subsequent shader here:
//	// MeshType::OTHER_MESH_TYPE
//	s_ShaderLibrary.AddShader(
//		ParseShader("Source/core/rendering/shader_source_files/other_3D_vertex_shd.glsl"),
//		ParseShader("Source/core/rendering/shader_source_files/other_3D_fragment_shd.glsl")
//	);


	SetAspectRatio(s_AspectRatio);

	return result;
}


void Renderer::Draw(Mesh* mesh, float rotation_angle, float offset_x, float offset_y)
{
	if (mesh == nullptr)
		return;

	s_ShaderLibrary.BindShader(mesh->GetMeshType());
	// shader uniforms should be uploaded here later on

	GLint whichID;
	glGetIntegerv(GL_CURRENT_PROGRAM, &whichID);

	GLint loc = glGetUniformLocation(whichID, "rotation_angle");
	glUniform1f(loc, rotation_angle);
	loc = glGetUniformLocation(whichID, "offset_x");
	glUniform1f(loc, offset_x);
	loc = glGetUniformLocation(whichID, "offset_y");
	glUniform1f(loc, offset_y);

	mesh->Draw();
}

void Renderer::Draw(Mesh* mesh)
{
	if (mesh == nullptr)
		return;

	s_ShaderLibrary.BindShader(mesh->GetMeshType());
	// shader uniforms should be uploaded here later on
	mesh->Draw();
}

void Renderer::SetAspectRatio(float aspect_ratio)
{
	s_AspectRatio = aspect_ratio;
	s_ShaderLibrary.SetAspectRatio(aspect_ratio);
}
