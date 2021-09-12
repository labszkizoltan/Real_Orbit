
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


	s_ShaderLibrary.AddShader(
		ParseShader("Source/core/rendering/shader_source_files/basic_3D_vertex_shd.glsl"),
		ParseShader("Source/core/rendering/shader_source_files/basic_3D_fragment_shd.glsl")
	);

	return result;
}

void Renderer::SetAspectRatio(float aspect_ratio)
{
	s_ShaderLibrary.SetAspectRatio(aspect_ratio);
}
