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
    // 获取名为 "event" 的层，用于检测 Cultivated 属性
    _eventLayer = _tiledMap ? _tiledMap->getLayer("event") : nullptr;
    _items.clear();

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

    // 获取 GID 并去除翻转位
    unsigned int gid = _eventLayer->getTileGIDAt(tileCoord);
    if (gid == 0) return false;

    unsigned int rawGid = stripFlags(gid);
    
    // 获取 GID 属性
    auto propsValue = _tiledMap->getPropertiesForGID(rawGid);
    if (propsValue.getType() != cocos2d::Value::Type::MAP) return false;

    auto props = propsValue.asValueMap();
    auto it = props.find("Cultivated");
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

bool FarmItemManager::hasItem(const Vec2& tileCoord) const {
    return (_items.find(keyFor(tileCoord))) != (_items.end());
}

FarmItem* FarmItemManager::getItem(const Vec2& tileCoord) const {
    auto it = _items.find(keyFor(tileCoord));
    if (it != _items.end()) {
        return it->second;
    }
    return nullptr;
}

bool FarmItemManager::addItem(FarmItemType type, const Vec2& tileCoord) {
    // 1. 检查是否允许耕种 (event 层 Cultivated == 1)
    if (!isCultivated(tileCoord)) {
        return false;
    }

    // 2. 检查该位置是否已有物品
    if (hasItem(tileCoord)) {
        return false;
    }

    // 3. 创建物品
    FarmItem* item = nullptr;
    switch (type) {
        case FarmItemType::Wood:
            item = WoodItem::create(tileCoord);
            break;
        case FarmItemType::Grass:
            item = GrassItem::create(tileCoord);
            break;
        //case FarmItemType::Cultivated:
          //  item = CultivatedItem::create(tileCoord);
            //break;
        default:
            return false;
    }

    if (item) {
        // 将 Item 添加到 Map 上
        if (_tiledMap && _gameMap) {
             Vec2 pos = _gameMap->calWorldPos(tileCoord);
             item->setPosition(pos);
             _tiledMap->addChild(item, 5); // ZOrder 5
        }

        item->retain(); // 保持引用，因为存储在 unordered_map 中
        _items.emplace(keyFor(tileCoord), item);
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

    // 从 Map 移除并释放
    it->second->removeFromParent();
    CC_SAFE_RELEASE(it->second);
    _items.erase(it);
    return true;
}

void FarmItemManager::clear() {
    for (auto& kv : _items) {
        kv.second->removeFromParent();
        CC_SAFE_RELEASE(kv.second);
    }
    _items.clear();
}

void FarmItemManager::spawnInitialItems() {
    if (!_tiledMap || !_eventLayer) return;

    Size mapSize = _eventLayer->getLayerSize();
    int woodCount = 0;
    int grassCount = 0;
    const int maxItems = 15;
    
    int attempts = 0;
    // 尝试次数限制防止死循环
    while ((woodCount < maxItems || grassCount < maxItems) && attempts < 1000) {
        attempts++;
        int x = RandomHelper::random_int(0, (int)mapSize.width - 1);
        int y = RandomHelper::random_int(0, (int)mapSize.height - 1);
        Vec2 coord((float)x, (float)y);

        if (hasItem(coord)) continue;
        if (!isCultivated(coord)) continue;

        FarmItemType type;
        if (woodCount < maxItems && grassCount < maxItems) {
            type = RandomHelper::random_int(0, 1) == 0 ? FarmItemType::Wood : FarmItemType::Grass;
        } else if (woodCount < maxItems) {
            type = FarmItemType::Wood;
        } else {
            type = FarmItemType::Grass;
        }

        if (addItem(type, coord)) {
            if (type == FarmItemType::Wood) woodCount++;
            else grassCount++;
        }
    }
}
