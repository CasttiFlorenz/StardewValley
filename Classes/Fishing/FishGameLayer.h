#ifndef __FISH_GAME_LAYER_H__
#define __FISH_GAME_LAYER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class FishGameLayer : public cocos2d::Layer
{
public:
    static FishGameLayer* create();
    virtual bool init();

    void update(float delta) override;
    void setupUI();
    void setupEvents();
    void onMouseClick();
    void endGame(bool isSuccess);

    void updateProgressBar();
    void updateFishPosition();

private:
    cocos2d::Sprite* _background;
    cocos2d::DrawNode* _progressBar;
    cocos2d::Sprite* _fishSprite;
    cocos2d::Label* _resultLabel;

    float _currentValue;
    bool _isGameActive;

    // 位置常量（相对于背景图片的局部坐标）
    cocos2d::Rect _progressBarRect;
    cocos2d::Vec2 _fishBasePosition;
    float _progressBarHeight;
};

#endif // __FISH_GAME_LAYER_H__