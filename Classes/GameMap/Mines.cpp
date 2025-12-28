#include "Mines.h"
#include "../Inventory/InventoryScene.h"

// 单例实例
GameMap* Mines::_instance = nullptr;

// 创建实例
Mines* Mines::create() {
    auto p = new (std::nothrow) Mines();
    if (p && p->init()) {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

// 获取单例
GameMap* Mines::getInstance() {
    if (!_instance) {
        _instance = Mines::create();
        CC_SAFE_RETAIN(_instance);
    }
    return _instance;
}

// 销毁单例
void Mines::destroyInstance() {
    CC_SAFE_RELEASE_NULL(_instance);
}

// 初始化
bool Mines::init()
{
    if (!Scene::init()) return false;

    _mapName = MapType::MINES;

    // 加载地图
    _map = TMXTiledMap::create(TILED_MAP_MINES_PATH);
    if (!_map) return false;

    // 隐藏事件层
    if (auto eventLayer = _map->getLayer(EVENT_LAYER_NAME)) {
        eventLayer->setVisible(false);
    }

    // 初始化管理器
    _minesItemManager = MinesItemManager::getInstance(this);

    this->addChild(_map);

    return true;
}

// 离开地图逻辑
MapType Mines::leaveMap(const Vec2& curPos, bool isStart, const Direction& direction)
{
    // 向下移动返回农场
    if (direction == Direction::DOWN) {
        const Rect goToFarmRect = getObjectRect(GO_TO_FARM);
        if (goToFarmRect.containsPoint(curPos)) {
            return MapType::FARM;
        }
    }

    return MapType::NONE;
}

// 进入地图逻辑
void Mines::IntoMap(MapType lastMap)
{
    const Vec2 visibleSize = Director::getInstance()->getVisibleSize();

    _map->setScale(TILED_MAP_SCALE);
    _map->setPosition((visibleSize - _map->getContentSize() * _map->getScale()) / 2);
}

// 获取玩家初始位置
Vec2 Mines::getPlayerStartPosition(MapType lastMap)
{
    const Rect goToFarmRect = getObjectRect(GO_TO_FARM);
    if (!goToFarmRect.equals(Rect::ZERO)) {
        return Vec2(goToFarmRect.getMidX(), goToFarmRect.getMidY());
    }
    return Vec2(PLAYER_DEFAULT_POS_X, PLAYER_DEFAULT_POS_Y);
}

// 左键点击处理（挖掘）
MouseEvent Mines::onLeftClick(const Vec2& playerPos,
    const Direction direction,
    ItemType objects)
{
    // 必须使用镐子
    if (objects == ItemType::PICKAXE) {

        Vec2 basePos = calMapPos(playerPos);
        ApplyDirectionOffset(basePos, direction);

        const int yOffsets[] = { Y_OFFSET_1, Y_OFFSET_0, Y_OFFSET_NEG_1 };

        for (const int offset : yOffsets) {
            Vec2 checkPos = basePos;
            checkPos.y += static_cast<float>(offset);

            const EnvironmentItem* item =
                _minesItemManager ? _minesItemManager->getItem(checkPos) : nullptr;

            if (item) {
                const auto type = item->getType();

                // 挖石头
                if (type == EnvironmentItemType::STONE) {
                    _minesItemManager->removeItem(checkPos);
                    if (auto inv = InventoryScene::getInstance()) {
                        inv->ToolUseAnimation();
                        inv->addItemCount(ItemType::STONE, static_cast<int>(ITEM_COUNT_1));

                        if (const auto skill = SkillLevel::getInstance()) {
                            skill->increaseSkillLevel(SkillType::MINING);
                        }
                    }
                    return MouseEvent::USE_TOOL;
                }
                // 挖铜矿
                else if (type == EnvironmentItemType::COPPER) {
                    _minesItemManager->removeItem(checkPos);
                    if (auto inv = InventoryScene::getInstance()) {
                        inv->ToolUseAnimation();
                        inv->addItemCount(ItemType::COPPER_ORE, static_cast<int>(ITEM_COUNT_1));

                        if (const auto skill = SkillLevel::getInstance()) {
                            skill->increaseSkillLevel(SkillType::MINING);
                        }
                    }
                    return MouseEvent::USE_TOOL;
                }
            }
        }
    }

    return MouseEvent::USE_TOOL;
}

// 碰撞检测
bool Mines::isCollidable(Vec2 worldPos)
{
    if (!_map) return false;

    const Vec2 tilePos = calMapPos(worldPos);
    const int x = static_cast<int>(tilePos.x);
    const int y = static_cast<int>(tilePos.y);

    const Size mapSize = _map->getMapSize();

    // 边界检测
    if (x < 0 || x >= mapSize.width || y < 0 || y >= mapSize.height) {
        return true;
    }

    // 物品碰撞
    if (_minesItemManager && _minesItemManager->hasItem(tilePos))
        return true;

    // 地图属性碰撞
    auto layer = _map->getLayer(EVENT_LAYER_NAME);
    if (!layer) return false;

    const int tileGID = layer->getTileGIDAt(tilePos);
    if (tileGID) {
        auto properties = _map->getPropertiesForGID(tileGID).asValueMap();
        if (!properties.empty() &&
            properties.find(COLLIDABLE_PROPERTY_NAME) != properties.end() &&
            properties.at(COLLIDABLE_PROPERTY_NAME).asBool()) {
            return true;
        }
    }

    return false;
}