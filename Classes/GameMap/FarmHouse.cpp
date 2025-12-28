#include "FarmHouse.h"
#include "../Inventory/InventoryScene.h"

// 单例实例
GameMap* FarmHouse::_instance = nullptr;

// 创建实例
FarmHouse* FarmHouse::create() {
    auto p = new (std::nothrow) FarmHouse();
    if (p && p->init()) {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

// 获取单例
GameMap* FarmHouse::getInstance() {
    if (!_instance) {
        _instance = FarmHouse::create();
        CC_SAFE_RETAIN(_instance);
    }
    return _instance;
}

// 销毁单例
void FarmHouse::destroyInstance() {
    CC_SAFE_RELEASE_NULL(_instance);
}

// 初始化
bool FarmHouse::init()
{
    if (!Scene::init()) return false;

    _mapName = MapType::FARM_HOUSE;

    // 加载地图
    _map = TMXTiledMap::create(TILED_MAP_FARM_HOUSE_PATH);
    if (_map == nullptr) {
        CCLOG("Failed to load map: %s", TILED_MAP_FARM_HOUSE_PATH.c_str());
        return false;
    }

    // 隐藏碰撞层
    if (auto collisionLayer = _map->getLayer(EVENT_LAYER_NAME)) {
        collisionLayer->setVisible(false);
    }

    this->addChild(_map);

    return true;
}

// 离开地图逻辑
MapType FarmHouse::leaveMap(const Vec2& curPos, bool isStart, const Direction& direction)
{
    if (isStart) {
        return _mapName;
    }

    // 向下移动时检查是否回到农场
    if (direction == Direction::DOWN) {
        const Rect goToFarm = getObjectRect(GO_TO_FARM);
        if (goToFarm.containsPoint(curPos)) {
            return MapType::FARM;
        }
    }

    return MapType::NONE;
}

// 进入地图逻辑
void FarmHouse::IntoMap(MapType lastMap)
{
    const Vec2 visibleSize = Director::getInstance()->getVisibleSize();

    _map->setScale(TILED_MAP_SCALE);
    _map->setPosition((visibleSize - _map->getContentSize() * _map->getScale()) / 2);
}

// 获取玩家初始位置
Vec2 FarmHouse::getPlayerStartPosition(MapType lastMap)
{
    // 游戏开始
    if (lastMap == _mapName) {
        const Rect startRect = getObjectRect(GAME_START);
        return Vec2(startRect.getMidX(), startRect.getMidY());
    }
    // 从农场进入
    else if (lastMap == MapType::FARM) {
        const Rect goToFarmRect = getObjectRect(GO_TO_FARM);
        return Vec2(goToFarmRect.getMidX(), goToFarmRect.getMidY());
    }

    return Vec2(PLAYER_DEFAULT_POS_X, PLAYER_DEFAULT_POS_Y);
}

// 右键点击处理
MouseEvent FarmHouse::onRightClick(const Vec2& playerPos, const Direction direction)
{
    // 检查是否点击了床（触发睡觉）
    const auto bedRect = getObjectRect(GO_TO_BED_OBJECT_NAME);
    if (bedRect.containsPoint(playerPos)) {
        return MouseEvent::SLEEP;
    }
    return MouseEvent::NONE;
}