
#include "InstanceBuffer.h"

#include <glad/glad.h>
#include <core/scene/Components.h>

#define GLCall(x) x

// fwd declare this, defined in Buffer.cpp:
static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type);

// replicate the structure of the TransformComponent
BufferLayout InstanceBuffer::s_Layout = {
		{ShaderDataType::Float3, "aLocation"},
		{ShaderDataType::Float3, "aOrientation_1"},
		{ShaderDataType::Float3, "aOrientation_2"},
		{ShaderDataType::Float3, "aOrientation_3"},
		{ShaderDataType::Float, "aScale"}
};



InstanceBuffer::InstanceBuffer(uint32_t size, uint32_t startIdx)
{
	m_Capacity = size;
	m_AttribPtrStartIdx = startIdx;

	glCreateBuffers(1, &m_RendererID);
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	glBufferData(GL_ARRAY_BUFFER, m_Capacity * sizeof(TransformComponent), nullptr, GL_DYNAMIC_DRAW);
}

InstanceBuffer::InstanceBuffer(const std::vector<TransformComponent>& data, uint32_t startIdx)
{
	m_Capacity = data.size();
	m_AttribPtrStartIdx = startIdx;

	glCreateBuffers(1, &m_RendererID);
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	glBufferData(GL_ARRAY_BUFFER, m_Capacity * sizeof(TransformComponent), nullptr, GL_DYNAMIC_DRAW);
}

InstanceBuffer::~InstanceBuffer()
{
	glDeleteBuffers(1, &m_RendererID);
}

void InstanceBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void InstanceBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void InstanceBuffer::SetData(const std::vector<TransformComponent>& data)
{
	Bind();
	if (m_Capacity >= data.size())
	{
		glBufferSubData(GL_ARRAY_BUFFER, 0, data.size()*sizeof(TransformComponent), (void*)&data[0]);
	}
	else
	{
		m_Capacity = data.size();
		//GLCall(glDeleteBuffers(1, &m_RendererID));
		//GLCall(glCreateBuffers(1, &m_RendererID));
		GLCall(glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(TransformComponent), (void*)&data[0], GL_STATIC_DRAW));
	}
}

void InstanceBuffer::SetLayout()
{
	Bind();

	uint32_t index = m_AttribPtrStartIdx;

	for (int i = 0; i < s_Layout.m_Elements.size(); i++)
	{
		glVertexAttribPointer(
			index,
			s_Layout.m_Elements[i].GetComponentCount(),
			ShaderDataTypeToOpenGLBaseType(s_Layout.m_Elements[i].Type),
			s_Layout.m_Elements[i].Normalized ? GL_TRUE : GL_FALSE,
			s_Layout.GetStride(),
			(const void*)s_Layout.m_Elements[i].Offset
		);
		glEnableVertexAttribArray(index);
		index++;
	}

}

void InstanceBuffer::SetAttribStartIdx(uint32_t idx)
{
	m_AttribPtrStartIdx = idx;
}

uint32_t InstanceBuffer::GetCapacity()
{
	return m_Capacity;
}






