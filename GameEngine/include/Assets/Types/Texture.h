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
#include <d3dcommon.h>
#include <dxgiformat.h>

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

		/** @brief 画像のの次元タイプ */
		D3D_SRV_DIMENSION m_dimention;

		/** @brief 画像のフォーマット */
		DXGI_FORMAT m_format;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param srv 画像本体
		 */
		Texture(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv);

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

		/**
		 * @brief 画像のフォーマットを取得する
		 */
		DXGI_FORMAT GetFormat() const { return m_format; }

		/**
		 * @brief 画像の次元タイプを取得する
		 */
		D3D_SRV_DIMENSION GetDimention() const { return m_dimention; }
	};
}	// namespace RHEngine
