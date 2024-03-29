
#ifndef SHADER_LIBRARY_H
#define SHADER_LIBRARY_H

#include <core/rendering/Shader.h>
#include <core/rendering/drawables/Mesh.h>
#include <core/scene/CoreComponents.h>
#include <vector>

class ShaderLibrary
{
public:
	ShaderLibrary() = default;
	~ShaderLibrary();

	void AddShader(std::shared_ptr<Shader> shader);
	void AddShader(const std::string& vertexSrc, const std::string& fragmentSrc);

	void SetAspectRatio(float aspectRatio);
	void SetWindowSize(float width, float height);
	void SetCamera(TransformComponent camera_transform);
	void SetZoomLevel(float zoom_level);
	void SetLightPosition(Vec3D light_pos);
	void SetMinMaxRange(float min, float max);
	void SetTextureSlots();

	std::shared_ptr<Shader> BindShader(MeshType meshType);
	std::shared_ptr<Shader> GetLastBoundShader();


private:
	std::vector<std::shared_ptr<Shader>> m_Shaders; // this may need to be changed to a map, or unordered map, where the shader can be selected based on MeshType
	std::shared_ptr<Shader> m_LastBoundShader = nullptr;


friend class Renderer;

};







#endif // SHADER_LIBRARY_H
