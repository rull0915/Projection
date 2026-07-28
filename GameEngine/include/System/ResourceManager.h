//====================================================//
// ファイル名   : ResourceManager.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/03/31
//
// 概要 :
//
// 更新履歴 : リソース関連のゲーム全体で共有する変数をまとめたクラス
// 2026/03/31 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <Effects.h>
#include <Audio.h>

//====================================================//
// クラス宣言
//====================================================//

namespace REngine
{
	class ResourceManager
	{
	private:
		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// 音管理クラス
		std::unique_ptr<DirectX::AudioEngine> m_audioEngine;

		// エフェクトファクトリ
		std::unique_ptr<DirectX::EffectFactory> m_effect;

	private:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		ResourceManager();

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------
	public:

		~ResourceManager();

		// シングルトン化
		static ResourceManager& Instance()
		{
			static ResourceManager instance;
			return instance;
		}

		// 初期化
		void Initialize();

		// 更新
		void Update();

		// ゲッター
		DirectX::EffectFactory* GetEffectFactory() { return m_effect.get(); }

		// ゲッター
		DirectX::AudioEngine* GetAudioEngine() { return m_audioEngine.get(); }
	};

} // namespace REngine
