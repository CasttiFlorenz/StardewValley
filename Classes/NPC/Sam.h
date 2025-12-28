/****************************************************************
 * Project Name:  StardewValley
 * File Name:     Sam.h
 * File Function: Sam类的实现
 * Author:        赵睿妍、蔡锦慧
 * Update Date:   2025/12/24
 * License:       MIT License
 ****************************************************************/
#ifndef __SAM_NPC_H__
#define __SAM_NPC_H__

#include "NPCBase.h"

class Sam : public NPCBase
{
public:
    CREATE_FUNC(Sam);

    // 初始化 Sam 及其纹理
    virtual bool init() override;

    // 播放 Sam 的待机动画
    virtual void playAnimation() override;

    // 获取 Sam 的对话内容
    virtual std::vector<std::string> getConversation(bool isFirstMet) override;

    // 检查 Sam 对礼物的喜好
    virtual int checkGiftTaste(ItemType itemTag) override;

    virtual void startWalkingAnimation(int animationRow) override;

    virtual void stopWalkingAnimation()override;
private:
    // 创建具体的动画动作（可能涉及拼接）
    void createAndPlayAnimation();
};

#endif // __SAM_NPC_H__