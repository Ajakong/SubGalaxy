#include"MyLib/Physics/ColliderSphere.h"
#include "SpherePlanet.h"

namespace
{
	constexpr float kGroundRadius = 500;
	constexpr float  kGravityRange = 1500;
	constexpr float  kGravityPower = 3;


}

SpherePlanet::SpherePlanet(Vec3 pos):Planet(),
m_enemyCount(0)
{
	gravityPower = 3;
	AddCollider(MyEngine::ColliderBase::Kind::Sphere);//ここで入れたのは重力の影響範囲
	m_colliders.back()->isTrigger = true;
	auto item = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back());
	item->radius = kGravityRange;
	AddThroughTag(ObjectTag::Stage);
	AddCollider(MyEngine::ColliderBase::Kind::Sphere);//マップの当たり判定
	auto item2 = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back());
	item2->radius = kGroundRadius;
	m_rigid.SetPos(pos);
}

SpherePlanet::~SpherePlanet()
{
}

void SpherePlanet::Init()
{
}

void SpherePlanet::Update()
{
}

void SpherePlanet::Draw()
{
	DrawSphere3D(m_rigid.GetPos().VGet(), kGravityRange, 10, 0xddddff, 0x0000ff, false);
	DrawSphere3D(m_rigid.GetPos().VGet(), kGroundRadius, 50, 0xaadd33, 0xff0000, true);
	//printfDX("m_enemyCount:%d", m_enemyCount);
}

Vec3 SpherePlanet::GravityEffect(std::shared_ptr<Collidable> obj)//成分ごとに計算し、補正後のベクトルを返す
{
	////惑星の中心からy方向に伸ばした線を軸にし、オブジェクトの位置を見て軸と惑星の中心からオブジェクトに向かうベクトルの角度分だけオブジェクトのベロシティのy方向に影響させるという考え方、Xに進みたい場合軸のXを基準に,Zに進みたい場合軸のZを基準
	////Yは法線の角度に回転させる
	Vec3 ansVelocity;
	Vec3 objPos=obj->PlanetOnlyGetRigid().GetPos();
	Vec3 objVelocity = obj->PlanetOnlyGetRigid().GetVelocity();
	Vec3 toObj= m_rigid.GetPos()-objPos;
	toObj=toObj.GetNormalized();

	//float angleX = DX_PI_F / 2 + atan2(toObj.y, toObj.x);
	//float angleZ= DX_PI_F / 2 + atan2(toObj.y, toObj.z);
	//ansVelocity = { objVelocity.x * cos(angleX), objVelocity.x * sin(angleX) + objVelocity.z * sin(angleZ), objVelocity.z * cos(angleZ) };
	//ansVelocity += toObj*objVelocity.y;//プレイヤーのジャンプ分のベクトルの加算

	//ansVelocity += toObj * kGravityPower;
	//obj->SetReverceGravityVec(toObj.GetNormalized());

	///*VECTOR ANSVECTOR = VGet(objVelocity.x * cos(angleX), objVelocity.x * sin(angleX) + objVelocity.z * sin(angleZ), objVelocity.z * cos(angleZ));
	//ANSVECTOR = VAdd(ANSVECTOR, objVelocity.y * toObj);
	//ansVelocity = ANSVECTOR;*/
	////ansVelocity -= toObj;
	//return ansVelocity;
	
	//重力のみ
	toObj = toObj * gravityPower+objVelocity;
	return toObj;
}

Vec3 SpherePlanet::GetNormVec(Vec3 pos)
{
	Vec3 norm = pos - m_rigid.GetPos();
	norm.Normalize();
	return norm;
}

void SpherePlanet::OnTriggerEnter(std::shared_ptr<Collidable> colider)
{
	if (colider->GetTag() == ObjectTag::Takobo)
	{
		m_enemyCount++;
	}
}

void SpherePlanet::OnTriggerExit(std::shared_ptr<Collidable> colider)
{
	if (colider->GetTag() == ObjectTag::Takobo)
	{
		m_enemyCount--;
	}
	if (m_enemyCount <= 0)
	{
		clearFlag = true;
	}
}
