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
        Rect goToFarm = getObjectRect("goToFarm");
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

// 每帧更新（当前未实现逻辑）
void FarmHouse::update(float dt)
{
}

// 根据上一个地图确定玩家出生位置
Vec2 FarmHouse::getPlayerStartPosition(MapType lastMap)
{
    // 如果是第一次进入农舍（即初始就在农舍中）
    if (lastMap == _mapName) {
        const Rect startRect = getObjectRect("start");
        return Vec2(startRect.getMidX(), startRect.getMidY());
    }
    // 如果是从农场进入农舍
    else if (lastMap == MapType::FARM) {
        const Rect goToFarmRect = getObjectRect("goToFarm");
        return Vec2(goToFarmRect.getMidX(), goToFarmRect.getMidY());
    }

    // 默认返回无效位置
    return Vec2(-1, -1);
}

// 鼠标左键点击（农舍内部暂不处理特殊逻辑）
MouseEvent FarmHouse::onLeftClick(const Vec2& playerPos, const Direction direction, ItemType objects)
{
    return MouseEvent::NONE;
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

// 弹出睡觉确认对话框
void FarmHouse::sleep() {
    auto runningScene = Director::getInstance()->getRunningScene();
    if (runningScene) {

        // 防止重复打开睡觉对话框
        if (runningScene->getChildByName("goToBed")) return;

        auto dialog = DialogueLayer::create();
        if (dialog) {
            dialog->setName("goToBed");

            // 显示是否睡觉的选择框
            dialog->showChoice(
                "Are you sure you want to go to sleep?",

                // 玩家选择回调
                [this, runningScene](bool choice) {
                    CCLOG("Player choice: %s", choice ? "YES" : "NO");

                    // 关闭对话框
                    if (auto dlg = runningScene->getChildByName("goToBed")) {
                        dlg->removeFromParent();
                    }

                    if (choice) {
                        CCLOG("Player chose to sleep");
                        // 这里可扩展：时间流逝、保存、第二天开始等逻辑
                        // this->performSleep();
                    }
                    else {
                        CCLOG("Player chose not to sleep");
                    }
                }
            );

            // 将对话框加入当前场景
            runningScene->addChild(dialog, 9999);
            dialog->setCameraMask((unsigned short)CameraFlag::DEFAULT);
        }
    }
}
