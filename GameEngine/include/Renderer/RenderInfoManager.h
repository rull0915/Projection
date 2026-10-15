//====================================================//
// ファイル名   : RenderInfoManager.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/10/15
//
// 概要 : 描画に必要な情報を管理するクラス
//
// 更新履歴 :
// 2026/10/15 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//

#include "RendererInfo.h"
#include "Scene/SceneInfo.h"

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class RenderInfoManager
	{
	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// デバイスコンテキスト
		ID3D11DeviceContext* m_pContext;

		// GPU側バッファ
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_perFrameCBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_perCameraCBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_perObjectCBuffer;

		// CPU側バッファ
		PerFrame m_perFrame;
		PerCamera m_perCamera;
		PerObject m_perObject;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
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

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		// 初期化関数
		void Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

		// PerFrameバッファをシーン情報から更新する関数
		void UpdatePerFrame(const SceneData& data);

		// PerCameraバッファをカメラ情報から更新する関数
		void UpdatePerCamera(const SceneCameraData& cameraData);

		// PerObjectバッファをワールド行列から更新する関数
		void UpdatePerObject(const DirectX::SimpleMath::Matrix& world);

		// PerFrameバッファをバインドする関数
		void BindPerFrame();

		// PerCameraバッファをバインドする関数
		void BindPerCamera();
		
		// PerObjectバッファをバインドする関数
		void BindPerObject();
	};
}
