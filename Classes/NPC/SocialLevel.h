/****************************************************************
 * Project Name:  StardewValley
 * File Name:     SocialLevel.cpp
 * File Function: SocialLevel类的实现
 * Author:        于恩熙
 * Update Date:   2025/12/21
 * License:       MIT License
 ****************************************************************/
#ifndef __SOCIAL_LEVEL_H__
#define __SOCIAL_LEVEL_H__

#include "cocos2d.h"
#include "NPCManager.h" 
#include "NPCBase.h"

class SocialLevel {
public:
    // 创建社交界面
    static cocos2d::Node* createSocialInterface(cocos2d::Node* parent, cocos2d::Sprite* background);

private:
    // 创建单个社交人物项
    static cocos2d::Node* createCharacterItem(NPCBase* npc, const cocos2d::Vec2& position);

    // 创建好感度爱心显示
    static cocos2d::Node* createFriendshipHearts(int level);

    // 获取NPC头像数据
    static void getNPCStyle(const std::string& name, std::string& outIconPath, float& outScale);

};

#endif