/**
 * @file    RenderInfoManager.h
 * @author  Hoshino Ryunosuke
 * @date    2026/10/15
 *
 * @brief   描画に必要な情報を管理するクラス
 */

#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <SimpleMath.h>

#include "RendererInfo.h"
#include "Scene/SceneInfo.h"

namespace RHEngine
{
	/**
	 * @brief 描画に必要な情報を管理する
	 */
	class RenderInfoManager
	{
	private:

		/** @brief デバイスコンテキスト */
		ID3D11DeviceContext* m_pContext;

		/** @brief フレーム毎のバッファ */
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_perFrameCBuffer;

		/** @brief カメラ毎のバッファ */
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_perCameraCBuffer;

		/** @brief オブジェクト毎のバッファ */
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_perObjectCBuffer;

		/** @brief CPU側フレームバッファ */
		PerFrame m_perFrame;

		/** @brief CPU側カメラバッファ */
		PerCamera m_perCamera;

		/** @brief CPU側オブジェクトバッファ */
		PerObject m_perObject;

	public:

		/// @name ライフサイクル
		/// @{

		RenderInfoManager()
			: m_pContext{ nullptr }
			, m_perFrameCBuffer{}
			, m_perCameraCBuffer{}
			, m_perObjectCBuffer{}
			, m_perFrame{}
			, m_perCamera{}
			, m_perObject{}
		{
		}

		~RenderInfoManager() = default;

		/// @}

		/**
		 * @brief バッファの作成などの初期化を行う
		 * @param pDevice デバイスポインタ
		 * @param pContext コンテキストポインタ
		 */
		void Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

		/**
		 * @brief PerFrameバッファをシーンデータから更新する
		 * @param data 最新のシーンデータ
		 */
		void UpdatePerFrame(const SceneData& data);

		/**
		 * @brief PerCameraバッファをカメラデータから更新する
		 * @param data 最新のカメラデータ
		 */
		void UpdatePerCamera(const SceneCameraData& cameraData);

		/**
		 * @brief PerObjectバッファをワールド行列から更新する
		 * @param data 最新のワールド行列
		 */
		void UpdatePerObject(const DirectX::SimpleMath::Matrix& world);

		/**
		 * @brief PerFrameバッファをバインドする
		 */
		void BindPerFrame();

		/**
		 * @brief PerCameraバッファをバインドする
		 */
		void BindPerCamera();
		
		/**
		 * @brief PerObjectバッファをバインドする
		 */
		void BindPerObject();
	};
}	// namespace RHEngine
