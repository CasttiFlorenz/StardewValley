/****************************************************************
 * Project Name:  StardewValley
 * File Name:     CultivationManager.cpp
 * File Function: CultivationManager类的实现
 * Author:        郭芷烟
 * Update Date:   2025/12/16
 * License:       MIT License
 ****************************************************************/
#include "CultivationManager.h"

CultivationManager* CultivationManager::_instance = nullptr;

// 获取单例
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
    // 重新初始化（切换地图时可能需要）
    else if ((farmItemManager || gameMap) && !_instance->_gameMap) {
        _instance->init(farmItemManager, gameMap);
    }

    return _instance;
}

// 销毁单例
void CultivationManager::destroyInstance()
{
    CC_SAFE_RELEASE_NULL(_instance);
}

// 构造函数
CultivationManager::CultivationManager()
    : _farmItemManager(nullptr)
    , _gameMap(nullptr)
    , _tiledMap(nullptr)
{
}

// 析构函数
CultivationManager::~CultivationManager()
{
    for (auto& kv : _soils) {
        if (kv.second) {
            kv.second->removeFromParent();
            kv.second->release();
        }
    }
    _soils.clear();

    CC_SAFE_RELEASE_NULL(_farmItemManager);
}

// 初始化
bool CultivationManager::init(FarmItemManager* farmItemManager, GameMap* gameMap)
{
    // 清理现有数据
    for (auto& kv : _soils) {
        if (kv.second) {
            kv.second->removeFromParent();
            kv.second->release();
        }
    }
    _soils.clear();

    // 更新引用
    if (_farmItemManager != farmItemManager) {
        CC_SAFE_RELEASE_NULL(_farmItemManager);
        _farmItemManager = farmItemManager;
    }

    _gameMap = gameMap;
    _tiledMap = _gameMap ? _gameMap->getTiledMap() : nullptr;

    return _farmItemManager && _gameMap;
}

// 获取键值
long long CultivationManager::keyFor(const Vec2& tileCoord) {
    long long x = static_cast<long long>(tileCoord.x);
    long long y = static_cast<long long>(tileCoord.y);
    return (x << 32) | (y & TILE_COORD_MASK);
}

// 尝试开垦
bool CultivationManager::attemptCultivate(const Vec2& tileCoord) {
    if (!_farmItemManager || !_gameMap) {
        return false;
    }

    // 检查是否可开垦
    if (!_farmItemManager->isCultivated(tileCoord)) {
        return false;
    }

    // 检查是否有障碍物
    if (_farmItemManager->hasItem(tileCoord)) {
        return false;
    }

    const long long key = keyFor(tileCoord);
    if (_soils.find(key) != _soils.end()) {
        return false;
    }

    auto soil = CultivatedSoil::create(tileCoord);
    if (!soil) {
        return false;
    }

    // 注册到 FarmItemManager
    _farmItemManager->addItem(EnvironmentItemType::CULTIVATED_SOIL, tileCoord);

    // 添加到地图
    if (_tiledMap) {
        soil->setPosition(_gameMap->calWorldPos(tileCoord));
        _tiledMap->addChild(soil, SOIL_SPRITE_Z_ORDER);
    }

    _soils.emplace(key, soil);
    soil->retain();

    return true;
}

// 浇水
bool CultivationManager::waterSoil(const Vec2& tileCoord) {
    auto it = _soils.find(keyFor(tileCoord));
    if (it == _soils.end() || !it->second) {
        return false;
    }

    it->second->water();
    return true;
}

// 种植
bool CultivationManager::plantCrop(const Vec2& tileCoord, ItemType type) {
    auto it = _soils.find(keyFor(tileCoord));
    if (it == _soils.end() || !it->second) {
        return false;
    }

    return it->second->plant(type);
}

// 新的一天
void CultivationManager::onNewDay() {
    for (auto& pair : _soils) {
        if (pair.second) {
            pair.second->updateDay();
        }
    }
}

// 移除耕地
bool CultivationManager::removeSoil(const Vec2& tileCoord) {
    auto it = _soils.find(keyFor(tileCoord));
    if (it == _soils.end()) {
        return false;
    }

    if (it->second) {
        it->second->removeFromParent();
        it->second->release();
    }

    _soils.erase(it);

    if (_farmItemManager) {
        _farmItemManager->removeItem(tileCoord);
    }

    return true;
}

// 收获作物
ItemType CultivationManager::harvestCrop(const Vec2& tileCoord) {
    auto it = _soils.find(keyFor(tileCoord));
    if (it == _soils.end() || !it->second) {
        return ItemType::NONE;
    }

    return it->second->harvest();
}

// 全部浇水
void CultivationManager::waterAllSoils()
{
    for (auto& pair : _soils) {
        if (pair.second) {
            pair.second->water();
        }
    }
}

// 获取存档数据
std::vector<CultivationManager::SoilSaveData> CultivationManager::getSoilsData() const
{
    std::vector<SoilSaveData> data;
    for (const auto& kv : _soils) {
        auto* soil = kv.second;
        if (!soil) continue;

        SoilSaveData d;
        Vec2 pos = soil->getTileCoord();
        d.x = pos.x;
        d.y = pos.y;
        d.status = static_cast<int>(soil->getStatus());

        if (soil->hasCrop()) {
            d.cropType = static_cast<int>(soil->getCrop()->getCropType());
            d.cropStage = soil->getCrop()->getGrowthStage();
            d.cropStatus = static_cast<int>(soil->getCrop()->getStatus());
        }
        else {
            d.cropType = static_cast<int>(ItemType::NONE);
            d.cropStage = 0;
            d.cropStatus = static_cast<int>(CropStatus::SEEDS);
        }

        data.push_back(d);
    }
    return data;
}

// 恢复存档数据
void CultivationManager::restoreData(const std::vector<SoilSaveData>& data)
{
    // 清理现有数据
    for (auto& kv : _soils) {
        if (kv.second) {
            kv.second->removeFromParent();
            kv.second->release();
        }
    }
    _soils.clear();

    if (!_farmItemManager || !_gameMap) return;

    for (const auto& d : data) {
        Vec2 coord(d.x, d.y);

        // 确保 FarmItemManager 状态同步
        if (!_farmItemManager->isCultivated(coord)) {
            _farmItemManager->addItem(EnvironmentItemType::CULTIVATED_SOIL, coord);
        }

        auto soil = CultivatedSoil::create(coord);
        if (!soil) continue;

        // 添加到地图
        if (_tiledMap) {
            soil->setPosition(_gameMap->calWorldPos(coord));
            _tiledMap->addChild(soil, SOIL_SPRITE_Z_ORDER);
        }

        // 恢复状态
        soil->setStatus(SoilStatus::DRY); // 默认干土，如需恢复湿润可修改此处

        // 恢复作物
        if (static_cast<ItemType>(d.cropType) != ItemType::NONE) {
            soil->plant(static_cast<ItemType>(d.cropType));
            if (soil->hasCrop()) {
                soil->getCrop()->setGrowthStage(d.cropStage, static_cast<CropStatus>(d.cropStatus));
            }
        }

        long long key = keyFor(coord);
        _soils.emplace(key, soil);
        soil->retain();
    }

}
