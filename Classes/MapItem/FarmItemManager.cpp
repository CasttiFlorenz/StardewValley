#include "FarmItemManager.h"

// 单例实例
FarmItemManager* FarmItemManager::_instance = nullptr;

FarmItemManager* FarmItemManager::getInstance(GameMap* gameMap)
{
    if (!_instance) {
        // 首次创建必须提供 GameMap
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

void FarmItemManager::destroyInstance()
{
    if (_instance) {
        _instance->clear();
        CC_SAFE_RELEASE_NULL(_instance);
    }
}

bool FarmItemManager::init(GameMap* gameMap)
{
    _gameMap = gameMap;
    _tiledMap = _gameMap ? _gameMap->getTiledMap() : nullptr;
    _eventLayer = _tiledMap ? _tiledMap->getLayer("event") : nullptr;

    _items.clear();
    _cultivatedSoils.clear();

    _woodCount = 0;
    _grassCount = 0;
    _daffodilsCount = 0;
    _leekCount = 0;

    // 初始化时生成初始物品
    if (_gameMap) {
        spawnInitialItems();
    }

    return _gameMap != nullptr;
}

long long FarmItemManager::keyFor(const Vec2& tileCoord)
{
    // 将瓦片坐标打包为 64 位 key
    long long x = static_cast<long long>(tileCoord.x);
    long long y = static_cast<long long>(tileCoord.y);
    return (x << 32) | (y & 0xffffffffLL);
}

unsigned int FarmItemManager::stripFlags(unsigned int gid)
{
    // 去除 TMX 的翻转标志位
    return gid & ~(0x80000000 | 0x40000000 | 0x20000000);
}

bool FarmItemManager::isCultivated(const Vec2& tileCoord) const
{
    // 地图或事件层未准备好
    if (!_eventLayer || !_tiledMap) return false;

    unsigned int gid = _eventLayer->getTileGIDAt(tileCoord);
    if (gid == 0) return false;

    unsigned int rawGid = stripFlags(gid);

    auto propsValue = _tiledMap->getPropertiesForGID(rawGid);
    if (propsValue.getType() != Value::Type::MAP) return false;

    auto props = propsValue.asValueMap();
    auto it = props.find("Cultivated");
    if (it == props.end()) return false;

    // 兼容多种配置写法
    if (it->second.getType() == Value::Type::BOOLEAN)
        return it->second.asBool();
    if (it->second.getType() == Value::Type::INTEGER)
        return it->second.asInt() == 1;
    if (it->second.getType() == Value::Type::STRING)
        return it->second.asString() == "1" || it->second.asString() == "true";

    return false;
}

bool FarmItemManager::hasItem(const Vec2& tileCoord) const
{
    long long key = keyFor(tileCoord);
    // 普通物品或耕地任意存在即可
    return _items.count(key) || _cultivatedSoils.count(key);
}

EnvironmentItem* FarmItemManager::getItem(const Vec2& tileCoord) const
{
    auto it = _items.find(keyFor(tileCoord));
    return it != _items.end() ? it->second : nullptr;
}

bool FarmItemManager::addItem(EnvironmentItemType type, const Vec2& tileCoord)
{
    // 必须在可耕种区域
    if (!isCultivated(tileCoord)) return false;

    // 该位置已有物品
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
        // 耕地只记录标记，不生成实体
        _cultivatedSoils[keyFor(tileCoord)] = true;
        return true;
    default:
        return false;
    }

    if (!item) return false;

    // 添加到地图中显示
    if (_tiledMap && _gameMap) {
        item->setPosition(_gameMap->calWorldPos(tileCoord));
        _tiledMap->addChild(item, 5);
    }

    item->retain();
    _items.emplace(keyFor(tileCoord), item);

    // 数量统计
    if (type == EnvironmentItemType::WOOD) _woodCount++;
    else if (type == EnvironmentItemType::GRASS) _grassCount++;
    else if (type == EnvironmentItemType::DAFFODILS) _daffodilsCount++;
    else if (type == EnvironmentItemType::LEEK) _leekCount++;

    return true;
}

bool FarmItemManager::removeItem(const Vec2& tileCoord)
{
    long long k = keyFor(tileCoord);

    // 优先移除耕地标记
    if (_cultivatedSoils.erase(k)) {
        return true;
    }

    auto it = _items.find(k);
    if (it == _items.end()) return false;

    EnvironmentItem* item = it->second;
    if (!item) {
        _items.erase(it);
        return false;
    }

    EnvironmentItemType type = item->getType();

    // 更新统计
    if (type == EnvironmentItemType::WOOD) _woodCount--;
    else if (type == EnvironmentItemType::GRASS) _grassCount--;
    else if (type == EnvironmentItemType::DAFFODILS) _daffodilsCount--;
    else if (type == EnvironmentItemType::LEEK) _leekCount--;

    item->removeFromParent();
    CC_SAFE_RELEASE(item);
    _items.erase(it);

    return true;
}

bool FarmItemManager::removeCultivation(const Vec2& tileCoord)
{
    // 仅移除耕地标记
    return _cultivatedSoils.erase(keyFor(tileCoord)) > 0;
}

void FarmItemManager::spawnInitialItems()
{
    // 地图未就绪
    if (!_tiledMap || !_eventLayer) return;

    Size mapSize = _eventLayer->getLayerSize();
    int attempts = 0;

    // 随机生成，限制尝试次数
    while ((_woodCount < MAX_WOOD_COUNT ||
        _grassCount < MAX_GRASS_COUNT ||
        _daffodilsCount < MAX_DAFFODILS_COUNT ||
        _leekCount < MAX_LEEK_COUNT)
        && attempts < 2000)
    {
        attempts++;

        Vec2 coord(
            (float)RandomHelper::random_int(0, (int)mapSize.width - 1),
            (float)RandomHelper::random_int(0, (int)mapSize.height - 1)
        );

        if (hasItem(coord)) continue;
        if (!isCultivated(coord)) continue;

        std::vector<EnvironmentItemType> types;
        if (_woodCount < MAX_WOOD_COUNT) types.push_back(EnvironmentItemType::WOOD);
        if (_grassCount < MAX_GRASS_COUNT) types.push_back(EnvironmentItemType::GRASS);
        if (_daffodilsCount < MAX_DAFFODILS_COUNT) types.push_back(EnvironmentItemType::DAFFODILS);
        if (_leekCount < MAX_LEEK_COUNT) types.push_back(EnvironmentItemType::LEEK);

        if (types.empty()) break;

        addItem(types[RandomHelper::random_int(0, (int)types.size() - 1)], coord);
    }
}

bool FarmItemManager::isCollidable(const Vec2& tileCoord) const
{
    // 有实体物品即可视为可碰撞
    return _items.count(keyFor(tileCoord)) > 0;
}

void FarmItemManager::onNewDay()
{
    // 新的一天补充物品
    spawnInitialItems();
}

void FarmItemManager::clear()
{
    // 移除并释放所有物品
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
