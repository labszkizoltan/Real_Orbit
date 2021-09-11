
#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <iostream>

// this is compiler specific, gcc probably cant recognize this
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError(); x; ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << ")\n" << function << " " << file << ": " << line << std::endl;
		return false;
	}
	return true;
}



// the renderer could have a library of shaders, which could be initialized at Init()
// then it could have a draw method to draw various objects passed to it (this could hava a target FBO as a parameter)
// also setting the aspect ratio should be centralized here
// maybe even setting the camera position could go through this class
class Renderer
{
public:

	static int Init();
//	static something Draw();
	static void SetAspectRatio();



private:
	float m_AspectRatio;
//	ShaderLibrary m_ShaderLibrary;


};







#endif // RENDERER_H
