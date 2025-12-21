/****************************************************************
 * Project Name:  StardewValley
 * File Name:     SocialLevel.h
 * File Function: SocialLevel类的实现
 * Author:        于恩熙
 * Update Date:   2025/12/21
 * License:       MIT License
 ****************************************************************/

#ifndef __SOCIAL_LEVEL_H__
#define __SOCIAL_LEVEL_H__

#include "cocos2d.h"
#include "Constant.h"

class SocialLevel
{ 
private:
    // 人物数据结构
    struct CharacterData
    {
        std::string name;           // 人物名字
        int friendshipLevel;        // 好感度等级 
        std::string iconPath;       // 头像图片路径
        float scale;                // 图片缩放比例
    };

    // 常量
    static const int MAX_FRIENDSHIP_LEVEL = 1500;  // 最大好感度等级
    static const int CHARACTER_COUNT = 3;  // 固定3个人物

    // 爱心图片路径
    static const char* HEART_FILLED;   // 红心（已得）
    static const char* HEART_EMPTY;    // 空心（未得）

    // 静态成员
    static CharacterData s_characterData[CHARACTER_COUNT];
    static bool s_isInitialized;

    // 创建好感度爱心显示
    static cocos2d::Node* createFriendshipHearts(int level);

    // 创建单个社交人物项
    static cocos2d::Node* createCharacterItem(CharacterType characterType, const cocos2d::Vec2& position);

public:
    // 初始化人物数据
    static void initCharacterData();

    // 设置好感度等级
    static void setFriendshipLevel(CharacterType character, int level);

    // 增加好感度
    static bool increaseFriendshipLevel(CharacterType character, int amount = 150);

    // 减少好感度
    static bool decreaseFriendshipLevel(CharacterType character, int amount = 150);

    // 获取好感度等级
    static int getFriendshipLevel(CharacterType character);

    // 创建社交界面
    static cocos2d::Node* createSocialInterface(cocos2d::Node* parent, cocos2d::Sprite* background);
};

#endif // __SOCIAL_LEVEL_H__#pragma once
