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
#include <type_traits>

#include "MaterialParams.h"
#include "Assets/Objects/Handle.h"
#include "Assets/Types/Shader/ShaderAsset.h"
#include "Assets/Types/Texture.h"

#include "Assets/Objects/AssetBase.h"

namespace RHEngine
{
	class AssetManager;
	class SamplerList;
	namespace Loader
	{
		class MaterialLoader;
	}
	namespace Saver
	{
		class MaterialSaver;
	}

	class MaterialAsset : public AssetBase
	{
		// ロード、保存クラスをfriend指定
		friend class Loader::MaterialLoader;
		friend class Saver::MaterialSaver;

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

		// 変更前のハンドル
		Handle<ShaderAsset> m_oldVertexShader;	// 頂点シェーダ
		Handle<ShaderAsset> m_oldPixelShader;	// ピクセルシェーダ

		// 各ステージの定数バッファマップ
		std::map<std::pair<ShaderType, uint32_t>, Microsoft::WRL::ComPtr<ID3D11Buffer>> m_constantBuffers;

		// パラメータの一覧
		std::unordered_map<ShaderType, std::unordered_map<std::string, Parameter>> m_params;

		// ロード時に読み込まれたパラメータ
		std::unordered_map<ShaderType, std::unordered_map<std::string, Parameter>> m_pendingParams;

		// バッファの変更済みフラグ
		bool m_isDirty;

		// アセットマネージャー
		const AssetManager* m_assetManager;

		// サンプラーリスト
		const SamplerList* m_samplerList;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		MaterialAsset();
		~MaterialAsset() = default;

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		// アセットマネージャーを受け取る関数
		void ReceiveAssetManager(AssetManager* am) override
		{
			m_assetManager = am;
		}

		// サンプラーリストをセットする関数
		void SetSamplerList(const SamplerList* samplerList)
		{
			m_samplerList = samplerList;
		}

		// パラメータを変更する関数
		template<typename T>
		void SetParam(ShaderType type, const std::string& name, T value)
		{
			// 変更
			m_params[type][name] = Parameter{ MaterialParams::GetParamType<T>(), value };

			// Dirtyに
			m_isDirty = true;
		}

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

		// 各シェーダーのロード用パラメータ一覧を取得する関数
		std::unordered_map<std::string, Parameter>& GetPendingParams(ShaderType type)
		{
			// エラー用static変数
			static std::unordered_map<std::string, Parameter> error{};

			// 検索
			auto it = m_pendingParams.find(type);

			// 見つかれば
			if (it != m_pendingParams.end()) return it->second;

			// なければエラー値
			return error;
		}

		// シェーダーのパラメータをセットする関数
		void SetPendingParams(ShaderType type, const std::unordered_map<std::string, Parameter>& params)
		{
			m_pendingParams[type] = params;
		}

		// GUI変更時の関数
		void OnValidate() override
		{
			m_isDirty = true;
		}

		// シェーダーをcontextにバインドする関数
		void Apply(ID3D11DeviceContext* context);

		// 1つのシェーダーをバインドする関数
		bool BindShader(ID3D11DeviceContext* context, ShaderType type);

		// シェーダーを変更する関数
		void SetShader(Handle<ShaderAsset> shader, ShaderType type)
		{
			switch (type)
			{
			case RHEngine::ShaderType::Vertex:
				m_vertexShader = shader;
				break;
			case RHEngine::ShaderType::Pixel:
				m_pixelShader = shader;
				break;
			default:
				break;
			}
		}

		//----- PropertyObjectの実装 -----//

		// プロパティ一覧を取得する関数
		std::vector<Property> GetProperties() override;

	private:

		// テクスチャをバインドする関数
		void BindTexture(ID3D11DeviceContext* context, ShaderAsset* shader, RHEngine::Texture* texture, const std::string& name, ShaderType type);

		// サンプラーをバインドする関数
		void BindSampler(ID3D11DeviceContext* context, ShaderAsset* shader, Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler, const std::string& name, ShaderType type);

		// シェーダーのパラメータから自身のパラメータを作り直す関数
		void RebuildParams();

		// パラメータからプロパティを作成する関数
		Property CreatePropertyFromParameter(const std::string& name, Parameter& parameter);

		// パラメータのリビルドが必要かを調べ、実行する関数
		void CheckAndDoRebuild();

		// タイプからシェーダー本体を取得する関数
		ShaderAsset* GetShaderAsset(ShaderType type);
	};
}	// namespace RHEngine
