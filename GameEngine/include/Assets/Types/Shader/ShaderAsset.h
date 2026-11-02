/**
 * @file    ShaderAsset.h
 * @author  Hoshino Ryunosuke
 * @date    2026/08/03
 *
 * @brief   シェーダーを管理するアセット
 */

#pragma once

#include <vector>
#include <string>
#include <variant>
#include <cstdint>
#include <filesystem>
#include <d3d11.h>
#include <wrl/client.h>

#include "ShaderLoader.h"
#include "Assets/Objects/AssetBase.h"
#include "ShaderParam.h"
#include "Assets/Managers/AssetLoadResult.h"

namespace RHEngine
{
	/**
	 * @brief シェーダーのステージタイプ
	 */
	enum class ShaderType 
	{
		Vertex, 
		Hull, 
		Domain,
		Geometry,
		Pixel, 
		Compute, 
	};

	/**
	 * @brief シェーダーアセット
	 * @details 1つのステージのシェーダーを管理する
	 */
	class ShaderAsset : public AssetBase
	{
		/**
		 * @details 読み込み関数をfriendに指定
		 */
		friend LoadResult Loader::ShaderLoader(const std::filesystem::path& path, AssetContext& ctx);

	public:
		/**
		 * @brief あらゆるステージのシェーダーを保持するためのvariant型
		 */
		using ShaderObjectVariant = std::variant<
			Microsoft::WRL::ComPtr<ID3D11VertexShader>,		// 頂点
			Microsoft::WRL::ComPtr<ID3D11GeometryShader>,	// ジオメトリ
			Microsoft::WRL::ComPtr<ID3D11PixelShader>,		// ピクセル
			Microsoft::WRL::ComPtr<ID3D11HullShader>,		// ハル
			Microsoft::WRL::ComPtr<ID3D11ComputeShader>,	// コンピュート
			Microsoft::WRL::ComPtr<ID3D11DomainShader>		// ドメイン
		>;

	private:

		/** @brief 保持するシェーダーのステージ */
		ShaderType m_type;

		/** @brief シェーダー本体 */
		ShaderObjectVariant m_shader;

		/** @brief コンパイル済みシェーダーのバイナリ文字列 */
		Microsoft::WRL::ComPtr<ID3DBlob> m_byteCode;

		/** @brief 自身が持つパラメータの一覧 */
		std::vector<ShaderParam> m_params;

		/** @brief 定数バッファの一覧 */
		std::vector<ConstantBufferInfo> m_cBuffers;

		/** 
		 * @brief シェーダー識別用ID 
		 * @details InputLayoutCacheで識別するために使用される
		 *			読み込み時に自動で割り当てられる
		 */
		uint32_t m_shaderID;

	public:

		/// @ name ライフサイクル
		/// @{ 
		
		/**
		 * @param id シェーダー識別ID
		 */
		ShaderAsset(uint32_t id);

		~ShaderAsset() = default;

		/// @}

		/**
		 * @brief コンテキストに自身をセットする
		 * @param pContext コンテキストポインタ
		 */
		void Bind(ID3D11DeviceContext* pContext);

		/**
		 * @brief パラメータを名前で検索する
		 * @param name 検索対象の名前
		 * @return パラメータポインタ
		 */
		ShaderParam* FindParam(const std::string& name);

		/// @name ゲッター
		/// @{

		/** @brief タイプ */
		ShaderType GetType() const { return m_type; }

		/** @brief ID */
		uint32_t GetShaderID() const { return m_shaderID; }

		/** @brief パラメータ一覧 */
		const std::vector<ShaderParam>& GetParams() const { return m_params; }

		/** @brief 定数バッファ一覧 */
		const std::vector<ConstantBufferInfo>& GetBuffers() const { return m_cBuffers; };

		/** @brief コンパイル済みバイナリ */
		ID3DBlob* GetBlob() const { return m_byteCode.Get(); }

		/// @}
	};
}	// namespace RHEngine
