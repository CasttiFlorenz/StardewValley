#include "CultivationManager.h"

CultivationManager* CultivationManager::_instance = nullptr;

// 获取单例（首次可传入依赖）
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
    // 已存在实例但尚未绑定地图（如管理器提前创建）
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

CultivationManager::CultivationManager()
    : _farmItemManager(nullptr)
    , _gameMap(nullptr)
    , _tiledMap(nullptr)
{
}

CultivationManager::~CultivationManager()
{
    // 清理并释放所有耕地
    for (auto& kv : _soils) {
        if (kv.second) {
            kv.second->removeFromParent();
            kv.second->release();
        }
    }
    _soils.clear();

    CC_SAFE_RELEASE_NULL(_farmItemManager);
}

// 初始化 / 重新初始化（地图切换时）
bool CultivationManager::init(FarmItemManager* farmItemManager, GameMap* gameMap)
{
    // 清空旧耕地数据
    for (auto& kv : _soils) {
        if (kv.second) {
            kv.second->removeFromParent();
            kv.second->release();
        }
    }
    _soils.clear();

    // 更新 FarmItemManager 引用
    if (_farmItemManager != farmItemManager) {
        CC_SAFE_RELEASE_NULL(_farmItemManager);
        _farmItemManager = farmItemManager;
    }

    _gameMap = gameMap;
    _tiledMap = _gameMap ? _gameMap->getTiledMap() : nullptr;

    return _farmItemManager && _gameMap;
}

// 根据瓦片坐标生成唯一 key
long long CultivationManager::keyFor(const Vec2& tileCoord) {
    long long x = static_cast<long long>(tileCoord.x);
    long long y = static_cast<long long>(tileCoord.y);
    return (x << 32) | (y & 0xffffffffLL);
}

// 尝试开垦指定瓦片
bool CultivationManager::attemptCultivate(const Vec2& tileCoord) {
    if (!_farmItemManager || !_gameMap) {
        return false;
    }

    // 必须是可耕作区域
    if (!_farmItemManager->isCultivated(tileCoord)) {
        return false;
    }

    // 不能被其他物体占用
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

    // 逻辑层登记
    _farmItemManager->addItem(EnvironmentItemType::CULTIVATED_SOIL, tileCoord);

    // 添加到地图显示
    if (_tiledMap) {
        soil->setPosition(_gameMap->calWorldPos(tileCoord));
        _tiledMap->addChild(soil, 100);
    }

    _soils.emplace(key, soil);
    soil->retain();

    return true;
}

// 给指定耕地浇水
bool CultivationManager::waterSoil(const Vec2& tileCoord) {
    auto it = _soils.find(keyFor(tileCoord));
    if (it == _soils.end() || !it->second) {
        return false;
    }

    it->second->water();
    return true;
}

// 在指定耕地上种植作物
bool CultivationManager::plantCrop(const Vec2& tileCoord, ItemType type) {
    auto it = _soils.find(keyFor(tileCoord));
    if (it == _soils.end() || !it->second) {
        return false;
    }

    return it->second->plant(type);
}

// 新的一天到来时调用
void CultivationManager::onNewDay() {
    for (auto& pair : _soils) {
        if (pair.second) {
            pair.second->updateDay();
        }
    }
}

// 移除指定耕地
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

// 收获指定耕地上的作物
ItemType CultivationManager::harvestCrop(const Vec2& tileCoord) {
    auto it = _soils.find(keyFor(tileCoord));
    if (it == _soils.end() || !it->second) {
        return ItemType::NONE;
    }

    return it->second->harvest();
}

// 给所有耕地浇水
void CultivationManager::waterAllSoils()
{
    for (auto& pair : _soils) {
        if (pair.second) {
            pair.second->water();
        }
    }
}
