/**
 * @file    AudioSource.h
 * @author  Hoshino Ryunosuke
 * @date    2026/05/27
 *
 * @brief   音再生コンポーネント
 */

#pragma once

#include <memory>
#include <Audio.h>

#include "Components/ComponentBase.h"
#include "Components/Both/BothComponentBase.h"
#include "Components/Interface/IAssetDependent.h"
#include "Assets/Objects/Handle.h"
#include "Assets/Types/AudioClip.h"

namespace RHEngine
{
	class AssetManager;
	class IComponentOwner;

	/**
	 * @brief 音を再生するコンポーネント
	 */
	class AudioSource : public BothComponentBase, public IAssetDependent
	{
	public:
		DECLARE_COMPONENT_TYPE(BothComponentBase);

	private:

		/** @brief 音声データアセット */
		Handle<AudioClip> m_audioClip;

		/** @brief サウンドインスタンス */
		std::unique_ptr<DirectX::SoundEffectInstance> m_soundInstance;

		/** @brief アセットマネージャーポインタ */
		AssetManager* m_pAssetManager;

		/** @brief 音量 */
		float m_volume;

		/** @brief パン */
		float m_pan;

		/** @brief ピッチ */
		float m_pitch;

		/** @brief ループ再生するかどうか */
		bool m_loop;

		/** @brief 開始時に再生するかどうか */
		bool m_playOnStart;

		/** @brief 3D空間で再生するかどうか */
		bool m_use3DAudio;

	public:
		
		/// @name ライフサイクル
		/// @{

		/**
		 * @param pOwner 所有者のポインタ
		 */
		AudioSource(IComponentOwner* pOwner);

		~AudioSource() = default;

		/// @}

		/**
		 * @brief オーディオを読み込む
		 * @param handle 読み込み対象の音声アセットハンドル
		 */
		void Load(Handle<AudioClip> handle);

		/**
		 * @brief 開始時に一度呼ばれる
		 */
		void Start() override;

		/**
		 * @brief アセットマネージャーを受け取る
		 */
		void ReceiveAssetManager(AssetManager& assetManager) override
		{
			m_pAssetManager = &assetManager;
		}

		/**
		 * @brief サウンドを再生する
		 */
		void Play() const;

		/**
		 * @brief 再生中のサウンドを停止する
		 */
		void Stop() const;

		/**
		 * @brief GUI上で値が変更された際に呼ばれる
		 */
		void OnValidate() override;

		/**
		 * @brief 設定を反映させる
		 */
		void ReflectSetting();

		/// @name ゲッター
		/// @{

		/** @brief サウンドインスタンス */
		DirectX::SoundEffectInstance* GetSoundInstance() const { return m_soundInstance.get(); }

		/** @brief 3D空間で再生するかどうか */
		bool Is3D() const { return m_use3DAudio; }

		/** @brief ボリューム */
		float GetVolume() const { return m_volume; };

		/** @brief パン */
		float GetPan() const { return m_pan; }

		/** @brief ピッチ */
		float GetPitch() const { return m_pitch; }

		/** @brief ループ再生するかどうか */
		bool GetLoop() const { return m_loop; }

		/** @brief 開始時に再生するかどうか */
		bool GetPlayOnStart() const { return m_loop; }

		/// @}

		/// @name セッター
		/// @{

		/** @brief ボリューム */
		void SetVolume(float volume)
		{
			m_volume = volume;

			ReflectSetting();
		};

		/** @brief パン */
		void SetPan(float pan)
		{
			m_pan = pan;

			ReflectSetting();
		}

		/** @brief ピッチ */
		void SetPitch(float pitch)
		{
			m_pitch = pitch;

			ReflectSetting();
		}

		/** @brief ループするかどうか */
		void SetLoop(bool f) { m_loop = f; }

		/** @brief 開始時再生するかどうか */
		void SetPlayOnStart(bool f) { m_playOnStart = f; }

		/** @brief 3D空間で再生するかどうか */
		void SetUse3D(bool f) { m_use3DAudio = f; }

		/// @}
	};
} // namespace RHEngine
