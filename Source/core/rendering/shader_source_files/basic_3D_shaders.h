
#ifndef BASIC_3D_SHADERS
#define BASIC_3D_SHADERS

// vertex shader:
const char* basic_3d_shader_vertexSrc =
"#version 460 core\n"
"layout(location = 0) in vec3 aPos;\n"
"layout(location = 1) in vec3 aColor;\n"
"uniform float aspect_ratio;\n"
"uniform float rotation_angle;\n"
"uniform float offset_x;\n"
"uniform float offset_y;\n"
"out vec3 outColor;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(\n"
"		(cos(rotation_angle) * aPos.x - sin(rotation_angle) * aPos.y + offset_x) / aspect_ratio,\n"
"		sin(rotation_angle) * aPos.x + cos(rotation_angle) * aPos.y + offset_y,\n"
"		aPos.z,\n"
"		1.0f);\n"
"	outColor = aColor;\n"
"}\0";



// fragment shader
const char* basic_3d_shader_fragmentSrc = 
"#version 460 core\n"
"in vec3 outColor;\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(outColor, 1.0f);\n"
"}\0";


//**********************************//
//***** 3D perspective shaders *****//
//**********************************//

// vertex shader:
const char* perspective_3d_shader_vertexSrc =
"#version 460 core\n"

"layout(location = 0) in vec3 aPos; \n"
"layout(location = 1) in vec3 aColor; \n"

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
"	vec3 position_tmp = body_location - camera_location + body_scale * (aPos[0] * body_orientation[0] + aPos[1] * body_orientation[1] + aPos[2] * body_orientation[2]); \n"

// point coordinate in the observers coordinate system
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

"	gl_Position = vec4(\n"
"		new_r * position_tmp.x / (rho * aspect_ratio), \n"
"		new_r * position_tmp.y / rho, \n"
"		2.0 * (sign(position_tmp[2]) * r - r_min) / r_max - 1, \n"
"		1.0f\n"
"); \n"

"	outColor = aColor; \n"
"}\0";


// fragment shader
const char* perspective_3d_shader_fragmentSrc =
"#version 460 core\n"
"in vec3 outColor; \n"
"uniform float alpha; \n"
"out vec4 FragColor; \n"
"void main()\n"
"{\n"
"	FragColor = vec4(outColor, 1.0); \n"
//"	FragColor = vec4(vec3(gl_FragCoord.z), 1.0); \n"
"}\0";


#endif // BASIC_3D_SHADERS
