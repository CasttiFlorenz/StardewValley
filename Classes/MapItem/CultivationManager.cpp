#include "CultivationManager.h"

CultivationManager* CultivationManager::_instance = nullptr;

CultivationManager* CultivationManager::getInstance(
    FarmItemManager* farmItemManager,
    GameMap* gameMap
) {
    if (!_instance) {
        _instance = new (std::nothrow) CultivationManager();
        if (_instance && _instance->init(farmItemManager, gameMap)) {
            _instance->autorelease();
            CC_SAFE_RETAIN(_instance);
        }
        else {
            CC_SAFE_DELETE(_instance);
        }
    }
    else if ((farmItemManager || gameMap) && !_instance->_gameMap) {
        _instance->init(farmItemManager, gameMap);
    }
    return _instance;
}

void CultivationManager::destroyInstance()
{
    CC_SAFE_RELEASE_NULL(_instance);
}


CultivationManager::CultivationManager()
    : _farmItemManager(nullptr)
    , _gameMap(nullptr)
    , _tiledMap(nullptr)
{
}

CultivationManager::~CultivationManager()
{
    for (auto& kv : _soils) {
        if (kv.second) {
            kv.second->removeFromParent();
            CC_SAFE_RELEASE(kv.second);
        }
    }
    _soils.clear();

    CC_SAFE_RELEASE_NULL(_farmItemManager);
}

bool CultivationManager::init(FarmItemManager* farmItemManager, GameMap* gameMap)
{
    // Çå¿Õ¾É soil
    for (auto& kv : _soils) {
        if (kv.second) {
            kv.second->removeFromParent();
            CC_SAFE_RELEASE(kv.second);
        }
    }
    _soils.clear();

    if (_farmItemManager != farmItemManager) {
        CC_SAFE_RELEASE_NULL(_farmItemManager);
        _farmItemManager = farmItemManager;
    }

    _gameMap = gameMap;
    _tiledMap = _gameMap ? _gameMap->getTiledMap() : nullptr;
    return _farmItemManager != nullptr && _gameMap != nullptr;
}



long long CultivationManager::keyFor(const Vec2& tileCoord) {
    long long x = static_cast<long long>(tileCoord.x);
    long long y = static_cast<long long>(tileCoord.y);
    return (x << 32) | (y & 0xffffffffLL);
}

bool CultivationManager::attemptCultivate(const Vec2& tileCoord) {
    if (!_farmItemManager) return false;

    // Check if valid cultivated layer
    if (!_farmItemManager->isCultivated(tileCoord)) {
        return false;
    }

    // Check if occupied in FarmItemManager (e.g. wood/grass)
    if (_farmItemManager->hasItem(tileCoord)) {
        return false;
    }

    // Check if already cultivated in our manager
    long long key = keyFor(tileCoord);
    if (_soils.find(key) != _soils.end()) {
        return false; // Already cultivated
    }

    // Create CultivatedSoil
    auto soil = CultivatedSoil::create(tileCoord);
    if (soil) {
        // Add item to FarmItemManager for logic tracking
        _farmItemManager->addItem(EnvironmentItemType::CULTIVATED_SOIL, tileCoord);
        
        // Add to map visually
        if (_gameMap && _tiledMap) {
            Vec2 pos = _gameMap->calWorldPos(tileCoord);
            soil->setPosition(pos);
            _tiledMap->addChild(soil, 100); // ZOrder 4 (below items which are 5)
        }


        _soils[key] = soil;
        soil->retain(); 
        
        return true;
    }
    return false;
}

bool CultivationManager::waterSoil(const Vec2& tileCoord) {
    long long key = keyFor(tileCoord);
    if (_soils.find(key) != _soils.end()) {
        _soils[key]->water();
        return true;
    }
    return false;
}

bool CultivationManager::plantCrop(const Vec2& tileCoord, ItemType type) {
    long long key = keyFor(tileCoord);
    if (_soils.find(key) != _soils.end()) {
        return _soils[key]->plant(type);
    }
    return false;
}

void CultivationManager::onNewDay() {
    for (auto& pair : _soils) {
        CultivatedSoil* soil = pair.second;
        soil->updateDay();
    }
}

bool CultivationManager::removeSoil(const Vec2& tileCoord) {
    long long key = keyFor(tileCoord);
    auto it = _soils.find(key);
    if (it == _soils.end()) {
        return false;
    }
    CultivatedSoil* soil = it->second;
    if (soil) {
        soil->removeFromParent();
        soil->release();
    }
    _soils.erase(it);
    if (_farmItemManager) {
        _farmItemManager->removeItem(tileCoord);
    }
    return true;
}

ItemType CultivationManager::harvestCrop(const Vec2& tileCoord) {
    long long key = keyFor(tileCoord);
    auto it = _soils.find(key);
    if (it != _soils.end()) {
        return it->second->harvest();
    }
    return ItemType::NONE;
}
