#ifndef __EVELYN_NPC_H__
#define __EVELYN_NPC_H__

#include "NPCBase.h"

class Evelyn : public NPCBase
{
public:
    CREATE_FUNC(Evelyn);

    // 初始化 Evelyn 及其纹理
    virtual bool init() override;

    // 播放动画（此处为空实现，因为她是静态 NPC）
    virtual void playAnimation() override;

    // 获取 Evelyn 的对话内容
    virtual std::vector<std::string> getConversation(bool isFirstMet) override;

    // 检查 Evelyn 对礼物的喜好
    virtual int checkGiftTaste(ItemType itemTag) override;

private:
    // 设置静态纹理帧
    void setupStaticFrame();
};

#endif // __EVELYN_NPC_H__