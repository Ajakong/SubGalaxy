#include "Enemy.h"

namespace
{
	constexpr int kMaxHp = 50;
}

Enemy::Enemy(int handle, Priority priority, ObjectTag tag):Collidable(priority,tag),
	//m_Hp(50),
	m_handle(handle)
{
}

Enemy::~Enemy()
{
}
