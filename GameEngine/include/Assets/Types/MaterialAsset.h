//====================================================//
// ファイル名   : MaterialAsset.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/08/05
//
// 概要 : マテリアル
//
// 更新履歴 :
// 2026/08/05 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <map>
#include <utility>
#include <variant>

#include "Assets/Objects/Handle.h"
#include "Shader/ShaderAsset.h"
#include "Assets/Types/Texture.h"

#include "Assets/Objects/AssetBase.h"
#include "Assets/Types/Shader/SamplerType.h"
#include "Assets/Types/Shader/SamplerList.h"

namespace REngine
{
	// マテリアルがもつパラメータのキー
	struct MaterialParamKey
	{
		ShaderType stage;	// シェーダーのステージ
		std::string name;			// 名前

		// 等価演算子
		bool operator==(const MaterialParamKey& other) const noexcept
		{
			return stage == other.stage && name == other.name; 
		}
	};
}

// unordered_mapで使うためのハッシュ特殊化
namespace std
{
	template<>
	struct hash<REngine::MaterialParamKey>
	{
		size_t operator()(const REngine::MaterialParamKey& k) const noexcept
		{
			size_t h1 = std::hash<int>()(static_cast<int>(k.stage));	// 列挙型をintに変換しハッシュを取得
			size_t h2 = std::hash<std::string>()(k.name);				// stringのハッシュを取得
			return h1 ^ (h2 << 1); // 2つのハッシュを結合する
		}
	};
}

namespace REngine
{
	// バッファとして存在する型を全て扱うVariant
	using MaterialParamVariant = std::variant<
		float,
		DirectX::SimpleMath::Vector2,
		DirectX::SimpleMath::Vector3,
		DirectX::SimpleMath::Vector4,
		DirectX::SimpleMath::Color,
		DirectX::SimpleMath::Matrix,
		Handle<Texture>,
		SamplerType
	>;

	class AssetManager;

	class MaterialAsset : public AssetBase
	{
	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// 各ステージのシェーダー
		Handle<ShaderAsset> m_vertexShader;	// 頂点シェーダ
		Handle<ShaderAsset> m_pixelShader;	// ピクセルシェーダ	

		// 各ステージの定数バッファマップ
		std::map<std::pair<ShaderType, uint32_t>, Microsoft::WRL::ComPtr<ID3D11Buffer>> m_constantBuffers;

		// パラメータの一覧
		std::unordered_map<MaterialParamKey, MaterialParamVariant> m_params;

		// バッファの変更済みフラグ
		bool m_isDirty;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		MaterialAsset();
		~MaterialAsset() = default;

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		// パラメータを変更する関数
		template<typename T>
		void SetParam(ShaderType type, const std::string& name, T value)
		{
			// 変更
			m_params[{ type, name }] = value;

			// Dirtyに
			m_isDirty = true;
		}

		// パラメータを名前検索する関数
		ShaderParam* FindParam(ShaderType stage, const std::string& name, AssetManager& assetManager);

		// 定数バッファを更新する関数
		void UpdateConstantBuffers(ID3D11Device* device, ID3D11DeviceContext* context, AssetManager& assetManager);

		// シェーダーをcontextにバインドする関数
		void Bind(ID3D11DeviceContext* context, AssetManager& assetManager, const SamplerList& samplerList);

		// 有効かどうか
		bool IsValid() 
		{
			return m_vertexShader != ERROR_HANDLE<ShaderAsset> || m_pixelShader != ERROR_HANDLE<ShaderAsset>; 
		}

	private:

		// テクスチャをバインドする関数
		void BindTexture(ID3D11DeviceContext* context, ShaderAsset* shader, REngine::Texture* texture, const MaterialParamKey& key);

		// サンプラーをバインドする関数
		void BindSampler(ID3D11DeviceContext* context, ShaderAsset* shader, const Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler, const MaterialParamKey& key);
	};
}	// namespace REngine
