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
#include <Effects.h>
#include <type_traits>

#include "Assets/Objects/Handle.h"
#include "Shader/ShaderAsset.h"
#include "Assets/Types/Texture.h"

#include "Assets/Objects/AssetBase.h"
#include "Assets/Types/Shader/SamplerType.h"
#include "Assets/Types/Shader/SamplerList.h"

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

	class MaterialAsset : public AssetBase, public DirectX::IEffect
	{
	public:

		struct Parameter
		{
			ShaderParamType type{};
			MaterialParamVariant value{};
		};

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
		std::unordered_map<ShaderType, std::unordered_map<std::string, Parameter>> m_params;

		// バッファの変更済みフラグ
		bool m_isDirty;

		// アセットマネージャー
		const AssetManager* m_assetManager;

		// サンプラーリスト
		const SamplerList* m_samplerList;

		// パラメータの再構築が必要かどうかのフラグ
		bool m_needRebuildParams;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		MaterialAsset();
		~MaterialAsset() = default;

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		// アセットマネージャー・サンプラーリストをセットする関数
		void SetReference(const AssetManager* assetManager, const SamplerList* samplerList)
		{
			m_assetManager = assetManager;
			m_samplerList = samplerList;
		}

		// パラメータを変更する関数
		template<typename T>
		void SetParam(ShaderType type, const std::string& name, T value)
		{
			// 変更
			m_params[type][name] = Parameter{ GetParamType<T>(), value };

			// Dirtyに
			m_isDirty = true;
		}

		// パラメータタイプを取得する関数
		template<typename T>
		ShaderParamType GetParamType()
		{
			if constexpr (std::is_same_v<T, float>)
				return ShaderParamType::Float;
			else if constexpr (std::is_same_v<T, DirectX::SimpleMath::Vector2>)
				return ShaderParamType::Float2;
			else if constexpr (std::is_same_v<T, DirectX::SimpleMath::Vector3>)
				return ShaderParamType::Float3;
			else if constexpr (std::is_same_v<T, DirectX::SimpleMath::Vector4>)
				return ShaderParamType::Float4;
			else if constexpr (std::is_same_v<T, DirectX::SimpleMath::Matrix>)
				return ShaderParamType::Float4x4;
			else if constexpr (std::is_same_v<T, Handle<Texture>>)
				return ShaderParamType::Texture2D;
			else if constexpr (std::is_same_v<T, SamplerType>)
				return ShaderParamType::Sampler;
			else return ShaderParamType::None;
		}

		// パラメータを名前検索する関数
		ShaderParam* FindParam(ShaderType stage, const std::string& name);

		// 定数バッファを更新する関数
		void UpdateConstantBuffers(ID3D11Device* device, ID3D11DeviceContext* context);
		
		// 有効かどうか
		bool IsValid()
		{
			return m_vertexShader != ERROR_HANDLE<ShaderAsset>;
		}

		// 各シェーダーのコンパイル済みバイナリを取得する関数
		ID3DBlob* GetBlob(ShaderType type);

		// 各シェーダーのパラメータ一覧を取得する関数
		const std::unordered_map<std::string, Parameter>& GetParams(ShaderType type) const
		{
			// エラー用static変数
			static std::unordered_map<std::string, Parameter> error{};

			// 検索
			auto it = m_params.find(type);

			// 見つかれば
			if (it != m_params.end()) return it->second;

			// なければエラー値
			return error;
		}

		// GUI変更時の関数
		void OnValidate() override
		{
			m_isDirty = true;
			m_needRebuildParams = true;
		}

		//------ IEffectの実装 ------//

		// シェーダーをcontextにバインドする関数
		void Apply(ID3D11DeviceContext* context) override;

		// InputLayoutのセットは手動で行っているのでセットはしない
		void __cdecl GetVertexShaderBytecode(void const** pShaderByteCode, size_t* pByteCodeLength) override
		{
			*pShaderByteCode = nullptr;
			*pByteCodeLength = 0;
		}

		//----- PropertyObjectの実装 -----//

		// プロパティ一覧を取得する関数
		std::vector<Property> GetProperties() override;

	private:

		// テクスチャをバインドする関数
		void BindTexture(ID3D11DeviceContext* context, ShaderAsset* shader, REngine::Texture* texture, const std::string& name, ShaderType type);

		// サンプラーをバインドする関数
		void BindSampler(ID3D11DeviceContext* context, ShaderAsset* shader, const Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler, const std::string& name, ShaderType type);

		// シェーダーのパラメータから自身のパラメータを作り直す関数
		void RebuildParams();

		// パラメータのデフォルト値を返す関数
		MaterialParamVariant GetDefaultParam(ShaderParamType type);
		
		// パラメータからプロパティを作成する関数
		Property CreatePropertyFromParameter(const std::string& name, Parameter& parameter);

		// パラメータのリビルドが必要かを調べ、実行する関数
		void CheckAndDoRebuild();

		// タイプからシェーダー本体を取得する関数
		ShaderAsset* GetShaderAsset(ShaderType type);
	};
}	// namespace REngine
