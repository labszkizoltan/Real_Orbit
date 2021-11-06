
#ifndef TEXTURED_SHADERS_H
#define TEXTURED_SHADERS_H

//**********************************//
//***** 3D perspective shaders *****//
//**********************************//

// vertex shader:
const char* textured_3d_shader_vertexSrc =
"#version 460 core\n"

"layout(location = 0) in vec3 aPos; \n"
"layout(location = 1) in vec2 aTexCoord; \n"

"uniform vec3 body_location; \n"
"uniform mat3 body_orientation; \n"
"uniform float body_scale; \n"

"uniform vec3 light_location; \n"

"uniform float zoom_level; \n"
//uniform float scale;

"uniform vec3 camera_location; \n"
"uniform mat3 camera_orientation; \n"

"uniform float aspect_ratio; \n"

"out vec2 texCoord; \n"
"out vec3 lightCoordinates; \n"

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
"	float r_max = 20.0f; \n"
"	float r_min = 0.05; \n"

"	float new_r = zoom_level * theta / theta_max; \n"

"	gl_Position = vec4(\n"
"		new_r * position_tmp.x / (rho * aspect_ratio), \n"
"		new_r * position_tmp.y / rho, \n"
"		2.0 * (sign(position_tmp[2]) * r - r_min) / r_max - 1, \n"
"		1.0f\n"
"); \n"

// light space coordinate calculation
"	vec3 light_position_tmp = body_location - light_location + body_scale * (aPos[0] * body_orientation[0] + aPos[1] * body_orientation[1] + aPos[2] * body_orientation[2]); \n"

"	float light_r = length(light_position_tmp); \n"

"	lightCoordinates = vec3(\n"
"		atan(light_position_tmp.y, light_position_tmp.x)/(3.1415926535), \n" // this atan returns values between -pi and pi
//"		2*theta/(3.1415926535)-1, \n"
"		light_position_tmp.z/light_r, \n"
"		2.0 * (light_r - r_min) / r_max - 1 \n"
//"		atan(r)/(3.1415926535/2) \n" // this atan returns values between -pi/2 and pi/2
"); \n"

"	texCoord = aTexCoord; \n"
"}\0";


// fragment shader
const char* textured_3d_shader_fragmentSrc =
"#version 460 core\n"
"layout(location = 0) out vec4 FragColor; \n"
"in vec2 texCoord; \n"
"in vec3 lightCoordinates; \n"
"uniform sampler2D u_Textures[32]; \n"
"uniform float alpha; \n"
"void main()\n"
"{\n"
//"	float shadowMultiplier = texture(u_Textures[1], (lightCoordinates.xy+1)/2); \n"
"	vec3 color = texture(u_Textures[0], texCoord).rgb; \n"
"	vec3 depthSample = texture(u_Textures[1], (lightCoordinates.xy+1)/2).rgb; \n"
"	float closestDepth = depthSample.r; \n"
"	float currentDepth = (lightCoordinates.z+1)/2; \n"
"	float bias = 0.005; \n"
"	float shadow = (currentDepth - bias) > closestDepth ? 1.0 : 0.0; \n"
"	FragColor = vec4(color*(1-shadow/2), 1.0); \n"
//"	FragColor = vec4((color+depthSample)/2, 1.0); \n"
//"	FragColor = vec4(vec3(shadow), 1.0); \n"
//"	FragColor = vec4(vec3(lightCoordinates), 1.0); \n"
//"	FragColor = vec4(vec3(closestDepth), 1.0); \n"
//"	FragColor = vec4(vec3(depthSample), 1.0); \n"
//"	FragColor = vec4(color, 1.0); \n"
//"	FragColor = vec4(vec3(lightCoordinates.z/2+0.5), 1.0); \n"
//"	FragColor = texture(u_Textures[0], texCoord); \n"
//"	FragColor = vec4(texCoord, 0.0, 1.0); \n"
//"	FragColor = vec4(vec3(gl_FragCoord.z), 1.0); \n"
"}\0";









#endif // TEXTURED_SHADERS_H
