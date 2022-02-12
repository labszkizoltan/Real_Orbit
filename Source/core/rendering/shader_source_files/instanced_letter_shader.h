
#ifndef INSTANCED_LETTER_SHADER_H
#define INSTANCED_LETTER_SHADER_H



// vertex shader:
const char* instanced_letter_shader_vertexSrc =
"#version 460 core\n"

// aPos contains the quads vertex coordinates
"layout(location = 0) in vec3 aPos; \n"

// the transform component instances will contain the letters screen coord (aInstancePos.xy)
// the letters width and height (aInstancePos.z, aInstanceOrientation_1.z)
// the texture coordinates of the instance (aInstanceOrientation_1.xy)
// the scale of the letter (aInstanceOrientation_2.x)
// and the colour of the letter (aInstanceOrientation_3.xyz)
"layout(location = 3) in vec3 aInstancePos; \n"
"layout(location = 4) in vec3 aInstanceOrientation_1; \n"
"layout(location = 5) in vec3 aInstanceOrientation_2; \n"
"layout(location = 6) in vec3 aInstanceOrientation_3; \n"
"layout(location = 7) in float aInstanceScale; \n"


"uniform float screen_w; \n"
"uniform float screen_h; \n"
"uniform float aspect_ratio; \n"
"uniform sampler2D u_Textures[32]; \n"

"out vec3 outColor; \n"
"out vec2 outTexCoord; \n"
"out vec2 outWidthHeight; \n"

"void main()\n"
"{\n"
"	vec2 instance_xy = aInstancePos.xy; \n"
"	vec2 instance_uv = aInstanceOrientation_1.xy; \n"
"	float width = aInstancePos.z; \n"
"	float height = aInstanceOrientation_1.z; \n"
"	float letter_scale = aInstanceOrientation_2.x; \n"
"	vec3 letter_colour = aInstanceOrientation_3; \n"

"	vec2 texture_size = textureSize(u_Textures[0], 0); \n"

"	vec2 corner_position = 2* vec2(instance_xy.x/screen_w, instance_xy.y/screen_h)-1; \n"
"	vec2 vertex_position = corner_position + 2 * letter_scale * vec2(width*aPos.x/screen_w, height*aPos.y/screen_h); \n"

"	gl_Position = vec4(\n"
"		vertex_position.x, \n"
"		vertex_position.y, \n"
"		0.0f, \n"
"		1.0f\n"
"); \n"

"	vec2 texture_corner_position = vec2(instance_uv.x/texture_size.x, 1-instance_uv.y/texture_size.y); \n"
"	vec2 texture_vertex_position = texture_corner_position + vec2(width*aPos.x/texture_size.x, height*aPos.y/texture_size.y); \n"

"	outColor = letter_colour; \n"
"	outTexCoord = texture_vertex_position; \n"
"}\0";


// fragment shader
const char* instanced_letter_shader_fragmentSrc =
"#version 460 core\n"
"layout(location = 0) out vec4 FragColor; \n"
"layout(location = 1) out vec4 BrightColor; \n"
"in vec3 outColor; \n"
"in vec2 outTexCoord; \n"

"uniform sampler2D u_Textures[32]; \n"
"uniform float alpha; \n"
"void main()\n"
"{\n"
"	vec3 color = texture(u_Textures[0], outTexCoord).rgb * outColor; \n"
"	FragColor = vec4(color, 1.0); \n"
//"	BrightColor = vec4(color, 1.0); \n"
"	BrightColor = vec4(0,0,0,0); \n"
"}\0";














#endif // INSTANCED_LETTER_SHADER_H