/****************************************************************
 * Project Name:  StardewValley
 * File Name:     FarmHouse.cpp
 * File Function: FarmHouse类的实现
 * Author:        郭芷烟
 * Update Date:   2025/12/11
 * License:       MIT License
 ****************************************************************/
#include "FarmHouse.h"

GameMap* FarmHouse::createScene()
{
    return FarmHouse::create();
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
