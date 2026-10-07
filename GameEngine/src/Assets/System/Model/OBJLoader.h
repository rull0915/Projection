//====================================================//
// ファイル名   : OBJLoader.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/10/07
//
// 概要 : .obj形式を読み込んでModel構造体を作成するクラス
//
// 更新履歴 :
// 2026/10/07 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "Assets/Types/Model/Model.h"

namespace REngine
{
	namespace Loader
	{
		// 頂点を識別するキー
		struct VertexKey
		{
			int position;	// 座標のインデックス
			int texcoord;	// uv座標のインデックス
			int normal;		// 法線のインデックス

			bool operator==(const VertexKey& other) const
			{
				return position == other.position &&
					texcoord == other.texcoord &&
					normal == other.normal;
			}
		};

		// unordered_mapで使うためのハッシュ構造体
		struct VertexKeyHash
		{
			size_t operator()(const VertexKey& key) const
			{
				size_t h = std::hash<int>{}(key.position);
				h ^= std::hash<int>{}(key.texcoord) << 1;
				h ^= std::hash<int>{}(key.normal) << 2;
				return h;
			}
		};

		/// <summary>
		/// .obj形式のモデルを読み込む関数
		/// </summary>
		/// <param name="path">ファイルパス</param>
		/// <returns>モデルアセット</returns>
		std::unique_ptr<Model> OBJLoader(const std::filesystem::path& path);

		/// <summary>
		/// OBJ形式の要素を取得する関数
		/// </summary>
		/// <typeparam name="T">型</typeparam>
		/// <param name="elements">取得対象の配列</param>
		/// <param name="index">.obj形式に記述されていた番号</param>
		/// <returns></returns>
		template<class T>
		T GetOBJElement(const std::vector<T>& elements, int index)
		{
			// index0はエラー値として扱うためデフォルトの値を返す
			if (index == 0) return T{};

			// 実際にアクセスするインデックス
			int finalIndex = index;

			// indexが正の場合
			if (index > 0)
			{
				// objのインデックスは1スタートのため1つ減らしてアクセス
				finalIndex--;
			}
			// 負の場合
			else
			{
				// 末尾基準のオフセットとして扱う
				finalIndex = static_cast<int>(elements.size()) + index;
			}

			// 範囲外チェック
			if (finalIndex < 0 || static_cast<int>(finalIndex >= elements.size())) return T{};

			return elements[finalIndex];
		}
	}
}
