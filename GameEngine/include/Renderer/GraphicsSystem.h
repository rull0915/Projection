//====================================================//
// ファイル名   : GraphicsSystem.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/04/29
//
// 概要 : レンダーが使用するステートを管理するクラス
//
// 更新履歴 :
// 2026/04/29 新規作成
//====================================================//

#pragma once

#include "Command/DrawCommandContainer.h"
#include "Command/DrawCommandExecutor.h"

// Material
#include "Assets/Types/MaterialAsset.h"
#include "Assets/Objects/Handle.h"

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class GraphicsSystem
	{
	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// 各行列
		DirectX::SimpleMath::Matrix m_world;
		DirectX::SimpleMath::Matrix m_view;
		DirectX::SimpleMath::Matrix m_projection;

		// マテリアルハンドル
		Handle<MaterialAsset> m_material;

		// 描画コマンドを管理するクラス
		DrawCommandContainer m_drawCommandContainer;

		// 描画コマンドを実行するクラス
		DrawCommandExecutor m_drawCommandExecutor;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		GraphicsSystem()
			: m_world		{ DirectX::SimpleMath::Matrix::Identity }
			, m_view		{ DirectX::SimpleMath::Matrix::Identity }
			, m_projection	{ DirectX::SimpleMath::Matrix::Identity }
			, m_material	{ ERROR_HANDLE<MaterialAsset> }
			, m_drawCommandContainer{}
		{
		};
		~GraphicsSystem() = default;
		
		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		// 初期化関数
		void Initialize()
		{
			// コマンド実行者の初期化
			m_drawCommandExecutor.Initialize();

			// 初期PrimitiveCommandを生成しておく
			auto& cmd = m_drawCommandContainer.AddPrimitive();
			cmd.world = DirectX::SimpleMath::Matrix::Identity;
		}

		//-----------------------------------------------------
		// ゲッター
		//-----------------------------------------------------

		const DirectX::SimpleMath::Matrix& GetWorld() const { return m_world; }
		const DirectX::SimpleMath::Matrix& GetView() const { return m_view; }
		const DirectX::SimpleMath::Matrix& GetProjection() const { return m_projection; }

		Handle<MaterialAsset> GetMaterial() const { return m_material; }

		DrawCommandContainer& GetCommandContainer() { return m_drawCommandContainer; }

		//-----------------------------------------------------
		// セッター
		//-----------------------------------------------------

		void SetWorld(const DirectX::SimpleMath::Matrix& world)
		{
			if (world != m_world)
			{
				m_world = world;

				// PrimitiveCommandを更新
				auto& p = m_drawCommandContainer.AddPrimitive();
				p.world = world;
				p.material = m_material;
			}
		}
		void SetVPMatrix(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
		{
			if (view != m_view || proj != m_projection)
			{
				// コマンド実行
				ExecuteCommands();

				// 行列の更新
				m_view = view;
				m_projection = proj;
			}
		}
		void SetMaterial(Handle<MaterialAsset> handle)
		{
			// 違うマテリアルを設定するとき
			if (handle != m_material)
			{
				m_material = handle;

				// PrimitiveCommandを更新
				auto& p = m_drawCommandContainer.AddPrimitive();
				p.world = m_world;
				p.material = handle;
			}
		}

		// コマンドの実行
		void ExecuteCommands()
		{
			// 貯めたコマンドを実行
			m_drawCommandExecutor.DrawCommandExecute(m_drawCommandContainer, m_view, m_projection);

			// コマンドのリセット
			m_drawCommandContainer.Clear();

			// PrimitiveCommandを初期化
			auto& p = m_drawCommandContainer.AddPrimitive();
			p.world = m_world;
			p.material = m_material;
		}
	};
}	// namespace REngine
