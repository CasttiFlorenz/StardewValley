/****************************************************************
 * Project Name:  StardewValley
 * File Name:     InventoryManager.h
 * File Function: InventoryManager类的实现
 * Author:        于恩熙
 * Update Date:   2025/12/21
 * License:       MIT License
 ****************************************************************/

#ifndef INVENTORYMANAGER_H
#define INVENTORYMANAGER_H

#include "cocos2d.h"
#include "Constant.h"
#include "../Player/SkillLevel.h"
#include "../NPC/SocialLevel.h"
#include "../Exit/ExitConfirm.h"
#include <array>
#include <functional>

class InventoryManager
{
public:
    // 背包精灵结构
    struct BackpackInfo {
        cocos2d::Sprite* sprite;
        cocos2d::Vec2 position;
        float scale;
    };

    // 创建整个界面（包括背景、按钮、背包）
    static void createInterface(
        cocos2d::Node* parent,
        std::array<BackpackInfo, 3>& backpacks,
        std::array<cocos2d::Sprite*, 4>& buttons,
        cocos2d::Sprite*& background,
        float& backpackWidth,
        float& backpackHeight,
        float& cellWidth
    );

    // 切换界面
    static void switchToInventory();
    static void switchToSkills();
    static void switchToSocial();
    static void switchToExit();

    // 获取当前状态
    static int getCurrentButtonIndex();

    typedef std::function<void(int)> ButtonCallback;   // 添加回调函数类型
    // 设置回调
    static void setButtonCallback(ButtonCallback callback);

    //设置打开呈现背包界面
    static void InventoryManager::setInitPress();   

private:
    static int s_currentPressedIndex ;

    // 创建背包
    static void createBackpacks(cocos2d::Node* parent, std::array<BackpackInfo, 3>& backpacks);

    // 创建背景
    static cocos2d::Sprite* createBackground(cocos2d::Node* parent);

    // 创建按钮
    static void createButtons(cocos2d::Node* parent, cocos2d::Sprite* background,
        std::array<cocos2d::Sprite*, 4>& buttons,
        float backpackWidth);

    // 按钮触摸回调
    static bool onButtonTouch(cocos2d::Touch* touch, cocos2d::Event* event);

    static void removeContainer();

    // 静态变量
    static cocos2d::Node* s_parent;
    static cocos2d::Sprite* s_background;
    static std::array<BackpackInfo, 3> s_backpacks;
    static std::array<cocos2d::Sprite*, 4> s_buttons;
    static std::array<cocos2d::Vec2, 4> s_buttonPositions;
    static cocos2d::RefPtr<cocos2d::Node> s_container;
    static float s_backpackWidth;
    static float s_backpackHeight;
    static ButtonCallback s_buttonCallback;
   

};

#endif // INVENTORYMANAGER_H