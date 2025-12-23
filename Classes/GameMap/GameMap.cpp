#include "GameMap.h"

bool GameMap::init()
{
    // 初始化 Scene 基类
    if (!Scene::init())
    {
        return false;
    }

    // 基类中不加载具体地图，由子类负责
    _map = nullptr;
    _mapName = MapType::NONE;

    return true;
}

 // 世界坐标 -> 瓦片坐标
Vec2 GameMap::calMapPos(Vec2 worldPos)
{
    // 转换到 TMXTiledMap 的节点坐标系
    const Vec2 nodePos = _map->convertToNodeSpace(worldPos);

    // 单个瓦片的像素大小
    const int tileSize = _map->getContentSize().width / _map->getMapSize().width;

    // Tiled 的 y 轴方向与世界坐标相反，需要反转
    const int x = nodePos.x / tileSize;
    const int y = _map->getMapSize().height - nodePos.y / tileSize;

    return Vec2(x, y);
}

// 瓦片坐标 -> 世界坐标（瓦片中心点）
Vec2 GameMap::calWorldPos(const Vec2& tileCoord)
{
    if (!_map) return Vec2::ZERO;

    const int tileSize = _map->getContentSize().width / _map->getMapSize().width;
    const Size mapSize = _map->getMapSize();

    // TiledMap 中瓦片索引原点在左上角
    float x = tileCoord.x * tileSize + tileSize / 2;
    float y = (mapSize.height - tileCoord.y - 1) * tileSize + tileSize / 2;

    return Vec2(x, y);
}

Rect GameMap::getObjectRect(const std::string& objectName)
{
    if (!_map) return Rect::ZERO;

    // 遍历所有对象层
    auto objectGroups = _map->getObjectGroups();
    for (auto& group : objectGroups)
    {
        auto object = group->getObject(objectName);
        if (!object.empty())
        {
            // 读取对象在 TMX 中的数据
            const float x = object.at("x").asFloat();
            const float y = object.at("y").asFloat();
            const float width = object.at("width").asFloat();
            const float height = object.at("height").asFloat();

            Rect nodeRect(x, y, width, height);

            // 转换为世界坐标
            Vec2 worldOrigin = _map->convertToWorldSpace(nodeRect.origin);

            // 考虑地图缩放
            const float scaleX = _map->getScaleX();
            const float scaleY = _map->getScaleY();
            Size worldSize(nodeRect.size.width * scaleX,
                nodeRect.size.height * scaleY);

            return Rect(worldOrigin, worldSize);
        }
    }

    return Rect::ZERO;
}

 // 判断世界坐标是否发生碰撞
bool GameMap::isCollidable(Vec2 worldPos)
{
    if (!_map) return false;

    // 转换为瓦片坐标
    const Vec2 tilePos = calMapPos(worldPos);
    const int x = static_cast<int>(tilePos.x);
    const int y = static_cast<int>(tilePos.y);

    const Size mapSize = _map->getMapSize();

    // 地图边界外视为不可通行
    if (x < 0 || x >= mapSize.width || y < 0 || y >= mapSize.height) {
        return true;
    }

    // 获取事件层（通常用于碰撞/触发）
    auto layer = _map->getLayer("event");
    if (!layer) return false;

    // 检查该瓦片是否有 Collidable 属性
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

 // 根据玩家朝向，对瓦片坐标进行偏移（前方一格）
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

// 判断 TMX 属性值是否为 true（兼容多种类型）
bool GameMap::IsTrueProperty(const Value& v)
{
    switch (v.getType()) {
    case Value::Type::BOOLEAN:
        return v.asBool();
    case Value::Type::INTEGER:
        return v.asInt() == 1;
    case Value::Type::STRING: {
        auto s = v.asString();
        return s == "true" || s == "1";
    }
    default:
        return false;
    }
}
