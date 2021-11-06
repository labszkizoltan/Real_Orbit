
#ifndef SHADOW_SHADERS_H
#define SHADOW_SHADERS_H


//**********************************//
//***** 3D perspective shaders *****//
//**********************************//

// vertex shader:
const char* shadow_mapper_vertexSrc =
"#version 460 core\n"

"layout(location = 0) in vec3 aPos; \n"

"uniform vec3 body_location; \n"
"uniform mat3 body_orientation; \n"
"uniform float body_scale; \n"

"uniform vec3 light_location; \n"

"void main()\n"
"{\n"
// point coordinate in the absolute coordinate system
"	vec3 position_tmp = body_location - light_location + body_scale * (aPos[0] * body_orientation[0] + aPos[1] * body_orientation[1] + aPos[2] * body_orientation[2]); \n"

"	float r = length(position_tmp); \n"
"	float r_max = 20.0f; \n"
"	float r_min = 0.05; \n"

"	gl_Position = vec4(\n"
"		atan(position_tmp.y, position_tmp.x)/(3.1415926535), \n" // this atan returns values between -pi and pi
//"		2*theta/(3.1415926535)-1, \n"
"		position_tmp.z/r, \n"
"		2.0 * (r - r_min) / r_max - 1, \n"
//"		atan(r)/(3.1415926535/2), \n" // this atan returns values between -pi/2 and pi/2
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
