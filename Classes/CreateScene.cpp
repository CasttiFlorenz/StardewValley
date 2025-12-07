/****************************************************************
 * Project Name:  StardewValley
 * File Name:     CreateScene.cpp
 * File Function: CreateScene类的实现
 * Author:        郭芷烟
 * Update Date:   2025/12/07
 * License:       MIT License
 ****************************************************************/

#include"CreateScene.h"

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

    auto background0 = Sprite::create("/CreateScene/Background-0.jpg");             // 创建背景-0
    auto background1 = Sprite::create("/CreateScene/Background-1.png");             // 创建背景-1
    auto playerBackground = Sprite::create("/CreateScene/PlayerBackground.png");    // 创建人物预览图背景
    auto player = Sprite::create("/CreateScene/Player.png");                        // 创建人物预览图
    auto textBox = Sprite::create("/CreateScene/textBox.png");                      // 创建文本框背景
    auto textField = ui::TextField::create("Enter Name", "/fonts/pixel.ttf", 24);   // 创建姓名输入文本框
    auto okButton = HoverButton::create(                                            // 创建确认按钮
        "/CreateScene/OK.png",
        "/CreateScene/OKHover.png",
        "/CreateScene/OKHover.png");
    auto backButton = HoverButton::create(                                          // 创建回退按钮
        "/CreateScene/BackDefaultButton.png",
        "/CreateScene/BackHoverButton.png",
        "/CreateScene/BackHoverButton.png");

    background0->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    background1->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    playerBackground->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 9 * 5 + origin.y));
    player->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 9 * 5 + origin.y));
    textBox->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y - textBox->getContentSize().height * 2));
    okButton->setPosition(Vec2(textBox->getPosition().x + textBox->getContentSize().width / 2 + okButton->getContentSize().width, textBox->getPosition().y));
    backButton->setPosition(Vec2(origin.x + visibleSize.width - backButton->getContentSize().width / 2, origin.y + backButton->getContentSize().height));
    textField->setPosition(textBox->getPosition());

    // 为背景-1设置比例
    const float scale = MAX(visibleSize.width / background0->getContentSize().width, visibleSize.height / background0->getContentSize().height);
    background0->setScale(scale);

    // 为按钮注册监听器
    okButton->addTouchEventListener([](Ref* sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::BEGAN) {
        }
        });

    backButton->addTouchEventListener([](Ref* sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::BEGAN) {
            auto initial = InitialMenuScene::createScene();
            Director::getInstance()->replaceScene(initial);
        }
        });

    // 设置文本框参数
    textField->setPlaceHolderColor(Color4B::GRAY);
    textField->setTextColor(Color4B::BLACK);
    textField->setMaxLengthEnabled(true);
    textField->setMaxLength(10);
    textField->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {
        std::cout << "editing a TextField" << std::endl;
        });

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
