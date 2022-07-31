
#ifndef INSTANCED_NORMAL_SHADER_H
#define INSTANCED_NORMAL_SHADER_H


// vertex shader:
const char* instanced_normal_shader_vertexSrc =
"#version 460 core\n"

"layout(location = 0) in vec3 aPos; \n"
"layout(location = 1) in vec3 aSurfaceNormal; \n"
"layout(location = 2) in vec2 aTexCoord; \n"

"layout(location = 3) in vec3 aInstancePos; \n"
"layout(location = 4) in vec3 aInstanceOrientation_1; \n"
"layout(location = 5) in vec3 aInstanceOrientation_2; \n"
"layout(location = 6) in vec3 aInstanceOrientation_3; \n"
"layout(location = 7) in float aInstanceScale; \n"

"uniform vec3 light_location; \n"

"uniform float zoom_level; \n"

"uniform vec3 camera_location; \n"
"uniform mat3 camera_orientation; \n"

"uniform float r_max; \n"
"uniform float r_min; \n"

"uniform float aspect_ratio; \n"

"out vec2 texCoord; \n"
"out vec3 lightCoordinates; \n"
"out float light_normal_dot; \n"

"void main()\n"
"{\n"
// point coordinate in the absolute coordinate system
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

// light space coordinate calculation
"	vec3 light_position_tmp = aInstancePos - light_location + aInstanceScale * (aPos[0] * aInstanceOrientation_1 + aPos[1] * aInstanceOrientation_2 + aPos[2] * aInstanceOrientation_3); \n"

"	float light_r = length(light_position_tmp); \n"

"	lightCoordinates = vec3(\n"
"		atan(light_position_tmp.y, light_position_tmp.x)/(3.1415926535), \n" // this atan returns values between -pi and pi
//"		2*theta/(3.1415926535)-1, \n"
"		light_position_tmp.z/light_r, \n"
"		2.0 * (light_r - r_min) / r_max - 1 \n"
//"		atan(r)/(3.1415926535/2) \n" // this atan returns values between -pi/2 and pi/2
"); \n"

// lighting calculation
"	vec3 lightDirection = light_location-position_abs; \n"
"	lightDirection = lightDirection / length(lightDirection); \n"
"	vec3 surfaceDirection = aSurfaceNormal[0] * aInstanceOrientation_1 + aSurfaceNormal[1] * aInstanceOrientation_2 + aSurfaceNormal[2] * aInstanceOrientation_3; \n"
"	light_normal_dot = dot(lightDirection, surfaceDirection / length(surfaceDirection)); \n"

"	texCoord = aTexCoord; \n"
"}\0";


// fragment shader
const char* instanced_normal_shader_fragmentSrc =
"#version 460 core\n"
"layout(location = 0) out vec4 FragColor; \n"
"layout(location = 1) out vec4 BrightColor; \n"
"in vec2 texCoord; \n"
"in vec3 lightCoordinates; \n"
"in float light_normal_dot; \n"
"uniform sampler2D u_Textures[32]; \n"
"uniform float alpha; \n"
"void main()\n"
"{\n"
"	vec3 color = texture(u_Textures[0], texCoord).rgb; \n"
"	vec3 depthSample = texture(u_Textures[1], (lightCoordinates.xy+1)/2).rgb; \n"
"	float closestDepth = depthSample.r; \n"
"	float currentDepth = (lightCoordinates.z+1)/2; \n"
"	float bias = 0.001; \n"
"	float shadow = (currentDepth - bias) > closestDepth ? 1.0 : 0.0; \n"
"	FragColor = light_normal_dot < 0.0 ? vec4(color/2, 1.0) : vec4(color*min((1-shadow/2),(1+light_normal_dot)/2), 1.0); \n"
"	BrightColor = vec4(0,0,0,0); \n"
//"	BrightColor = vec4(texCoord,0,0); \n"
"}\0";











#endif // INSTANCED_NORMAL_SHADER_H
