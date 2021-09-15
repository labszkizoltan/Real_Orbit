
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



#endif // BASIC_3D_SHADERS
