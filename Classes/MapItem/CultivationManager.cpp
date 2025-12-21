#include "CultivationManager.h"

CultivationManager* CultivationManager::_instance = nullptr;

CultivationManager* CultivationManager::getInstance() {
    if (!_instance) {
        _instance = new CultivationManager();
    }
    return _instance;
}

CultivationManager::CultivationManager() : _farmItemManager(nullptr), _gameMap(nullptr), _tiledMap(nullptr){}

CultivationManager::~CultivationManager() {
    _soils.clear();
    CC_SAFE_RELEASE_NULL(_farmItemManager);
}

void CultivationManager::init(FarmItemManager* farmItemManager, GameMap* gameMap) {
    // Clear existing soils if any (e.g. from previous session)
    for (auto& pair : _soils) {
        if (pair.second) {
            pair.second->removeFromParent();
            pair.second->release();
        }
    }
    _soils.clear();

    if (_farmItemManager != farmItemManager) {
        CC_SAFE_RELEASE_NULL(_farmItemManager);
        _farmItemManager = farmItemManager;
        CC_SAFE_RETAIN(_farmItemManager);
    }
    _gameMap = gameMap;
    if (_gameMap) {
        _tiledMap = _gameMap->getTiledMap();
    }

    // Register callback for new day
    auto timeManager = TimeManager::getInstance();
    if (timeManager) {
        timeManager->onDayStartCallback = [this]() {
            this->onNewDay();
        };
    }
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

bool CultivationManager::plantCrop(const Vec2& tileCoord, CropType type) {
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

CropType CultivationManager::harvestCrop(const Vec2& tileCoord) {
    long long key = keyFor(tileCoord);
    auto it = _soils.find(key);
    if (it != _soils.end()) {
        return it->second->harvest();
    }
    return CropType::NONE;
}
