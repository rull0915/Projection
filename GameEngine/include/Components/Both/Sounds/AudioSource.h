//====================================================//
// ファイル名   : AudioSource.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/05/27
//
// 概要 : 音再生コンポーネント
//
// 更新履歴 :
// 2026/05/27 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <memory>
#include <Audio.h>

#include "Components/Both/BothComponentBase.h"
#include "Components/Interface/IAssetDependent.h"
#include "Assets/Objects/Handle.h"
#include "Assets/Types/AudioClip.h"

//====================================================//
// クラス宣言
//====================================================//

namespace REngine
{
	class AudioSource : public BothComponentBase, public IAssetDependent
	{
	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// オーディオハンドル
		Handle<AudioClip> m_audioClip;

		// インスタンス
		std::unique_ptr<DirectX::SoundEffectInstance> m_soundInstance;

		// アセットマネージャー
		AssetManager* m_assetManager;

		// 音量
		float m_volume;

		// パン
		float m_pan;

		// ピッチ
		float m_pitch;

		// ループ再生
		bool m_loop;

		// 開始時に呼ぶかどうか
		bool m_playOnStart;

		// 3Dオーディオにするかどうか
		bool m_use3DAudio;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		AudioSource(IComponentOwner* own);
		~AudioSource() = default;

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		// オーディオ読み込み関数
		void Load(Handle<AudioClip> handle);

		void Start() override;

		void ReceiveAssetManager(AssetManager& assetManager) override
		{
			m_assetManager = &assetManager;
		}

		// 再生
		void Play() const;

		// 停止
		void Stop() const;

		// GUI変更時
		void OnValidate() override;

		//-----------------------------------------------------
		// ゲッター
		//-----------------------------------------------------

		// ID取得
		unsigned int GetID() override
		{
			return TypeIDGenerator::GetID<AudioSource>();
		}

		// インスタンス
		DirectX::SoundEffectInstance* GetSoundInstance() const { return m_soundInstance.get(); }

		// 3Dかどうか
		bool Is3D() const { return m_use3DAudio; }

		// ボリューム
		float GetVolume() const { return m_volume; };

		// パン
		float GetPan() const { return m_pan; }

		// ピッチ
		float GetPitch() const { return m_pitch; }

		// ループ
		bool GetLoop() const { return m_loop; }

		// 開始時
		bool GetPlayOnStart() const { return m_loop; }

		// 3Dオーディオ
		bool GetUse3D() const { return m_use3DAudio; }

		//-----------------------------------------------------
		// セッター
		//-----------------------------------------------------

		// ボリューム
		void SetVolume(float volume)
		{
			m_volume = volume;

			ReflectSetting();
		};

		// パン
		void SetPan(float pan)
		{
			m_pan = pan;

			ReflectSetting();
		}

		// ピッチ
		void SetPitch(float pitch)
		{
			m_pitch = pitch;

			ReflectSetting();
		}

		// ループ
		void SetLoop(bool f) { m_loop = f; }

		// 開始時
		void SetPlayOnStart(bool f) { m_playOnStart = f; }

		// 3Dオーディオ
		void SetUse3D(bool f) { m_use3DAudio = f; }

		// 設定の反映
		void ReflectSetting();
	};
} // namespace REngine
