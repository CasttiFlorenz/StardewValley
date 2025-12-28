/****************************************************************
 * Project Name:  StardewValley
 * File Name:     FarmItemManager.cpp
 * File Function: FarmItemManager类的实现
 * Author:        郭芷烟
 * Update Date:   2025/12/16
 * License:       MIT License
 ****************************************************************/
#include "FarmItemManager.h"

FarmItemManager* FarmItemManager::_instance = nullptr;

// 获取单例
FarmItemManager* FarmItemManager::getInstance(GameMap* gameMap)
{
    if (!_instance) {
        if (!gameMap) {
            CCLOG("FarmItemManager::getInstance failed: gameMap is null");
            return nullptr;
        }

        _instance = new (std::nothrow) FarmItemManager();
        if (_instance && _instance->init(gameMap)) {
            CC_SAFE_RETAIN(_instance);
        }
        else {
            CC_SAFE_DELETE(_instance);
        }
    }
    return _instance;
}

// 销毁单例
void FarmItemManager::destroyInstance()
{
    if (_instance) {
        _instance->clear();
        CC_SAFE_RELEASE_NULL(_instance);
    }
}

// 初始化
bool FarmItemManager::init(GameMap* gameMap)
{
    _gameMap = gameMap;
    _tiledMap = _gameMap ? _gameMap->getTiledMap() : nullptr;
    _eventLayer = _tiledMap ? _tiledMap->getLayer(EVENT_LAYER_NAME) : nullptr;

    _items.clear();
    _cultivatedSoils.clear();

    _woodCount = 0;
    _grassCount = 0;
    _daffodilsCount = 0;
    _leekCount = 0;

    if (_gameMap && _tiledMap && _eventLayer) {
        spawnInitialItems();
    }

    return _gameMap != nullptr;
}

// 坐标键值转换
long long FarmItemManager::keyFor(const Vec2& tileCoord)
{
    const long long x = static_cast<long long>(tileCoord.x);
    const long long y = static_cast<long long>(tileCoord.y);
    return (x << 32) | (y & TILE_COORD_MASK);
}

// 移除 TMX 标志
unsigned int FarmItemManager::stripFlags(unsigned int gid)
{
    return gid & ~TMX_FLIP_FLAGS_MASK;
}

// 检查是否为耕地
bool FarmItemManager::isCultivated(const Vec2& tileCoord) const
{
    if (!_eventLayer || !_tiledMap) return false;

    const unsigned int gid = _eventLayer->getTileGIDAt(tileCoord);
    if (gid == INVALID_TILE_GID) return false;

    const unsigned int rawGid = stripFlags(gid);

    auto propsValue = _tiledMap->getPropertiesForGID(rawGid);
    if (propsValue.getType() != Value::Type::MAP) return false;

    auto props = propsValue.asValueMap();
    const auto it = props.find(CULTIVATED_PROPERTY_NAME);
    if (it == props.end()) return false;

    if (it->second.getType() == Value::Type::BOOLEAN)
        return it->second.asBool();
    if (it->second.getType() == Value::Type::INTEGER)
        return it->second.asInt() == TRUE_INTEGER_VALUE;
    if (it->second.getType() == Value::Type::STRING)
        return it->second.asString() == TRUE_STRING_VALUE_1 || it->second.asString() == TRUE_STRING_VALUE_2;

    return false;
}

// 检查是否有物品
bool FarmItemManager::hasItem(const Vec2& tileCoord) const
{
    long long key = keyFor(tileCoord);
    return _items.count(key) || _cultivatedSoils.count(key);
}

// 获取物品
EnvironmentItem* FarmItemManager::getItem(const Vec2& tileCoord) const
{
    const auto it = _items.find(keyFor(tileCoord));
    return it != _items.end() ? it->second : nullptr;
}

// 添加物品
bool FarmItemManager::addItem(EnvironmentItemType type, const Vec2& tileCoord)
{
    if (!isCultivated(tileCoord)) return false;
    if (hasItem(tileCoord)) return false;

    EnvironmentItem* item = nullptr;

    switch (type) {
    case EnvironmentItemType::WOOD:
        item = WoodItem::create(tileCoord);
        break;
    case EnvironmentItemType::GRASS:
        item = GrassItem::create(tileCoord);
        break;
    case EnvironmentItemType::DAFFODILS:
        item = DaffodilsItem::create(tileCoord);
        break;
    case EnvironmentItemType::LEEK:
        item = LeekItem::create(tileCoord);
        break;
    case EnvironmentItemType::CULTIVATED_SOIL:
        _cultivatedSoils[keyFor(tileCoord)] = true;
        return true;
    default:
        return false;
    }

    if (!item) return false;

    if (_tiledMap && _gameMap) {
        item->setPosition(_gameMap->calWorldPos(tileCoord));
        _tiledMap->addChild(item, ITEM_SPRITE_Z_ORDER);
    }
    else {
        // 防止无父节点但被 retain
        return false;
    }

    item->retain();
    _items.emplace(keyFor(tileCoord), item);

    if (type == EnvironmentItemType::WOOD) _woodCount++;
    else if (type == EnvironmentItemType::GRASS) _grassCount++;
    else if (type == EnvironmentItemType::DAFFODILS) _daffodilsCount++;
    else if (type == EnvironmentItemType::LEEK) _leekCount++;

    return true;
}

// 移除物品
bool FarmItemManager::removeItem(const Vec2& tileCoord)
{
    const long long k = keyFor(tileCoord);

    if (_cultivatedSoils.erase(k)) {
        return true;
    }

    const auto it = _items.find(k);
    if (it == _items.end()) return false;

    EnvironmentItem* item = it->second;
    if (!item) {
        _items.erase(it);
        return false;
    }

    const EnvironmentItemType type = item->getType();

    if (type == EnvironmentItemType::WOOD) _woodCount--;
    else if (type == EnvironmentItemType::GRASS) _grassCount--;
    else if (type == EnvironmentItemType::DAFFODILS) _daffodilsCount--;
    else if (type == EnvironmentItemType::LEEK) _leekCount--;

    item->removeFromParent();
    CC_SAFE_RELEASE(item);
    _items.erase(it);

    return true;
}

// 移除耕地标记
bool FarmItemManager::removeCultivation(const Vec2& tileCoord)
{
    return _cultivatedSoils.erase(keyFor(tileCoord)) > 0;
}

// 生成初始物品
void FarmItemManager::spawnInitialItems()
{
    if (!_tiledMap || !_eventLayer) return;

    const Size mapSize = _eventLayer->getLayerSize();
    int attempts = 0;

    while ((_woodCount < MAX_WOOD_COUNT ||
        _grassCount < MAX_GRASS_COUNT ||
        _daffodilsCount < MAX_DAFFODILS_COUNT ||
        _leekCount < MAX_LEEK_COUNT)
        && attempts < MAX_SPAWN_ATTEMPTS)
    {
        attempts++;

        Vec2 coord(
            static_cast<float>(RandomHelper::random_int(0, static_cast<int>(mapSize.width) - 1)),
            static_cast<float>(RandomHelper::random_int(0, static_cast<int>(mapSize.height) - 1))
        );

        if (hasItem(coord)) continue;
        if (!isCultivated(coord)) continue;

        std::vector<EnvironmentItemType> types;
        if (_woodCount < MAX_WOOD_COUNT) types.push_back(EnvironmentItemType::WOOD);
        if (_grassCount < MAX_GRASS_COUNT) types.push_back(EnvironmentItemType::GRASS);
        if (_daffodilsCount < MAX_DAFFODILS_COUNT) types.push_back(EnvironmentItemType::DAFFODILS);
        if (_leekCount < MAX_LEEK_COUNT) types.push_back(EnvironmentItemType::LEEK);

        if (types.empty()) break;

        addItem(types[RandomHelper::random_int(0, static_cast<int>(types.size()) - 1)], coord);
    }
}

// 检查碰撞
bool FarmItemManager::isCollidable(const Vec2& tileCoord) const
{
    return _items.count(keyFor(tileCoord)) > 0;
}

// 新的一天
void FarmItemManager::onNewDay()
{
    spawnInitialItems();
}

// 清理资源
void FarmItemManager::clear()
{
    for (auto& kv : _items) {
        if (kv.second) {
            kv.second->removeFromParent();
            CC_SAFE_RELEASE(kv.second);
        }
    }

    _items.clear();
    _cultivatedSoils.clear();

    _woodCount = 0;
    _grassCount = 0;
    _daffodilsCount = 0;
    _leekCount = 0;
}

// 获取所有物品
std::vector<FarmItemManager::ItemData> FarmItemManager::getItems() const
{
    std::vector<ItemData> data;
    for (const auto& kv : _items) {
        if (kv.second) {
            const Vec2 pos = kv.second->getTileCoord();
            data.push_back({ kv.second->getType(), pos.x, pos.y });
        }
    }
    return data;
}

// 恢复数据
void FarmItemManager::restoreData(const std::vector<ItemData>& items)
{
    this->clear();

    for (const auto& itemData : items) {
        if (itemData.type == EnvironmentItemType::CULTIVATED_SOIL) continue;
        this->addItem(itemData.type, Vec2(itemData.x, itemData.y));
    }
}

