/****************************************************************
 * Project Name:  StardewValley
 * File Name:     FarmHouse.cpp
 * File Function: FarmHouse实现
 * Author:        郭芷烟
 * Update Date:   2025/12/11
 * License:       MIT License
 ****************************************************************/
#include "FarmHouse.h"

GameMap* FarmHouse::_instance = nullptr;

GameMap* FarmHouse::getInstance() {
    if (!_instance) {
        _instance = FarmHouse::create();
        CC_SAFE_RETAIN(_instance);
    }
    return _instance;
}

void FarmHouse::destroyInstance() {
    CC_SAFE_RELEASE_NULL(_instance);
}

bool FarmHouse::init()
{
    if (!Scene::init())
    {
        return false;
    }

    _mapName = "FarmHouse";

    // 加载 TMX 地图
    _map = TMXTiledMap::create("TiledMap/FarmHouse/FarmHouse.tmx");

    if (_map == nullptr)
    {
        CCLOG("Failed to load map: TiledMap/FarmHouse/FarmHouse.tmx");
        return false;
    }

    // 隐藏碰撞层
    auto collisionLayer = _map->getLayer("event");
    if (collisionLayer) {
        collisionLayer->setVisible(false);
    }

    this->addChild(_map);
    this->scheduleUpdate();

    return true;
}

std::string FarmHouse::getNewMap(const Vec2& curPos, bool isStart, const Direction& direction)
{
    if (isStart) {
        return _mapName;
    }

    if (direction == Direction::DOWN) {
        Rect goToFarm = getObjectRect("goToFarm");
        if (goToFarm.containsPoint(curPos)) {
            return "Farm";
        }
    }

    return "";
}

void FarmHouse::setStartPosition(std::string lastMap)
{
    const Vec2 visibleSize = Director::getInstance()->getVisibleSize();
    _map->setScale(TILED_MAP_SCALE);
    
    // 居中显示
    _map->setPosition((visibleSize - _map->getContentSize() * _map->getScale()) / 2);
}

void FarmHouse::update(float dt)
{
    // 每帧更新逻辑
}

Vec2 FarmHouse::getPlayerStartPosition(std::string lastMap)
{
    if (lastMap == _mapName) {
        const Rect startRect = getObjectRect("start");
        return Vec2(startRect.getMidX(), startRect.getMidY());
    }
    else if (lastMap == "Farm") {
        const Rect goToFarmRect = getObjectRect("goToFarm");
        return Vec2(goToFarmRect.getMidX(), goToFarmRect.getMidY());
    }

    return Vec2(-1, -1);
}

MouseEvent FarmHouse::onLeftClick(const Vec2& playerPos, const Direction direction, Objects objects)
{
    return MouseEvent::USE_TOOL;
}

MouseEvent FarmHouse::onRightClick(const Vec2& playerPos, const Direction direction)
{
    auto objectGroup = _map->getObjectGroup("event");

    if (objectGroup) {
        auto& objects = objectGroup->getObjects();

        for (const auto& obj : objects) {
            ValueMap dict = obj.asValueMap();

            std::string name = dict["name"].asString();
            float x = dict["x"].asFloat();
            float y = dict["y"].asFloat();
            float w = dict["width"].asFloat();
            float h = dict["height"].asFloat();

            Rect rect(x, y, w, h);

            if (rect.containsPoint(playerPos)) {
                if (name == "goToBed")
                    return MouseEvent::SLEEP;
            }
        }
    }
        return MouseEvent::NONE;
}
void FarmHouse::sleep() {
    auto runningScene = Director::getInstance()->getRunningScene();
    if (runningScene) {
        if (runningScene->getChildByName("goToBed")) return;

        auto dialog = DialogueLayer::create();
        if (dialog) {
            dialog->setName("goToBed");

            // 使用新的 showChoice 签名
            dialog->showChoice(
                "Are you sure you want to go to sleep?",  // 问题内容

                // 选择回调函数，参数为 bool: true=YES, false=NO
                [this, runningScene](bool choice) {
                    CCLOG("Player choice: %s", choice ? "YES" : "NO");

                    // 移除对话框
                    auto dialog = runningScene->getChildByName("goToBed");
                    if (dialog) {
                        dialog->removeFromParent();
                    }

                    // 根据选择执行不同的逻辑
                    if (choice) {
                        // YES: 执行睡觉逻辑
                        CCLOG("Player chose to sleep");
                        //this->performSleep();
                    }
                    else {
                        // NO: 取消睡觉
                        CCLOG("Player chose not to sleep");
                        // 可以播放取消音效等
                    }
                }
            );

            runningScene->addChild(dialog, 9999);
            dialog->setCameraMask((unsigned short)CameraFlag::DEFAULT);
        }
    }
}