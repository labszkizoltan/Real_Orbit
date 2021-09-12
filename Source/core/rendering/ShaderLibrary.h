
#ifndef SHADER_LIBRARY_H
#define SHADER_LIBRARY_H

#include <core/rendering/Shader.h>
#include <vector>

class ShaderLibrary
{
public:
	ShaderLibrary() = default;
	~ShaderLibrary();

	void AddShader(Shader* shader);
	void AddShader(const std::string& vertexSrc, const std::string& fragmentSrc);

	void SetAspectRatio(float aspectRatio);

private:
	std::vector<Shader*> m_Shaders;

};







#endif // SHADER_LIBRARY_H
