
#ifndef SHADOW_SHADERS_H
#define SHADOW_SHADERS_H


//**********************************//
//***** 3D perspective shaders *****//
//**********************************//

// vertex shader:
const char* shadow_mapper_vertexSrc =
"#version 460 core\n"

"layout(location = 0) in vec3 aPos; \n"

"layout(location = 3) in vec3 aInstancePos; \n"
"layout(location = 4) in vec3 aInstanceOrientation_1; \n"
"layout(location = 5) in vec3 aInstanceOrientation_2; \n"
"layout(location = 6) in vec3 aInstanceOrientation_3; \n"
"layout(location = 7) in float aInstanceScale; \n"

"uniform float r_max; \n"
"uniform float r_min; \n"

"uniform vec3 light_location; \n"

"void main()\n"
"{\n"
// point coordinate in the absolute coordinate system
"	vec3 position_tmp = aInstancePos - light_location + aInstanceScale * (aPos[0] * aInstanceOrientation_1 + aPos[1] * aInstanceOrientation_2 + aPos[2] * aInstanceOrientation_3); \n"

"	float r = length(position_tmp); \n"
"	float scaler = zoom_level / (sign(position_tmp[2]) * position_tmp[2]);\n"
// "	float scaler = zoom_level / position_tmp[2];\n"

"	gl_Position = vec4(\n"
"		scaler * position_tmp.x / aspect_ratio, \n"
"		scaler * position_tmp.y, \n"
"		2.0 * (position_tmp[2] - r_min) / r_max - 1, \n"
"		1.0f\n"
"); \n"

"}\0";


// fragment shader
const char* shadow_mapper_fragmentSrc =
"#version 460 core\n"
"void main()\n"
"{\n"
"}\0";









#endif // SHADOW_SHADERS_H
