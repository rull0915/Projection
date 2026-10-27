/**
 * @file    MaterialAsset.h
 * @author  Hoshino Ryunosuke
 * @date    2026/08/05
 *
 * @brief   マテリアルアセット
 */

#pragma once

#include <map>
#include <vector>
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
	namespace Loader { class MaterialLoader; }
	namespace Saver { class MaterialSaver; }

	/**
	 * @brief マテリアルアセット
	 * @details 各ステージのシェーダーアセットを保持
	 *			ステージごとの定数バッファ、テクスチャ、サンプラーを管理します
	 */
	class MaterialAsset : public AssetBase
	{
		friend class Loader::MaterialLoader;
		friend class Saver::MaterialSaver;

	public:

		/**
		 * @brief 1パラメータの構造体
		 */
		struct Parameter
		{
			/** @brief パラメータのタイプ */
			ShaderParamType type{};

			/** @brief パラメータの値 */
			MaterialParamVariant value{};
		};

	private:

		/// @name 各ステージのシェーダー
		/// @{
		 
		/** @brief 頂点シェーダ */
		Handle<ShaderAsset> m_vertexShader;
		/** @brief ピクセルシェーダ */
		Handle<ShaderAsset> m_pixelShader;

		/// @}

		/// @name 変更検知用
		/// @{

		/** @brief 頂点シェーダ */
		Handle<ShaderAsset> m_oldVertexShader;
		/** @brief ピクセルシェーダ */
		Handle<ShaderAsset> m_oldPixelShader;

		/// @}

		/** @brief 各ステージの定数バッファマップ */
		std::map<std::pair<ShaderType, uint32_t>, Microsoft::WRL::ComPtr<ID3D11Buffer>> m_constantBuffers;

		/** @brief パラメータの一覧 */
		std::unordered_map<ShaderType, std::unordered_map<std::string, Parameter>> m_params;

		/** @brief 解決待ちのロード後パラメータ */
		std::unordered_map<ShaderType, std::unordered_map<std::string, Parameter>> m_pendingParams;

		/** @brief バッファの変更フラグ */
		bool m_isDirty;

		/** @brief アセットマネージャー */
		const AssetManager* m_pAssetManager;

		/** @brief サンプラーリスト */
		const SamplerList* m_pSamplerList;

	public:

		/// @name ライフサイクル
		/// @{

		MaterialAsset();

		~MaterialAsset() = default;

		/// @}

		/**
		 * @brief アセットマネージャーを受け取る
		 * @param am 渡されるアセットマネージャー
		 */
		void ReceiveAssetManager(AssetManager* pAssetManager) override
		{
			m_pAssetManager = pAssetManager;
		}

		/**
		 * @brief サンプラーリストをセットする
		 * @param samplerList サンプラーリストポインタ
		 */
		void SetSamplerList(const SamplerList* pSamplerList)
		{
			m_pSamplerList = pSamplerList;
		}

		/**
		 * @brief パラメータを変更する
		 * @tparam T パラメータの型
		 * @param type 変更するシェーダーのステージ
		 * @param name 変更するパラメータの名前
		 * @param value 変更後の値
		 */
		template<typename T>
		void SetParam(ShaderType type, const std::string& name, T value)
		{
			// 変更
			m_params[type][name] = Parameter{ MaterialParams::GetParamType<T>(), value };

			// Dirtyに
			m_isDirty = true;
		}

		/**
		 * @brief 定数バッファを更新する
		 * @param device デバイスポインタ
		 * @param context コンテキストポインタ
		 */
		void UpdateConstantBuffers(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
		
		/**
		 * @brief 有効なマテリアルかどうか調べる
		 * @return 有効かどうか
		 */
		bool IsValid()
		{
			return m_vertexShader != ERROR_HANDLE<ShaderAsset>;
		}

		/**
		 * @brief シェーダーのパラメータ一覧を取得する
		 * @param type 取得対象のステージ
		 * @return パラメータとパラメータ名のマップ
		 */
		const std::unordered_map<std::string, Parameter>& GetParams(ShaderType type) const;

		/**
		 * @brief 未解決パラメータ一覧を取得する
		 * @param type 取得対象のステージ
		 * @return パラメータとパラメータ名のマップ
		 */
		std::unordered_map<std::string, Parameter>& GetPendingParams(ShaderType type);

		/**
		 * @brief 未解決パラメータをセットする
		 * @param type セットするシェーダーステージ
		 * @param params パラメータ一覧
		 */
		void SetPendingParams(ShaderType type, const std::unordered_map<std::string, Parameter>& params)
		{
			m_pendingParams[type] = params;
		}

		/**
		 * @brief GUI上での値変更時に呼ばれる
		 */
		void OnValidate() override
		{
			m_isDirty = true;
		}

		/**
		 * @brief コンテキストに自身を割り当てる
		 * @param context コンテキストポインタ
		 * @return 
		 */
		bool Apply(ID3D11DeviceContext* pContext);

		/**
		 * @brief 1つのシェーダをバインドする
		 * @details シェーダー本体のバインド
		 *			定数バッファ、テクスチャ、サンプラーのバインドを行う
		 * @param context コンテキストポインタ
		 * @param type バインドするシェーダーステージ
		 * @return 成功したかどうか
		 */
		bool BindShader(ID3D11DeviceContext* pContext, ShaderType type);

		/**
		 * @brief シェーダーを変更する
		 * @param shader 変更後のシェーダー
		 * @param type 変更するシェーダーステージ
		 */
		void SetShader(Handle<ShaderAsset> shader, ShaderType type);

		/**
		 * @brief シェーダーステージ列挙型からシェーダーアセット本体を取得する
		 * @param type シェーダーステージ
		 * @return シェーダーアセット本体
		 */
		ShaderAsset* GetShaderAsset(ShaderType type);

		/// @name PropertyObjectのoverride
		/// @{

		/**
		 * @brief プロパティ一覧を取得する
		 * @return プロパティ一覧
		 */
		std::vector<Property> GetProperties() override;

		/// @}

	private:

		/**
		 * @brief テクスチャをバインドする
		 * @param pContext コンテキストポインタ
		 * @param pShader シェーダー本体
		 * @param pTexture バインドするテクスチャ
		 * @param name テクスチャパラメータ名
		 * @param type バインドするシェーダーステージ
		 */
		void BindTexture(ID3D11DeviceContext* pContext, ShaderAsset* pShader, RHEngine::Texture* pTexture, const std::string& name, ShaderType type);

		/**
		 * @brief サンプラーをバインドする
		 * @param pContext コンテキストポインタ
		 * @param pShader シェーダー本体
		 * @param sampler バインドするサンプラー
		 * @param name サンプラーパラメータ名
		 * @param type バインドするシェーダーステージ
		 */
		void BindSampler(ID3D11DeviceContext* pContext, ShaderAsset* pShader, Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler, const std::string& name, ShaderType type);

		/**
		 * @brief シェーダーのパラメータから自身のパラメータを再構築する
		 */
		void RebuildParams();

		/**
		 * @brief パラメータからプロパティを作成する
		 * @param name パラメータ名
		 * @param parameter パラメータ
		 * @return 作成したプロパティ
		 */
		Property CreatePropertyFromParameter(const std::string& name, Parameter& parameter);

		/**
		 * @brief パラメータの再構築が必要か調べ実行する
		 */
		void CheckAndDoRebuild();
	};
}	// namespace RHEngine
