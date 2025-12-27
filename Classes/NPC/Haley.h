#ifndef __HALEY_NPC_H__
#define __HALEY_NPC_H__

#include "NPCBase.h"

class Haley : public NPCBase
{
public:
    CREATE_FUNC(Haley);

    // 初始化 Haley 及其纹理
    virtual bool init() override;

    // 播放 Haley 的待机动画
    virtual void playAnimation() override;

    // 获取 Haley 的对话内容
    virtual std::vector<std::string> getConversation(bool isFirstMet) override;

    // 检查 Haley 对礼物的喜好
    virtual int checkGiftTaste(ItemType itemTag) override;

private:
    // 创建具体的动画动作
    void createAndPlayAnimation();
};

#endif // __HALEY_NPC_H__