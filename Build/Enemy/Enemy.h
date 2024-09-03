#pragma once
#include "../MyLib/Physics/Collidable.h"

class EnemyMover;
class EnemyAI;

class Enemy : public MyEngine :: Collidable
{
public:
	Enemy(int handle,Priority priority,ObjectTag tag);
	virtual ~Enemy();

	virtual void Init() = 0;
	virtual void Draw() = 0;
	virtual void Update()=0;
	//virtual int GetHP() { return m_Hp; }
protected:
	//int m_Hp;
	int m_handle;

};

