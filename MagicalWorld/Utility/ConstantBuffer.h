/**
* File: ConstantBuffer.h
*
* GPUに送信するデータをラップするクラス
*
* Date: 2016.12.25
* Author: koshi ito
*/
#pragma once
#include <d3d11.h>

// Strongly typed wrapper around a D3D constant buffer.
template<typename T>
class __declspec(align(16)) ConstantBuffer : public T
{
public:
	// Constructor.
	ConstantBuffer() {}
	explicit ConstantBuffer(_In_ ID3D11Device* device)
	{
		Create(device);
	}

	void Create(_In_ ID3D11Device* device)
	{
		D3D11_BUFFER_DESC desc = { 0 };

		desc.ByteWidth = sizeof(T);
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		device->CreateBuffer(&desc, nullptr, mConstantBuffer.ReleaseAndGetAddressOf());
	}


	// Writes new data into the constant buffer.
	void SetData(_In_ ID3D11DeviceContext* deviceContext)
	{
		assert(mConstantBuffer.Get() != 0);

		D3D11_MAPPED_SUBRESOURCE mappedResource;

		deviceContext->Map(mConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

		*(T*)mappedResource.pData = *(T*)this;

		deviceContext->Unmap(mConstantBuffer.Get(), 0);
	}


	// Looks up the underlying D3D constant buffer.
	ID3D11Buffer* GetBuffer()
	{
		return mConstantBuffer.Get();
	}


	// Allocate aligned memory.
	static void* operator new (size_t size)
	{
		const size_t alignment = __alignof(ConstantBuffer<T>);

	static_assert(alignment > 8, "AlignedNew is only useful for types with > 8 byte alignment. Did you forget a __declspec(align) on TDerived?");

	void* ptr = _aligned_malloc(size, alignment);

	if (!ptr)
		throw std::bad_alloc();

	return ptr;
	}

		// Free aligned memory.	
		static void operator delete (void* ptr)
	{
		_aligned_free(ptr);
	}


private:
	// The underlying D3D object.
	Microsoft::WRL::ComPtr<ID3D11Buffer> mConstantBuffer;


	// Prevent copying.
	ConstantBuffer(ConstantBuffer const&) = delete;
	ConstantBuffer& operator= (ConstantBuffer const&) = delete;


};