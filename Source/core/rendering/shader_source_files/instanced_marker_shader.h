
#ifndef INSTANCED_MARKER_SHADER_H
#define INSTANCED_MARKER_SHADER_H


// vertex shader:
const char* instanced_marker_shader_vertexSrc =
"#version 460 core\n"

"layout(location = 0) in vec3 aPos; \n"

"layout(location = 3) in vec3 aInstancePos; \n"
"layout(location = 4) in vec3 aInstanceOrientation_1; \n"
"layout(location = 5) in vec3 aInstanceOrientation_2; \n"
"layout(location = 6) in vec3 aInstanceOrientation_3; \n"
"layout(location = 7) in float aInstanceScale; \n"

"layout(location = 8) in vec4 aInstanceColour; \n"

"uniform vec3 body_location; \n"
"uniform mat3 body_orientation; \n"
"uniform float body_scale; \n"

"uniform float r_max; \n"
"uniform float r_min; \n"

"uniform float zoom_level; \n"
//uniform float scale;

"uniform vec3 camera_location; \n"
"uniform mat3 camera_orientation; \n"

"uniform float aspect_ratio; \n"

"out vec4 outColor; \n"

"void main()\n"
"{\n"
// point coordinate in the observers coordinate system
"	vec3 position_tmp = aInstancePos - camera_location; \n"
"	position_tmp = vec3(\n"
"		dot(position_tmp, camera_orientation[0]), \n"
"		dot(position_tmp, camera_orientation[1]), \n"
"		dot(position_tmp, camera_orientation[2])\n"
"	); \n"

"	float r = length(position_tmp); \n"
"	float rho = length(vec2(position_tmp.x, position_tmp.y)); \n"
//	float theta = acos(position_tmp.z / r);
"	float theta = rho / position_tmp.z < 0.1 ? asin(rho / position_tmp.z) : acos(position_tmp.z / r); \n"
// 60 degrees
"	float theta_max = 1.0471955;\n"

"	float new_r = zoom_level * theta / theta_max; \n"

"	vec4 centerPos = vec4(\n"
"		new_r * position_tmp.x / (rho * aspect_ratio), \n"
"		new_r * position_tmp.y / rho, \n"
"		2.0 * (sign(position_tmp[2]) * r - r_min) / r_max - 1, \n"
"		1.0f\n"
"); \n"

"	float yDisplacement = -0.005; \n"
"	gl_Position = vec4(aPos.x / aspect_ratio, aPos.y + yDisplacement, aPos.z, 0) + centerPos; \n"

"	outColor = aInstanceColour; \n"
"}\0";


// fragment shader
const char* instanced_marker_shader_fragmentSrc =
"#version 460 core\n"
"layout(location = 0) out vec4 FragColor; \n"
"in vec4 outColor; \n"
"uniform float alpha; \n"
"void main()\n"
"{\n"
"	FragColor = outColor; \n"
"}\0";






#endif // INSTANCED_MARKER_SHADER_H
