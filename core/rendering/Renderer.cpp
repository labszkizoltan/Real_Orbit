
#include "Renderer.h"

int Renderer::Init()
{
	int result = gladLoadGL();
	if (!result)
		std::cout << "Failed to initialize OpenGL context" << std::endl;

	return result;
}

void Renderer::SetAspectRatio()
{
	// m_ShaderLibrary.SetAspectRatio();
}
