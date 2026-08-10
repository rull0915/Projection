//====================================================//
// ファイル名   : DrawCommand.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/07/18
//
// 概要 : 描画commandをまとめたヘッダ
//
// 更新履歴 :
// 2026/07/18 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//

// stl
#include <cstdint>
#include <optional>
// DXTK
#include <SimpleMath.h>
#include <Model.h>
#include <SpriteFont.h>
#include <VertexTypes.h>
// User
#include "../RendererType.h"
#include "Assets/Objects/Handle.h"

namespace REngine
{
	class MaterialAsset;

	//====================================================//
	// 構造体宣言
	//====================================================//

	/// <summary>
	/// 描画コマンドの基底
	/// </summary>
	struct DrawCommandBase
	{
		// 描画順
		uint64_t sortKey;
		
		// 描画タイプ
		RendererType type;

		// コンストラクタ
		DrawCommandBase(RendererType t, uint64_t s = 0)
			: type{ t }, sortKey{ s } 
		{}

		// 仮想デストラクタ
		virtual ~DrawCommandBase() = default;
	};

	/// <summary>
	/// モデル描画コマンド
	/// </summary>
	struct DrawModelCommand : public DrawCommandBase
	{
		// モデル
		DirectX::Model* pModel = nullptr;

		// マテリアル
		Handle<MaterialAsset> material = ERROR_HANDLE<MaterialAsset>;

		// ワールド行列
		DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::Identity;

		// コンストラクタ
		DrawModelCommand()
			: DrawCommandBase(RendererType::Model)
		{}
	};

	/// <summary>
	/// スプライト描画コマンド
	/// </summary>
	struct DrawSpriteCommand : public DrawCommandBase
	{
		// テクスチャ
		ID3D11ShaderResourceView* pTexture = nullptr;

		// マテリアル
		Handle<MaterialAsset> material = ERROR_HANDLE<MaterialAsset>;

		// 座標
		DirectX::SimpleMath::Vector2 pos = DirectX::SimpleMath::Vector2::Zero;

		// 矩形
		std::optional<RECT> srcRect = std::nullopt;

		// 拡大率
		DirectX::SimpleMath::Vector2 scale = DirectX::SimpleMath::Vector2(1, 1);

		// 回転角
		float angle = 0;

		// 原点
		DirectX::SimpleMath::Vector2 origin = DirectX::SimpleMath::Vector2(0.5f, 0.5f);

		// 乗算色
		DirectX::SimpleMath::Color color = DirectX::SimpleMath::Color(1, 1, 1, 1);

		// コンストラクタ
		DrawSpriteCommand()
			: DrawCommandBase(RendererType::Sprite)
		{}	
	};

	/// <summary>
	/// テキスト描画コマンド
	/// </summary>
	struct DrawTextCommand : public DrawCommandBase
	{
		// フォント
		DirectX::SpriteFont* spriteFont = nullptr;

		// 文字列
		std::wstring text = std::wstring();

		// マテリアル
		Handle<MaterialAsset> material = ERROR_HANDLE<MaterialAsset>;

		// 座標
		DirectX::SimpleMath::Vector2 pos = DirectX::SimpleMath::Vector2::Zero;

		// 拡大率
		DirectX::SimpleMath::Vector2 scale = DirectX::SimpleMath::Vector2(1, 1);

		// 回転角
		float angle = 0;

		// 原点
		DirectX::SimpleMath::Vector2 origin = DirectX::SimpleMath::Vector2(0.5f, 0.5f);

		// 乗算色
		DirectX::SimpleMath::Color color = DirectX::SimpleMath::Color(1, 1, 1, 1);

		// コンストラクタ
		DrawTextCommand()
			: DrawCommandBase(RendererType::Text)
		{}
	};

	/// <summary>
	/// プリミティブ描画コマンド
	/// </summary>
	struct DrawPrimitiveCommand : public DrawCommandBase
	{
		// 線の構造体
		struct Line
		{
			DirectX::VertexPositionColorTexture s;
			DirectX::VertexPositionColorTexture e;
		};

		// 三角形の構造体
		struct Triangle
		{
			DirectX::VertexPositionColorTexture a;
			DirectX::VertexPositionColorTexture b;
			DirectX::VertexPositionColorTexture c;
		};

		// ワールド行列
		DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::Identity;

		// マテリアル
		Handle<MaterialAsset> material = ERROR_HANDLE<MaterialAsset>;

		// 線の配列
		std::vector<Line> lines{};
		
		// 三角形の配列
		std::vector<Triangle> triangles{};

		// コンストラクタ
		DrawPrimitiveCommand()
			: DrawCommandBase(RendererType::Primitive)
		{}
	};
}	// namespace REngine
