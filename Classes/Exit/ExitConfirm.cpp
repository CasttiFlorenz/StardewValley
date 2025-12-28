#include "ExitConfirm.h"
#include "../Scene/InitialMenuScene.h"
#include "cocos2d.h"

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

bool ExitConfirm::init()
{
    if (!Node::init()) {
        return false;
    }

    auto returnToMenuBtn = Sprite::create("/Items/button/exit to title.png");
    if (returnToMenuBtn) {
        returnToMenuBtn->setPosition(Vec2(0, 60));
        returnToMenuBtn->setScale(1.3f);

        auto listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        listener->onTouchBegan = [=](Touch* touch, Event* event) {
            Vec2 localPos = returnToMenuBtn->convertTouchToNodeSpace(touch);
            Size btnSize = returnToMenuBtn->getContentSize();

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

    auto exitGameBtn = Sprite::create("/Items/button/exit to desktop.png");
    if (exitGameBtn) {
        exitGameBtn->setPosition(Vec2(0, -60));
        exitGameBtn->setScale(1.3f);

        auto listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        listener->onTouchBegan = [=](Touch* touch, Event* event) {
            Vec2 localPos = exitGameBtn->convertTouchToNodeSpace(touch);
            Size btnSize = exitGameBtn->getContentSize();

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

void ExitConfirm::onReturnToTitle()
{
    auto init = InitialMenuScene::createScene();
    auto game = Director::getInstance()->getRunningScene();

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
    NPCManager::destroyInstance();
    TimeManager::destroyInstance();
    WeatherManager::destroyInstance();
    MusicManager::destroyInstance();

    game->removeFromParent();
    Director::getInstance()->replaceScene(init);
}

void ExitConfirm::onExitGame()
{
    WeatherManager::destroyInstance();
    CultivationManager::destroyInstance();
    BarnManager::destroyInstance();
    FarmItemManager::destroyInstance();
    MinesItemManager::destroyInstance();
    MusicManager::destroyInstance();

    Player::destroyInstance();
    Farm::destroyInstance();
    Town::destroyInstance();
    FarmHouse::destroyInstance();
    Barn::destroyInstance();
    Mines::destroyInstance();

    Director::getInstance()->end();

}

