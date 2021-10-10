
#ifndef SHADER_LIBRARY_H
#define SHADER_LIBRARY_H

#include <core/rendering/Shader.h>
#include <core/rendering/drawables/Mesh.h>
#include <core/scene/Components.h>
#include <vector>

class ShaderLibrary
{
public:
	ShaderLibrary() = default;
	~ShaderLibrary();

	void AddShader(Shader* shader);
	void AddShader(const std::string& vertexSrc, const std::string& fragmentSrc);

	void SetAspectRatio(float aspectRatio);
	void SetCamera(TransformComponent camera_transform);

	Shader* BindShader(MeshType meshType);
	Shader* GetLastBoundShader();


private:
	std::vector<Shader*> m_Shaders; // this may need to be changed to a map, or unordered map, where the shader can be selected based on MeshType
	Shader* m_LastBoundShader = nullptr;

};







#endif // SHADER_LIBRARY_H
