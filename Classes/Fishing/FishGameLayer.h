#ifndef __FISH_GAME_LAYER_H__
#define __FISH_GAME_LAYER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

// 钓鱼小游戏图层
class FishGameLayer : public cocos2d::Layer
{
public:
    // 静态创建函数
    static FishGameLayer* create();

    // 初始化函数
    virtual bool init() override;

    // 每一帧更新游戏逻辑
    void update(float delta) override;

    // 鼠标点击事件处理
    void onMouseClick();

    // 结束游戏
    void endGame(bool isSuccess);

private:
    // 设置游戏 UI 元素
    void setupUI();

    // 设置事件监听器
    void setupEvents();

    // 更新进度条显示
    void updateProgressBar();

    // 更新鱼的位置
    void updateFishPosition();

private:
    // UI 元素
    cocos2d::Sprite* _background = nullptr;     // 游戏背景图
    cocos2d::DrawNode* _progressBar = nullptr;  // 进度条绘制节点
    cocos2d::Sprite* _fishSprite = nullptr;     // 鱼的精灵
    cocos2d::Label* _resultLabel = nullptr;     // 结果显示标签

    // 游戏状态
    float _currentValue = 0.0f;     // 当前进度值
    bool _isGameActive = false;     // 游戏是否处于活动状态

    // 布局参数
    cocos2d::Rect _progressBarRect;     // 进度条在背景图片上的局部区域
    cocos2d::Vec2 _fishBasePosition;    // 鱼在背景图片上的基准位置
    float _progressBarHeight = 0.0f;    // 进度条总高度
};

#endif // __FISH_GAME_LAYER_H__