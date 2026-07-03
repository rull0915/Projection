//====================================================//
// ファイル名   : ShaderTestScene.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/07/03
//
// 概要 : ShaderTestSceneシーン
//
// 更新履歴 : 
// 2026/07/03 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "Scene/Scene.h"

//====================================================//
// 前方宣言
//====================================================//
class Game;

//====================================================//
// クラス宣言
//====================================================//
class ShaderTestScene : public Scene
{
	// 情報受け渡し用構造体
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix world;
		DirectX::SimpleMath::Matrix view;
		DirectX::SimpleMath::Matrix projection;
		DirectX::SimpleMath::Vector3 lightDir;
		float padding;	// サイズを16の倍数に補正する用の未使用変数
	};

private:

	//-----------------------------------------------------
	// 定数
	//-----------------------------------------------------


	//-----------------------------------------------------
	// メンバ変数
	//-----------------------------------------------------
	Game* m_pGame;

	// カメラ
	GameObject* m_camera;

	//-------- シェーダー --------//

	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;	// 頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;	// ピクセルシェーダ

	Microsoft::WRL::ComPtr<ID3D11InputLayout>  m_inputLayout;	// インプットレイアウト

	// 送信用バッファオブジェクト
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_constBuffer;

	// 頂点バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;

public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	ShaderTestScene(Game* pGame);
	~ShaderTestScene();

	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------

	void Initialize() override;
	void Update(const GameTimer& gameTimer) override;
	void Render(Renderer& renderer) override;
	void Finalize() override;

	//-----------------------------------------------------
	// ゲッター
	//-----------------------------------------------------

	//-----------------------------------------------------
	// セッター
	//-----------------------------------------------------

private:

	//-----------------------------------------------------
	// 内部実装
	//-----------------------------------------------------

	// シェーダーの初期設定を行う関数
	void InitializeShader();
};
