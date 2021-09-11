
#ifndef BUFFER_H
#define BUFFER_H

#include <core/Core.h>

enum class ShaderDataType
{
	None = 0, Float, Float2, Float3, Float4,
	Mat3, Mat4,
	Int, Int2, Int3, Int4,
	Bool
};

// size in bytes
static uint32_t ShaderDataTypeSize(ShaderDataType type)
{
	switch (type)
	{
	case ShaderDataType::None:		return 0;
	case ShaderDataType::Float:		return 4;
	case ShaderDataType::Float2:	return 8;
	case ShaderDataType::Float3:	return 12;
	case ShaderDataType::Float4:	return 16;
	case ShaderDataType::Mat3:		return 36;
	case ShaderDataType::Mat4:		return 64;
	case ShaderDataType::Int:		return 4;
	case ShaderDataType::Int2:		return 8;
	case ShaderDataType::Int3:		return 12;
	case ShaderDataType::Int4:		return 16;
	case ShaderDataType::Bool:		return 1;
	}

	LOG_CORE_WARN("Unknown ShaderDataType in ShaderDataTypeSize()!");
	return 0;
}

struct BufferElement
{
	std::string Name;
	ShaderDataType Type;
	uint32_t Size;
	uint32_t Offset;
	bool Normalized;

	BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
		: Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized)
	{
	}

	uint32_t GetComponentCount() const
	{
		switch (Type)
		{
		case ShaderDataType::Float:		return 1;
		case ShaderDataType::Float2:	return 2;
		case ShaderDataType::Float3:	return 3;
		case ShaderDataType::Float4:	return 4;
		case ShaderDataType::Mat3:		return 9;
		case ShaderDataType::Mat4:		return 16;
		case ShaderDataType::Int:		return 1;
		case ShaderDataType::Int2:		return 2;
		case ShaderDataType::Int3:		return 3;
		case ShaderDataType::Int4:		return 4;
		case ShaderDataType::Bool:		return 1;
		}

		LOG_CORE_WARN("Unknown ShaderDataType in GetElementCount()!");
		return 0;
	}
};

class BufferLayout
{
public:
	BufferLayout() {}

	BufferLayout(const std::initializer_list<BufferElement>& elements)
		:m_Elements(elements)
	{
		CalculateOffsetAndStride();
	}

	inline uint32_t GetStride() const { return m_Stride; }
	inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }

	std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
	std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
	std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
	std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

private:
	void CalculateOffsetAndStride()
	{
		uint32_t offset = 0;
		m_Stride = 0;

		for (BufferElement& element : m_Elements)
		{
			element.Offset = offset;
			offset += element.Size;
			m_Stride += element.Size;
		}
	}

public:
	std::vector<BufferElement> m_Elements;
	uint32_t m_Stride = 0;
};


class VertexBuffer
{
public:
	VertexBuffer() = default;
	VertexBuffer(uint32_t size);
	VertexBuffer(float* vertices, uint32_t size);

	VertexBuffer(const VertexBuffer& other) = delete;// copy constructor
	VertexBuffer& operator=(const VertexBuffer& other) = delete; // copy assignment
	VertexBuffer(VertexBuffer&& other) noexcept; // move constructor
	VertexBuffer& operator=(VertexBuffer&& other) noexcept; // move assignment
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;

	void SetData(const void* data, uint32_t size);

	const BufferLayout& GetLayout() const { return m_Layout; };
	void SetLayout(const BufferLayout& layout);
	void SetLayout();

public:
	uint32_t m_RendererID = 0;
	BufferLayout m_Layout;
};


class IndexBuffer
{
public:
	IndexBuffer() = default;
	IndexBuffer(uint32_t* indices, uint32_t count);

	IndexBuffer(const IndexBuffer& other) = delete;// copy constructor
	IndexBuffer& operator=(const IndexBuffer& other) = delete;// copy assignment
	IndexBuffer(IndexBuffer&& other) noexcept; // move constructor
	IndexBuffer& operator=(IndexBuffer&& other) noexcept; // move assignment
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	uint32_t GetCount() const { return m_Count; };

public:
	uint32_t m_RendererID = 0;
	uint32_t m_Count;
};







#endif BUFFER_H
