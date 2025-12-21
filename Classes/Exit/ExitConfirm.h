/****************************************************************
 * Project Name:  StardewValley
 * File Name:     ExitConfirm.h
 * File Function:  ExitConfirm类的实现
 * Author:        于恩熙
 * Update Date:   2025/12/21
 * License:       MIT License
 ****************************************************************/

#ifndef __EXIT_CONFIRM_H__
#define __EXIT_CONFIRM_H__

#include "cocos2d.h"

class ExitConfirm : public cocos2d::Node
{
public:
    CREATE_FUNC(ExitConfirm);

    // 创建退出确认界面
    static ExitConfirm* create(cocos2d::Node* parent, cocos2d::Sprite* background);

    virtual bool init() override;

private:
    void onReturnToTitle();
    void onExitGame();
};

#endif // __EXIT_CONFIRM_H__