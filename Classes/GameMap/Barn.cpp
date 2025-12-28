/****************************************************************
 * Project Name:  StardewValley
 * File Name:     Barn.cpp
 * File Function: Barn类的实现
 * Author:        郭芷烟
 * Update Date:   2025/12/28
 * License:       MIT License
 ****************************************************************/
#include "Barn.h"
#include "../Inventory/InventoryScene.h"

// 单例实例
GameMap* Barn::_instance = nullptr;

// 创建实例
Barn* Barn::create() {
    auto p = new (std::nothrow) Barn();
    if (p && p->init()) {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

// 获取单例
GameMap* Barn::getInstance() {
    if (!_instance) {
        _instance = Barn::create();
        CC_SAFE_RETAIN(_instance);
    }
    return _instance;
}

// 销毁单例
void Barn::destroyInstance() {
    CC_SAFE_RELEASE_NULL(_instance);
}

// 初始化
bool Barn::init()
{
    if (!Scene::init()) return false;

    // 设置地图类型
    _mapName = MapType::BARN;

    // 加载 Tiled 地图
    _map = TMXTiledMap::create(TILED_MAP_BARN_PATH);
    if (_map == nullptr) {
        CCLOG("Failed to load map: %s", TILED_MAP_BARN_PATH.c_str());
        return false;
    }

    // 隐藏事件层
    auto eventLayer = _map->getLayer(EVENT_LAYER_NAME);
    if (eventLayer != nullptr) {
        eventLayer->setVisible(false);
    }

    // 获取管理器
    _barnManager = BarnManager::getInstance(this);

    this->addChild(_map);
    return true;
}

// 离开地图逻辑
MapType Barn::leaveMap(const Vec2& curPos, bool isStart, const Direction& direction)
{
    // 向下移动时检查是否到达传送点
    if (direction == Direction::DOWN) {
        const Rect goToFarmRect = getObjectRect(GO_TO_FARM);
        if (goToFarmRect.containsPoint(curPos)) {
            // 切换前停止动画
            if (_barnManager != nullptr) {
                _barnManager->stopAnimations();
            }
            return MapType::FARM;
        }
    }
    return MapType::NONE;
}

// 进入地图逻辑
void Barn::IntoMap(MapType lastMap)
{
    const Vec2 visibleSize = Director::getInstance()->getVisibleSize();

    _map->setScale(TILED_MAP_SCALE);
    _map->setPosition((visibleSize - _map->getContentSize() * _map->getScale()) / 2);

    // 恢复动画
    if (_barnManager != nullptr) {
        _barnManager->startAnimations();
    }
}

// 获取玩家初始位置
Vec2 Barn::getPlayerStartPosition(MapType lastMap)
{
    const Rect goToFarmRect = getObjectRect(GO_TO_FARM);
    if (!goToFarmRect.equals(Rect::ZERO))
        return Vec2(goToFarmRect.getMidX(), goToFarmRect.getMidY());

    return Vec2(PLAYER_DEFAULT_POS_X, PLAYER_DEFAULT_POS_Y);
}

// 左键点击处理（放置干草）
MouseEvent Barn::onLeftClick(const Vec2& playerPos, const Direction direction, ItemType objects)
{
    Vec2 basePos = this->calMapPos(playerPos);
    this->ApplyDirectionOffset(basePos, direction);

    // 检查上下偏移范围
    const int yOffsets[] = { Y_OFFSET_1, Y_OFFSET_0, Y_OFFSET_NEG_1 };

    for (const int offset : yOffsets) {
        Vec2 checkPos = basePos;
        checkPos.y += static_cast<float>(offset);

        switch (objects) {
        case ItemType::HAY:
            // 尝试添加干草
            if (_barnManager != nullptr && _barnManager->addHayAt(checkPos)) {
                auto inv = InventoryScene::getInstance();
                if (inv) inv->removeItemCount(ItemType::HAY, static_cast<int>(HAY_ITEM_COUNT));
                return MouseEvent::NONE;
            }
            break;
        default:
            break;
        }
    }

    return MouseEvent::NONE;
}

// 右键点击处理（收集产物）
MouseEvent Barn::onRightClick(const Vec2& playerPos, const Direction direction)
{
    Vec2 basePos = this->calMapPos(playerPos);
    this->ApplyDirectionOffset(basePos, direction);

    const int yOffsets[] = { Y_OFFSET_1, Y_OFFSET_0, Y_OFFSET_NEG_1 };

    for (const int offset : yOffsets) {
        Vec2 checkPos = basePos;
        checkPos.y += static_cast<float>(offset);

        // 尝试收集产物
        const ItemType collected = (_barnManager != nullptr)
            ? _barnManager->collectProductionAt(checkPos)
            : ItemType::NONE;

        switch (collected) {
        case ItemType::MILK:
            if (auto inv = InventoryScene::getInstance())
                inv->addItemCount(ItemType::MILK, static_cast<int>(MILK_ITEM_COUNT));
            break;
        case ItemType::EGG:
            if (auto inv = InventoryScene::getInstance())
                inv->addItemCount(ItemType::EGG, static_cast<int>(EGG_ITEM_COUNT));
        default:
            break;
        }
    }

    return MouseEvent::NONE;

}
