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
#include <algorithm>

#include "Components/World/WorldComponentBase.h"
#include "GameObject/Interface/IComponentOwner.h"

//====================================================//
// 前方宣言
//====================================================//
class Gam;

//====================================================//
// クラス宣言
//====================================================//
class AudioSource : public WorldComponentBase
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
	void Load(const std::string& key, bool use3D = false);

	void Start() override;

	// 再生
	void Play() const;

	// 停止
	void Stop() const;

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
		m_volume = std::clamp(volume, 0.0f, 1.0f); 
		
		if (m_soundInstance) m_soundInstance->SetVolume(m_volume);
	};

	// パン
	void SetPan(float pan) 
	{
		m_pan = std::clamp(pan, -1.0f, 1.0f); 

		if (m_soundInstance) m_soundInstance->SetPan(m_pan);
	}

	// ピッチ
	void SetPitch(float pitch) 
	{
		m_pitch = std::clamp(pitch, -1.0f, 1.0f); 
		
		if (m_soundInstance) m_soundInstance->SetPitch(m_pitch);
	}

	// ループ
	void SetLoop(bool f) { m_loop = f; }

	// 開始時
	void SetPlayOnStart(bool f) { m_playOnStart = f; }

	// 3Dオーディオ
	void SetUse3D(bool f) { m_use3DAudio = f; }

private:

	//-----------------------------------------------------
	// 内部実装
	//-----------------------------------------------------

};
