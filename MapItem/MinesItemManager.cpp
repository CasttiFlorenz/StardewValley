#include "MinesItemManager.h"

MinesItemManager* MinesItemManager::create(GameMap* gameMap) {
    auto p = new (std::nothrow) MinesItemManager();
    if (p && p->init(gameMap)) {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

bool MinesItemManager::init(GameMap* gameMap) {
    _gameMap = gameMap;
    _tiledMap = _gameMap ? _gameMap->getTiledMap() : nullptr;
    // 获取名为 "Stone" 的层
    _eventLayer = _tiledMap ? _tiledMap->getLayer("event") : nullptr;
    _items.clear();
    _stoneCount = 0;
    _copperCount = 0;

    if (_gameMap) {
        spawnInitialItems();
    }
    return _gameMap != nullptr;
}

long long MinesItemManager::keyFor(const Vec2& tileCoord) {
    long long x = static_cast<long long>(tileCoord.x);
    long long y = static_cast<long long>(tileCoord.y);
    return (x << 32) | (y & 0xffffffffLL);
}

unsigned int MinesItemManager::stripFlags(unsigned int gid) {
    return gid & ~(0x80000000 | 0x40000000 | 0x20000000);
}

bool MinesItemManager::isStone(const Vec2& tileCoord) const {
    if (!_eventLayer || !_tiledMap) return false;

    // 获取 GID 并去除翻转位
    unsigned int gid = _eventLayer->getTileGIDAt(tileCoord);
    if (gid == 0) return false;

    unsigned int rawGid = stripFlags(gid);

    // 获取 GID 属性
    auto propsValue = _tiledMap->getPropertiesForGID(rawGid);
    if (propsValue.getType() != cocos2d::Value::Type::MAP) return false;

    auto props = propsValue.asValueMap();
    auto it = props.find("Stone");
    if (it == props.end()) return false;

    // 检查属性值是否为 true (支持布尔、整数或字符串形式)
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

bool MinesItemManager::hasItem(const Vec2& tileCoord) const {
    return (_items.find(keyFor(tileCoord))) != (_items.end());
}

EnvironmentItem* MinesItemManager::getItem(const Vec2& tileCoord) const {
    auto it = _items.find(keyFor(tileCoord));
    if (it != _items.end()) {
        return it->second;
    }
    return nullptr;
}

bool MinesItemManager::addItem(EnvironmentItemType type, const Vec2& tileCoord) {
    // 1. 检查是否在 Stone 层
    if (!isStone(tileCoord)) {
        return false;
    }

    // 2. 检查该位置是否已有物品
    if (hasItem(tileCoord)) {
        return false;
    }

    // 3. 创建物品
    EnvironmentItem* item = nullptr;
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

    if (item) {
        // 将 Item 添加到 Map 中
        if (_tiledMap && _gameMap) {
            Vec2 pos = _gameMap->calWorldPos(tileCoord);
            item->setPosition(pos);
            _tiledMap->addChild(item, 5); // ZOrder 5
        }

        item->retain(); // 保持引用，因为存储在 unordered_map 中
        _items.emplace(keyFor(tileCoord), item);

        if (type == EnvironmentItemType::STONE) {
            _stoneCount++;
        }
        else if (type == EnvironmentItemType::COPPER) {
            _copperCount++;
        }

        return true;
    }

    return false;
}

bool MinesItemManager::removeItem(const Vec2& tileCoord) {
    long long k = keyFor(tileCoord);
    auto it = _items.find(k);
    if (it == _items.end()) {
        return false;
    }

    // 从 Map 移除并释放
    EnvironmentItem* item = it->second;
    EnvironmentItemType type = item->getType();

    if (type == EnvironmentItemType::STONE) {
        _stoneCount--;
    }
    else if (type == EnvironmentItemType::COPPER) {
        _copperCount--;
    }

    item->removeFromParent();
    CC_SAFE_RELEASE(item);
    _items.erase(it);
    return true;
}

void MinesItemManager::clear() {
    for (auto& kv : _items) {
        kv.second->removeFromParent();
        CC_SAFE_RELEASE(kv.second);
    }
    _items.clear();
    _stoneCount = 0;
    _copperCount = 0;
}

void MinesItemManager::spawnInitialItems() {
    if (!_tiledMap || !_eventLayer) return;

    Size mapSize = _eventLayer->getLayerSize();

    int attempts = 0;
    // 尝试生成物品，防止死循环
    while ((_stoneCount < MAX_STONE_COUNT || _copperCount < MAX_COPPER_COUNT) && attempts < 1000) {
        attempts++;
        int x = RandomHelper::random_int(0, (int)mapSize.width - 1);
        int y = RandomHelper::random_int(0, (int)mapSize.height - 1);
        Vec2 coord((float)x, (float)y);

        if (hasItem(coord)) continue;
        if (!isStone(coord)) continue;

        EnvironmentItemType type;
        if (_stoneCount < MAX_STONE_COUNT && _copperCount < MAX_COPPER_COUNT) {
            type = RandomHelper::random_int(0, 1) == 0 ? EnvironmentItemType::STONE : EnvironmentItemType::COPPER;
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
