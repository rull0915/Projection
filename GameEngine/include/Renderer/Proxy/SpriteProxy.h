//====================================================//
// ファイル名   : SpriteProxy.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/05/26
//
// 概要 : スプライト描画仲介クラス
//
// 更新履歴 :
// 2026/05/26 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "Renderer/EachRenderer/SpriteRenderer.h"

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//

	// 各パラメータが既に設定されたかどうかをテンプレート引数として持つ
	template <bool HasRect = false, bool HasSrc = false, bool HasExtend = false, bool HasRotate = false, bool HasOrigin = false>
	class SpriteProxy
	{
		// 全てのタイプをfriendに指定
		template <bool R, bool S, bool E, bool Ro, bool O>
		friend class SpriteProxy;

	private:
		SpriteRenderer* m_sprite = nullptr;

		// 描画のパラメータ
		DirectX::SimpleMath::Vector2 m_min = { 0, 0 };          // 描画先の矩形の左上座標
		DirectX::SimpleMath::Vector2 m_max = { 0, 0 };          // 描画先の矩形の右下座標
		RECT m_srcRect = { 0, 0, 1, 1 };                        // 描画元の矩形
		DirectX::SimpleMath::Vector2 m_scale = { 1.0f, 1.0f };  // 拡大率
		float m_angle = 0.0f;                                   // 回転角
		DirectX::SimpleMath::Vector2 m_origin = { 0, 0 };       // 回転と拡大の中心座標（0～1の範囲で指定）

	public:

		// デフォルトコンストラクタ 
		SpriteProxy()
			: m_sprite(nullptr)
			, m_min{ 0, 0 }
			, m_max{ 0, 0 }
			, m_srcRect{ 0, 0, 1, 1 }
			, m_scale{ 1.0f, 1.0f }
			, m_angle(0.0f)
			, m_origin{ 0, 0 }
		{}

		// コピーコンストラクタ（状態が変わる時に設定を引き継ぐため）
		template <bool R, bool S, bool E, bool Ro, bool O>
		SpriteProxy(const SpriteProxy<R, S, E, Ro, O>& other)
			: m_sprite(other.m_sprite)
			, m_min(other.m_min)
			, m_max(other.m_max)
			, m_srcRect(other.m_srcRect)
			, m_scale(other.m_scale)
			, m_angle(other.m_angle)
			, m_origin(other.m_origin)
		{}

		// 描画クラスをセットする関数
		void SetSprite(SpriteRenderer* sprite) { m_sprite = sprite; }

		// 各パラメータを追加する関数

		// 描画先の矩形
		template <typename = std::enable_if_t<!HasRect>>
		SpriteProxy<true, HasSrc, HasExtend, HasRotate, HasOrigin> Rect(DirectX::SimpleMath::Vector2 min, DirectX::SimpleMath::Vector2 max) {
			m_min = min;
			m_max = max;
			return SpriteProxy<true, HasSrc, HasExtend, HasRotate, HasOrigin>(*this);
		}

		// 描画元の矩形
		template <typename = std::enable_if_t<!HasSrc>>
		SpriteProxy<HasRect, true, HasExtend, HasRotate, HasOrigin> Src(RECT srcRect) {
			m_srcRect = srcRect;
			return SpriteProxy<HasRect, true, HasExtend, HasRotate, HasOrigin>(*this);
		}

		// 拡大率
		template <typename = std::enable_if_t<!HasExtend>>
		SpriteProxy<HasRect, HasSrc, true, HasRotate, HasOrigin> Extend(DirectX::SimpleMath::Vector2 scale) {
			m_scale = scale;
			return SpriteProxy<HasRect, HasSrc, true, HasRotate, HasOrigin>(*this);
		}
		template <typename = std::enable_if_t<!HasExtend>>
		SpriteProxy<HasRect, HasSrc, true, HasRotate, HasOrigin> Extend(float scale) {
			m_scale = DirectX::SimpleMath::Vector2(scale, scale);
			return SpriteProxy<HasRect, HasSrc, true, HasRotate, HasOrigin>(*this);
		}

		// 回転角
		template <typename = std::enable_if_t<!HasRotate>>
		SpriteProxy<HasRect, HasSrc, HasExtend, true, HasOrigin> Rotate(float angle) {
			m_angle = angle;
			return SpriteProxy<HasRect, HasSrc, HasExtend, true, HasOrigin>(*this);
		}

		// 回転と拡大の中心座標
		template <typename = std::enable_if_t<!HasOrigin>>
		SpriteProxy<HasRect, HasSrc, HasExtend, HasRotate, true> Origin(DirectX::SimpleMath::Vector2 origin) {
			m_origin = origin;
			return SpriteProxy<HasRect, HasSrc, HasExtend, HasRotate, true>(*this);
		}
		template <typename = std::enable_if_t<!HasOrigin>>
		SpriteProxy<HasRect, HasSrc, HasExtend, HasRotate, true> Origin(Origin::Type type) {
			m_origin = Origin::GetPosition(type);
			return SpriteProxy<HasRect, HasSrc, HasExtend, HasRotate, true>(*this);
		}

		// 状態のリセット
		void Reset()
		{
			m_min = { 0, 0 };
			m_max = { 0, 0 };
			m_srcRect = { 0, 0, 1, 1 };
			m_scale = { 1.0f, 1.0f };
			m_angle = 0.0f;
			m_origin = { 0, 0 };
		}

		// 描画を確定する関数
		template <typename = std::enable_if_t<HasRect>> // Rectを指定している場合
		void Execute(ID3D11ShaderResourceView* texture, DirectX::SimpleMath::Color color = { 1, 1, 1, 1 })
		{
			// Originを考慮して中心を計算
			DirectX::SimpleMath::Vector2 center = m_min + (m_max - m_min) * m_origin;

			DirectX::SimpleMath::Vector2 textureSize = SpriteRenderer::GetTextureSize(texture);

			// スケールを計算（矩形のサイズをテクスチャのサイズで割る）
			DirectX::SimpleMath::Vector2 finalScale = (m_max - m_min) / textureSize;
			if constexpr (HasExtend) {
				finalScale *= m_scale; // Extendが呼ばれている時だけ掛け算する
			}

			// 描画

			// SpriteRendererの描画関数を呼び出す
			m_sprite->Draw(
				texture,
				center,
				(HasSrc ? &m_srcRect : nullptr),
				finalScale,
				m_angle,
				textureSize * m_origin,
				color
			);
		}
		template <typename = std::enable_if_t<!HasRect>> // Rectを指定していない場合
		void Execute(ID3D11ShaderResourceView* texture, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Color color = { 1, 1, 1, 1 })
		{
			if constexpr (HasOrigin)
			{
				DirectX::SimpleMath::Vector2 textureSize = SpriteRenderer::GetTextureSize(texture);

				// SpriteRendererの描画関数を呼び出す
				m_sprite->Draw(
					texture,
					position,
					(HasSrc ? &m_srcRect : nullptr),
					m_scale,
					m_angle,
					textureSize * m_origin,
					color
				);
			}
			else
			{
				// SpriteRendererの描画関数を呼び出す
				m_sprite->Draw(
					texture,
					position,
					(HasSrc ? &m_srcRect : nullptr),
					m_scale,
					m_angle,
					{ 0, 0 },
					color
				);
			}
		}
	};
}	// namespace REngine
