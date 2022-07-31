
#ifndef INSTANCED_BRIGHT_COLOUR_SHADER_H
#define INSTANCED_BRIGHT_COLOUR_SHADER_H


// vertex shader:
const char* instanced_bright_shader_vertexSrc =
"#version 460 core\n"

"layout(location = 0) in vec3 aPos; \n"
"layout(location = 1) in vec3 aColor; \n"

"layout(location = 3) in vec3 aInstancePos; \n"
"layout(location = 4) in vec3 aInstanceOrientation_1; \n"
"layout(location = 5) in vec3 aInstanceOrientation_2; \n"
"layout(location = 6) in vec3 aInstanceOrientation_3; \n"
"layout(location = 7) in float aInstanceScale; \n"

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

"out vec3 outColor; \n"

"void main()\n"
"{\n"
// point coordinate in the absolute coordinate system
//"	vec3 position_tmp = body_location - camera_location + body_scale * (aPos[0] * body_orientation[0] + aPos[1] * body_orientation[1] + aPos[2] * body_orientation[2]); \n"
"	vec3 position_abs = aInstancePos + aInstanceScale * (aPos[0] * aInstanceOrientation_1 + aPos[1] * aInstanceOrientation_2 + aPos[2] * aInstanceOrientation_3); \n"

// point coordinate in the observers coordinate system
"	vec3 position_tmp = position_abs - camera_location; \n"
"	position_tmp = vec3(\n"
"		dot(position_tmp, camera_orientation[0]), \n"
"		dot(position_tmp, camera_orientation[1]), \n"
"		dot(position_tmp, camera_orientation[2])\n"
"	); \n"

"	float r = length(position_tmp); \n"
"	float scaler = zoom_level / (sign(position_tmp[2]) * position_tmp[2]);\n"
// "	float scaler = zoom_level / position_tmp[2];\n"

"	gl_Position = vec4(\n"
"		scaler * position_tmp.x / aspect_ratio, \n"
"		scaler * position_tmp.y, \n"
"		2.0 * (position_tmp[2] - r_min) / r_max - 1, \n"
"		1.0f\n"
"); \n"

"	outColor = aColor; \n"
"}\0";


// fragment shader
const char* instanced_bright_shader_fragmentSrc =
"#version 460 core\n"
"layout(location = 0) out vec4 FragColor; \n"
"layout(location = 1) out vec4 BrightColor; \n"
"in vec3 outColor; \n"
"uniform float alpha; \n"
"void main()\n"
"{\n"
"	FragColor = vec4(outColor, 1.0); \n"
"	BrightColor = vec4(outColor, 1.0); \n"
//"	FragColor = vec4(vec3(gl_FragCoord.z), 1.0); \n"
"}\0";



#endif // INSTANCED_BRIGHT_COLOUR_SHADER_H

