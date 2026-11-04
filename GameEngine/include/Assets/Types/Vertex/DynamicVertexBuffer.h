/**
 * @file    DynamicVertexBuffer.h
 * @author  Hoshino Ryunosuke
 * @date    2026/10/25
 *
 * @brief   CPU側で頻繁に変更される頂点バッファを管理するクラス
 */

#pragma once

#include <cstdint>
#include <wrl/client.h>
#include <d3d11.h>

namespace RHEngine
{
	/**
	 * @brief 動的頂点バッファを管理する
	 */
	class DynamicVertexBuffer
	{
	private:
		/**	@brief 頂点バッファ */
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pBuffer;

		/**	@brief 最大頂点数 */
		size_t m_maxVertices = 0;

		/**	@brief 1頂点のバイトサイズ */
		size_t m_vertexStride = 0;

		/**	@brief 現在の書き込みオフセット(頂点単位) */
		size_t m_currentOffset = 0;

		/**	@brief 次回の書き込みでWRITE_DISCARDを使用するか */
		bool m_needDiscard = true;

	public:
		/// @name ライフサイクル
		/// @{
		
		DynamicVertexBuffer() = default;

		~DynamicVertexBuffer() = default;

		/// @}

		/**
		 * @brief 頂点バッファを作成する
		 * @param device デバイスポインタ
		 * @param maxVertices 最大頂点数
		 * @param vertexStride 1頂点のバイトサイズ
		 * @return 成功したかどうか
		 */
		bool Initialize(ID3D11Device* pDevice, size_t maxVertices, size_t vertexStride);

		/**
		 * @brief 頂点バッファを書き込む
		 * @param context デバイスコンテキスト
		 * @param pVertex 頂点配列の先頭アドレス
		 * @param vertexCount 頂点数
		 * @return 頂点バッファ内の開始オフセット(頂点単位。Draw の StartVertexLocation に使用)
		 */
		uint32_t Write(ID3D11DeviceContext* pContext, const void* pVertex, size_t vertexCount);

		/**
		 * @brief IAステージに頂点バッファをバインドをする
		 * @param pContext コンテキストポインタ
		 * @param slot バインドするスロット番号
		 */
		void Bind(ID3D11DeviceContext* pContext, UINT slot = 0) const;

		/** @brief データの破棄をリクエストする */
		void RequestDiscard() { m_needDiscard = true; }
	};
}	// namespace RHEngine
