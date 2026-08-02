//====================================================//
// ファイル名   : IAssetDependent.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/07/26
//
// 概要 : Assetに依存することを明示するインターフェース
//
// 更新履歴 :
// 2026/07/26 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//

namespace REngine
{
	class AssetManager;

	//====================================================//
	// クラス宣言
	//====================================================//
	class IAssetDependent
	{
	public:
		// 仮想デストラクタ
		virtual ~IAssetDependent() = default;

		// AssetManagerを受け取るクラス
		virtual void ReceiveAssetManager(AssetManager& assets) = 0;
	};
}	// namespace REngine
