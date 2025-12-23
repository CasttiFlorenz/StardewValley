// Sam.h
#ifndef __SAM_NPC_H__
#define __SAM_NPC_H__

#include "NPCBase.h"

USING_NS_CC;

class Sam : public NPCBase
{
public:
    CREATE_FUNC(Sam);
    virtual bool init() override;
    virtual void playAnimation() override;
    virtual std::vector<std::string> Sam::getConversation(bool isFirstMet)override;
    virtual int checkGiftTaste(ItemType itemTag)override;
private:
    void createAndPlayAnimation();
};

#endif // __SAM_NPC_H__