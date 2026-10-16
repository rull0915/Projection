//====================================================//
// ファイル名   : LightManager.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/10/11
//
// 概要 : ライトコンポーネントを管理するクラス
//
// 更新履歴 :
// 2026/10/11 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "Components/World/Light/LightComponent.h"
#include "Managers/ReserveContainer.h"
#include "Scene/SceneInfo.h"

namespace RHEngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class LightManager : public ReserveContainer<LightComponent>
	{
	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// ライトデータ
		SceneLightData m_lightData;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		LightManager()
			: m_lightData{}
		{
		};
		~LightManager() = default;

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		// ライトデータを構築する関数
		void RebuildLightData();

		// ライトデータを取得する関数
		const SceneLightData& GetLightData() const { return m_lightData; }

		// カメラデータとライトからライトのカメラデータを構築する関数
		SceneCameraData GetLightData(const SceneCameraData& cameraData, LightComponent* light, float shadowMapSize) const;

		// 特定のタイプのライトを1つ返す関数
		LightComponent* GetLight(LightType type);

		//-----------------------------------------------------
		// 内部実装
		//-----------------------------------------------------
	private:

		// DirectionalLight
		SceneCameraData GetDirectionalLightData(const SceneCameraData& cameraData, LightComponent* light, float shadowMapSize) const;
	};
}
