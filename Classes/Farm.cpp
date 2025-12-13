#include "Farm.h"
/****************************************************************
 * Project Name:  StardewValley
 * File Name:     Farm.cpp
 * File Function: Farm类的实现
 * Author:        郭芷烟
 * Update Date:   2025/12/13
 * License:       MIT License
 ****************************************************************/

GameMap* Farm::createScene()
{
    return Farm::create();
}

bool Farm::init()
{
    if (!Scene::init())
    {
        return false;
    }

    _mapName = "Farm";

    _map = TMXTiledMap::create("/TiledMap/Farm/Farm.tmx");

    if (_map == nullptr)
    {
        CCLOG("Failed to load map: TiledMap/Farm/Farm.tmx");
        return false;
    }

    auto eventLayer = _map->getLayer("event");
    if (eventLayer) {
        eventLayer->setVisible(false);
    }

    this->addChild(_map);
    this->scheduleUpdate();

    return true;
}

std::string Farm::getNewMap(const Vec2& curPos,  bool isStart, const Direction& direction)
{
    if (direction == Direction::UP) {
        const Rect goToHouse = getObjectRect("goToHouse");
        if (goToHouse.containsPoint(curPos)) {
            return "FarmHouse";
        }
    }

    return "";
}

void Farm::setStartPosition(std::string lastMap)
{
    const Vec2 visibleSize = Director::getInstance()->getVisibleSize();
    _map->setScale(TILED_MAP_SCALE);

    _map->setPosition(Vec2::ZERO);
}

void Farm::update(float dt)
{
}

Vec2 Farm::getPlayerStartPosition(std::string lastMap)
{
    if (lastMap == "FarmHouse") {
        const Rect startRect = this->getObjectRect("goToHouse");
        if (!startRect.equals(Rect::ZERO)) {

            return Vec2(startRect.getMidX(), startRect.getMidY());
        }
    }

    return Vec2(100, 100);
}