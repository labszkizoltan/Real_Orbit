
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

"	float r = length(position_tmp); \n"
"	float rho = length(vec2(position_tmp.x, position_tmp.y)); \n"
//	float theta = acos(position_tmp.z / r);
"	float theta = rho / position_tmp.z < 0.1 ? asin(rho / position_tmp.z) : acos(position_tmp.z / r); \n"
// 60 degrees
"	float theta_max = 1.0471955;\n"
"	float r_max = 20.0f; \n"
"	float r_min = 0.05; \n"

"	float new_r = zoom_level * theta / theta_max; \n"

"	gl_Position = vec4(\n"
"		new_r * position_tmp.x / (rho * aspect_ratio), \n"
"		new_r * position_tmp.y / rho, \n"
"		2.0 * (sign(position_tmp[2]) * r - r_min) / r_max - 1, \n"
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
