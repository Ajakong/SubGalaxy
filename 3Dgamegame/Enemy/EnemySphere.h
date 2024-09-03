#pragma once
#include "../MyLib/Physics/Collidable.h"

class Enemy;
//
//class EnemySphere : public MyEngine::Collidable
//{
//public:
//	EnemySphere(Priority priority, ObjectTag tag);
//	virtual ~EnemySphere();
//
//	void Init();
//	void Update();
//	void Draw();
//
//private:
//	int m_color;
//	bool m_deleteFlag;
//
//	float m_radius = 0;
//
//	Vec3 m_velocity;
//	std::shared_ptr<Enemy>m_enemy;
//};

/// <summary>
/// Enemyが攻撃の際に生成する球体オブジェクト
/// </summary>
class EnemySphere : public MyEngine::Collidable
{
public:
	EnemySphere(MyEngine::Collidable::Priority priority, ObjectTag tag,std::shared_ptr<MyEngine::Collidable>enemy, Vec3 pos, Vec3 velocity, int moveNum, int color = 0xff0000);
	virtual ~EnemySphere();

	virtual void Init();
	virtual void Update();
	virtual void Draw();

	virtual void Hit();

	virtual bool IsDelete() { return m_isDeleteFlag; }

	//メンバ関数ポインタ
	using MoveState_t = void(EnemySphere::*)();
	MoveState_t m_moveUpdate;

protected:
	void  StraightUpdate();//球を直線状に飛ばす

protected:


	int m_color = 0;
	bool m_isDeleteFlag = 0;

	float m_radius = 0;

	Vec3 m_velocity;
	std::shared_ptr<Enemy>m_enemy;

private:
	void DeleteJudge();

private:

};