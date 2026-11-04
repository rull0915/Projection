/**
 * @file    OBJLoader.h
 * @author  Hoshino Ryunosuke
 * @date    2026/10/07
 *
 * @brief   .obj形式を読み込んでModel構造体を作成するクラス
 */

#pragma once

#include <filesystem>

#include "Assets/Managers/AssetLoadResult.h"

namespace RHEngine
{
	class AssetContext;

	namespace Loader
	{
		/**
		 * @brief .objを読み込む
		 */
		class OBJLoader
		{
		public:
			/**
			 * @brief 頂点を識別するキー
			 */
			struct VertexKey
			{
				int position;	///< 座標のインデックス
				int texcoord;	///< uv座標のインデックス
				int normal;		///< 法線のインデックス

				bool operator==(const VertexKey& other) const
				{
					return position == other.position &&
						texcoord == other.texcoord &&
						normal == other.normal;
				}
			};

			/**
			 * @brief VertexKeyのハッシュ構造体
			 */
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

			/**
			 * @brief obj形式のモデルを読み込む
			 * @param path 読み込むファイルパス
			 * @param ctx アセットコンテキスト
			 * @return 読み込み結果
			 */
			static LoadResult Load(const std::filesystem::path& path, AssetContext& ctx);

			/// <summary>
			/// OBJ形式の要素を取得する関数
			/// </summary>
			/// <typeparam name="T">型</typeparam>
			/// <param name="elements">取得対象の配列</param>
			/// <param name="index">.obj形式に記述されていた番号</param>
			/// <returns></returns>
			 
			/**
			 * @brief obj形式の要素を配列から取得する
			 * @details objでは、インデックスが1始まりだったり負のインデックスが末尾からのオフセットを表したりするため、それに対応する
			 * @param elements 取得対象の配列
			 * @param index objファイルに記述されていたインデックス
			 * @return 取得した要素
			 */
			template<class T>
			static T GetOBJElement(const std::vector<T>& elements, int index)
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
		};
	}
}	// namespace RHEngine
