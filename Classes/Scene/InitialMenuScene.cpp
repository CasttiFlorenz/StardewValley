/****************************************************************
 * Project Name:  StardewValley
 * File Name:     InitialMenuScene.cpp
 * File Function: InitialMenuScene类的实现
 * Author:        郭芷烟
 * Update Date:   2025/12/07
 * License:       MIT License
 ****************************************************************/

#include "InitialMenuScene.h"
#include "../Button/HoverButton.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* InitialMenuScene::createScene()
{
    auto scene = Scene::create();
    auto layer = InitialMenuScene::create();

    scene->addChild(layer);

    return scene;
}

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in InitialMenuScene.cpp\n");
}

bool InitialMenuScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 加载背景
    auto backGround = Sprite::create("/InitialMenuScene/InitialMenu_bg.png");
    if (backGround == nullptr)
    {
        problemLoading("'InitialMenu_bg.png'");
    }
    else
    {
        backGround->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
        float scale = MAX(visibleSize.width / backGround->getContentSize().width, visibleSize.height / backGround->getContentSize().height);
        backGround->setScale(scale);
        this->addChild(backGround, 0);
    }

    // 加载标题
    auto title = Sprite::create("/InitialMenuScene/Title.png");
    if (title == nullptr)problemLoading("'Title.png'");
    else {
        title->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 3 * 2 + origin.y);
        this->addChild(title, 1);
    }

    // 创建三个按钮
    auto exitButton = HoverButton::create("/InitialMenuScene/ExitDefaultButton.png",
        "/InitialMenuScene/ExitHoverButton.png",
        "/InitialMenuScene/ExitHoverButton.png");
    auto createButton = HoverButton::create("/InitialMenuScene/CreateDefualtButton.png",
        "/InitialMenuScene/CreateHoverButton.png",
        "/InitialMenuScene/CreateHoverButton.png");
    auto loadButton = HoverButton::create("/InitialMenuScene/LoadDefaultButton.png",
        "/InitialMenuScene/LoadHoverButton.png",
        "/InitialMenuScene/LoadHoverButton.png");

    // 设置按钮位置并为按钮添加事件监听器

    exitButton->setPosition(Vec2(origin.x + visibleSize.width / 4 * 3 - exitButton->getContentSize().width / 2,
        origin.y + visibleSize.height / 6 + exitButton->getContentSize().height / 2));
    createButton->setPosition(Vec2(origin.x + visibleSize.width / 4 + createButton->getContentSize().width / 2,
        origin.y + visibleSize.height / 6 + createButton->getContentSize().height / 2));
    loadButton->setPosition(Vec2(origin.x + visibleSize.width / 2, 
        origin.y + visibleSize.height / 6 + loadButton->getContentSize().height / 2));

    // 监听到按下按钮事件时退出
    exitButton->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::BEGAN)

            MusicManager::getInstance()->playButtonClick();

            Director::getInstance()->end();
        });
    this->addChild(exitButton, 1);

    // 监听到按下按钮事件时转到创建角色界面
    createButton->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::BEGAN);

        MusicManager::getInstance()->playButtonClick();

        auto create = CreateScene::createScene();
        Director::getInstance()->replaceScene(create);
        });
    this->addChild(createButton, 1);

    loadButton->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::BEGAN);

        MusicManager::getInstance()->playButtonClick();

        // 事件待添加
        });
    this->addChild(loadButton, 1);

    return true;
}

