/****************************************************************
 * Project Name:  StardewValley
 * File Name:     ExitConfirm.cpp
 * File Function:  ExitConfirm类的实现
 * Author:        于恩熙
 * Update Date:   2025/12/21
 * License:       MIT License
 ****************************************************************/

#include "ExitConfirm.h"
#include "cocos2d.h"

USING_NS_CC;

// 创建退出确认界面
ExitConfirm* ExitConfirm::create(Node* parent, Sprite* background)
{
    if (!parent || !background) {
        return nullptr;
    }

    ExitConfirm* confirm = ExitConfirm::create();
    if (confirm) {
        // 以背景为中心
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

    // ========== 按钮1：返回菜单 ==========
    auto returnToMenuBtn = Sprite::create("/Items/button/exit to title.png");
    if (returnToMenuBtn) {
        returnToMenuBtn->setPosition(Vec2(0, 60));  
        returnToMenuBtn->setScale(1.3f);

        // 触摸检测
        auto listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        listener->onTouchBegan = [=](Touch* touch, Event* event) {
            Vec2 localPos = returnToMenuBtn->convertTouchToNodeSpace(touch);
            Size btnSize = returnToMenuBtn->getContentSize();

            // 考虑缩放
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

    // ========== 按钮2：退出游戏 ==========
    auto exitGameBtn = Sprite::create("/Items/button/exit to desktop.png");
    if (exitGameBtn) {
        exitGameBtn->setPosition(Vec2(0, -60));  
        exitGameBtn->setScale(1.3f);             

        // 触摸检测
        auto listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        listener->onTouchBegan = [=](Touch* touch, Event* event) {
            Vec2 localPos = exitGameBtn->convertTouchToNodeSpace(touch);
            Size btnSize = exitGameBtn->getContentSize();

            // 考虑缩放
            Rect btnRect = Rect(-btnSize.width / 2 * 1.3f, -btnSize.height / 2 * 1.3f,
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

// 返回标题
void ExitConfirm::onReturnToTitle()
{
    // 这里由外部调用时实现
    this->removeFromParent();
}

// 退出游戏
void ExitConfirm::onExitGame()
{
    Director::getInstance()->end();
}