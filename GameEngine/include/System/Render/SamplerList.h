/**
 * @file    SamplerList.h
 * @author  Hoshino Ryunosuke
 * @date    2026/08/08
 *
 * @brief   サンプラーを保持するクラス
 */

#pragma once

#include <unordered_map>
#include <d3d11.h>
#include <wrl/client.h>

#include "Assets/Types/Shader/SamplerType.h"

namespace RHEngine
{
	/**
	 * @brief サンプラーを保持する
	 */
	class SamplerList
	{
	private:

		/** @brief 種類をキーとしたサンプラーステートのマップ */
		std::unordered_map<SamplerType, Microsoft::WRL::ComPtr<ID3D11SamplerState>> m_samplers;

	public:

		/// @name ライフサイクル
		/// @{

		SamplerList() = default;

		~SamplerList() = default;

		/// @}

		/**
		 * @brief 各サンプラータイプに対応するサンプラーの作成をする
		 * @param pDevice デバイス
		 */
		void Initialize(ID3D11Device* pDevice);

		/**
		 * @brief サンプラーを取得する
		 * @param type 取得するサンプラーのタイプ
		 * @return サンプラー
		 */
		const Microsoft::WRL::ComPtr<ID3D11SamplerState>& GetSampler(SamplerType type) const;

	private:

		/**
		 * @brief サンプラーを作成する
		 * @param device デバイス
		 * @param type 作成するタイプ
		 * @param filter フィルター
		 * @param mode モード
		 */
		void CreateSampler(ID3D11Device* device, SamplerType type, D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE mode);
	};
}	// namespace RHEngine
