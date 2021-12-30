
// this class is almost identical to the instance buffer,
// so that could be generalized, and used instead of this
// as well

#ifndef COLOUR_INSTANCE_BUFFER
#define COLOUR_INSTANCE_BUFFER

#include <core/rendering/Buffer.h>

struct ColourComponent;

class ColourInstanceBuffer
{
public:
	ColourInstanceBuffer() = default;
	ColourInstanceBuffer(uint32_t size, uint32_t startIdx = 0);
	ColourInstanceBuffer(const std::vector<ColourComponent>& data, uint32_t startIdx = 0);
	~ColourInstanceBuffer();

	void Bind() const;
	void Unbind() const;

	void SetData(const std::vector<ColourComponent>& data);

	const BufferLayout& GetLayout() const { return s_Layout; };
	void SetLayout();

	void SetAttribStartIdx(uint32_t idx);
	uint32_t GetCapacity();
	uint32_t GetElementCount();

private:
	uint32_t m_RendererID = 0;
	uint32_t m_AttribPtrStartIdx = 0;
	size_t m_ElementCount = 0;
	size_t m_Capacity = 0;

private:
	static BufferLayout s_Layout;

};

#endif // COLOUR_INSTANCE_BUFFER
