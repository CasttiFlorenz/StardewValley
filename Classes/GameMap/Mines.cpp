#include "Mines.h"
#include "../Inventory/InventoryScene.h"

// Mines 场景单例指针
GameMap* Mines::_instance = nullptr;

// 创建 Mines 对象（标准 cocos2d-x 写法）
Mines* Mines::create() {
    auto p = new (std::nothrow) Mines();
    if (p && p->init()) {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

// 获取 Mines 单例
GameMap* Mines::getInstance() {
    if (!_instance) {
        _instance = Mines::create();
        CC_SAFE_RETAIN(_instance);
    }
    return _instance;
}

// 销毁 Mines 单例
void Mines::destroyInstance() {
    CC_SAFE_RELEASE_NULL(_instance);
}

// 初始化矿洞场景
bool Mines::init()
{
    // 初始化 Scene 基类
    if (!Scene::init()) return false;

    // 设置地图类型
    _mapName = MapType::MINES;

    // 加载矿洞 TMX 地图
    _map = TMXTiledMap::create("/TiledMap/Mines/Mines.tmx");
    if (!_map) return false;

    // 隐藏事件层
    if (auto eventLayer = _map->getLayer("event")) {
        eventLayer->setVisible(false);
    }

    // 创建矿洞物品管理器
    _minesItemManager = MinesItemManager::getInstance(this);

    // 将地图加入场景
    this->addChild(_map);

    return true;
}

// 判断是否离开矿洞
MapType Mines::leaveMap(const Vec2& curPos, bool isStart, const Direction& direction)
{
    // 向下走并进入出口区域，返回农场
    if (direction == Direction::DOWN) {
        const Rect goToFarmRect = getObjectRect(GO_TO_FARM);
        if (goToFarmRect.containsPoint(curPos)) {
            return MapType::FARM;
        }
    }

    return MapType::NONE;
}

// 进入矿洞时设置地图显示
void Mines::IntoMap(MapType lastMap)
{
    const Vec2 visibleSize = Director::getInstance()->getVisibleSize();

    // 设置地图缩放
    _map->setScale(TILED_MAP_SCALE);

    // 将地图居中
    _map->setPosition((visibleSize - _map->getContentSize() * _map->getScale()) / 2);
}

// 每帧更新（当前未实现逻辑）
void Mines::update(float dt)
{
}

// 获取玩家在矿洞中的出生位置
Vec2 Mines::getPlayerStartPosition(MapType lastMap)
{
    const Rect goToFarmRect = getObjectRect(GO_TO_FARM);
    if (!goToFarmRect.equals(Rect::ZERO)) {
        return Vec2(goToFarmRect.getMidX(), goToFarmRect.getMidY());
    }
    return Vec2(100, 100);
}

// 左键交互（使用镐子挖矿）
MouseEvent Mines::onLeftClick(const Vec2& playerPos,
    const Direction direction,
    ItemType objects)
{
    // 只处理镐子行为
    if (objects == ItemType::PICKAXE) {

        // 计算玩家前方瓦片坐标
        Vec2 basePos = calMapPos(playerPos);
        ApplyDirectionOffset(basePos, direction);

        const int yOffsets[] = { 1, 0, -1 };

        // 检测前方及上下三个格子
        for (const int offset : yOffsets) {
            Vec2 checkPos = basePos;
            checkPos.y += offset;

            const EnvironmentItem* item =
                _minesItemManager ? _minesItemManager->getItem(checkPos) : nullptr;

            if (item) {
                const auto type = item->getType();

                // 挖石头
                if (type == EnvironmentItemType::STONE) {
                    _minesItemManager->removeItem(checkPos);
                    if (auto inv = InventoryScene::getInstance()) {
                        inv->ToolUseAnimation();
                        inv->addItemCount(ItemType::STONE, 1);
                    }
                    return MouseEvent::USE_TOOL;
                }
                // 挖铜矿
                else if (type == EnvironmentItemType::COPPER) {
                    _minesItemManager->removeItem(checkPos);
                    if (auto inv = InventoryScene::getInstance()) {
                        inv->ToolUseAnimation();
                        inv->addItemCount(ItemType::COPPER_ORE, 1);
                    }
                    return MouseEvent::USE_TOOL;
                }

            }
        }
    }

    return MouseEvent::USE_TOOL;
}

// 矿洞碰撞检测（含矿石与事件层）
bool Mines::isCollidable(Vec2 worldPos)
{
    if (!_map) return false;

    // 转换为瓦片坐标
    const Vec2 tilePos = calMapPos(worldPos);
    const int x = static_cast<int>(tilePos.x);
    const int y = static_cast<int>(tilePos.y);

    const Size mapSize = _map->getMapSize();

    // 边界检测
    if (x < 0 || x >= mapSize.width || y < 0 || y >= mapSize.height) {
        return true;
    }

    // 检测矿洞物品是否阻挡
    if (_minesItemManager && _minesItemManager->hasItem(tilePos))
        return true;

    // 检测事件层的 Collidable 属性
    auto layer = _map->getLayer("event");
    if (!layer) return false;

    const int tileGID = layer->getTileGIDAt(tilePos);
    if (tileGID) {
        auto properties = _map->getPropertiesForGID(tileGID).asValueMap();
        if (!properties.empty() &&
            properties.find("Collidable") != properties.end() &&
            properties.at("Collidable").asBool()) {
            return true;
        }
    }

    return false;
}
