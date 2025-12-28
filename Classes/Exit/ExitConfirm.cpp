/****************************************************************
 * Project Name:  StardewValley
 * File Name:     ExitComfirm.cpp
 * File Function: ExitComfirm类的实现
 * Author:        于恩熙
 * Update Date:   2025/12/28
 * License:       MIT License
 ****************************************************************/

#include "ExitConfirm.h"
#include "../Scene/InitialMenuScene.h"
#include "cocos2d.h"

// 引入需要销毁的管理器头文件
#include "../MapItem/CultivationManager.h"
#include "../MapItem/BarnManager.h"
#include "../MapItem/FarmItemManager.h"
#include "../MapItem/MinesItemManager.h"
#include "../Player/Player.h"
#include "../GameMap/Farm.h"
#include "../GameMap/Town.h"
#include "../GameMap/FarmHouse.h"
#include "../GameMap/Barn.h"
#include "../GameMap/Mines.h"
#include "../Inventory/InventoryScene.h"
#include "../Inventory/PlaceItems.h"
#include "../NPC/NPCManager.h"
#include "../Time/TimeManager.h"
#include "../Weather/WeatherManager.h"
#include "../Money/Money.h"
#include "../Music/MusicManager.h"

USING_NS_CC;

// 静态创建
ExitConfirm* ExitConfirm::create(Node* parent, Sprite* background)
{
    if (!parent || !background) {
        return nullptr;
    }

    ExitConfirm* confirm = ExitConfirm::create();
    if (confirm) {
        Vec2 bgPos = background->getPosition();
        confirm->setPosition(bgPos);
        parent->addChild(confirm, 100);
    }
    return confirm;
}

// 初始化
bool ExitConfirm::init()
{
    if (!Node::init()) {
        return false;
    }

    // 1. 创建"返回标题"按钮
    auto returnToMenuBtn = Sprite::create("/Items/button/exit to title.png");
    if (returnToMenuBtn) {
        returnToMenuBtn->setPosition(Vec2(0, 60));
        returnToMenuBtn->setScale(1.3f);

        auto listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        listener->onTouchBegan = [=](Touch* touch, Event* event) {
            Vec2 localPos = returnToMenuBtn->convertTouchToNodeSpace(touch);
            Size btnSize = returnToMenuBtn->getContentSize();

            // 扩大点击区域
            Rect btnRect = Rect(-btnSize.width / 2 * 1.3f, -btnSize.height / 2 * 1.3f,
                btnSize.width * 1.3f, btnSize.height * 1.3f);

            if (btnRect.containsPoint(localPos)) {
                MusicManager::getInstance()->playButtonClick();
                this->onReturnToTitle();
                return true;
            }
            return false;
            };

        Director::getInstance()->getEventDispatcher()
            ->addEventListenerWithSceneGraphPriority(listener, returnToMenuBtn);

        this->addChild(returnToMenuBtn);
    }

    // 2. 创建"退出游戏"按钮
    auto exitGameBtn = Sprite::create("/Items/button/exit to desktop.png");
    if (exitGameBtn) {
        exitGameBtn->setPosition(Vec2(0, -60));
        exitGameBtn->setScale(1.3f);

        auto listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        listener->onTouchBegan = [=](Touch* touch, Event* event) {
            const Vec2 localPos = exitGameBtn->convertTouchToNodeSpace(touch);
            const Size btnSize = exitGameBtn->getContentSize();

            const Rect btnRect = Rect(-btnSize.width / 2 * 1.3f, -btnSize.height / 2 * 1.3f,
                btnSize.width * 1.3f, btnSize.height * 1.3f);

            if (btnRect.containsPoint(localPos)) {
                MusicManager::getInstance()->playButtonClick();
                this->onExitGame();
                return true;
            }
            return false;
            };

        Director::getInstance()->getEventDispatcher()
            ->addEventListenerWithSceneGraphPriority(listener, exitGameBtn);

        this->addChild(exitGameBtn);
    }

    return true;
}

// 返回标题逻辑
void ExitConfirm::onReturnToTitle()
{
    // 销毁所有单例
    CultivationManager::destroyInstance();
    BarnManager::destroyInstance();
    FarmItemManager::destroyInstance();
    MinesItemManager::destroyInstance();

    Player::destroyInstance();
    Farm::destroyInstance();
    Town::destroyInstance();
    FarmHouse::destroyInstance();
    Barn::destroyInstance();
    Mines::destroyInstance();

    InventoryScene::destroyInstance();
    InventoryManager::resetStaticVariables();
    PlaceItems::destroyInstance();
    NPCManager::destroyInstance();
    TimeManager::destroyInstance();
    WeatherManager::destroyInstance();
    Money::destroyInstance();
    MusicManager::destroyInstance();

    // 切换场景
    auto initScene = InitialMenuScene::createScene();
    auto runningScene = Director::getInstance()->getRunningScene();

    if (runningScene) {
        runningScene->removeFromParent();
    }
    Director::getInstance()->replaceScene(initScene);
}

// 退出游戏逻辑
void ExitConfirm::onExitGame()
{
    // 销毁所有单例
    CultivationManager::destroyInstance();
    BarnManager::destroyInstance();
    FarmItemManager::destroyInstance();
    MinesItemManager::destroyInstance();

    Player::destroyInstance();
    Farm::destroyInstance();
    Town::destroyInstance();
    FarmHouse::destroyInstance();
    Barn::destroyInstance();
    Mines::destroyInstance();

    InventoryScene::destroyInstance();
    InventoryManager::resetStaticVariables();
    PlaceItems::destroyInstance();
    NPCManager::destroyInstance();
    TimeManager::destroyInstance();
    WeatherManager::destroyInstance();
    Money::destroyInstance();
    MusicManager::destroyInstance();

    // 结束程序
    Director::getInstance()->end();

}

