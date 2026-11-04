/**
 * @file    Texture.h
 * @author  Hoshino Ryunosuke
 * @date    2026/07/26
 *
 * @brief   画像アセット
 */

#pragma once

#include <wrl/client.h>
#include <d3d11.h>

#include "Assets/Objects/AssetBase.h"

namespace RHEngine
{
	/**
	 * @brief 画像アセット
	 */
	class Texture : public AssetBase
	{
	private:

		/** @brief 画像本体のSRV */
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_srv;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param srv 画像本体
		 */
		Texture(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv)
			: m_srv{ srv }
		{
		}

		~Texture() = default;

		/// @}

		/**
		 * @brief 画像を取得する
		 * @return 画像へのポインタ
		 */
		ID3D11ShaderResourceView* Get() const { return m_srv.Get(); }

		/**
		 * @brief テクスチャポインタを格納するメモリのアドレスを取得する
		 * @return 画像のポインタへのポインタ
		 */
		ID3D11ShaderResourceView* const* GetAddressOf() const { return m_srv.GetAddressOf(); }
	};
}	// namespace RHEngine
