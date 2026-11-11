/**
 * @file    DrawCommand.h
 * @author  Hoshino Ryunosuke
 * @date    2026/07/18
 *
 * @brief   描画commandをまとめたヘッダ
 */

#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <Windows.h>
#include <d3d11.h>
#include <d3dcommon.h>
#include <optional>
#include <SimpleMath.h>
#include <SpriteFont.h>

#include "../RendererType.h"
#include "Assets/Objects/Handle.h"
#include "Assets/Types/Model/Mesh.h"
#include "Assets/Types/Vertex/VertexTypes.h"

namespace RHEngine
{
	class MaterialAsset;

	/**
	 * @brief 描画コマンドの基底
	 */
	struct DrawCommandBase
	{
		uint64_t sortKey;	///< 描画順
		
		RendererType type;	///< 描画コマンドの種類

		/// @name ライフサイクル
		/// @{
		
		/**
		 * @param ttype 描画コマンドの種類
		 * @param sort 描画順
		 */
		DrawCommandBase(RendererType type, uint64_t sort = 0)
			: type{ type }, sortKey{ sort } 
		{}

		virtual ~DrawCommandBase() = default;

		/// @}
	};

	/**
	 * @brief メッシュ描画コマンド
	 */
	struct DrawMeshCommand : public DrawCommandBase
	{
		Mesh* pMesh = nullptr;	///< メッシュ

		size_t subMeshIndex = 0;	///< サブメッシュインデックス

		Handle<MaterialAsset> material = ERROR_HANDLE<MaterialAsset>;	///< マテリアルハンドル

		DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::Identity;	///< ワールド行列

		/// @name ライフサイクル
		/// @{

		DrawMeshCommand()
			: DrawCommandBase(RendererType::Mesh)
		{}

		/// @}
	};

	/**
	 * @brief スプライト描画コマンド
	 */
	struct DrawSpriteCommand : public DrawCommandBase
	{
		ID3D11ShaderResourceView* pTexture = nullptr;	///< テクスチャ

		Handle<MaterialAsset> material = ERROR_HANDLE<MaterialAsset>;	///< マテリアルハンドル

		DirectX::SimpleMath::Vector2 pos = DirectX::SimpleMath::Vector2::Zero;	///< 描画座標

		std::optional<RECT> srcRect = std::nullopt;	///< 切り取り矩形

		DirectX::SimpleMath::Vector2 scale = DirectX::SimpleMath::Vector2(1, 1);	///< 拡大率

		float angle = 0;	///< 回転角度 (ラジアン)

		DirectX::SimpleMath::Vector2 origin = DirectX::SimpleMath::Vector2(0.5f, 0.5f);	///< 原点 (uv座標)

		DirectX::SimpleMath::Color color = DirectX::SimpleMath::Color(1, 1, 1, 1);	///< 乗算色

		/// @name ライフサイクル
		/// @{

		DrawSpriteCommand()
			: DrawCommandBase(RendererType::Sprite)
		{}	

		/// @}
	};

	/**
	 * @brief テキスト描画コマンド
	 */
	struct DrawTextCommand : public DrawCommandBase
	{
		DirectX::SpriteFont* spriteFont = nullptr;	///< フォント
		
		std::wstring text = std::wstring();	///< 文字列

		Handle<MaterialAsset> material = ERROR_HANDLE<MaterialAsset>;	///< マテリアルハンドル

		DirectX::SimpleMath::Vector2 pos = DirectX::SimpleMath::Vector2::Zero;	///< 座標

		DirectX::SimpleMath::Vector2 scale = DirectX::SimpleMath::Vector2(1, 1);	///< 拡大率

		float angle = 0;	///< 回転角度 (ラジアン)

		DirectX::SimpleMath::Vector2 origin = DirectX::SimpleMath::Vector2(0.5f, 0.5f);	///< 原点 (uv座標)
			
		DirectX::SimpleMath::Color color = DirectX::SimpleMath::Color(1, 1, 1, 1);	///< 乗算色

		/// @name ライフサイクル
		/// @{

		DrawTextCommand()
			: DrawCommandBase(RendererType::Text)
		{}

		/// @}
	};

	/**
	 * @brief プリミティブ描画コマンド
	 */
	struct DrawPrimitiveCommand : public DrawCommandBase
	{
		// 最大頂点数
		static constexpr size_t MAX_VERTEX_COUNT = 4096;

		DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::Identity;	///< ワールド行列

		Handle<MaterialAsset> material = ERROR_HANDLE<MaterialAsset>;	///< マテリアルハンドル

		std::vector<VertexPositionColorTexture> vertices;		///< 頂点配列

		D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;	///< トポロジー

		/// @name ライフサイクル
		/// @{
 
		DrawPrimitiveCommand()
			: DrawCommandBase(RendererType::Primitive)
		{}

		/// @}
	};

	struct DrawSkyboxCommand : public DrawCommandBase
	{
		Mesh* pMesh = nullptr;	///< スカイボックスを張り付けるメッシュ

		float scale = 1.0f;		///< スカイボックス描画時のスケール (Perspectiveの場合値を変えても見た目に変化はない)

		Handle<MaterialAsset> material = ERROR_HANDLE<MaterialAsset>;	///< マテリアルハンドル

		/// @name ライフサイクル
		/// @{
 
		DrawSkyboxCommand()
			: DrawCommandBase(RendererType::Skybox)
		{}

		/// @}
	};

	struct DrawWorldSpriteCommand : public DrawCommandBase
	{
		ID3D11ShaderResourceView* pTexture = nullptr;	///< テクスチャ

		Mesh* pMesh = nullptr;	///< テクスチャを張り付けるメッシュ
		
		DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::Identity;	///< ワールド行列

		DirectX::SimpleMath::Color color = { 1.0f, 1.0f, 1.0f, 1.0f };	///< 乗算色

		DirectX::SimpleMath::Vector4 uv = { 0.0f, 0.0f, 1.0f, 1.0f };	///< 切り取り矩形

		Handle<MaterialAsset> material = ERROR_HANDLE<MaterialAsset>;	///< マテリアルハンドル

		/// @name ライフサイクル
		/// @{
 
		DrawWorldSpriteCommand()
			: DrawCommandBase(RendererType::WorldSprite)
		{}

		/// @}
	};
}	// namespace RHEngine
