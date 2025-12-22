#include "FishGameScene.h"

USING_NS_CC;

Scene* FishGameScene::createScene()
{
    return FishGameScene::create();
}

bool FishGameScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();

    // 创建蓝色背景
    auto background = LayerColor::create(Color4B(100, 149, 237, 255));
    this->addChild(background);

    // 创建FishGameLayer并添加到场景
    auto fishGameLayer = FishGameLayer::create();
    if (fishGameLayer)
    {
        this->addChild(fishGameLayer);
    }

    return true;
}