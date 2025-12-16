/****************************************************************
 * Project Name:  StardewValley
 * File Name:     CreateScene.cpp
 * File Function: CreateScene类的实现
 * Author:        郭芷烟
 * Update Date:   2025/12/11
 * License:       MIT License
 ****************************************************************/
#include "CreateScene.h"

USING_NS_CC;

Scene* CreateScene::createScene()
{
    auto scene = Scene::create();
    auto layer = CreateScene::create();

    scene->addChild(layer);

    return scene;
}

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in CreateScene.cpp\n");
}

bool CreateScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 创建各种UI元素
    auto background0 = Sprite::create("/CreateScene/Background-0.jpg");             // 背景-0
    auto background1 = Sprite::create("/CreateScene/Background-1.png");             // 背景-1
    auto playerBackground = Sprite::create("/CreateScene/PlayerBackground.png");    // 玩家背景预览
    auto player = Sprite::create("/CreateScene/Player.png");                        // 玩家预览图
    auto textBox = Sprite::create("/CreateScene/textBox.png");                      // 文本框背景
    auto textField = ui::TextField::create("Enter Name", "/fonts/pixel.ttf", 24);   // 输入文本框
    auto okButton = HoverButton::create(                                            // 确认按钮
        "/CreateScene/OK.png",
        "/CreateScene/OKHover.png",
        "/CreateScene/OKHover.png");
    auto backButton = HoverButton::create(                                          // 返回按钮
        "/CreateScene/BackDefaultButton.png",
        "/CreateScene/BackHoverButton.png",
        "/CreateScene/BackHoverButton.png");

    // 设置位置
    background0->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    background1->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    playerBackground->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 9 * 5 + origin.y));
    player->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 9 * 5 + origin.y));
    textBox->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y - textBox->getContentSize().height * 2));
    okButton->setPosition(Vec2(textBox->getPosition().x + textBox->getContentSize().width / 2 + okButton->getContentSize().width, textBox->getPosition().y));
    backButton->setPosition(Vec2(origin.x + visibleSize.width - backButton->getContentSize().width / 2, origin.y + backButton->getContentSize().height));
    textField->setPosition(textBox->getPosition());

    // 为背景-0设置缩放比例
    const float scale = MAX(visibleSize.width / background0->getContentSize().width, visibleSize.height / background0->getContentSize().height);
    background0->setScale(scale);

    // 为按钮注册点击事件
    okButton->addTouchEventListener([](Ref* sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::BEGAN) {
            auto scene = GameScene::createScene();

            // 切换到游戏场景
            cocos2d::Director::getInstance()->replaceScene(scene);
        }
        });

    backButton->addTouchEventListener([](Ref* sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::BEGAN) {
            auto initial = InitialMenuScene::createScene();
            Director::getInstance()->replaceScene(initial);
        }
        });

    // 设置文本框属性
    textField->setPlaceHolderColor(Color4B::GRAY);
    textField->setTextColor(Color4B::BLACK);
    textField->setMaxLengthEnabled(true);
    textField->setMaxLength(10);
    textField->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {
        std::cout << "editing a TextField" << std::endl;
        });

    // 添加到场景
    this->addChild(background0, 0);
    this->addChild(background1, 0);
    this->addChild(playerBackground, 0);
    this->addChild(player, 0);
    this->addChild(textBox, 0);
    this->addChild(okButton, 1);
    this->addChild(backButton, 1);
    this->addChild(textField, 2);

    textField->attachWithIME();

    return true;
}