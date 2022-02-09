
#include "Renderer.h"
#include <core/Application.h>
#include <core/rendering/shader_source_files/ShaderSourceCollection.h>
#include <core/rendering/ImageProcessor.h>
//#include <core/rendering/shader_source_files/basic_3D_shaders.h>
#include <core/scene/CoreComponents.h>
#include <core/GlobalConstants.h>


float Renderer::s_AspectRatio = 1.0f;
std::shared_ptr<Framebuffer> Renderer::s_FrameBuffer = nullptr;
std::shared_ptr<Framebuffer> Renderer::s_BlurBuffer = nullptr;
std::shared_ptr<Framebuffer> Renderer::s_DepthBuffer = nullptr;
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

	// Create the required framebuffers
	result = InitFramebuffers();
	if (!result)
		std::cout << "Failed to initialize ShaderLibrary\n";

	// Add the shaders to the shader library
	result = InitShaderLibrary();
	if(!result)
		std::cout << "Failed to initialize ShaderLibrary\n";

//	// Init ImageProcessor
//	result = ImageProcessor::Init();
//	if(!result)
//		std::cout << "Failed to initialize ImageProcessor\n";

	float w = Application::Get().GetWindow().GetWidth();
	float h = Application::Get().GetWindow().GetHeight();
	SetAspectRatio(w / h);

	TransformComponent camera_trf;
	camera_trf.location = Vec3D({ 0,0,-1 });
	camera_trf.orientation = Identity(1.0f);
	SetCamera(camera_trf);
	SetZoomLevel(g_InitialZoomLevel);
	SetLightPosition(Vec3D(0.0f, 0.0f, -3.0f));
	SetMinMaxRange(0.05f, 1000.0f);
	s_ShaderLibrary.SetTextureSlots();

	glViewport(0, 0, Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight());
	Renderer::SetAspectRatio((float)Application::Get().GetWindow().GetWidth() / (float)Application::Get().GetWindow().GetHeight());

	glEnable(GL_DEPTH_TEST);

//	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	return result;
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

	s_FrameBuffer->Bind();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	s_FrameBuffer->Unbind();

	s_BlurBuffer->Bind();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	s_BlurBuffer->Unbind();

}

std::shared_ptr<Shader> Renderer::BindShader(MeshType meshType)
{
	return s_ShaderLibrary.BindShader(meshType);
}

std::shared_ptr<Texture> Renderer::GetColorAttachment()
{
	return s_FrameBuffer->GetColorAttachment();
}

std::shared_ptr<Texture> Renderer::GetBrightColorAttachment()
{
//	return s_FrameBuffer->GetBrightColorAttachment();
	return s_FrameBuffer->GetColorAttachment(1);
}

std::shared_ptr<Texture> Renderer::GetDepthAttachment()
{
	return s_DepthBuffer->GetDepthAttachment();
}

std::shared_ptr<Texture> Renderer::GetBlurredAttachment()
{
//	return nullptr;
	return s_BlurBuffer->GetColorAttachment();
}

int Renderer::InitFramebuffers()
{
	// Create the framebuffer for the shadow map:
	FrameBufferSpecification dbSpec;
	dbSpec.Width = 4096;
	dbSpec.Height = 4096;
	dbSpec.HasDepthBuffer = true;
	dbSpec.ColourAttachmentCount = 0;
	s_DepthBuffer = std::make_shared<Framebuffer>(dbSpec); // the constructor automatically binds the depthbuffer
	s_DepthBuffer->GetDepthAttachment()->SetSlot(g_RendererShadowDepthSlot);
	s_DepthBuffer->Unbind();

	// Create the framebuffer for the double colour attachment buffer (this is the main draw buffer):
	FrameBufferSpecification fbSpec;
	fbSpec.Width = Application::Get().GetWindow().GetWidth();
	fbSpec.Height = Application::Get().GetWindow().GetHeight();
	fbSpec.Samples = 1;
	fbSpec.HasDepthBuffer = true;
	fbSpec.ColourAttachmentCount = 2;
	s_FrameBuffer = std::make_shared<Framebuffer>(fbSpec); // the constructor automatically binds the framebuffer
	s_FrameBuffer->GetColorAttachment(0)->SetSlot(g_RendererColorAttchSlot);
	s_FrameBuffer->GetColorAttachment(1)->SetSlot(g_RendererBrightColAttchSlot);
	s_FrameBuffer->GetDepthAttachment()->SetSlot(g_RendererDepthAttchSlot);
	s_FrameBuffer->Unbind();

	// Create the framebuffer for the single colour attachment buffer (for blurring)
	FrameBufferSpecification blurSpec;
	blurSpec.Width = Application::Get().GetWindow().GetWidth();
	blurSpec.Height = Application::Get().GetWindow().GetHeight();
	blurSpec.Samples = 1;
	blurSpec.HasDepthBuffer = true;
	blurSpec.ColourAttachmentCount = 1;
	s_BlurBuffer = std::make_shared<Framebuffer>(blurSpec); // the constructor automatically binds the framebuffer
	s_BlurBuffer->GetColorAttachment(0)->SetSlot(g_RendererBlurredSlot);
	s_BlurBuffer->GetDepthAttachment()->SetSlot(g_RendererBlurDepthSlot);
	s_BlurBuffer->Unbind();

	// frame/depth buffer construction somehow invalidates the texture binding,
	// so Im doing it after both framebuffers are done
	s_DepthBuffer->GetDepthAttachment()->Bind();
	s_FrameBuffer->GetColorAttachment(0)->Bind();
	s_FrameBuffer->GetColorAttachment(1)->Bind();
	s_FrameBuffer->GetDepthAttachment()->Bind();
	s_BlurBuffer->GetColorAttachment(0)->Bind();
	s_BlurBuffer->GetDepthAttachment()->Bind();

	return 1;
}

int Renderer::InitShaderLibrary()
{	// MeshType::COLOURED_MESH
	s_ShaderLibrary.AddShader(
		std::string(instanced_colour_shader_vertexSrc),
		std::string(instanced_colour_shader_fragmentSrc)
	);

	// MeshType::BRIGHT_COLOURED_MESH
	s_ShaderLibrary.AddShader(
		std::string(instanced_bright_shader_vertexSrc),
		std::string(instanced_bright_shader_fragmentSrc)
	);

	// MeshType::NORMAL_MESH
	s_ShaderLibrary.AddShader(
		std::string(instanced_normal_shader_vertexSrc),
		std::string(instanced_normal_shader_fragmentSrc)
	);

	// MeshType::ALPHA_MESH
	s_ShaderLibrary.AddShader(
		std::string(instanced_alpha_shader_vertexSrc),
		std::string(instanced_alpha_shader_fragmentSrc)
	);

	// MeshType::MARKER_MESH
	s_ShaderLibrary.AddShader(
		std::string(instanced_marker_shader_vertexSrc),
		std::string(instanced_marker_shader_fragmentSrc)
	);

	// MeshType::LETTER_MESH
	s_ShaderLibrary.AddShader(
		std::string(instanced_letter_shader_vertexSrc),
		std::string(instanced_letter_shader_fragmentSrc)
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

	return 1;
}


