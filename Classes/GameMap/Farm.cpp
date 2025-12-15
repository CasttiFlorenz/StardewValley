/****************************************************************
 * Project Name:  StardewValley
 * File Name:     Farm.cpp
 * File Function: Farm类的实现
 * Author:        郭芷烟
 * Update Date:   2025/12/13
 * License:       MIT License
 ****************************************************************/

#include "Farm.h"

GameMap* Farm::_instance = nullptr;

GameMap* Farm::getInstance() {
    if (!_instance) {
        _instance = Farm::create();
        CC_SAFE_RETAIN(_instance);
    }
    return _instance;
}

void Farm::destroyInstance() {
    CC_SAFE_RELEASE_NULL(_instance);
}

Farm::~Farm() {
    CC_SAFE_RELEASE(_farmItemManager);
}

bool Farm::init()
{
    if (!Scene::init())
    {
        return false;
    }

    _mapName = "Farm";

    _map = TMXTiledMap::create("TiledMap/Farm/Farm.tmx");

    if (_map == nullptr)
    {
        CCLOG("Failed to load map: TiledMap/Farm/Farm.tmx");
        return false;
    }

    auto eventLayer = _map->getLayer("event");
    if (eventLayer) {
        eventLayer->setVisible(false);
    }

    _farmItemManager = FarmItemManager::create(this);
    CC_SAFE_RETAIN(_farmItemManager);

    this->addChild(_map,0);
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
        const Rect goToMines = getObjectRect("goToMines");
        if (goToMines.containsPoint(curPos)) {
            return "Mines";
        }
        const Rect goToBarn = getObjectRect("goToBarn");
        if (goToBarn.containsPoint(curPos)) {
            return "Barn";
        }
    }
    if (direction == Direction::RIGHT) {
        const Rect goToTown = getObjectRect("goToTown");
        if (goToTown.containsPoint(curPos)) {
            return "Town";
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
    if (lastMap == "Mines") {
        const Rect goToMinesRect = this->getObjectRect("goToMines");
        if (!goToMinesRect.equals(Rect::ZERO)) {

            return Vec2(goToMinesRect.getMidX(), goToMinesRect.getMidY());
        }
    }
    if (lastMap == "Barn") {
        const Rect goToBarnRect = this->getObjectRect("goToBarn");
        if (!goToBarnRect.equals(Rect::ZERO)) {

            return Vec2(goToBarnRect.getMidX(), goToBarnRect.getMidY());
        }
    }
    if (lastMap == "Town") {
        const Rect goToTownRect = this->getObjectRect("goToTown");
        if (!goToTownRect.equals(Rect::ZERO)) {

            return Vec2(goToTownRect.getMidX(), goToTownRect.getMidY());
        }
    }

    return Vec2(100, 100);
}

bool Farm::isCollidable(Vec2 worldPos)
{
    if (!_map) return false;

    const Vec2 tilePos = calMapPos(worldPos);
    const int x = static_cast<int>(tilePos.x);
    const int y = static_cast<int>(tilePos.y);

    const Size mapSize = _map->getMapSize();

    if (x < 0 || x >= mapSize.width || y < 0 || y >= mapSize.height) {
        return true;
    }

    if (_farmItemManager && _farmItemManager->hasItem(tilePos))
      return true;

    auto layer = _map->getLayer("event");
    if (!layer) return false;

    const int tileGID = layer->getTileGIDAt(tilePos);
    if (tileGID) {
        auto properties = _map->getPropertiesForGID(tileGID).asValueMap();
        if (!properties.empty() &&
            properties.find("Collidable") != properties.end() &&
            properties.at("Collidable").asBool()) {
                return true;
        }
    }


    return false;
}

MouseEvent Farm::onLeftClick(const Vec2& playerPos, const Direction direction)
{
    return MouseEvent::USE_TOOL;
}

MouseEvent Farm::onRightClick(const Vec2& playerPos, const Direction direction)
{
    return MouseEvent::NONE;
}
