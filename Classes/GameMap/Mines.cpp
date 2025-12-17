/****************************************************************
 * Project Name:  StardewValley
 * File Name:     Barn.cpp
 * File Function: Barn类的实现
 * Author:        郭芷烟
 * Update Date:   2025/12/14
 * License:       MIT License
 ****************************************************************/

#include "Mines.h"

GameMap* Mines::_instance = nullptr;

GameMap* Mines::getInstance() {
    if (!_instance) {
        _instance = Mines::create();
        CC_SAFE_RETAIN(_instance);
    }
    return _instance;
}

void Mines::destroyInstance() {
    CC_SAFE_RELEASE_NULL(_instance);
}

bool Mines::init()
{
    if (!Scene::init()) return false;
    _mapName = "Mines";

    _map = TMXTiledMap::create("/TiledMap/Mines/Mines.tmx");
    if (!_map) return false;
    auto eventLayer = _map->getLayer("event");
    if (eventLayer) eventLayer->setVisible(false);

    _minesItemManager = MinesItemManager::create(this);
    CC_SAFE_RETAIN(_minesItemManager);

    this->addChild(_map);
    this->scheduleUpdate();

    

    return true;
}

std::string Mines::getNewMap(const Vec2& curPos, bool isStart, const Direction& direction)
{
    if (direction == Direction::DOWN) {
        const Rect goToFarmRect = getObjectRect("goToFarm");
        if (goToFarmRect.containsPoint(curPos)) {
            return "Farm";
        }
    }

    return "";
}

void Mines::setStartPosition(std::string lastMap)
{
    const Vec2 visibleSize = Director::getInstance()->getVisibleSize();
    _map->setScale(TILED_MAP_SCALE);
    _map->setPosition((visibleSize - _map->getContentSize() * _map->getScale()) / 2);
}

void Mines::update(float dt)
{
}

Vec2 Mines::getPlayerStartPosition(std::string lastMap)
{
    const Rect goToFarmRect = getObjectRect("goToFarm");
    if (!goToFarmRect.equals(Rect::ZERO)) 
        return Vec2(goToFarmRect.getMidX(), goToFarmRect.getMidY());
    return Vec2(100, 100);
}


MouseEvent Mines::onLeftClick(const Vec2& playerPos, const Direction direction, Objects objects)
{

    if (objects == Objects::PICKAXE) {
        // 1. 计算基准瓦片坐标
        Vec2 basePos = this->calMapPos(playerPos);

        // 2. 根据朝向调整基准坐标
        switch (direction) {
        case Direction::DOWN:  basePos.y++; break;
        case Direction::UP:    basePos.y--; break;
        case Direction::LEFT:  basePos.x--; break;
        case Direction::RIGHT: basePos.x++; break;
        default: break;
        }

        // 3. 定义检测偏移量顺序：原位置(0)，上方(-1)，下方(+1)
        // 对应原代码逻辑：tiledPos -> tiledPos.y-- -> tiledPos.y+=2
        const int yOffsets[] = { 1,0, -1 };

        // 4. 遍历检测
        for (int offset : yOffsets) {
            Vec2 checkPos = basePos;
            checkPos.y += offset;

            EnvironmentItem* item = _minesItemManager->getItem(checkPos);
            if (item) {
                auto type = item->getType();
                if (type == EnvironmentItemType::STONE) {
                    _minesItemManager->removeItem(checkPos);
                    return MouseEvent::GET_STONE;
                }
                else if (type == EnvironmentItemType::COPPER) {
                    _minesItemManager->removeItem(checkPos);
                    return MouseEvent::GET_COPPER;
                }
            }
        }
    }

    return MouseEvent::USE_TOOL;
}

MouseEvent Mines::onRightClick(const Vec2& playerPos, const Direction direction)
{
    return MouseEvent::NONE;
}

bool Mines::isCollidable(Vec2 worldPos)
{
    if (!_map) return false;

    const Vec2 tilePos = calMapPos(worldPos);
    const int x = static_cast<int>(tilePos.x);
    const int y = static_cast<int>(tilePos.y);

    const Size mapSize = _map->getMapSize();

    if (x < 0 || x >= mapSize.width || y < 0 || y >= mapSize.height) {
        return true;
    }

    if (_minesItemManager && _minesItemManager->hasItem(tilePos))
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


