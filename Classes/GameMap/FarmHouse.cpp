#include "FarmHouse.h"
#include "../Inventory/InventoryScene.h"

// FarmHouse 场景的单例指针
GameMap* FarmHouse::_instance = nullptr;

// 创建 FarmHouse 对象（Cocos2d-x 标准写法）
FarmHouse* FarmHouse::create() {
    auto p = new (std::nothrow) FarmHouse();
    if (p && p->init()) {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

// 获取 FarmHouse 场景单例
GameMap* FarmHouse::getInstance() {
    if (!_instance) {
        _instance = FarmHouse::create();
        CC_SAFE_RETAIN(_instance);
    }
    return _instance;
}

// 销毁 FarmHouse 场景单例
void FarmHouse::destroyInstance() {
    CC_SAFE_RELEASE_NULL(_instance);
}

bool FarmHouse::init()
{
    // 初始化 Scene 基类
    if (!Scene::init()) return false;

    // 设置当前地图类型为 FARM_HOUSE
    _mapName = MapType::FARM_HOUSE;

    // 加载农舍 TMX 地图
    _map = TMXTiledMap::create("TiledMap/FarmHouse/FarmHouse.tmx");
    if (_map == nullptr) {
        CCLOG("Failed to load map: TiledMap/FarmHouse/FarmHouse.tmx");
        return false;
    }

    // 隐藏事件层（用于碰撞或触发，不需要显示）
    if (auto collisionLayer = _map->getLayer("event")) {
        collisionLayer->setVisible(false);
    }

    // 将地图加入场景
    this->addChild(_map);

    return true;
}

// 判断是否需要切换地图
MapType FarmHouse::leaveMap(const Vec2& curPos, bool isStart, const Direction& direction)
{
    // 刚进入地图时不进行切换判断
    if (isStart) {
        return _mapName;
    }

    // 玩家向下走并进入出口区域，则回到农场
    if (direction == Direction::DOWN) {
        const Rect goToFarm = getObjectRect("goToFarm");
        if (goToFarm.containsPoint(curPos)) {
            return MapType::FARM;
        }
    }

    return MapType::NONE;
}

// 进入农舍时设置地图显示位置
void FarmHouse::IntoMap(MapType lastMap)
{
    const Vec2 visibleSize = Director::getInstance()->getVisibleSize();

    // 设置地图缩放比例
    _map->setScale(TILED_MAP_SCALE);

    // 将地图居中显示
    _map->setPosition((visibleSize - _map->getContentSize() * _map->getScale()) / 2);
}


// 根据上一个地图确定玩家出生位置
Vec2 FarmHouse::getPlayerStartPosition(MapType lastMap)
{
    // 如果是第一次进入农舍（即初始就在农舍中）
    if (lastMap == _mapName) {
        const Rect startRect = getObjectRect(GAME_START);
        return Vec2(startRect.getMidX(), startRect.getMidY());
    }
    // 如果是从农场进入农舍
    else if (lastMap == MapType::FARM) {
        const Rect goToFarmRect = getObjectRect(GO_TO_FARM);
        return Vec2(goToFarmRect.getMidX(), goToFarmRect.getMidY());
    }

    // 默认返回无效位置
    return Vec2(-1, -1);
}

// 鼠标右键点击
MouseEvent FarmHouse::onRightClick(const Vec2& playerPos, const Direction direction)
{
    // 如果点击床的位置，触发睡觉事件
    const auto bedRect = getObjectRect("goToBed");
    if (bedRect.containsPoint(playerPos)) {
        return MouseEvent::SLEEP;
    }
    return MouseEvent::NONE;
}

