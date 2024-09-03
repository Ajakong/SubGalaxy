#pragma once
#include <memory>
#include <vector>
#include "../ObjectTag.h"
#include "Rigidbody.h"
#include "ColliderBase.h"

namespace MyEngine
{
	class Physics;
//	class ColliderBase;

	/// <summary>
	/// 衝突できるもの
	/// </summary>
	class Collidable : public std::enable_shared_from_this<Collidable>
	{
		friend Physics;
	public:
		// 優先度判別
		enum class Priority : int
		{
			Low,		// 低
			Middle,		// 中
			High,		// 高
			Static,		// 動かない（最高）
		};
	public:
		Collidable(Priority priority, ObjectTag tag);
		Collidable(Collidable* col);
		virtual ~Collidable();

		// 衝突したとき
		virtual void OnCollideEnter(std::shared_ptr<Collidable> colider) {}
		virtual void OnCollideStay(std::shared_ptr<Collidable> colider) {}
		virtual void OnCollideExit(std::shared_ptr<Collidable> colider) {}
		virtual void OnTriggerEnter(std::shared_ptr<Collidable> colider) {}
		virtual void OnTriggerStay(std::shared_ptr<Collidable> colider) {}
		virtual void OnTriggerExit(std::shared_ptr<Collidable> colider) {}

		/* Getter */
		ObjectTag GetTag() const { return m_tag; }
		Priority GetPriority() const { return m_priority; }

		// 当たり判定を無視（スルー）するタグの追加/削除
		void AddThroughTag(ObjectTag tag);
		void RemoveThroughTag(ObjectTag tag);
		Rigidbody PlanetOnlyGetRigid() { return m_rigid; };
		void SetReverceGravityVec(Vec3 gravityReverce) { m_upVec = gravityReverce; }

		// 当たり判定を無視（スルー）する対象かどうか
		bool IsThroughTarget(std::shared_ptr<Collidable>) const;

	protected:
		std::shared_ptr<ColliderBase> AddCollider(const ColliderBase::Kind& kind);

	protected:
		// 物理データ
		Rigidbody m_rigid;
		// 当たり判定データ
		std::list<std::shared_ptr<ColliderBase>> m_colliders;
		Vec3 m_upVec;
	
	private:
		std::list<ObjectTag>	throughTags;

		ObjectTag m_tag;
		Priority m_priority;
	};
}

