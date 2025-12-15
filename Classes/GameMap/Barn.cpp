/****************************************************************
 * Project Name:  StardewValley
 * File Name:     Barn.cpp
 * File Function: Barn类的实现
 * Author:        郭芷烟
 * Update Date:   2025/12/14
 * License:       MIT License
 ****************************************************************/

#include "Barn.h"

GameMap* Barn::_instance = nullptr;

GameMap* Barn::getInstance() {
    if (!_instance) {
        _instance = Barn::create();
        CC_SAFE_RETAIN(_instance);
    }
    return _instance;
}

void Barn::destroyInstance() {
    CC_SAFE_RELEASE_NULL(_instance);
}

bool Barn::init()
{
    if (!Scene::init()) return false;
    _mapName = "Barn";
    _map = TMXTiledMap::create("/TiledMap/Barn/Barn.tmx");
    if (!_map) return false;
    auto eventLayer = _map->getLayer("event");
    if (eventLayer) eventLayer->setVisible(false);
    this->addChild(_map);
    this->scheduleUpdate();
    return true;
}

std::string Barn::getNewMap(const Vec2& curPos, bool isStart, const Direction& direction)
{
    if (direction == Direction::DOWN) {
        const Rect goToFarmRect = getObjectRect("goToFarm");
        if (goToFarmRect.containsPoint(curPos)) {
            return "Farm";
        }
    }
    return "";
}

void Barn::setStartPosition(std::string lastMap)
{
    const Vec2 visibleSize = Director::getInstance()->getVisibleSize();
    _map->setScale(TILED_MAP_SCALE);
    _map->setPosition((visibleSize - _map->getContentSize() * _map->getScale()) / 2);
}

void Barn::update(float dt)
{
}

Vec2 Barn::getPlayerStartPosition(std::string lastMap)
{
    const Rect goToFarmRect = getObjectRect("goToFarm");
    if (!goToFarmRect.equals(Rect::ZERO))
        return Vec2(goToFarmRect.getMidX(), goToFarmRect.getMidY());
    return Vec2(100, 100);
}


MouseEvent Barn::onLeftClick(const Vec2& playerPos, const Direction direction)
{
    return MouseEvent::USE_TOOL;
}

MouseEvent Barn::onRightClick(const Vec2& playerPos, const Direction direction)
{
    return MouseEvent::NONE;
}