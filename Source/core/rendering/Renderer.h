
#ifndef RENDERER_H
#define RENDERER_H

#include <core/rendering/ShaderLibrary.h>
#include <core/rendering/Depthbuffer.h>
#include <core/rendering/drawables/Mesh.h>
#include <core/scene/Entity.h>
#include <core/scene/Components.h>
#include <glad/glad.h>
#include <iostream>

#include <wingdi.h>


// this is compiler specific, gcc probably cant recognize this
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError(); x; ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearError()
{
	bool has_opengl_context = wglGetCurrentContext();
	if(has_opengl_context)
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
	Renderer() = default;
	~Renderer();

	static int Init();
//	static void Draw(Mesh* mesh);

//	static void Draw(Entity entity);
//	static void DrawToShadowMap(Entity entity);
//	static void DrawToTexture(Entity entity, Framebuffer fbo);
	static void SetAspectRatio(float aspect_ratio);
	static void SetCamera(TransformComponent camera_transform);
	static void SetZoomLevel(float zoom_level);
	static void SetMinMaxRange(float min, float max);
	static void SetLightPosition(Vec3D light_pos);

	static void Refresh();

	// a few helper function could go here, like something that uploads various uniform combinations into the shaders
private:


private:
	static float s_AspectRatio;
	static std::shared_ptr<Depthbuffer> s_DepthBuffer;
	static ShaderLibrary s_ShaderLibrary;



	friend class SceneRenderer;

};







#endif // RENDERER_H
