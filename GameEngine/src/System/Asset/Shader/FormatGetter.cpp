//====================================================//
// ファイル名  : FormatGetter.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/08/04
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "FormatGetter.h"

//====================================================//
// 関数の実体宣言
//====================================================//

DXGI_FORMAT REngine::Format::GetFormat(D3D_REGISTER_COMPONENT_TYPE type, BYTE mask)
{
	// タイプで分岐
	switch (type)
	{
	case D3D_REGISTER_COMPONENT_UINT32:
		switch (mask)
		{
		case 0b0001:
			return DXGI_FORMAT_R32_UINT;
		case 0b0011:
			return DXGI_FORMAT_R32G32_UINT;
		case 0b0111:
			return DXGI_FORMAT_R32G32B32_UINT;
		case 0b1111:
			return DXGI_FORMAT_R32G32B32A32_UINT;
		default:
			return DXGI_FORMAT_UNKNOWN;
		}
	case D3D_REGISTER_COMPONENT_SINT32:
		switch (mask)
		{
		case 0b0001:
			return DXGI_FORMAT_R32_SINT;
		case 0b0011:
			return DXGI_FORMAT_R32G32_SINT;
		case 0b0111:
			return DXGI_FORMAT_R32G32B32_SINT;
		case 0b1111:
			return DXGI_FORMAT_R32G32B32A32_SINT;
		default:
			return DXGI_FORMAT_UNKNOWN;
		}
	case D3D_REGISTER_COMPONENT_FLOAT32:
		switch (mask)
		{
		case 0b0001:
			return DXGI_FORMAT_R32_FLOAT;
		case 0b0011:
			return DXGI_FORMAT_R32G32_FLOAT;
		case 0b0111:
			return DXGI_FORMAT_R32G32B32_FLOAT;
		case 0b1111:
			return DXGI_FORMAT_R32G32B32A32_FLOAT;
		default:
			return DXGI_FORMAT_UNKNOWN;
		}
	default:
		return DXGI_FORMAT_UNKNOWN;
	}
}
