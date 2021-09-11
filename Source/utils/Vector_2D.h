
#ifndef VECTOR_2D_H
#define VECTOR_2D_H

#include <iostream>

#include <glm/glm.hpp>

// just quickly created this struct, to keep the texture coordinates in it
struct Vec2D
{
	float x, y;
	Vec2D();
	Vec2D(float x, float y);

	Vec2D& operator+=(const Vec2D& v);
	Vec2D& operator-=(const Vec2D& v);
	Vec2D& operator*=(float c);
	Vec2D& operator/=(float c);
	float length();
	glm::vec2 Glm();
};

#endif // VECTOR_2D_H
