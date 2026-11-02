/**
 * @file    VertexTypes.h
 * @author  Hoshino Ryunosuke
 * @date    2026/09/24
 *
 * @brief   エンジンにデフォルトで用意される頂点構造を宣言したヘッダ
 */

#pragma once

#include <concepts>
#include <cstdint>
#include <vector>
#include <DirectXMath.h>

#include "VertexElementInfo.h"

namespace RHEngine
{
	/**
	 * @brief 頂点構造であることを制限するconcept
	 */
	template <typename T>
	concept VertexType = requires
	{
		{ T::GetLayout() } -> std::convertible_to<const std::vector<VertexElementInfo>&>;
	};

	/**
	 * @brief 頂点構造 頂点座標のみ
	 */
	struct VertexPosition
	{
		DirectX::XMFLOAT3 position;	///< 頂点座標

		static const std::vector<VertexElementInfo>& GetLayout()
		{
			static const std::vector<VertexElementInfo> layout =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, offsetof(VertexPosition, position), 0 },
			};
			return layout;
		}
	};

	/**
	 * @brief 頂点構造 頂点座標と頂点カラー
	 */
	struct VertexPositionColor
	{
		DirectX::XMFLOAT3 position;	///< 頂点座標
		DirectX::XMFLOAT4 color;	///< 頂点カラー

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

	/**
	 * @brief 頂点構造 頂点座標とUV座標
	 */
	struct VertexPositionTexture
	{
		DirectX::XMFLOAT3 position;	///< 頂点座標
		DirectX::XMFLOAT2 texcoord;	///< UV座標

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

	/**
	 * @brief 頂点構造 頂点座標と2つのUV座標
	 */
	struct VertexPositionDualTexture
	{
		DirectX::XMFLOAT3 position;		///< 頂点座標
		DirectX::XMFLOAT2 texcoord0;	///< UV座標
		DirectX::XMFLOAT2 texcoord1;	///< UV座標

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

	/**
	 * @brief 頂点構造 頂点座標と法線ベクトル
	 */
	struct VertexPositionNormal
	{
		DirectX::XMFLOAT3 position;	///< 頂点座標
		DirectX::XMFLOAT3 normal;	///< 法線ベクトル

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

	/**
	 * @brief 頂点構造 頂点座標と頂点カラーとUV座標
	 * @details Primitive描画で使用されるデフォルトの頂点構造
	 */
	struct VertexPositionColorTexture
	{
		DirectX::XMFLOAT3 position;	///< 頂点座標
		DirectX::XMFLOAT4 color;	///< 頂点カラー
		DirectX::XMFLOAT2 texcoord;	///< UV座標

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

	/**
	 * @brief 頂点構造 頂点座標と法線ベクトルとUV座標
	 * @details Mesh描画で使用されるデフォルトの頂点構造
	 */
	struct VertexPositionNormalTexture
	{
		DirectX::XMFLOAT3 position;	///< 頂点座標
		DirectX::XMFLOAT3 normal;	///< 法線ベクトル
		DirectX::XMFLOAT2 texcoord;	///< UV座標

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

	/**
	 * @brief 頂点構造 頂点座標と法線ベクトルと頂点カラーとUV座標
	 */
	struct VertexPositionNormalColorTexture
	{
		DirectX::XMFLOAT3 position;	///< 頂点座標
		DirectX::XMFLOAT3 normal;	///< 法線ベクトル
		DirectX::XMFLOAT4 color;	///< 頂点カラー
		DirectX::XMFLOAT2 texcoord;	///< UV座標

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

	/**
	 * @brief 頂点構造 頂点座標と法線ベクトルと接線ベクトルと頂点カラーとUV座標
	 * @details メモリ節約のために頂点カラーはuint32_tの0xRRGGBBAAで表します
	 */
	struct VertexPositionNormalTangentColorTexture
	{
		DirectX::XMFLOAT3 position;	///< 頂点座標
		DirectX::XMFLOAT3 normal;	///< 法線ベクトル
		DirectX::XMFLOAT4 tangent;	///< 接線
		uint32_t color;				///< 頂点カラー (メモリ節約のために32bit)
		DirectX::XMFLOAT2 texcoord;	///< UV座標

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
}	// namespace RHEngine
