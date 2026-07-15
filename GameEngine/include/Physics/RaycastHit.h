//====================================================//
// ファイル名   : RaycastHit.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/05/31
//
// 概要 : レイの衝突情報構造体
//
// 更新履歴 :
// 2026/05/31 新規作成
//====================================================//

#pragma once

namespace REngine
{
	//====================================================//
	// 前方宣言
	//====================================================//
	class ColliderBase;

	//====================================================//
	// 構造体宣言
	//====================================================//
	struct RaycastHit
	{
		ColliderBase* collider = nullptr;   // 衝突したコライダ

		DirectX::SimpleMath::Vector3 point{};
		DirectX::SimpleMath::Vector3 normal{};

		float distance = FLT_MAX;
	};
}	// namespace REngine
