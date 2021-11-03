
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
//"	vec3 position_tmp = body_location + body_scale * (aPos[0] * body_orientation[0] + aPos[1] * body_orientation[1] + aPos[2] * body_orientation[2]); \n"

"	float r = length(position_tmp); \n"
"	float rho = length(vec2(position_tmp.x, position_tmp.y)); \n"
"	float theta = acos(position_tmp.z/r); \n" // acos returns values between 0 and pi
//"	float theta = rho / position_tmp.z < 0.1 ? asin(rho / position_tmp.z) : acos(position_tmp.z / r); \n"
"	float r_max = 20.0f; \n"
"	float r_min = 0.05; \n"

"	gl_Position = vec4(\n"
"		atan(position_tmp.y, position_tmp.x)/(3.1415926535), \n" // this atan returns values between -pi and pi
//"		2*theta/(3.1415926535)-1, \n"
"		position_tmp.z/r, \n"
"		2.0 * (r - r_min) / r_max - 1, \n"
//"		atan(r)/(3.1415926535/2), \n"
//"		2.0 * (sign(position_tmp.z) * r - r_min) / r_max - 1, \n"
"		1.0f\n"
"); \n"

//"	gl_Position = vec4(\n"
//"		position_tmp.x/2, \n"
//"		position_tmp.y/2, \n"
//"		position_tmp.z/10, \n"
//"		1.0f\n"
//"); \n"

"}\0";


// fragment shader
const char* shadow_mapper_fragmentSrc =
"#version 460 core\n"
"void main()\n"
"{\n"
"}\0";









#endif // SHADOW_SHADERS_H
