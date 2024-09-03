#pragma once
#include "Enemy.h"
#include"EnemySphere.h"



/// <summary>
/// 左右に移動し、一定間隔で射撃してくる敵e
/// 踏みつけられるとつぶれる
/// </summary>
class Takobo : public Enemy
{
public:

	/// <param name="m_modelhandle">エネミーのモデル</param>
	/// <param name="obj">影響を与えるオブジェ</param>
	Takobo(Vec3 pos);
	~Takobo();

	void Init();
	void Update();
	void SetMatrix();
	void Draw();

	void WantToPlayer(Vec3 toPlayer);

	virtual void OnCollideEnter(std::shared_ptr<Collidable> colider);


	void SetCameraAngle(float cameraAngle);

	void SetAttackDir(Vec3 targetPos);
	Vec3 GetMyPos();
	int WatchHp() const { return m_Hp; }
	float GetColRadius() { return m_radius; }

	float GetIdleSpeed() { return m_idleSpeed; }


	std::list<std::shared_ptr<EnemySphere>> GetAttackObj() { return m_sphere; }

	//メンバ関数ポインタ
	using enemyState_t = void(Takobo::*)();
	enemyState_t m_enemyUpdate;
private:
	/// <summary>
	/// 登場時
	/// </summary>
	void StartUpdate();
	/// <summary>
	/// 通常(クールダウン)状態
	/// </summary>
	void IdleUpdate();
	/// <summary>
	/// ダメージを受けた時
	/// </summary>
	void OnDamageUpdate();
	/// <summary>
	/// 球体を生成して攻撃
	/// </summary>
	void AttackSphereUpdate();
	/// <summary>
	/// 爆弾を上から投下して攻撃
	/// </summary>
	void AttackBombUpdate();

	Vec3 GetAttackDir() const;
private:
	int m_Hp;

	int m_attackCoolDownCount;

	float m_radius = 0;

	int m_modelHandle = 0;

	int m_createFrameCount = 0;

	float m_centerToEnemyAngle;

	float m_idleSpeed = 0;

	int m_sphereNum = 0;
	int m_bombNum = 0;

	Vec3 m_vec;
	Vec3 m_attackDir;
	Vec3 m_moveShaftPos;
	//アニメーション変数
	int m_anim_nutral = 0;
	int m_anim_move = 0;
	int m_anim_jump = 0;
	int m_anim_attack = 0;

	std::list<std::shared_ptr<EnemySphere>> m_sphere;
};

