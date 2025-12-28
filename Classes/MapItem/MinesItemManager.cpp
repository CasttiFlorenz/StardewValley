/****************************************************************
 * Project Name:  StardewValley
 * File Name:     MinesItemManager.cpp
 * File Function: MinesItemManager类的实现
 * Author:        郭芷烟
 * Update Date:   2025/12/28
 * License:       MIT License
 ****************************************************************/
#include "MinesItemManager.h"

// 单例实例指针
MinesItemManager* MinesItemManager::_instance = nullptr;

MinesItemManager* MinesItemManager::getInstance(GameMap* gameMap)
{
    // 尚未创建实例
    if (!_instance) {
        // 第一次创建必须提供 GameMap
        if (!gameMap) {
            CCLOG("MinesItemManager::getInstance failed: gameMap is null");
            return nullptr;
        }

        _instance = new (std::nothrow) MinesItemManager();
        if (_instance && _instance->init(gameMap)) {
            // 交由 cocos2d-x 引用计数管理
            CC_SAFE_RETAIN(_instance);
        }
        else {
            CC_SAFE_DELETE(_instance);
        }
    }
    return _instance;
}

void MinesItemManager::destroyInstance()
{
    if (_instance) {
        // 先清理所有生成的物体
        _instance->clear();
        CC_SAFE_RELEASE_NULL(_instance);
    }
}

bool MinesItemManager::init(GameMap* gameMap)
{
    // 保存地图相关指针
    _gameMap = gameMap;
    _tiledMap = _gameMap ? _gameMap->getTiledMap() : nullptr;
    _eventLayer = _tiledMap ? _tiledMap->getLayer(EVENT_LAYER_NAME) : nullptr;

    // 重置内部状态
    _items.clear();
    _stoneCount = 0;
    _copperCount = 0;

    // 初始化时生成初始矿物
    if (_gameMap) {
        spawnInitialItems();
    }

    return _gameMap != nullptr;
}

long long MinesItemManager::keyFor(const Vec2& tileCoord)
{
    // 将瓦片坐标打包成一个 64 位 key
    const long long x = static_cast<long long>(tileCoord.x);
    const long long y = static_cast<long long>(tileCoord.y);
    return (x << 32) | (y & TILE_COORD_MASK);
}

unsigned int MinesItemManager::stripFlags(unsigned int gid)
{
    // 去除 TMX 瓦片翻转相关的高位标志
    return gid & ~TMX_FLIP_FLAGS_MASK;
}

bool MinesItemManager::isStone(const Vec2& tileCoord) const
{
    // 地图或事件层不存在时直接失败
    if (!_eventLayer || !_tiledMap) return false;

    // 获取事件层该位置的 GID
    const unsigned int gid = _eventLayer->getTileGIDAt(tileCoord);
    if (gid == INVALID_TILE_GID) return false;

    // 去掉翻转标记，得到真实 GID
    const unsigned int rawGid = stripFlags(gid);

    // 读取该 GID 的属性
    const auto propsValue = _tiledMap->getPropertiesForGID(rawGid);
    if (propsValue.getType() != Value::Type::MAP) return false;

    const auto props = propsValue.asValueMap();
    const auto it = props.find(STONE_PROPERTY_NAME);
    if (it == props.end()) return false;

    // 兼容布尔 / 数字 / 字符串三种写法
    if (it->second.getType() == Value::Type::BOOLEAN) {
        return it->second.asBool();
    }
    if (it->second.getType() == Value::Type::INTEGER) {
        return it->second.asInt() == TRUE_INTEGER_VALUE;
    }
    if (it->second.getType() == Value::Type::STRING) {
        return it->second.asString() == TRUE_STRING_VALUE_1 || it->second.asString() == TRUE_STRING_VALUE_2;
    }

    return false;
}

bool MinesItemManager::hasItem(const Vec2& tileCoord) const
{
    // 判断该瓦片是否已经生成过物体
    return _items.find(keyFor(tileCoord)) != _items.end();
}

EnvironmentItem* MinesItemManager::getItem(const Vec2& tileCoord) const
{
    // 获取指定瓦片上的物体（不存在则返回 nullptr）
    auto it = _items.find(keyFor(tileCoord));
    return it != _items.end() ? it->second : nullptr;
}

bool MinesItemManager::addItem(EnvironmentItemType type, const Vec2& tileCoord)
{
    // 只能在 Stone 区域生成
    if (!isStone(tileCoord)) return false;

    // 已有物体则不能再生成
    if (hasItem(tileCoord)) return false;

    EnvironmentItem* item = nullptr;

    // 根据类型创建对应物体
    switch (type) {
    case EnvironmentItemType::STONE:
        item = StoneItem::create(tileCoord);
        break;
    case EnvironmentItemType::COPPER:
        item = CopperItem::create(tileCoord);
        break;
    default:
        return false;
    }

    if (!item) return false;

    // 添加到地图中显示
    if (_tiledMap && _gameMap && item) {
        const Vec2 pos = _gameMap->calWorldPos(tileCoord);
        item->setPosition(pos);
        _tiledMap->addChild(item, ITEM_SPRITE_Z_ORDER);
    }

    // 保存引用，交由管理器统一释放
    item->retain();
    _items.emplace(keyFor(tileCoord), item);

    // 维护数量统计
    if (type == EnvironmentItemType::STONE) {
        _stoneCount++;
    }
    else if (type == EnvironmentItemType::COPPER) {
        _copperCount++;
    }

    return true;
}

bool MinesItemManager::removeItem(const Vec2& tileCoord)
{
    const long long k = keyFor(tileCoord);
    auto it = _items.find(k);
    if (it == _items.end()) return false;

    EnvironmentItem* item = it->second;
    if (!item) {
        _items.erase(it);
        return false;
    }

    const EnvironmentItemType type = item->getType();

    // 更新数量统计
    if (type == EnvironmentItemType::STONE) {
        _stoneCount--;
    }
    else if (type == EnvironmentItemType::COPPER) {
        _copperCount--;
    }

    // 从场景移除并释放
    item->removeFromParent();
    CC_SAFE_RELEASE(item);
    _items.erase(it);

    return true;
}

void MinesItemManager::clear()
{
    // 移除并释放所有生成的物体
    for (auto& kv : _items) {
        if (kv.second) {
            kv.second->removeFromParent();
            CC_SAFE_RELEASE(kv.second);
        }
    }

    _items.clear();
    _stoneCount = 0;
    _copperCount = 0;
}

void MinesItemManager::spawnInitialItems()
{
    // 地图未就绪时不生成
    if (!_tiledMap || !_eventLayer) return;

    const Size mapSize = _eventLayer->getLayerSize();
    int attempts = 0;

    // 随机生成，限制最大尝试次数防止死循环
    while ((_stoneCount < MAX_STONE_COUNT || _copperCount < MAX_COPPER_COUNT)
        && attempts < MAX_SPAWN_ATTEMPTS)
    {
        attempts++;

        const int x = RandomHelper::random_int(0, static_cast<int>(mapSize.width) - 1);
        const int y = RandomHelper::random_int(0, static_cast<int>(mapSize.height) - 1);
        const Vec2 coord(static_cast<float>(x), static_cast<float>(y));

        if (hasItem(coord)) continue;
        if (!isStone(coord)) continue;

        EnvironmentItemType type;
        if (_stoneCount < MAX_STONE_COUNT && _copperCount < MAX_COPPER_COUNT) {
            type = RandomHelper::random_int(0, 1) == RANDOM_TYPE_THRESHOLD
                ? EnvironmentItemType::STONE
                : EnvironmentItemType::COPPER;
        }
        else if (_stoneCount < MAX_STONE_COUNT) {
            type = EnvironmentItemType::STONE;
        }
        else {
            type = EnvironmentItemType::COPPER;
        }

        addItem(type, coord);
    }
}

void MinesItemManager::onNewDay()
{
    // 新的一天尝试补充矿物
    spawnInitialItems();
}

std::vector<MinesItemManager::MineItemData> MinesItemManager::getItems() const
{
    std::vector<MineItemData> data;
    for (const auto& kv : _items) {
        if (kv.second) {
            const Vec2 pos = kv.second->getTileCoord();
            data.push_back({ kv.second->getType(), pos.x, pos.y });
        }
    }
    return data;
}

void MinesItemManager::restoreData(const std::vector<MineItemData>& items)
{
    // 1. 清空现有物品
    this->clear();

    // 2. 恢复物品
    for (const auto& d : items) {
        // 直接复用 addItem 逻辑
        this->addItem(d.type, Vec2(d.x, d.y));
    }
}

