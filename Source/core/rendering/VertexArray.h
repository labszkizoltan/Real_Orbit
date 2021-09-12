
#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include <cstdint> // needed for uint32_t

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void Bind();
	void UnBind();

private:
	uint32_t m_RendererID = 0;
};




#endif // VERTEX_ARRAY_H
