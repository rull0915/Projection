/**
 * @file    InputLayoutCache.h
 * @author  Hoshino Ryunosuke
 * @date    2026/09/25
 *
 * @brief   入力レイアウトを保持しておくクラス
 */

#pragma once

#include <vector>
#include <cstdint>
#include <d3d11.h>
#include <wrl/client.h>
#include <unordered_map>

#include "Assets/Types/Vertex/VertexElementInfo.h"
#include "Assets/Types/Shader/ShaderAsset.h"

namespace RHEngine
{
	/**
	 * @brief 入力レイアウトを保持する
	 * @details 頂点構造と頂点シェーダーをキーに、対応するInputLayoutを作成し保持する
	 *			頂点構造のIDと頂点シェーダーのIDをそれぞれ32bitで管理し、結合した64bit整数をキーとして扱う
	 */
	class InputLayoutCache
	{
	private:

		/** @brief 結合済みIDをキーとしたInputLayoutマップ */
		std::unordered_map<uint64_t, Microsoft::WRL::ComPtr<ID3D11InputLayout>> m_inputLayoutCache;

	public:

		/// @name ライフサイクル
		/// @{

		InputLayoutCache() = default;

		~InputLayoutCache() = default;

		/// @}

		/**
		 * @brief InputLayoutを取得する 必要に応じて作成する
		 * @param pDevice デバイス
		 * @param vertexId 頂点構造のID
		 * @param pVS 頂点シェーダーアセット
		 * @param layoutInfo 頂点レイアウト
		 * @return 対応するInputLayout
		 */
		Microsoft::WRL::ComPtr<ID3D11InputLayout> GetOrCreate(
			ID3D11Device* pDevice,	// デバイス
			uint32_t vertexId,		// 頂点構造体のID
			ShaderAsset* pVS,	// シェーダーアセット
			const std::vector<VertexElementInfo>& layoutInfo	// CPU側の頂点レイアウト
		);

		/**
		 * @brief 保持しているInputLayoutを全て解放する
		 */
		void Clear()
		{
			m_inputLayoutCache.clear();
		}
	};
}
