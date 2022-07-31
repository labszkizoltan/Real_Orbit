
#ifndef SKYBOX_SHADERS_H
#define SKYBOX_SHADERS_H


// vertex shader:
const char* skybox_shader_vertexSrc =
"#version 460 core\n"

"layout(location = 0) in vec3 aPos; \n"
"layout(location = 1) in vec2 aTexCoord; \n"
"layout(location = 2) in float aTexID; \n"

"uniform float zoom_level; \n"
"uniform mat3 camera_orientation; \n"

"uniform float aspect_ratio; \n"

"out vec2 vTexCoord; \n"
"out float vTexID; \n"

"void main()\n"
"{\n"
// point coordinate in the observers coordinate system
"	vec3 position_tmp = vec3(\n"
"		dot(aPos, camera_orientation[0]), \n"
"		dot(aPos, camera_orientation[1]), \n"
"		dot(aPos, camera_orientation[2])\n"
"	); \n"

"	float r_max = 20.0f; \n"
"	float r_min = 0.05; \n"

"	float r = length(position_tmp); \n"
"	float scaler = zoom_level / (sign(position_tmp[2]) * position_tmp[2]);\n"

"	gl_Position = vec4(\n"
"		scaler * position_tmp.x / aspect_ratio, \n"
"		scaler * position_tmp.y, \n"
"		2.0 * (position_tmp[2] - r_min) / r_max - 1, \n"
"		1.0f\n"
"); \n"

"	vTexCoord = aTexCoord; \n"
"	vTexID = aTexID; \n"
"}\0";


// fragment shader
const char* skybox_shader_fragmentSrc =
"	#version 460 core \n"
"	layout(location = 0) out vec4 color; \n"
"	layout(location = 1) out vec4 BrightColor; \n"
"	in vec2 vTexCoord; \n"
"	in float vTexID; \n"
"	uniform sampler2D u_Textures[32]; \n"
"	void main() \n"
"	{ \n"
"		color = texture(u_Textures[int(vTexID+2)], vTexCoord); \n"
//"		BrightColor = color; \n"
"		BrightColor = vec4(0,0,0,0); \n"
//"		color = vec4(vTexID/6); \n"
"	}\0";


#endif // SKYBOX_SHADERS_H
