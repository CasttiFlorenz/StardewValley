#include "GameMap.h"

// 初始化
bool GameMap::init()
{
    if (!Scene::init())
    {
        return false;
    }

    _map = nullptr;
    _mapName = MapType::NONE;

    return true;
}

// 世界坐标 -> 瓦片坐标
Vec2 GameMap::calMapPos(Vec2 worldPos)
{
    if (!_map) return Vec2::ZERO;

    // 转换为节点坐标
    const Vec2 nodePos = _map->convertToNodeSpace(worldPos);

    // 计算瓦片大小
    const Size mapSize = _map->getMapSize();
    if (mapSize.width <= 0 || mapSize.height <= 0) return Vec2::ZERO;

    const float tileSize = _map->getContentSize().width / mapSize.width;
    if (tileSize <= 0) return Vec2::ZERO;

    // 转换坐标 (Tiled y轴向下)
    const int x = static_cast<int>(nodePos.x / tileSize);
    const int y = static_cast<int>(mapSize.height - nodePos.y / tileSize);

    return Vec2(static_cast<float>(x), static_cast<float>(y));
}

// 瓦片坐标 -> 世界坐标
Vec2 GameMap::calWorldPos(const Vec2& tileCoord)
{
    if (!_map) return Vec2::ZERO;

    const Size mapSize = _map->getMapSize();
    if (mapSize.width <= 0 || mapSize.height <= 0) return Vec2::ZERO;

    const float tileSize = _map->getContentSize().width / mapSize.width;

    // 计算中心点坐标
    const float x = tileCoord.x * tileSize + tileSize / TILE_SIZE_HALF_DIVISOR;
    const float y = (mapSize.height - tileCoord.y - 1) * tileSize + tileSize / TILE_SIZE_HALF_DIVISOR;

    return Vec2(x, y);
}

// 获取对象矩形
Rect GameMap::getObjectRect(const std::string& objectName)
{
    if (!_map) return Rect::ZERO;

    auto objectGroups = _map->getObjectGroups();
    for (auto& group : objectGroups)
    {
        auto object = group->getObject(objectName);
        if (!object.empty())
        {
            const float x = object.at(OBJECT_PROPERTY_X).asFloat();
            const float y = object.at(OBJECT_PROPERTY_Y).asFloat();
            const float width = object.at(OBJECT_PROPERTY_WIDTH).asFloat();
            const float height = object.at(OBJECT_PROPERTY_HEIGHT).asFloat();

            Rect nodeRect(x, y, width, height);

            // 转换为世界坐标
            Vec2 worldOrigin = _map->convertToWorldSpace(nodeRect.origin);

            // 考虑缩放
            const float scaleX = _map->getScaleX();
            const float scaleY = _map->getScaleY();
            Size worldSize(nodeRect.size.width * scaleX,
                nodeRect.size.height * scaleY);

            return Rect(worldOrigin, worldSize);
        }
    }

    return Rect::ZERO;
}

// 碰撞检测
bool GameMap::isCollidable(Vec2 worldPos)
{
    if (!_map) return false;

    const Vec2 tilePos = calMapPos(worldPos);
    const int x = static_cast<int>(tilePos.x);
    const int y = static_cast<int>(tilePos.y);

    const Size mapSize = _map->getMapSize();

    // 边界检测
    if (x < 0 || x >= mapSize.width || y < 0 || y >= mapSize.height) {
        return true;
    }

    // 事件层检测
    auto layer = _map->getLayer(EVENT_LAYER_NAME);
    if (!layer) return false;

    const int tileGID = layer->getTileGIDAt(tilePos);
    if (tileGID) {
        auto properties = _map->getPropertiesForGID(tileGID).asValueMap();
        if (!properties.empty() &&
            properties.find(COLLIDABLE_PROPERTY_NAME) != properties.end() &&
            properties.at(COLLIDABLE_PROPERTY_NAME).asBool()) {
            return true;
        }
    }

    return false;
}

// 应用方向偏移
void GameMap::ApplyDirectionOffset(Vec2& basePos, Direction direction)
{
    switch (direction) {
    case Direction::DOWN:  basePos.y++; break;
    case Direction::UP:    basePos.y--; break;
    case Direction::LEFT:  basePos.x--; break;
    case Direction::RIGHT: basePos.x++; break;
    default: break;
    }
}

// 属性值真假判断
bool GameMap::IsTrueProperty(const Value& v)
{
    switch (v.getType()) {
    case Value::Type::BOOLEAN:
        return v.asBool();
    case Value::Type::INTEGER:
        return v.asInt() == static_cast<int>(TRUE_INTEGER_VALUE);
    case Value::Type::STRING: {
        auto s = v.asString();
        return s == TRUE_STRING_VALUE_1 || s == TRUE_STRING_VALUE_2;
    }
    default:
        return false;
    }
}