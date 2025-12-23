// Haley.h
#ifndef __HALEY_NPC_H__
#define __HALEY_NPC_H__

#include "NPCBase.h"

USING_NS_CC;

class Haley : public NPCBase
{
public:
    CREATE_FUNC(Haley);
    virtual bool init() override;
    virtual void playAnimation() override;
    virtual std::vector<std::string> Haley::getConversation(bool isFirstMet)override;
    virtual int checkGiftTaste(ItemType itemTag) override;
private:
    void createAndPlayAnimation();
};

#endif // __HALEY_NPC_H__