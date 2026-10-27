/**
 * @file    IColliderReceiver.h
 * @author  Hoshino Ryunosuke
 * @date    2026/05/28
 *
 * @brief   衝突応答仲介インターフェース
 */

#pragma once

namespace RHEngine
{
	struct HitContact;
	struct HitContact2D;

	/**
	 * @brief 衝突応答仲介インターフェース
	 */
	class IColliderReceiver
	{
	public:

		/// @name ライフサイクル
		/// @{

		virtual ~IColliderReceiver() = default;

		/// @}

		virtual void OnCollisionEnter(HitContact& contact) = 0;
		virtual void OnCollisionStay(HitContact& contact) = 0;
		virtual void OnCollisionExit(HitContact& contact) = 0;
		virtual void OnTriggerEnter(HitContact& contact) = 0;
		virtual void OnTriggerStay(HitContact& contact) = 0;
		virtual void OnTriggerExit(HitContact& contact) = 0;

		virtual void OnCollisionEnter2D(HitContact2D& contact) = 0;
		virtual void OnCollisionStay2D(HitContact2D& contact) = 0;
		virtual void OnCollisionExit2D(HitContact2D& contact) = 0;
		virtual void OnTriggerEnter2D(HitContact2D& contact) = 0;
		virtual void OnTriggerStay2D(HitContact2D& contact) = 0;
		virtual void OnTriggerExit2D(HitContact2D& contact) = 0;
	};
} // namespace RHEngine
