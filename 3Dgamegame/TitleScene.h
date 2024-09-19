#pragma once
#include "Scene.h"
#include <string>
#include <vector>
#include <map>
#include"MyLib/Vec3.h"
#include"SoundManager.h"

class SceneMain;


class TitleScene : public Scene
{

    struct Sphere
    {
        Vec3 pos;
        Vec3 axis;
        int color;
    };

public:
    TitleScene(SceneManager& manager);
    ~TitleScene();
    void Load();
    void Update();
    void Draw();

    void getSlectNumber(int a) { m_selectNum = a; }

private:

    int m_titleHandle = 0;
    int m_titleBGMHandle;
    int m_gameStartSEHandle;
    int m_modelHandle[10] = { 0 };
    int m_model_dance = 0;
    int m_attach_move[10] = { 0 };
    int m_gamePlayingBgmHandle;
    int m_frameHandle = 0;

    int m_btnFrame = 0;
    int m_fadeSpeed = 0;
    int m_frame = 0;
    int m_soundVolume = 0;

    bool m_back = false;

    std::vector<Sphere> m_spherePos;

    int m_stageBgm = 0;

    int m_backBlack = 0;
    float m_angle=0;

    float m_fps = 0.0f;

    int m_selectNum = 0;
    bool m_soundFlag = false;

    bool m_isGamePlaying = false;


    float m_playAnimTime = 0.0f;
    float m_animTime_move = 0.0f;

    // メンバ関数ポインタの宣言
    using UpdateFunc_t = void (TitleScene::*)();
    using DrawFunc_t = void (TitleScene::*)();
    UpdateFunc_t m_updateFunc = nullptr;
    DrawFunc_t m_drawFunc = nullptr;



    // 更新関数
    void FadeInUpdate();
    void NormalUpdate();
    void FadeOutUpdate();

    void ChangeScene(std::shared_ptr<Scene> next);


    // 描画関数
    void FadeDraw();
    void NormalDraw();

};



