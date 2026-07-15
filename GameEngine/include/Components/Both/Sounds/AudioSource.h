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
#include "GameObject/Interface/IComponentOwner.h"
#include "Components/Interface/IResourceReader.h"

//====================================================//
// 前方宣言
//====================================================//
class Gam;

//====================================================//
// クラス宣言
//====================================================//

namespace REngine
{
	class AudioSource : public BothComponentBase, public IResourceReader
	{
	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// サウンドエフェクト
		DirectX::SoundEffect* m_soundEffect;

		// インスタンス
		std::unique_ptr<DirectX::SoundEffectInstance> m_soundInstance;

		// 音声名
		std::string m_soundName;

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
		void Load(const std::string& key, bool use3D = false)
		{
			m_soundName = key;
			m_use3DAudio = use3D;

			LoadResource();
			ReflectLoading();
		}

		void Start() override;

		// 再生
		void Play() const;

		// 停止
		void Stop() const;

		// GUI変更時
		void OnValidate() override
		{
			ReflectSetting();
			LoadResource();
			ReflectLoading();
		}

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

		// 音声名
		const std::string& GetSoundName() const { return m_soundName; }

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

		// ---------- リソース関連 ---------- //

		// 読み込みを反映する
		void ReflectLoading() override;

	private:
		// リソースタイプ
		Type GetType() const override { return Type::Sound; }

		// キー名
		const std::string& GetKeyName() const override { return m_soundName; }

		// リソースポインタポインタ
		void** GetMyResource() const override { return (void**)&m_soundEffect; }
	};
} // namespace REngine
