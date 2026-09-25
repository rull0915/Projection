//====================================================//
// ファイル名   : VertexTypes.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/09/24
//
// 概要 : エンジンにデフォルトで用意される頂点構造を宣言したヘッダ
//
// 更新履歴 :
// 2026/09/24 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <concepts>
#include <cstdint>
#include <vector>
#include <DirectXMath.h>

#include "VertexElementInfo.h"

namespace REngine
{
	// staticなGetLayout関数を持っているかをチェックするConcept
	template <typename T>
	concept VertexType = requires
	{
		{ T::GetLayout() } -> std::convertible_to<const std::vector<VertexElementInfo>&>;
	};

	struct VertexPosition
	{
		DirectX::XMFLOAT3 position;	// 頂点座標

		// レイアウトを返す関数
		static const std::vector<VertexElementInfo>& GetLayout()
		{
			static const std::vector<VertexElementInfo> layout =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, offsetof(VertexPosition, position), 0 },
			};
			return layout;
		}
	};

	struct VertexPositionColor
	{
		DirectX::XMFLOAT3 position;	// 頂点座標
		DirectX::XMFLOAT4 color;	// 頂点カラー

		// レイアウトを返す関数
		static const std::vector<VertexElementInfo>& GetLayout()
		{
			static const std::vector<VertexElementInfo> layout =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    offsetof(VertexPositionColor, position), 0 },
				{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, offsetof(VertexPositionColor, color),    0 },
			};
			return layout;
		}
	};

	struct VertexPositionTexture
	{
		DirectX::XMFLOAT3 position;	// 頂点座標
		DirectX::XMFLOAT2 texcoord;	// テクスチャ座標

		// レイアウトを返す関数
		static const std::vector<VertexElementInfo>& GetLayout()
		{
			static const std::vector<VertexElementInfo> layout =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    offsetof(VertexPositionTexture, position), 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,		 offsetof(VertexPositionTexture, texcoord), 0 },
			};
			return layout;
		}
	};

	struct VertexPositionDualTexture
	{
		DirectX::XMFLOAT3 position;	// 頂点座標
		DirectX::XMFLOAT2 texcoord0;	// テクスチャ座標
		DirectX::XMFLOAT2 texcoord1;	// テクスチャ座標

		// レイアウトを返す関数
		static const std::vector<VertexElementInfo>& GetLayout()
		{
			static const std::vector<VertexElementInfo> layout =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    offsetof(VertexPositionDualTexture, position), 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,		 offsetof(VertexPositionDualTexture, texcoord0), 0 },
				{ "TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT,		 offsetof(VertexPositionDualTexture, texcoord1), 0 },
			};
			return layout;
		}
	};

	struct VertexPositionNormal
	{
		DirectX::XMFLOAT3 position;	// 頂点座標
		DirectX::XMFLOAT3 normal;	// 法線ベクトル

		// レイアウトを返す関数
		static const std::vector<VertexElementInfo>& GetLayout()
		{
			static const std::vector<VertexElementInfo> layout =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, offsetof(VertexPositionNormal, position), 0 },
				{ "NORMAL",	  0, DXGI_FORMAT_R32G32B32_FLOAT, offsetof(VertexPositionNormal, normal), 0 },
			};
			return layout;
		}
	};

	struct VertexPositionColorTexture
	{
		DirectX::XMFLOAT3 position;	// 頂点座標
		DirectX::XMFLOAT4 color;	// 頂点カラー
		DirectX::XMFLOAT2 texcoord;	// テクスチャ座標

		// レイアウトを返す関数
		static const std::vector<VertexElementInfo>& GetLayout()
		{
			static const std::vector<VertexElementInfo> layout =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    offsetof(VertexPositionColorTexture, position), 0 },
				{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, offsetof(VertexPositionColorTexture, color),    0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,		 offsetof(VertexPositionColorTexture, texcoord), 0 },
			};
			return layout;
		}
	};

	struct VertexPositionNormalTexture
	{
		DirectX::XMFLOAT3 position;	// 頂点座標
		DirectX::XMFLOAT3 normal;	// 法線ベクトル
		DirectX::XMFLOAT2 texcoord;	// テクスチャ座標

		// レイアウトを返す関数
		static const std::vector<VertexElementInfo>& GetLayout()
		{
			static const std::vector<VertexElementInfo> layout =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, offsetof(VertexPositionNormalTexture, position), 0 },
				{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, offsetof(VertexPositionNormalTexture, normal),   0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,	  offsetof(VertexPositionNormalTexture, texcoord), 0 },
			};
			return layout;
		}
	};

	struct VertexPositionNormalColorTexture
	{
		DirectX::XMFLOAT3 position;	// 頂点座標
		DirectX::XMFLOAT3 normal;	// 法線ベクトル
		DirectX::XMFLOAT4 color;	// 頂点カラー
		DirectX::XMFLOAT2 texcoord;	// テクスチャ座標

		// レイアウトを返す関数
		static const std::vector<VertexElementInfo>& GetLayout()
		{
			static const std::vector<VertexElementInfo> layout =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    offsetof(VertexPositionNormalColorTexture, position), 0 },
				{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,      offsetof(VertexPositionNormalColorTexture, normal),   0 },
				{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, offsetof(VertexPositionNormalColorTexture, color),    0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,		 offsetof(VertexPositionNormalColorTexture, texcoord), 0 },
			};
			return layout;
		}
	};

	struct VertexPositionNormalTangentColorTexture
	{
		DirectX::XMFLOAT3 position;	// 頂点座標
		DirectX::XMFLOAT3 normal;	// 法線ベクトル
		DirectX::XMFLOAT4 tangent;	// 接線
		uint32_t color;				// 頂点カラー (メモリ節約のために32bit)
		DirectX::XMFLOAT2 texcoord;	// テクスチャ座標

		// レイアウトを返す関数
		static const std::vector<VertexElementInfo>& GetLayout()
		{
			static const std::vector<VertexElementInfo> layout =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    offsetof(VertexPositionNormalTangentColorTexture, position), 0 },
				{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    offsetof(VertexPositionNormalTangentColorTexture, normal),   0 },
				{ "TANGENT",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, offsetof(VertexPositionNormalTangentColorTexture, tangent),  0 },
				{ "COLOR",    0, DXGI_FORMAT_R8G8B8A8_UNORM,     offsetof(VertexPositionNormalTangentColorTexture, color),    0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,		 offsetof(VertexPositionNormalTangentColorTexture, texcoord), 0 },
			};
			return layout;
		}
	};
}
