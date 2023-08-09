
#ifndef RENDERER_H
#define RENDERER_H

#include <core/rendering/ShaderLibrary.h>
#include <core/rendering/DoubleFramebuffer.h>
//#include <core/rendering/Framebuffer.h>
#include <core/rendering/Depthbuffer.h>
#include <core/rendering/drawables/Mesh.h>
#include <core/scene/Entity.h>
#include <core/scene/CoreComponents.h>
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

	static void SetAspectRatio(float aspect_ratio);
	static void SetWindowSize(float width, float height);
	static void SetCamera(TransformComponent camera_transform);
	static void SetZoomLevel(float zoom_level);
	static void SetMinMaxRange(float min, float max);
	static void SetLightPosition(Vec3D light_pos);

	static void Refresh();
	static void RefreshOther();

	static std::shared_ptr<Shader> BindShader(MeshType meshType);
	static std::shared_ptr<Texture> GetColorAttachment();
	static std::shared_ptr<Texture> GetBrightColorAttachment();
	static std::shared_ptr<Texture> GetDepthAttachment();
	static std::shared_ptr<Texture> GetBlurredAttachment();
	static std::shared_ptr<Texture> GetSpare1Attachment();
	static std::shared_ptr<Texture> GetSpare2Attachment();

	// a few helper function could go here, like something that uploads various uniform combinations into the shaders
private:
	static int InitFramebuffers();
	static int InitShaderLibrary();

public:
//private:
	static float s_AspectRatio;
	static std::shared_ptr<Framebuffer> s_DepthBuffer; // this will contain the depth values from thr light sources point of view
	static std::shared_ptr<Framebuffer> s_FrameBuffer; // this is to render the bright meshes separately
	static std::shared_ptr<Framebuffer> s_BlurBuffer; // this is to blur the layer of bright meshes
	static std::shared_ptr<Framebuffer> s_SpareBuffer1; // additional frames to use freely for anything
	static std::shared_ptr<Framebuffer> s_SpareBuffer2; // additional frames to use freely for anything
	static ShaderLibrary s_ShaderLibrary;
	


	friend class SceneRenderer;

};







#endif // RENDERER_H
