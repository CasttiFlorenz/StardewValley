#ifndef __SOCIAL_LEVEL_H__
#define __SOCIAL_LEVEL_H__

#include "cocos2d.h"
#include "NPCManager.h" 
#include "NPCBase.h"

class SocialLevel {
public:
    static cocos2d::Node* createSocialInterface(cocos2d::Node* parent, cocos2d::Sprite* background);

private:
    static cocos2d::Node* createCharacterItem(NPCBase* npc, const cocos2d::Vec2& position);

    static cocos2d::Node* createFriendshipHearts(int level);

    static void getNPCStyle(const std::string& name, std::string& outIconPath, float& outScale);

    static const char* HEART_FILLED;
    static const char* HEART_EMPTY;
};

#endif