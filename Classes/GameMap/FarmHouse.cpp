/****************************************************************
 * Project Name:  StardewValley
 * File Name:     FarmHouse.cpp
 * File Function: FarmHouseÊµÏÖ
 * Author:        ¹ùÜÆÑÌ
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

    // ¼ÓÔØ TMX µØÍ¼
    _map = TMXTiledMap::create("TiledMap/FarmHouse/FarmHouse.tmx");

    if (_map == nullptr)
    {
        CCLOG("Failed to load map: TiledMap/FarmHouse/FarmHouse.tmx");
        return false;
    }

    // Òþ²ØÅö×²²ã
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
    
    // ¾ÓÖÐÏÔÊ¾
    _map->setPosition((visibleSize - _map->getContentSize() * _map->getScale()) / 2);
}

void FarmHouse::update(float dt)
{
    // Ã¿Ö¡¸üÐÂÂß¼­
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
    return MouseEvent::NONE;
}