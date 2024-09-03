#pragma once
#include "Scene.h"

class ClearScene : public Scene
{
public:
    ClearScene(SceneManager& mgr);
    ~ClearScene();

    void Load();
    void Update();
    void Draw();

private:

    int m_btnFrame = 0;
    int m_frame;

    // メンバ関数ポインタの宣言
    using UpdateFunc_t = void (ClearScene::*)();
    using DrawFunc_t = void (ClearScene::*)();
    UpdateFunc_t m_updateFunc;
    DrawFunc_t m_drawFunc;

    // 更新状態を表す関数
    void FadeInUpdate();
    void NormalUpdate();
    void FadeOutUpdate();

    // 描画状態を表す関数
    void FadeDraw();
    void NormalDraw();

};

