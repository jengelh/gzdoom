#pragma once

#include "hwrenderer/data/buffers.h"
#include "utility/tarray.h"
#include <vector>

#ifdef _MSC_VER
// silence bogus warning C4250: 'PolyVertexBuffer': inherits 'PolyBuffer::PolyBuffer::SetData' via dominance
// According to internet infos, the warning is erroneously emitted in this case.
#pragma warning(disable:4250) 
#endif

struct TriVertex;

class PolyBuffer : virtual public IBuffer
{
public:
	PolyBuffer();
	~PolyBuffer();

	static void ResetAll();
	void Reset();

	void SetData(size_t size, const void *data, bool staticdata) override;
	void SetSubData(size_t offset, size_t size, const void *data) override;
	void Resize(size_t newsize) override;

	void Map() override;
	void Unmap() override;

	void *Lock(unsigned int size) override;
	void Unlock() override;

private:
	static PolyBuffer *First;
	PolyBuffer *Prev = nullptr;
	PolyBuffer *Next = nullptr;
	std::vector<uint32_t> mData;
};

class PolyVertexBuffer : public IVertexBuffer, public PolyBuffer
{
public:
	PolyVertexBuffer() { }
	void SetFormat(int numBindingPoints, int numAttributes, size_t stride, const FVertexBufferAttribute *attrs) override;

	void CopyVertices(TriVertex *dst, int count, int index);
	void CopyIndexed(TriVertex *dst, uint32_t *elements, int count, int index);

private:
	size_t mOffsets[VATTR_MAX] = {};
	size_t mStride = 0;
};

class PolyIndexBuffer : public IIndexBuffer, public PolyBuffer
{
public:
	PolyIndexBuffer() { }
};

class PolyDataBuffer : public IDataBuffer, public PolyBuffer
{
public:
	PolyDataBuffer(int bindingpoint, bool ssbo, bool needresize) : bindingpoint(bindingpoint)
	{
	}

	void BindRange(size_t start, size_t length) override;
	void BindBase() override;

	int bindingpoint;
};
