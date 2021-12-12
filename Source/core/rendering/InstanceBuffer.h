
#ifndef INSTANCE_BUFFER_H
#define INSTANCE_BUFFER_H

#include <core/rendering/Buffer.h>

struct TransformComponent;

class InstanceBuffer
{
public:
	InstanceBuffer() = default;
	InstanceBuffer(uint32_t size, uint32_t startIdx = 0);
	InstanceBuffer(const std::vector<TransformComponent>& data, uint32_t startIdx = 0);
	~InstanceBuffer();

	void Bind() const;
	void Unbind() const;

	void SetData(const std::vector<TransformComponent>& data);

	const BufferLayout& GetLayout() const { return s_Layout; };
//	void SetLayout(const BufferLayout& layout); // this is intentionally removed, this class should have 1 layout
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







#endif INSTANCE_BUFFER_H
