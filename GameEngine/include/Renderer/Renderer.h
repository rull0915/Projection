//====================================================//
// ファイル名   : Renderer.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/04/29
//
// 概要 : 描画を統括するクラス
//
// 更新履歴 :
// 2026/04/29 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//

#include "GraphicsSystem.h"
#include "RenderProxy.h"
#include "RenderInfoManager.h"
#include "Scene/SceneInfo.h"

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class Renderer
	{
	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// 描画仲介クラス
		RenderProxy m_renderProxy;

		// 描画システム
		GraphicsSystem m_graphicSystem;

		// 描画情報
		RenderInfoManager m_infoManager;

		// 初期化フラグ
		bool m_isInitialized;

		// 使用中のカメラデータ
		SceneCameraData m_currentCamera;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		Renderer(AssetManager& am);
		~Renderer() = default;

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		// 初期化
		void Initialize();

		// 描画開始
		void Begin(const SceneData& data);

		// カメラを変更する関数
		void ChangeCamera(const SceneCameraData& data);

		// 描画関数
		RenderProxy& Draw()
		{
			// 初期化チェック
			assert(m_isInitialized);

			// 仲介クラスを返す
			return m_renderProxy;
		}

		// 描画コマンドの発行
		void ExecuteCommands();

		// 描画終了
		void End();

		//-----------------------------------------------------
		// ゲッター
		//-----------------------------------------------------

		const DirectX::SimpleMath::Matrix& GetView() const { return m_currentCamera.view; };
		const DirectX::SimpleMath::Matrix& GetProjection() const { return m_currentCamera.proj; };

		//-----------------------------------------------------
		// セッター
		//-----------------------------------------------------

		void SetWorld(const DirectX::SimpleMath::Matrix& world) { m_graphicSystem.SetWorld(world); }

		void SetMaterial(Handle<MaterialAsset> handle) { m_graphicSystem.SetMaterial(handle); };
	};
}	// namespace REngine
