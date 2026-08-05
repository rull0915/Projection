//====================================================//
// ファイル名   : ShaderAsset.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/08/03
//
// 概要 : シェーダーを管理するアセット
//
// 更新履歴 :
// 2026/08/03 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <variant>
#include "ShaderParam.h"
#include "ShaderLoader.h"

namespace REngine
{
	/// <summary>
	/// シェーダーのタイプ
	/// </summary>
	enum class ShaderType { Vertex, Geomerty, Pixel, Hull, Compute, Domain };

	/// <summary>
	/// シェーダーアセット
	/// </summary>
	class ShaderAsset
	{
	public:
		// あらゆるShaderのComPtrを持つことが出来る型
		using ShaderObjectVariant = std::variant<
			Microsoft::WRL::ComPtr<ID3D11VertexShader>,		// 頂点
			Microsoft::WRL::ComPtr<ID3D11GeometryShader>,	// ジオメトリ
			Microsoft::WRL::ComPtr<ID3D11PixelShader>,		// ピクセル
			Microsoft::WRL::ComPtr<ID3D11HullShader>,		// ハル
			Microsoft::WRL::ComPtr<ID3D11ComputeShader>,	// コンピュート
			Microsoft::WRL::ComPtr<ID3D11DomainShader>		// ドメイン
		>;
	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// シェーダータイプ
		ShaderType m_type;

		// シェーダー本体
		ShaderObjectVariant m_shader;

		// コンパイルしたシェーダーのバイナリ文字列
		Microsoft::WRL::ComPtr<ID3DBlob> m_byteCode;

		// パラメータ一覧
		std::vector<ShaderParam> m_params;

		// 定数バッファ一覧
		std::vector<ConstantBufferInfo> m_cBuffers;

		// 入力レイアウト
		// このアセットがVertexShaderを扱う場合のみ構築されます
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		ShaderAsset() = default;
		~ShaderAsset() = default;

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		// コンテキストに自信を紐づける関数
		void Bind(ID3D11DeviceContext* context);

		//-----------------------------------------------------
		// ゲッター
		//-----------------------------------------------------

		// タイプ
		ShaderType GetType() const { return m_type; }

		//-----------------------------------------------------
		// セッター
		//-----------------------------------------------------

	private:

		//-----------------------------------------------------
		// 内部実装
		//-----------------------------------------------------

		friend std::unique_ptr<ShaderAsset> Loader::ShaderLoader(const std::wstring& path);
	};
}
