/****************************************************************
 * Project Name:  StardewValley
 * File Name:     SkillLevel.h
 * File Function: SkillLevel类的实现
 * Author:        于恩熙
 * Update Date:   2025/12/21
 * License:       MIT License
 ****************************************************************/

#ifndef SKILLLEVEL_H
#define SKILLLEVEL_H

#include "cocos2d.h"
#include "Constant.h"

USING_NS_CC;

// 技能数据结构
struct SkillData {
    std::string name;
    int level;
    float scale;
    std::string iconPath;
};


class SkillLevel {

public:
    // 单例方法
    static SkillLevel* getInstance();
    static void destroyInstance();

    // 创建技能界面
    static cocos2d::Node* createSkillsInterface(cocos2d::Node* parent, cocos2d::Sprite* background);

    // 增加技能水平（在原有等级上增加）
    static bool increaseSkillLevel(SkillType skillType, int amount = 1);

    // 减少技能水平（在原有等级上减少）
    static bool decreaseSkillLevel(SkillType skillType, int amount = 1);

    // 获取技能等级
    static int getSkillLevel(SkillType skillType);

    static SkillData* getSkillData() { return s_skillData; };


private:

    static SkillLevel* _instance;

    // 初始化技能数据
    static void initSkillData();

    // 全局技能数据数组
    static SkillData s_skillData[SKILL_COUNT];

    // 创建单个技能项
    static cocos2d::Node* createSkillItem(SkillType skillType, const cocos2d::Vec2& position);

    // 创建等级星星显示
    static cocos2d::Node* createLevelStars(int level);
};


#endif // SKILLLEVEL_H
