#pragma once
#include <memory>
#include <list>
#include <functional>
#include "../ObjectTag.h"

namespace MyEngine
{
	class Rigidbody;
	class Collidable;
	class ColliderBase;

	class Physics final
	{
	private:
		enum class OnCollideInfoKind
		{
			CollideEnter,
			CollideStay,
			CollideExit,
			TriggerEnter,
			TriggerStay,
			TriggerExit
		};
		struct OnCollideInfoData
		{
			Collidable* own;
			Collidable* send;
			OnCollideInfoKind kind;
		};
		using SendCollideInfo = std::unordered_map<Collidable*, std::list<Collidable*>>;
	private:
		Physics();

		Physics(const Physics&) = delete;
		void operator= (const Physics&) = delete;
	public:
		static Physics& GetInstance();

		void Entry(const std::shared_ptr<Collidable>& collidable);
		void Exit(const std::shared_ptr<Collidable>& collidable);

		void Update();

	private:
		void MoveNextPos() const;

		void CheckCollide();

		bool IsCollide(const Rigidbody& rigidA, const Rigidbody& rigidB, const std::shared_ptr<ColliderBase>& colliderA, const std::shared_ptr<ColliderBase>& colliderB) const;
		void FixNextPos(const Rigidbody& primaryRigid, Rigidbody& secondaryRigid, const std::shared_ptr<ColliderBase>& primaryCollider, const std::shared_ptr<ColliderBase>& secondaryCollider);
		void AddNewCollideInfo(Collidable* objA, Collidable* objB, SendCollideInfo& info);
		void CheckSendOnCollideInfo(SendCollideInfo& preSendInfo, SendCollideInfo& newSendInfo, bool isTrigger);
		void AddOnCollideInfo(Collidable* own, Collidable* send, OnCollideInfoKind kind);
		void OnCollideInfo(Collidable* own, Collidable* send, OnCollideInfoKind kind);
		void FixPos() const;

	private:
		std::list<std::shared_ptr<Collidable>> m_collidables;
		std::list<std::shared_ptr<Collidable>> m_stageCollidables;

		std::list<OnCollideInfoData> m_onCollideInfo;
		SendCollideInfo m_newCollideInfo;
		SendCollideInfo m_preCollideInfo;
		SendCollideInfo m_newTirrigerInfo;
		SendCollideInfo m_preTirrigerInfo;
	};
}

