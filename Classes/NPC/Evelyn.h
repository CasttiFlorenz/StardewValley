// Evelyn.h
#ifndef __EVELYN_NPC_H__
#define __EVELYN_NPC_H__

#include "NPCBase.h"

USING_NS_CC;

class Evelyn : public NPCBase
{
public:
    CREATE_FUNC(Evelyn);
    virtual bool init() override;
    virtual void playAnimation() override {}  // 空实现，不需要动画
    virtual std::vector<std::string> Evelyn::getConversation(bool isFirstMet)override;
    virtual int checkGiftTaste(ItemType itemTag)override;
private:
    void setupStaticFrame();
};

#endif // __EVELYN_NPC_H__