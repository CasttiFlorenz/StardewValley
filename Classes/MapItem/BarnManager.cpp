/****************************************************************
 * Project Name:  StardewValley
 * File Name:     BarnManager.cpp
 * File Function: BarnManager类的实现
 * Author:        郭芷烟
 * Update Date:   2025/12/28
 * License:       MIT License
 ****************************************************************/
#include "BarnManager.h"

BarnManager* BarnManager::_instance = nullptr;

// 获取单例
BarnManager* BarnManager::getInstance(GameMap* barn)
{
    if (!_instance) {
        if (!barn) {
            CCLOG("BarnManager::getInstance failed: barn map is null");
            return nullptr;
        }

        _instance = new (std::nothrow) BarnManager();
        if (_instance && _instance->init(barn)) {
            CC_SAFE_RETAIN(_instance);
        }
        else {
            CC_SAFE_DELETE(_instance);
        }
    }
    return _instance;
}

// 销毁单例
void BarnManager::destroyInstance()
{
    if (_instance) {
        _instance->clear();
        CC_SAFE_RELEASE_NULL(_instance);
    }
}

// 初始化
bool BarnManager::init(GameMap* barn)
{
    _gameMap = barn;
    _tiledMap = _gameMap ? _gameMap->getTiledMap() : nullptr;
    _feedLayer = _tiledMap ? _tiledMap->getLayer(FEED_LAYER_NAME) : nullptr;

    _nestCenters.clear();
    _productionCenters.clear();

    _productions.clear();
    _productions.resize(MAX_ANIMAL_COUNT);

    _productionTileKeys.clear();
    _productionTileKeys.resize(MAX_ANIMAL_COUNT, 0);

    // 获取位置点
    for (int i = 1; i <= MAX_ANIMAL_COUNT; ++i) {
        std::string nestName = NEST_OBJECT_PREFIX + std::to_string(i);
        std::string prodName = PRODUCTION_OBJECT_PREFIX + std::to_string(i);

        const Rect nr = _gameMap->getObjectRect(nestName);
        const Rect pr = _gameMap->getObjectRect(prodName);

        _nestCenters.emplace_back(nr.getMidX(), nr.getMidY());
        _productionCenters.emplace_back(pr.getMidX(), pr.getMidY());

        Vec2 prodTile = _gameMap->calMapPos(_productionCenters.back());
        _productionTileKeys[i - 1] = keyFor(prodTile);
    }

    _animals.clear();
    _haySprites.clear();

    return _tiledMap != nullptr;
}

// 坐标转换键值
long long BarnManager::keyFor(const Vec2& tileCoord)
{
    long long x = static_cast<long long>(tileCoord.x);
    long long y = static_cast<long long>(tileCoord.y);
    return (x << 32) | (y & TILE_COORD_MASK);
}

// 添加干草
bool BarnManager::addHayAt(const Vec2& tileCoord)
{
    if (!_feedLayer || !_tiledMap || !_gameMap) return false;

    Size s = _feedLayer->getLayerSize();
    if (s.width < 1 || s.height < 1) return false;
    if (tileCoord.x < 0 || tileCoord.x >= s.width ||
        tileCoord.y < 0 || tileCoord.y >= s.height) return false;

    unsigned int gid = _feedLayer->getTileGIDAt(tileCoord);
    if (gid == INVALID_TILE_GID) return false;

    long long k = keyFor(tileCoord);
    if (_haySprites.find(k) != _haySprites.end()) return false;

    auto sp = Sprite::create(HAY_SPRITE_TEXTURE_PATH);
    if (!sp) return false;

    Vec2 pos = _gameMap->calWorldPos(tileCoord);
    sp->setPosition(pos);
    _tiledMap->addChild(sp, HAY_SPRITE_Z_ORDER);
    _gameMap->setCameraMask((unsigned short)CameraFlag::USER1, true);

    sp->retain();
    _haySprites[k] = sp;

    return true;
}

// 创建动物
BarnAnimal* BarnManager::createAnimal(AnimalType type)
{
    switch (type) {
    case AnimalType::CHICKEN:
        return Chicken::create();
    case AnimalType::COW:
    default:
        return Cow::create();
    }
}

// 添加动物
bool BarnManager::addAnimal(AnimalType type)
{
    if (!_gameMap || !_tiledMap) return false;

    int idx = -1;
    for (int i = 0; i < MAX_ANIMAL_COUNT; ++i) {
        if (i >= static_cast<int>(_animals.size()) || _animals[i] == nullptr) {
            idx = i;
            break;
        }
    }
    if (idx == -1) return false;

    auto sp = createAnimal(type);
    if (!sp) return false;

    sp->setPosition(_nestCenters[idx]);
    _tiledMap->addChild(sp, ANIMAL_SPRITE_Z_ORDER);
    _gameMap->setCameraMask((unsigned short)CameraFlag::USER1, true);

    sp->startAnimation();
    sp->retain();

    if (idx < static_cast<int>(_animals.size()))
        _animals[idx] = sp;
    else {
        _animals.resize(idx + 1);
        _animals[idx] = sp;
    }

    return true;
}

// 新的一天逻辑
void BarnManager::onNewDay()
{
    int animalCount = 0;
    for (auto* a : _animals) {
        if (a) animalCount++;
    }

    int hayCount = static_cast<int>(_haySprites.size());
    int consumeCount = std::min(hayCount, animalCount);

    // 消耗干草
    if (consumeCount > 0) {
        int consumed = 0;
        std::vector<long long> keys;
        keys.reserve(_haySprites.size());

        for (auto& kv : _haySprites) {
            keys.push_back(kv.first);
        }
        std::sort(keys.begin(), keys.end());

        for (auto k : keys) {
            if (consumed >= consumeCount) break;

            auto sp = _haySprites[k];
            if (sp) {
                sp->removeFromParent();
                CC_SAFE_RELEASE(sp);
            }
            _haySprites.erase(k);
            consumed++;
        }
    }

    // 生成产物
    int produceCount = std::min(animalCount, hayCount);
    int produced = 0;

    for (int i = 0; i < MAX_ANIMAL_COUNT && produced < produceCount; ++i) {
        if (i >= static_cast<int>(_animals.size()) || !_animals[i]) continue;

        auto animal = _animals[i];
        auto path = animal->getProducePath();
        auto prod = Sprite::create(path);
        if (!prod) continue;

        prod->setPosition(_productionCenters[i]);
        _tiledMap->addChild(prod, PRODUCTION_SPRITE_Z_ORDER);
        _gameMap->setCameraMask((unsigned short)CameraFlag::USER1, true);

        prod->retain();
        _productions[i].push_back({ prod, animal->getProduceObject() });
        produced++;
    }
}

// 启动动画
void BarnManager::startAnimations()
{
    for (auto* a : _animals) {
        if (a) a->startAnimation();
    }
}

// 停止动画
void BarnManager::stopAnimations()
{
    for (auto* a : _animals) {
        if (a) a->stopAnimation();
    }
}

// 收集产物
ItemType BarnManager::collectProductionAt(const Vec2& tileCoord)
{
    if (!_gameMap || !_tiledMap) return ItemType::NONE;

    long long k = keyFor(tileCoord);

    for (int i = 0; i < MAX_ANIMAL_COUNT; ++i) {
        if (_productionTileKeys[i] == k) {
            if (_productions[i].empty()) return ItemType::NONE;

            auto pair = _productions[i].front();
            _productions[i].erase(_productions[i].begin());

            if (pair.first) {
                pair.first->removeFromParent();
                CC_SAFE_RELEASE(pair.first);
            }
            return pair.second;
        }
    }
    return ItemType::NONE;
}

// 清理资源
void BarnManager::clear()
{
    // 清理干草
    for (auto& kv : _haySprites) {
        if (kv.second) {
            kv.second->removeFromParent();
            CC_SAFE_RELEASE(kv.second);
        }
    }
    _haySprites.clear();

    // 清理动物
    for (auto* a : _animals) {
        if (a) {
            a->removeFromParent();
            CC_SAFE_RELEASE(a);
        }
    }
    _animals.clear();

    // 清理产物
    for (auto& list : _productions) {
        for (auto& pair : list) {
            if (pair.first) {
                pair.first->removeFromParent();
                CC_SAFE_RELEASE(pair.first);
            }
        }
        list.clear();
    }
    _productions.clear();

    _nestCenters.clear();
    _productionCenters.clear();
    _productionTileKeys.clear();
}

// 获取干草位置
std::vector<Vec2> BarnManager::getHayPositions() const
{
    std::vector<Vec2> positions;
    for (const auto& kv : _haySprites) {
        long long k = kv.first;
        int x = (k >> 32) & 0xFFFFFFFF;
        int y = k & TILE_COORD_MASK;
        positions.push_back(Vec2(static_cast<float>(x), static_cast<float>(y)));
    }
    return positions;
}

// 获取动物类型
std::vector<int> BarnManager::getAnimalTypes() const
{
    std::vector<int> types;
    for (auto* animal : _animals) {
        if (animal) {
            types.push_back(static_cast<int>(animal->getType()));
        }
    }
    return types;
}

// 获取产物信息
std::vector<std::pair<int, int>> BarnManager::getProductions() const
{
    std::vector<std::pair<int, int>> prods;
    for (int i = 0; i < static_cast<int>(_productions.size()); ++i) {
        for (const auto& pair : _productions[i]) {
            prods.push_back({ i, static_cast<int>(pair.second) });
        }
    }
    return prods;
}

// 恢复数据
void BarnManager::restoreData(const std::vector<Vec2>& hayPos,
    const std::vector<int>& animalTypes,
    const std::vector<std::pair<int, int>>& productions)
{
    // 初始化时直接 clear 了一次，所以这里直接重建

    // 重新初始化键值
    if (_productions.size() != MAX_ANIMAL_COUNT) {
        _productions.resize(MAX_ANIMAL_COUNT);
        _productionTileKeys.resize(MAX_ANIMAL_COUNT, 0);
        for (int i = 0; i < MAX_ANIMAL_COUNT; ++i) {
            if (i < static_cast<int>(_productionCenters.size())) {
                const Vec2 prodTile = _gameMap->calMapPos(_productionCenters[i]);
                _productionTileKeys[i] = keyFor(prodTile);
            }
        }
    }

    // 1. 恢复干草
    for (const auto& pos : hayPos) {
        this->addHayAt(pos);
    }

    // 2. 恢复动物
    for (const int typeVal : animalTypes) {
        this->addAnimal(static_cast<AnimalType>(typeVal));
    }

    // 3. 恢复产物
    for (const auto& entry : productions) {
        const int nestIdx = entry.first;
        ItemType itemType = static_cast<ItemType>(entry.second);

        if (nestIdx < 0 || nestIdx >= static_cast<int>(_productions.size())) continue;

        std::string path;
        if (itemType == ItemType::EGG) path = CHICKEN_PRODUCE_TEXTURE_PATH;
        else if (itemType == ItemType::MILK) path = COW_PRODUCE_TEXTURE_PATH;
        else continue;

        auto prod = Sprite::create(path);
        if (prod) {
            prod->setPosition(_productionCenters[nestIdx]);
            _tiledMap->addChild(prod, PRODUCTION_SPRITE_Z_ORDER);
            _gameMap->setCameraMask((unsigned short)CameraFlag::USER1, true);

            prod->retain();
            _productions[nestIdx].push_back({ prod, itemType });
        }
    }

}
