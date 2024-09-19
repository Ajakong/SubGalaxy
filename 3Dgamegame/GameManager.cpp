#include"DxLib.h"
// EffekseerForDXLib.hをインクルードします。
#include "EffekseerForDXLib.h"
#include "GameManager.h"
#include"Ui.h"
#include"Camera.h"
#include"MyLib/Physics/Physics.h"
#include"Player.h"
#include"Object/WarpGate.h"
#include"BossPlanet.h"
#include"Enemy/Takobo.h"
#include"Enemy/KillerTheSeeker.h"
#include"Enemy/Gorori.h"
#include"Object/Item.h"
#include"Object/ClearObject.h"
#include<cassert>
#include"WorldTimer.h"
#include"Pad.h"
#include"SoundManager.h"
#include"GraphManager.h"
#include"FontManager.h"
#include"ScreenManager.h"
#include"ModelManager.h"
#include"Game.h"


namespace
{
	//UI
	//ミッション
	constexpr int kUiText_SrkX = 150;
	constexpr int kUiText_SrkY = 170;
	constexpr int kUiText_Width = 1050;
	constexpr int kUiText_Height = 450;
	constexpr float kUiText_Exrate = 0.2f;

	//HPバーの枠
	constexpr int kUiHpbarFrame_PosX = 150;
	constexpr int kUiHpbarFrame_PosY = 35;
	constexpr int kUiHpbarFrame_SrkX = 2600;
	constexpr int kUiHpbarFrame_SrkY = 2560;
	constexpr int kUiHpbarFrame_Width = 1000;
	constexpr int kUiHpbarFrame_Height = 144;
	constexpr float kUiHpbarFrame_Exrate = 0.3f;

	//ChargeRemainTimeのバー
	constexpr int kUiCRT_PosX = 20;
	constexpr int kUiCRT_PosY = 70;
	constexpr int kUiCRT_SrkX = 2655;
	constexpr int kUiCRT_SrkY = 2720;
	constexpr int kUiCRT_Width = 30;
	constexpr int kUiCRT_DisX = 9;
	constexpr int kUiCRT_Height = 80;
	constexpr float kUiCRT_Exrate = 0.3f;

	//HPバー
	constexpr int kUiHpbar_PosX = 15;
	constexpr int kUiHpbar_PosY = 25;
	constexpr int kUiHpbar_Height = 23;
	constexpr float kUiHpbar_mag = 5.35f;//HPに掛けたらいい感じになる倍率

	//タイマーの枠
	constexpr int kUiTimeCountFrame_PosX = 1400;
	constexpr int kUiTimeCountFrame_PosY = 100;
	constexpr int kUiTimeCountFrame_SrkX = 1280;
	constexpr int kUiTimeCountFrame_SrkY = 130;
	constexpr int kUiTimeCountFrame_Width = 1040;
	constexpr int kUiTimeCountFrame_Height = 410;
	constexpr float kUiTimeCountFrame_Exrate = 0.3f;

	//タイマー
	constexpr int kUiTimeCount_PosX = 1350;
	constexpr int kUiTimeCount_PosY = 90;

	//カメラ
	constexpr float kCameraDistanceFront = 500.f;
	constexpr float kCameraDistanceAddFrontInJump = 200.f;
	constexpr float kCameraDistanceUp = 200.f;

	const char* kMiniMapScreenName = "MiniMap";
}

GameManager::GameManager() :
	// 通常のRT
	RT(MakeScreen(640, 480, true)),
	RT2(MakeScreen(640, 480, true)),
	// アウトラインのRT
	outlineRT(MakeScreen(640, 480, true)),
	// 法線情報のRT
	normRT(MakeScreen(640, 480)),
	// ぼかし用RT
	blurRT(MakeScreen(640, 480, true)),
	shrinkRT(MakeScreen(320, 240, true)),
	depthRT(MakeScreen(640, 480)),
	/*skyDomeH(MV1LoadModel("Model/Skydome/universe_skydome.mv1")),*/
	m_isClearFlag(false),
	itemNum(0),
	m_isBossWatch(false)
{
	ui = std::make_shared<Ui>();
	player = std::make_shared<Player>(modelH);
	camera = std::make_shared<Camera>();
	planet.push_back(std::make_shared<SpherePlanet>(Vec3(0, -500, 0), 0xaadd33,3,ModelManager::GetInstance().GetModelData("Sphere/planet_moon.mv1")));
	planet.push_back(std::make_shared<SpherePlanet>(Vec3(6000, 0, 2000),0x4444ff,3,ModelManager::GetInstance().GetModelData("Sphere/planet_ice.mv1")));
	planet.push_back(std::make_shared<SpherePlanet>(Vec3(-3000, 1000, -3000), 0xff4400,1.f, ModelManager::GetInstance().GetModelData("Sphere/planet_red.mv1")));
	bossPlanet = std::make_shared<BossPlanet>(Vec3(0, -6000, 0), 0x0000ff);
	takobo = { std::make_shared<Takobo>(Vec3(1000,0,500)),std::make_shared<Takobo>(Vec3(-300,0,500)),std::make_shared<Takobo>(Vec3(0,900,500)) };
	gorori = { std::make_shared<Gorori>(Vec3(7000,500,2300)),std::make_shared<Gorori>(Vec3(6500,500,1700)),std::make_shared<Gorori>(Vec3(5500,0,2000)) };
	poworStone.push_back(std::make_shared<Item>(Vec3(0, -800, 0),true));
	poworStone.push_back(std::make_shared<Item>(Vec3(-300, 0, 0), true));
	poworStone.push_back(std::make_shared<Item>(Vec3(300, 0, 0), true));
	poworStone.push_back(std::make_shared<Item>(Vec3(-200, -800, 0),true));
	poworStone.push_back(std::make_shared<Item>(Vec3(0, 0, 500), true));
	poworStone.push_back(std::make_shared<Item>(Vec3(0, 0, -500), true));
	poworStone.push_back(std::make_shared<Item>(Vec3(6000, 600, 2000), true));
	poworStone.push_back(std::make_shared<Item>(Vec3(6000, 500, 2200), true));
	poworStone.push_back(std::make_shared<Item>(Vec3(6000, 100, 1400), true));
	poworStone.push_back(std::make_shared<Item>(Vec3(6000, -500, 2000), true));
	poworStone.push_back(std::make_shared<Item>(Vec3(-3300, 1000, -3300), true));
	m_skyDomeH = ModelManager::GetInstance().GetModelData("Skybox.mv1");
	fontHandle = FontManager::GetInstance().GetFontData("disital.TTF", "Pocket Calculator",60,7,DX_FONTTYPE_NORMAL);

	MV1SetScale(m_skyDomeH, VGet(1, 1, 1));

	m_managerUpdate = &GameManager::IntroUpdate;
	m_managerDraw = &GameManager::IntroDraw;

	m_miniMapScreenHandle = ScreenManager::GetInstance().GetScreenData(kMiniMapScreenName,Game::kScreenWidth,Game::kScreenHeight);
}

GameManager::~GameManager()
{
	planet.clear();
	takobo.clear();
	gorori.clear();
	poworStone.clear();
	warpGate.clear();
}

void GameManager::Init()
{
	/*MATERIALPARAM Material;
	Material.Diffuse = GetColorF(1.0f, 1.0f, 1.0f, 1.0f);
	Material.Ambient = GetColorF(0.0f, 0.0f, 0.0f, 0.5f);
	Material.Specular = GetColorF(0.5f, 0.5f, 0.5f, 0.5f);
	Material.Emissive = GetColorF(0.0f, 0.0f, 0.0f, 0.0f);
	Material.Power = 20.0f;
	SetMaterialParam(Material);
	SetLightAmbColor(GetColorF(0.5f, 0.5f, 0.5f, 1.0f));*/
	SetGlobalAmbientLight(GetColorF(0.0f, 0.0f, 1.0f, 1.0f));
	
	player->SetMatrix();

	// メッシュの数を取ってくる
	auto meshNum = MV1GetMeshNum(modelH);

	VECTOR maxPos = { 0, 0, 0 };
	VECTOR minPos = { 1000, 1000, 1000 };
	bool hasNormalMap = false;
	for (int i = 0; i < meshNum; ++i)
	{
		// 輪切りの時は裏側も描画しないと変になる
		MV1SetMeshBackCulling(modelH, i, DX_CULLING_NONE);

		// モデルの大きさを取得
		auto modelMaxPos = MV1GetMeshMaxPosition(modelH, i);
		maxPos.x = std::max(maxPos.x, modelMaxPos.x);
		maxPos.y = std::max(maxPos.y, modelMaxPos.y);
		maxPos.z = std::max(maxPos.z, modelMaxPos.z);

		auto modelMinPos = MV1GetMeshMinPosition(modelH, i);
		minPos.x = std::min(minPos.x, modelMinPos.x);
		minPos.y = std::min(minPos.y, modelMinPos.y);
		minPos.z = std::min(minPos.z, modelMinPos.z);

		auto vtype = MV1GetTriangleListVertexType(modelH, i);
		if (vtype == DX_MV1_VERTEX_TYPE_NMAP_1FRAME)
		{
			hasNormalMap = true;
		}
	}
	userData->minY = minPos.y;
	userData->maxY = maxPos.y;
	userData->clickedU = 0.0f;
	userData->clickedV = 0.0f;

	// 深度値記録バッファ用RT
	DxLib::SetCreateGraphChannelBitDepth(32);
	DxLib::SetCreateDrawValidGraphChannelNum(1);

	MyEngine::Physics::GetInstance().Entry(player);
	MyEngine::Physics::GetInstance().Entry(bossPlanet);
	for (auto& item : planet)
	{
		item->Init();
		MyEngine::Physics::GetInstance().Entry(item);
	}
	for (auto& item : clearObject)
	{
		item->Init();
		MyEngine::Physics::GetInstance().Entry(item);
	}
	for (auto& item : poworStone)
	{
		item->Init();
		MyEngine::Physics::GetInstance().Entry(item);
	}
	for (auto& item : takobo)
	{
		MyEngine::Physics::GetInstance().Entry(item);
		item->SetTarget(player);
	}
	
	for (auto& item : gorori)
	{
		MyEngine::Physics::GetInstance().Entry(item);
		item->SetTarget(player);
	}
}

void GameManager::Update()
{
	(this->*m_managerUpdate)();
}

void GameManager::Draw()
{
	(this->*m_managerDraw)();
}

void GameManager::IntroUpdate()
{
	ui->FadeUpdate();

	MyEngine::Physics::GetInstance().Update();
	Vec3 planetToPlayer = player->GetPos() - player->GetNowPlanetPos();
	Vec3 sideVec = GetCameraRightVector();
	Vec3 front = Cross(planetToPlayer, sideVec).GetNormalized() * -1;
	player->SetSideVec(sideVec);
	player->SetFrontVec(front);
	player->SetUpVec(planetToPlayer);

	//本当はカメラとプレイヤーの角度が90度以内になったときプレイヤーの頭上を見たりできるようにしたい。
	//camera->SetCameraPoint(player->GetPos() + (Vec3(GetCameraUpVector()).GetNormalized() * 100 - Vec3(GetCameraFrontVector())* 300));
	camera->SetUpVec(player->GetNormVec());
	camera->SetCameraPoint(player->GetPos() + (Vec3(GetCameraUpVector()).GetNormalized() * kCameraDistanceUp - front * (kCameraDistanceFront + kCameraDistanceAddFrontInJump * player->GetJumpFlag())));

	camera->Update(player->GetPos());

	// カリング方向の反転
	for (int i = 0; i < MV1GetMeshNum(modelH); ++i)
	{
		MV1SetMeshBackCulling(modelH, i, DX_CULLING_RIGHT);
	}

	// カリング方向を元に戻す
	for (int i = 0; i < MV1GetMeshNum(modelH); ++i)
	{
		MV1SetMeshBackCulling(modelH, i, DX_CULLING_LEFT);
	}
	
	if (ui->GetFadeCount() > 100)
	{
		m_cameraUpVec = GetCameraUpVector();

		m_managerUpdate = &GameManager::GamePlayingUpdate;
		m_managerDraw = &GameManager::GamePlayingDraw;
	}
}

void GameManager::IntroDraw()
{
	MV1SetPosition(m_skyDomeH, VGet(0,0,0));
	
	MV1DrawModel(m_skyDomeH);
	bossPlanet->Draw();
	for(auto& item :planet)item->Draw();
	player->Draw();
	for (auto& item : warpGate)
	{
		item->Draw();
	}
	for (auto& item : poworStone)
	{
		item->Draw();
	}
	for (auto& item : takobo)item->Draw();
	//for (auto& item : killerTheSeeker)item->Draw();
	for (auto& item : gorori)item->Draw();
	camera->DebagDraw();
	DxLib::SetRenderTargetToShader(1, -1);		// RTの解除
	DxLib::SetRenderTargetToShader(2, -1);		// RTの解除

	ui->Draw(fontHandle, static_cast<float>(player->WatchHp()), player->GetSearchRemainTime());
}

void GameManager::GamePlayingUpdate()
{
	//// 使用するシェーダをセットしておく
	//SetUseVertexShader(vsH);
	//SetUsePixelShader(psH);

	//UpdateShaderConstantBuffer(cbuffH);
	//SetShaderConstantBuffer(cbuffH, DX_SHADERTYPE_PIXEL, 4);

	//// シェーダーやってる部分
	//SetUseTextureToShader(3, dissolveH);
	//SetUseTextureToShader(4, sphMapH);
	//SetUseTextureToShader(5, roughH);
	//SetUseTextureToShader(6, metalH);
	//SetUseTextureToShader(7, toonH);
	////		SetRenderTargetToShader(0, RT);	// 0番にRTを設定
	//SetRenderTargetToShader(1, depthRT);
	//SetRenderTargetToShader(2, normRT);

	//MV1SetUseOrigShader(true);

	/* カメラの設定
	 RTを設定するとカメラの初期化が入ってるかもなので、RTの設定後にカメラの設定を行う*/

	 /*camera->SetUpVec(planet->GetNormVec(player->GetPos()));
	 camera->Update(player->GetPos());*/
	bossPlanet->Update();
	for(auto& item : planet)item->Update();
	
	for (auto& item : poworStone)
	{
		item->Update();
	}
	for (auto& item : clearObject)
	{
		item->Update();
	}
	player->Update();

	for (auto& item : takobo)
	{
		item->Update();
	}
	for (auto& item : killerTheSeeker)
	{
		item->Update();
	}
	for (auto& item : gorori)item->Update();

	userData->dissolveY = player->GetRegenerationRange();

	MyEngine::Physics::GetInstance().Update();
	for (auto& item : warpGate)item->SetEffectPos();
	if (player->GetBoostFlag())
	{
		camera->m_cameraUpdate = &Camera::NeutralUpdate;
	}
	for (int i = 0; i < takobo.size(); i++)
	{
		takobo[i]->DeleteManage();
		if (takobo[i]->WatchHp() < 0)
		{
			MyEngine::Physics::GetInstance().Exit(takobo[i]);

			takobo.erase(takobo.begin() + i);//さっきの例をそのまま使うと(1,2,5,3,4)でitには5まで入ってるので取り除きたい3,4はitからend()までで指定できる
			i--;
		}
	}
	for (auto& item : warpGate)
	{
		
		item->Update();
		
	}

	for (int i = 0; i < killerTheSeeker.size(); i++)
	{
		killerTheSeeker[i]->DeleteManage();
		if (killerTheSeeker[i]->WatchHp() < 300)
		{
			bossPlanet->SetColor(0x000000);
			bossPlanet->SetGravityPower(20.f);
			
		}
		if (killerTheSeeker[i]->WatchHp() < 100)
		{
			bossPlanet->SetColor(0xffaa00);
			bossPlanet->SetGravityPower(1.f);
		}
		if (killerTheSeeker[i]->WatchHp() < 0)
		{
			clearObject.push_back(std::make_shared<ClearObject>(killerTheSeeker[i]->GetRigidbody()->GetPos()));
			clearObject.back()->Init();
			MyEngine::Physics::GetInstance().Entry(clearObject.back());
			
			camera->WatchThis(clearObject.back()->GetRigidbody()->GetPos(),Vec3(clearObject.back()->GetRigidbody()->GetPos()+ (bossPlanet->GetNormVec(clearObject.back()->GetRigidbody()->GetPos())*300)), bossPlanet->GetNormVec(clearObject.back()->GetRigidbody()->GetPos()));
			MyEngine::Physics::GetInstance().Exit(killerTheSeeker[i]);

			killerTheSeeker.erase(killerTheSeeker.begin() + i);//さっきの例をそのまま使うと(1,2,5,3,4)でitには5まで入ってるので取り除きたい3,4はitからend()までで指定できる
			i--;
		}
	}

	for (int i = 0; i < gorori.size(); i++)
	{
		if (gorori[i]->WatchHp() < 0)
		{
			MyEngine::Physics::GetInstance().Exit(gorori[i]);

			gorori.erase(gorori.begin() + i);//さっきの例をそのまま使うと(1,2,5,3,4)でitには5まで入ってるので取り除きたい3,4はitからend()までで指定できる
			i--;
		}
	}
	for (int i = 0; i < poworStone.size(); i++)
	{
		if (poworStone[i]->GetDeleteFlag())
		{
			MyEngine::Physics::GetInstance().Exit(poworStone[i]);

			poworStone.erase(poworStone.begin() + i);//さっきの例をそのまま使うと(1,2,5,3,4)でitには5まで入ってるので取り除きたい3,4はitからend()までで指定できる
			i--;
		}
	}
	
	Vec3 planetToPlayer = player->GetPos() - player->GetNowPlanetPos();
	Vec3 sideVec = GetCameraRightVector();
	Vec3 front = Cross(planetToPlayer, sideVec).GetNormalized() * -1;
	player->SetSideVec(sideVec);
	player->SetFrontVec(front);
	player->SetUpVec(planetToPlayer);
	/*Vec3 playerToCamera = camera->GetPos() -player->GetPos();
	float a = acos(Dot(planetToPlayer.GetNormalized(), playerToCamera.GetNormalized())) * 180 / DX_PI_F;

	if ( a> 68)*/

	camera->SetBoost(player->GetBoostFlag());
	//本当はカメラとプレイヤーの角度が90度以内になったときプレイヤーの頭上を見たりできるようにしたい。
	//camera->SetCameraPoint(player->GetPos() + (Vec3(GetCameraUpVector()).GetNormalized() * 100 - Vec3(GetCameraFrontVector())* 300));
	camera->SetUpVec(player->GetNormVec());
	
	camera->SetCameraPoint(player->GetPos() + (Vec3(GetCameraUpVector()).GetNormalized() * kCameraDistanceUp - front * (kCameraDistanceFront + kCameraDistanceAddFrontInJump * player->GetJumpFlag())));
	
	camera->Update(player->GetPos());
	//camera->SetCameraPos(player->GetPos());

	player->SetMatrix();
	for (auto& item : takobo)item->SetMatrix();
	// カリング方向の反転
	for (int i = 0; i < MV1GetMeshNum(modelH); ++i)
	{
		MV1SetMeshBackCulling(modelH, i, DX_CULLING_RIGHT);
	}
	/*SetUseVertexShader(outlineVsH);
	SetUsePixelShader(outlinePsH);*/
	// カリング方向を元に戻す
	for (int i = 0; i < MV1GetMeshNum(modelH); ++i)
	{
		MV1SetMeshBackCulling(modelH, i, DX_CULLING_LEFT);
	}

	if (player->GetHp() <= 0)
	{
		m_isGameOverFlag = true;
	}
	/*if (planet->GetClearFlag())
	{
		m_isClearFlag = true;
		MyEngine::Physics::GetInstance().Clear();
	}*/
	if (player->IsClear())
	{
		m_isClearFlag = true;
	}
	if (poworStone.size() <= 5&& warpGate.size() == 0)
	{
		warpGate.push_back(std::make_shared<WarpGate>(Vec3(800, 0, 300), m_warpEffectHandle));
		warpGate.back()->SetWarpPos(Vec3(6000, 0, 2000));
		MyEngine::Physics::GetInstance().Entry(warpGate.back());
		camera->WatchThis(warpGate.back()->GetRigidbody()->GetPos(), Vec3(1600, 0, 600), planet[0]->GetNormVec(warpGate.back()->GetRigidbody()->GetPos()));
		ui->FadeNextMission();
	}
	if (poworStone.size() <= 1 && warpGate.size() == 1)
	{
		warpGate.push_back(std::make_shared<WarpGate>(Vec3(5500, 500, 1700), m_warpEffectHandle));
		warpGate.back()->SetWarpPos(Vec3(-3000, 1000, -3000));
		MyEngine::Physics::GetInstance().Entry(warpGate.back());
		camera->WatchThis(warpGate.back()->GetRigidbody()->GetPos(), Vec3(4000, 700, 1000), planet[0]->GetNormVec(warpGate.back()->GetRigidbody()->GetPos()));

		/*Vec3 zero = Vec3(0, 0, 0);
		Vec3 offSetVec =Vec3(0,650,0);
		Quaternion myQ;
		m_angle += 0.05f;

		Vec3 front = (Vec3(5500, 500, 1700) - Vec3(-3000, 1000, -3000)).GetNormalized();
		for (int i = 0; i < 16; i++)
		{
			myQ.SetQuaternion(offSetVec);
			myQ.SetMove(DX_PI_F * 1 / 8 * i + m_angle, front);
			Vec3 offSet = myQ.Move(offSetVec, zero);
			takobo.push_back(std::make_shared<Takobo>(Vec3(-3000, 1000, -3000) + offSet));

			MyEngine::Physics::GetInstance().Entry(takobo.back());
			takobo.back()->SetTarget(player);

		}*/
	}
	if (poworStone.size() == 0&&warpGate.size()==2)
	{
		warpGate.push_back(std::make_shared<WarpGate>(Vec3(-2500, 500, -2800), m_warpEffectHandle));
		warpGate.back()->SetWarpPos(Vec3(0, -6000, 0));
		MyEngine::Physics::GetInstance().Entry(warpGate.back());
		camera->WatchThis(warpGate.back()->GetRigidbody()->GetPos(), Vec3(-2000, 0, -2000), planet[0]->GetNormVec(warpGate.back()->GetRigidbody()->GetPos()));
		killerTheSeeker.push_back(std::make_shared<KillerTheSeeker>(Vec3(0, -7000, 0)));
		for (auto& item : killerTheSeeker)
		{
			item->Init();
			MyEngine::Physics::GetInstance().Entry(item);
			item->SetTarget(player);
		}
	}
	if (player->GetNowPlanetPos() == Vec3(0, -6000, 0)&& !m_isBossWatch)
	{
		m_isBossWatch = true;
		camera->WatchThis(killerTheSeeker.back()->GetMyPos(), killerTheSeeker.back()->GetMyPos() + Vec3(0, 0, -1200), bossPlanet->GetNormVec(killerTheSeeker.back()->GetMyPos()));
	}
	if (player->GetNowPlanetPos() == Vec3(6000, 0, 2000))
	{
		ui->OutNextMission();
	}

	WorldTimer::Update();
}

void GameManager::GamePlayingDraw()
{
	DxLib::SetDrawScreen(m_miniMapScreenHandle);//カメラのnear,farが勝手に変わってる・・・？
	
	SetCameraNearFar(1.f, 100000);
	
	ClearDrawScreen();
	
	

	SetCameraPositionAndTargetAndUpVec((player->GetPos() + player->GetNormVec() * 300).VGet(), player->GetPos().VGet(), m_cameraUpVec.VGet());
	m_cameraUpVec = GetCameraUpVector();

	Vec3 pos = MV1GetPosition(m_skyDomeH);
	DxLib::MV1DrawModel(m_skyDomeH);
	
	bossPlanet->SetIsSearch(player->IsSearch());
	bossPlanet->Draw();
	for (auto& item : planet)
	{
		item->SetIsSearch(player->IsSearch());
		item->Draw();
	}

	player->Draw();
	for (auto& item : warpGate)
	{
		item->Draw();
	}
	if (player->IsSearch())
	{
		DxLib::SetDrawBlendMode(DX_BLENDMODE_MUL, 255);
		// ちょっと暗い矩形を描画
		DxLib::DrawBox(0, 0, 1600, 900,
			0x444488, true);
		DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	}
	for (auto& item : poworStone)
	{
		item->Draw();
	}
	for (auto& item : clearObject)
	{
		item->Draw();
	}

	for (auto& item : takobo)item->Draw();
	for (auto& item : killerTheSeeker)item->Draw();
	for (auto& item : gorori)item->Draw();

	camera->DebagDraw();
	DxLib::SetRenderTargetToShader(1, -1);		// RTの解除
	DxLib::SetRenderTargetToShader(2, -1);		// RTの解除

	DxLib::SetDrawScreen(DX_SCREEN_BACK);
	SetCameraNearFar(1.f, 100000);

	camera->Update(player->GetPos());
	


	DxLib::DrawRectRotaGraph(kUiText_SrkX, kUiText_SrkY, kUiText_SrkX, kUiText_SrkY, kUiText_Width, kUiText_Height, kUiText_Exrate, 0, textureUIHandle, true);

	bossPlanet->SetIsSearch(player->IsSearch());
	bossPlanet->Draw();
	for (auto& item : planet)
	{
		item->SetIsSearch(player->IsSearch());
		item->Draw();
	}
	
	player->Draw();
	for (auto& item : warpGate)
	{
		item->Draw();
	}
	if (player->IsSearch())
	{
		DxLib::SetDrawBlendMode(DX_BLENDMODE_MUL, 255);
		// ちょっと暗い矩形を描画
		DxLib::DrawBox(0, 0, 1600, 900,
			0x444488, true);
		DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		
	}
	for (auto& item : poworStone)
	{
		item->Draw();
	}
	for (auto& item : clearObject)
	{
		item->Draw();
	}
	
	for (auto& item : takobo)item->Draw();
	for (auto& item : killerTheSeeker)item->Draw();
	for (auto& item : gorori)item->Draw();

	camera->DebagDraw();
	DxLib::SetRenderTargetToShader(1, -1);		// RTの解除
	DxLib::SetRenderTargetToShader(2, -1);		// RTの解除

	ui->Draw(fontHandle, static_cast<float>(player->WatchHp()), player->GetSearchRemainTime());

	int alpha = static_cast<int>(255 * (static_cast<float>(player->GetDamageFrame()) / 60.0f));

	DxLib::SetDrawBlendMode(DX_BLENDMODE_MULA, alpha);
	DxLib::DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0xff4444, true);
	DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	
	DxLib::DrawExtendGraph(1200, 600,1600,900, m_miniMapScreenHandle, false);
	
	
}
