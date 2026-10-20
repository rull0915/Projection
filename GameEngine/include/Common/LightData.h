//====================================================//
// ファイル名   : LightData.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/10/11
//
// 概要 : ライトのデータをまとめたヘッダ
//
// 更新履歴 :
// 2026/10/11 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//

#include <SimpleMath.h>

namespace RHEngine
{
	struct alignas(16) PointLight
	{
		DirectX::SimpleMath::Vector3 pos;	// ライトの位置
		float radius;						// 光が届く最大の半径
		DirectX::SimpleMath::Vector3 color; // 光の色
		float intensity;					// 光の強さ
	};

	struct alignas(16) SpotLight
	{
		DirectX::SimpleMath::Vector3 pos;	// ライトの位置
		float length;						// ライトが届く最大距離
		DirectX::SimpleMath::Vector3 color; // ライトの色    
		float intensity;					// ライトの強度
		DirectX::SimpleMath::Vector3 direction; // ライトの向き
		float cosInnerAngle;
		float cosOuterAngle;
		DirectX::SimpleMath::Vector3 padding;
	};

	struct alignas(16) DirectionalLight
	{
		DirectX::SimpleMath::Vector3 direction; // ライトの向き
		float intensity;						// ライトの強さ
		DirectX::SimpleMath::Vector3 color;		// ライトの色
		float padding;
	};
}
