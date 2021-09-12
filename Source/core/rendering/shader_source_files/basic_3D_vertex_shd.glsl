
#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform float aspect_ratio;

uniform float rotation_angle;
uniform float offset_x;
uniform float offset_y;

out vec3 outColor;

void main()
{
	gl_Position = vec4(
		(cos(rotation_angle)*aPos.x-sin(rotation_angle)*aPos.y+offset_x)/aspect_ratio,
		sin(rotation_angle)*aPos.x+cos(rotation_angle)*aPos.y+offset_y,
		aPos.z,
		1.0f);

	outColor = aColor;
}




