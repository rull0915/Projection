//====================================================//
// ファイル名   : ArrayTester.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/09/03
//
// 概要 : 
//
// 更新履歴 :
// 2026/09/03 新規作成
//====================================================//

#pragma once

#define IS_COMPONENT(ArrayTester)

//====================================================//
// インクルードファイル
//====================================================//
#include "Components/World/WorldComponentBase.h"

class ABCD : public REngine::PropertyObject
{
	int id;
	std::string name;

public:
	ABCD()
	{
		ADD_PROPERTY(id);
		ADD_PROPERTY(name);
	}
};

//====================================================//
// クラス宣言
//====================================================//
class ArrayTester : public REngine::WorldComponentBase
{
private:

	//-----------------------------------------------------
	// メンバ変数
	//-----------------------------------------------------

	std::vector<int> m_testArray;
	std::vector<ABCD> m_colorArray;

public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	ArrayTester(REngine::IComponentOwner* own);
	~ArrayTester() = default;

	//-----------------------------------------------------
	// Type
	//-----------------------------------------------------
	
	COMPONENT_TYPE(ArrayTester, REngine::WorldComponentBase);
	
	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------

	void Start() override;

	void Update(const REngine::GameTimer&) override;

};
