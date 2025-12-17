#include "FarmItemManager.h"

FarmItemManager* FarmItemManager::create(GameMap* gameMap) {
    auto p = new (std::nothrow) FarmItemManager();
    if (p && p->init(gameMap)) {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

bool FarmItemManager::init(GameMap* gameMap) {
    _gameMap = gameMap;
    _tiledMap = _gameMap ? _gameMap->getTiledMap() : nullptr;
    _eventLayer = _tiledMap ? _tiledMap->getLayer("event") : nullptr;
    _items.clear();
    _woodCount = 0;
    _grassCount = 0;
    _daffodilsCount = 0;
    _leekCount = 0;

    if (_gameMap) {
        spawnInitialItems();
    }
    return _gameMap != nullptr;
}

long long FarmItemManager::keyFor(const Vec2& tileCoord) {
    long long x = static_cast<long long>(tileCoord.x);
    long long y = static_cast<long long>(tileCoord.y);
    return (x << 32) | (y & 0xffffffffLL);
}

unsigned int FarmItemManager::stripFlags(unsigned int gid) {
    return gid & ~(0x80000000 | 0x40000000 | 0x20000000);
}

bool FarmItemManager::isCultivated(const Vec2& tileCoord) const {
    if (!_eventLayer || !_tiledMap) return false;

    unsigned int gid = _eventLayer->getTileGIDAt(tileCoord);
    if (gid == 0) return false;

    unsigned int rawGid = stripFlags(gid);

    auto propsValue = _tiledMap->getPropertiesForGID(rawGid);
    if (propsValue.getType() != cocos2d::Value::Type::MAP) return false;

    auto props = propsValue.asValueMap();
    auto it = props.find("Cultivated");
    if (it == props.end()) return false;

    if (it->second.getType() == cocos2d::Value::Type::BOOLEAN) {
        return it->second.asBool();
    }
    if (it->second.getType() == cocos2d::Value::Type::INTEGER) {
        return it->second.asInt() == 1;
    }
    if (it->second.getType() == cocos2d::Value::Type::STRING) {
        return it->second.asString() == "1" || it->second.asString() == "true";
    }

    return false;
}

bool FarmItemManager::hasItem(const Vec2& tileCoord) const {
    return (_items.find(keyFor(tileCoord))) != (_items.end());
}

EnvironmentItem* FarmItemManager::getItem(const Vec2& tileCoord) const {
    auto it = _items.find(keyFor(tileCoord));
    if (it != _items.end()) {
        return it->second;
    }
    return nullptr;
}

bool FarmItemManager::addItem(EnvironmentItemType type, const Vec2& tileCoord) {
    if (!isCultivated(tileCoord)) {
        return false;
    }

    if (hasItem(tileCoord)) {
        return false;
    }

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
    default:
        return false;
    }

    if (item) {
        // 将 Item 添加到 Map 中
        if (_tiledMap && _gameMap) {
            Vec2 pos = _gameMap->calWorldPos(tileCoord);
            item->setPosition(pos);
            _tiledMap->addChild(item, 5); // ZOrder 5
        }

        item->retain(); // 保持引用，因为存储在 unordered_map 中
        _items.emplace(keyFor(tileCoord), item);

        if (type == EnvironmentItemType::WOOD) {
            _woodCount++;
        } else if (type == EnvironmentItemType::GRASS) {
            _grassCount++;
        } else if (type == EnvironmentItemType::DAFFODILS) {
            _daffodilsCount++;
        } else if (type == EnvironmentItemType::LEEK) {
            _leekCount++;
        }

        return true;
    }

    return false;
}

bool FarmItemManager::removeItem(const Vec2& tileCoord) {
    long long k = keyFor(tileCoord);
    auto it = _items.find(k);
    if (it == _items.end()) {
        return false;
    }

    EnvironmentItem* item = it->second;
    EnvironmentItemType type = item->getType();

    if (type == EnvironmentItemType::WOOD) {
        _woodCount--;
    } else if (type == EnvironmentItemType::GRASS) {
        _grassCount--;
    } else if (type == EnvironmentItemType::DAFFODILS) {
        _daffodilsCount--;
    } else if (type == EnvironmentItemType::LEEK) {
        _leekCount--;
    }

    item->removeFromParent();
    CC_SAFE_RELEASE(item);
    _items.erase(it);
    return true;
}

void FarmItemManager::clear() {
    for (auto& kv : _items) {
        kv.second->removeFromParent();
        CC_SAFE_RELEASE(kv.second);
    }
    _items.clear();
    _woodCount = 0;
    _grassCount = 0;
    _daffodilsCount = 0;
    _leekCount = 0;
}

void FarmItemManager::spawnInitialItems() {
    if (!_tiledMap || !_eventLayer) return;

    Size mapSize = _eventLayer->getLayerSize();

    int attempts = 0;
    // 尝试生成物品，防止死循环
    while ((_woodCount < MAX_WOOD_COUNT || _grassCount < MAX_GRASS_COUNT ||
        _daffodilsCount < MAX_DAFFODILS_COUNT || _leekCount < MAX_LEEK_COUNT) && attempts < 2000) {
        attempts++;
        int x = RandomHelper::random_int(0, (int)mapSize.width - 1);
        int y = RandomHelper::random_int(0, (int)mapSize.height - 1);
        Vec2 coord((float)x, (float)y);

        if (hasItem(coord)) continue;
        if (!isCultivated(coord)) continue;

        EnvironmentItemType type = EnvironmentItemType::NONE;

        // 随机选择一种未满的类型
        std::vector<EnvironmentItemType> availableTypes;
        if (_woodCount < MAX_WOOD_COUNT) availableTypes.push_back(EnvironmentItemType::WOOD);
        if (_grassCount < MAX_GRASS_COUNT) availableTypes.push_back(EnvironmentItemType::GRASS);
        if (_daffodilsCount < MAX_DAFFODILS_COUNT) availableTypes.push_back(EnvironmentItemType::DAFFODILS);
        if (_leekCount < MAX_LEEK_COUNT) availableTypes.push_back(EnvironmentItemType::LEEK);

        if (availableTypes.empty()) break;

        type = availableTypes[RandomHelper::random_int(0, (int)availableTypes.size() - 1)];

        addItem(type, coord);
    }
}