/****************************************************************
 * InitialMenuScene.cpp
 * 初始菜单场景的实现，包含开始、读档和退出按钮
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

// 资源加载失败时的简单提示
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Check resource path in InitialMenuScene.cpp\n");
}

bool InitialMenuScene::init()
{
    if (!Scene::init()) {
        return false;
    }

    // 获取屏幕大小与原点
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 播放菜单背景音乐
    MusicManager::getInstance()->playMusicForMap("FarmHouse");

    // ===== 背景 =====
    auto backGround = Sprite::create("/InitialMenuScene/InitialMenu_bg.png");
    if (!backGround) {
        problemLoading("'InitialMenu_bg.png'");
    }
    else {
        backGround->setPosition(Vec2(
            origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height / 2
        ));

        // 按比例缩放填满屏幕
        float scale = MAX(
            visibleSize.width / backGround->getContentSize().width,
            visibleSize.height / backGround->getContentSize().height
        );
        backGround->setScale(scale);
        this->addChild(backGround, 0);
    }

    // ===== 标题 =====
    auto title = Sprite::create("/InitialMenuScene/Title.png");
    if (!title) {
        problemLoading("'Title.png'");
    }
    else {
        title->setPosition(Vec2(
            origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height * 2 / 3
        ));
        this->addChild(title, 1);
    }

    // ===== 按钮 =====
    auto exitButton = HoverButton::create(
        "/InitialMenuScene/ExitDefaultButton.png",
        "/InitialMenuScene/ExitHoverButton.png",
        "/InitialMenuScene/ExitHoverButton.png"
    );

    auto createButton = HoverButton::create(
        "/InitialMenuScene/CreateDefualtButton.png",
        "/InitialMenuScene/CreateHoverButton.png",
        "/InitialMenuScene/CreateHoverButton.png"
    );

    auto loadButton = HoverButton::create(
        "/InitialMenuScene/LoadDefaultButton.png",
        "/InitialMenuScene/LoadHoverButton.png",
        "/InitialMenuScene/LoadHoverButton.png"
    );

    // 设置按钮位置
    exitButton->setPosition(Vec2(
        origin.x + visibleSize.width * 3 / 4 - exitButton->getContentSize().width / 2,
        origin.y + visibleSize.height / 6 + exitButton->getContentSize().height / 2
    ));

    createButton->setPosition(Vec2(
        origin.x + visibleSize.width / 4 + createButton->getContentSize().width / 2,
        origin.y + visibleSize.height / 6 + createButton->getContentSize().height / 2
    ));

    loadButton->setPosition(Vec2(
        origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height / 6 + loadButton->getContentSize().height / 2
    ));

    // ===== 退出按钮 =====
    exitButton->addTouchEventListener([&](Ref*, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::BEGAN) {
            if(auto music=MusicManager::getInstance())
                music->playButtonClick();
            Director::getInstance()->end();
        }
        });
    this->addChild(exitButton, 1);

    // ===== 创建角色按钮 =====
    createButton->addTouchEventListener([&](Ref*, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::BEGAN) {
            if (auto music = MusicManager::getInstance())
                music->playButtonClick();
            Director::getInstance()->replaceScene(CreateScene::createScene());
        }
        });
    this->addChild(createButton, 1);

    // ===== 读档按钮 =====
    loadButton->addTouchEventListener([&](Ref*, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::BEGAN) {
            if (auto music = MusicManager::getInstance())
                music->playButtonClick();
            if (auto game = GameScene::createScene()) {
                if (auto save = SaveManage::getInstance()) {
                    save->loadAllData();
                }
                Director::getInstance()->replaceScene(game);
            }
        }
        });
    this->addChild(loadButton, 1);

    return true;
}
