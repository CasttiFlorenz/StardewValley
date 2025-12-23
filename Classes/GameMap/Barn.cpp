#include "Barn.h"
#include "../Inventory/InventoryScene.h"

// Barn 单例实例指针
GameMap* Barn::_instance = nullptr;

Barn* Barn::create() {
    auto p = new (std::nothrow) Barn();
    if (p && p->init()) {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

// 获取 Barn 场景的单例实例
GameMap* Barn::getInstance() {
    if (!_instance) {
        _instance = Barn::create();
        CC_SAFE_RETAIN(_instance); // 引用计数 +1，防止被自动释放
    }
    return _instance;
}

// 销毁 Barn 单例实例
void Barn::destroyInstance() {
    CC_SAFE_RELEASE_NULL(_instance); // 安全释放并置空
}

// Barn 场景初始化
bool Barn::init()
{
    // 初始化 Scene 基类，失败直接返回
    if (!Scene::init()) return false;

    // 设置当前地图类型为 Barn
    _mapName = MapType::BARN;

    // 加载 Tiled 地图文件
    _map = TMXTiledMap::create("/TiledMap/Barn/Barn.tmx");
    if (_map == nullptr) {
        CCLOG("Failed to load map: /TiledMap/Barn/Barn.tmx");
        return false;
    }

    // 获取事件层（仅用于逻辑，不显示）
    auto eventLayer = _map->getLayer("event");
    if (eventLayer != nullptr) {
        eventLayer->setVisible(false);
    }

    // 获取并初始化 BarnManager（管理牲畜、产物等）
    _barnManager = BarnManager::getInstance(this);
    _barnManager->addAnimal(AnimalType::CHICKEN);

    // 将地图加入场景
    this->addChild(_map);
    return true;
}

// 判断是否需要从当前地图切换到新地图
MapType Barn::leaveMap(const Vec2& curPos, bool isStart, const Direction& direction)
{
    // 玩家向下移动时，检测是否进入通往农场的出口区域
    if (direction == Direction::DOWN) {
        const Rect goToFarmRect = getObjectRect(GO_TO_FARM);
        if (goToFarmRect.containsPoint(curPos)) {
            // 切换地图前停止牲畜动画
            if (_barnManager != nullptr) {
                _barnManager->stopAnimations();
            }
            return MapType::FARM;
        }
    }
    return MapType::NONE;
}

// 设置 Barn 场景显示时的初始状态
void Barn::IntoMap(MapType lastMap)
{
    // 获取屏幕可见区域大小
    const Vec2 visibleSize = Director::getInstance()->getVisibleSize();

    // 设置瓦片地图缩放比例
    _map->setScale(TILED_MAP_SCALE);

    // 居中显示地图
    _map->setPosition((visibleSize - _map->getContentSize() * _map->getScale()) / 2);

    // 启动牲畜动画
    if (_barnManager != nullptr) {
        _barnManager->startAnimations();
    }
}

// 获取玩家进入 Barn 时的起始位置
Vec2 Barn::getPlayerStartPosition(MapType lastMap)
{
    // 使用 goToFarm 对象中心作为出生点
    const Rect goToFarmRect = getObjectRect(GO_TO_FARM);
    if (!goToFarmRect.equals(Rect::ZERO))
        return Vec2(goToFarmRect.getMidX(), goToFarmRect.getMidY());

    // 兜底位置
    return Vec2(100, 100);
}

// 鼠标左键点击事件（通常为使用物品 / 放置）
MouseEvent Barn::onLeftClick(const Vec2& playerPos, const Direction direction, ItemType objects)
{
    // 计算玩家所在的瓦片坐标
    Vec2 basePos = this->calMapPos(playerPos);

    // 根据玩家朝向，偏移到正前方
    this->ApplyDirectionOffset(basePos, direction);

    // y 方向额外检测（扩大检测范围）
    const int yOffsets[] = { 1, 0, -1 };

    for (int offset : yOffsets) {
        Vec2 checkPos = basePos;
        checkPos.y += offset;

        switch (objects) {
        case ItemType::HAY:
            // 尝试在指定位置放置干草
            if (_barnManager != nullptr && _barnManager->addHayAt(checkPos)) {
                // 成功放置后，从背包中移除一个干草
                auto inv = InventoryScene::getInstance();
                if (inv) inv->removeItemCount(ItemType::HAY, 1);
            }
            break;
        default:
            break;
        }
    }

    // 表示已使用工具
    return MouseEvent::USE_TOOL;
}

// 鼠标右键点击事件（通常为采集）
MouseEvent Barn::onRightClick(const Vec2& playerPos, const Direction direction)
{
    // 计算玩家所在的瓦片坐标
    Vec2 basePos = this->calMapPos(playerPos);

    // 根据朝向偏移到前方
    this->ApplyDirectionOffset(basePos, direction);

    // y 方向额外检测（扩大检测范围）
    const int yOffsets[] = { 1, 0, -1 };

    for (int offset : yOffsets) {
        Vec2 checkPos = basePos;
        checkPos.y += offset;

        // 尝试采集该位置的产物
        const ItemType collected = (_barnManager != nullptr)
            ? _barnManager->collectProductionAt(checkPos)
            : ItemType::NONE;

        // 根据采集到的物品类型加入背包
        switch (collected) {
        case ItemType::MILK:
            if (auto inv = InventoryScene::getInstance())
                inv->addItemCount(ItemType::MILK, 1);
            break;
        case ItemType::EGG:
            if (auto inv = InventoryScene::getInstance())
                inv->addItemCount(ItemType::EGG, 1);
        default:
            break;
        }
    }

    return MouseEvent::NONE;
}
