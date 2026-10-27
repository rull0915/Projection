//====================================================//
// ファイル名  : DynamicVertexBuffer.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/10/25
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "Assets/Types/Vertex/DynamicVertexBuffer.h"

//====================================================//
// 関数の実体宣言
//====================================================//

namespace RHEngine
{
	bool DynamicVertexBuffer::Initialize(ID3D11Device* device, size_t maxVertices, size_t vertexStride)
	{
		// 設定の初期化
		m_maxVertices = maxVertices;
		m_vertexStride = vertexStride;
		m_currentOffset = 0;
		m_needDiscard = true;

		// 頂点バッファ用DESCの作成
		D3D11_BUFFER_DESC desc = {};
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.ByteWidth = static_cast<UINT>(maxVertices * vertexStride);	// 最大時のバッファサイズ
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;		// 頂点バッファ
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// CPUから書き換えられるように

		// 頂点バッファの作成
		HRESULT hr = device->CreateBuffer(&desc, nullptr, m_pBuffer.ReleaseAndGetAddressOf());

		// 成功したかどうかを返す
		return SUCCEEDED(hr);
	}

	uint32_t DynamicVertexBuffer::Write(ID3D11DeviceContext* context, const void* pVertex, size_t vertexCount)
	{
		// 不正な頂点構造の場合書き込まない
		if (!pVertex || vertexCount == 0 || vertexCount > m_maxVertices) return 0;

		// デフォルトはGPU領域を上書しない
		D3D11_MAP mapType = D3D11_MAP_WRITE_NO_OVERWRITE;

		// バッファ末尾を超える場合、またはフレーム最初の書き込み時はDISCARDでリセット
		if (m_needDiscard || (m_currentOffset + vertexCount > m_maxVertices))
		{
			// 古い内容を破棄する方式に設定
			mapType = D3D11_MAP_WRITE_DISCARD;

			// リセット
			m_currentOffset = 0;
			m_needDiscard = false;
		}

		// サブリソースを作成
		D3D11_MAPPED_SUBRESOURCE mapped = {};

		// マップ
		HRESULT hr = context->Map(m_pBuffer.Get(), 0, mapType, 0, &mapped);

		// 失敗したら中断
		if (FAILED(hr)) return 0;

		// 書き込み先アドレスの計算
		uint8_t* pDst = static_cast<uint8_t*>(mapped.pData) + (m_currentOffset * m_vertexStride);

		// コピー
		std::memcpy(pDst, pVertex, vertexCount * m_vertexStride);

		context->Unmap(m_pBuffer.Get(), 0);

		// 現在のオフセットを加算
		uint32_t startOffset = static_cast<uint32_t>(m_currentOffset);
		m_currentOffset += vertexCount;

		// 頂点バッファの開始オフセットを返す
		return startOffset;
	}

	void DynamicVertexBuffer::Bind(ID3D11DeviceContext* pContext, UINT slot) const
	{
		UINT stride = static_cast<UINT>(m_vertexStride);
		UINT offset = 0;

		// バッファ配列を作成
		ID3D11Buffer* buffers[] = { m_pBuffer.Get() };

		pContext->IASetVertexBuffers(slot, 1, buffers, &stride, &offset);
	}
}
