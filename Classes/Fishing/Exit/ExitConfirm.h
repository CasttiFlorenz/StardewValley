#ifndef __EXIT_CONFIRM_H__
#define __EXIT_CONFIRM_H__

#include "cocos2d.h"
#include "../Music/MusicManager.h"

// 退出确认界面
class ExitConfirm : public cocos2d::Node
{
public:
    CREATE_FUNC(ExitConfirm);

    // 静态创建函数
    static ExitConfirm* create(cocos2d::Node* parent, cocos2d::Sprite* background);

    // 初始化
    virtual bool init() override;

private:
    // 返回标题界面
    void onReturnToTitle();

    // 退出游戏
    void onExitGame();
};

#endif // __EXIT_CONFIRM_H__