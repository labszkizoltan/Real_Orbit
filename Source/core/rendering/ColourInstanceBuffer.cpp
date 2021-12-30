
#include "ColourInstanceBuffer.h"

#include <glad/glad.h>
#include <core/scene/CoreComponents.h>

#include <core/rendering/Renderer.h>
#include <core/GlobalConstants.h>


// fwd declare (defined in Buffer.cpp):
GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type);

// replicate the structure of the TransformComponent
BufferLayout ColourInstanceBuffer::s_Layout = {
		{ShaderDataType::Float4, "aColour"}
};


ColourInstanceBuffer::ColourInstanceBuffer(uint32_t size, uint32_t startIdx)
{
	m_ElementCount = size;
	m_Capacity = size;
	m_AttribPtrStartIdx = startIdx;

	glCreateBuffers(1, &m_RendererID);
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	glBufferData(GL_ARRAY_BUFFER, m_Capacity * sizeof(ColourComponent), nullptr, GL_DYNAMIC_DRAW);
}

ColourInstanceBuffer::ColourInstanceBuffer(const std::vector<ColourComponent>& data, uint32_t startIdx)
{
	m_ElementCount = data.size();
	m_Capacity = data.size();
	m_AttribPtrStartIdx = startIdx;

	glCreateBuffers(1, &m_RendererID);
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	glBufferData(GL_ARRAY_BUFFER, m_Capacity * sizeof(ColourComponent), nullptr, GL_DYNAMIC_DRAW);
}

ColourInstanceBuffer::~ColourInstanceBuffer()
{
	glDeleteBuffers(1, &m_RendererID);
}

void ColourInstanceBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void ColourInstanceBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ColourInstanceBuffer::SetData(const std::vector<ColourComponent>& data)
{
	m_ElementCount = std::min(g_MaxInstanceCount, data.size());
	if (data.size() > 0)
	{
		Bind();
		// when less than half of the capacity is utilized, shrink buffer to current size
		//----- this may not be a very good idea -----//
		if (m_Capacity >= (2 * data.size()))
		{
			m_Capacity = data.size();
			GLCall(glBufferData(GL_ARRAY_BUFFER, m_Capacity * sizeof(ColourComponent), (void*)&data[0], GL_DYNAMIC_DRAW)); // GL_STREAM_DRAW, GL_STATIC_DRAW, GL_DYNAMIC_DRAW.
		}
		else if (m_Capacity >= data.size())
		{
			GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, data.size() * sizeof(ColourComponent), (void*)&data[0]));
		}
		// when data cannot fit into the current capacity, 1.5x the size
		else
		{
			m_Capacity = std::min(g_MaxInstanceCount, 3 * data.size() / 2);
			GLCall(glBufferData(GL_ARRAY_BUFFER, m_Capacity * sizeof(ColourComponent), nullptr, GL_DYNAMIC_DRAW)); // GL_STREAM_DRAW, GL_STATIC_DRAW, GL_DYNAMIC_DRAW.
			GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, data.size() * sizeof(ColourComponent), (void*)&data[0]));
		}
	}
}

void ColourInstanceBuffer::SetLayout()
{
	Bind();

	uint32_t index = m_AttribPtrStartIdx;

	for (int i = 0; i < s_Layout.m_Elements.size(); i++)
	{
		GLCall(
			glVertexAttribPointer(
				index,
				s_Layout.m_Elements[i].GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(s_Layout.m_Elements[i].Type),
				s_Layout.m_Elements[i].Normalized ? GL_TRUE : GL_FALSE,
				s_Layout.GetStride(),
				(const void*)s_Layout.m_Elements[i].Offset
			)
		);
		GLCall(glVertexAttribDivisor(index, 1));
		GLCall(glEnableVertexAttribArray(index));
		index++;
	}
}

void ColourInstanceBuffer::SetAttribStartIdx(uint32_t idx)
{
	m_AttribPtrStartIdx = idx;
}

uint32_t ColourInstanceBuffer::GetCapacity()
{
	return m_Capacity;
}

uint32_t ColourInstanceBuffer::GetElementCount()
{
	return m_ElementCount;
}
