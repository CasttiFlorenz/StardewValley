/****************************************************************
 * Project Name:  StardewValley
 * File Name:     GameMap.cpp
 * File Function: GameMap实现
 * Author:        郭芷烟
 * Update Date:   2025/12/13
 * License:       MIT License
 ****************************************************************/

#include "GameMap.h"

bool GameMap::init()
{
    if (!Scene::init())
    {
        return false;
    }
    _map = nullptr;
    return true;
}

// 检测是否碰撞
bool GameMap::isCollidable(Vec2 worldPos)
{
    if (!_map) return false;

    // 转换为瓦片坐标
    const Vec2 tilePos = calMapPos(worldPos);
    const int x = static_cast<int>(tilePos.x);
    const int y = static_cast<int>(tilePos.y);

    const Size mapSize = _map->getMapSize();

    // 边界检测
    if (x < 0 || x >= mapSize.width || y < 0 || y >= mapSize.height) {
        return true;
    }

    // 获取事件层
    auto layer = _map->getLayer("event");
    if (!layer) return false;

    // 检查瓦片是否有 Collidable 属性
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

// 瓦片坐标转 TiledMap 内部节点坐标（中心点）
Vec2 GameMap::calWorldPos(const Vec2& tileCoord)
{
    if (!_map) return Vec2::ZERO;

    const int tileSize = _map->getContentSize().width / _map->getMapSize().width;
    const Size mapSize = _map->getMapSize();

    // TiledMap 坐标系中，原点在左上角，y 向下增长（对于瓦片索引）
    // OpenGL 坐标系原点在左下角
    float x = tileCoord.x * tileSize + tileSize / 2;
    float y = (mapSize.height - tileCoord.y - 1) * tileSize + tileSize / 2;

    return Vec2(x, y);
}

// 转换世界坐标到瓦片坐标
Vec2 GameMap::calMapPos(Vec2 worldPos)
{
    const Vec2 nodePos = _map->convertToNodeSpace(worldPos);
    const int tileSize = _map->getContentSize().width / _map->getMapSize().width;
    // Y轴反转
    const int y = _map->getMapSize().height - nodePos.y / tileSize;
    const int x = nodePos.x / tileSize;

    return Vec2(x, y);
}

// 获取对象矩形范围
Rect GameMap::getObjectRect(const std::string& objectName)
{
    if (!_map) return Rect::ZERO;

    auto objectGroups = _map->getObjectGroups();
    for (auto& group : objectGroups)
    {
        auto object = group->getObject(objectName);
        if (!object.empty())
        {
            const float x = object.at("x").asFloat();
            const float y = object.at("y").asFloat();
            const float width = object.at("width").asFloat();
            const float height = object.at("height").asFloat();

            const Rect nodeRect(x, y, width, height);

            // 转换为世界坐标
            const Vec2 worldOrigin = _map->convertToWorldSpace(nodeRect.origin);

            const float scaleX = _map->getScaleX();
            const float scaleY = _map->getScaleY();
            const Size worldSize(nodeRect.size.width * scaleX, nodeRect.size.height * scaleY);

            return Rect(worldOrigin, worldSize);
        }
    }

    return Rect::ZERO;
}
