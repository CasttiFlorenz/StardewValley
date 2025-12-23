#include "Farm.h"
#include "../Inventory/InventoryScene.h"

// Farm 场景单例指针
GameMap* Farm::_instance = nullptr;

Farm* Farm::create() {
    auto p = new (std::nothrow) Farm();
    if (p && p->init()) {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

// 获取 Farm 场景的单例实例
GameMap* Farm::getInstance() {
    if (!_instance) {
        _instance = Farm::create();
        CC_SAFE_RETAIN(_instance); // 增加引用计数，防止被自动释放
    }
    return _instance;
}

// 销毁 Farm 场景单例
void Farm::destroyInstance() {
    CC_SAFE_RELEASE_NULL(_instance);
}

bool Farm::init()
{
    // 初始化 Scene 基类
    if (!Scene::init()) return false;

    // 设置当前地图类型为 FARM
    _mapName = MapType::FARM;

    // 加载农场 Tiled 地图
    _map = TMXTiledMap::create("TiledMap/Farm/Farm.tmx");
    if (_map == nullptr)
    {
        CCLOG("Failed to load map: TiledMap/Farm/Farm.tmx");
        return false;
    }

    // event 层仅用于逻辑判断（碰撞、触发器等），不需要显示
    auto eventLayer = _map->getLayer("event");
    if (eventLayer) {
        eventLayer->setVisible(false);
    }

    // 初始化农场物品管理器（木头、杂草、采集物等）
    _farmItemManager = FarmItemManager::getInstance(this);
    if (_farmItemManager == nullptr)
    {
        CCLOG("Failed to init FarmItemManager");
        return false;
    }

    // 初始化耕种管理器（翻地、浇水、种植、收获）
    _cultivationManager = CultivationManager::getInstance(_farmItemManager,this);
    if (_cultivationManager == nullptr)
    {
        CCLOG("Failed to init CultivationManager");
        return false;
    }

    // 添加地图到场景
    this->addChild(_map, 0);

    return true;
}

// 判断玩家是否需要切换地图
MapType Farm::leaveMap(const Vec2& curPos, bool isStart, const Direction& direction)
{
    // 向上移动时的地图切换检测
    if (direction == Direction::UP) {
        if (getObjectRect(GO_TO_HOUSE).containsPoint(curPos))
            return MapType::FARM_HOUSE;

        else if (getObjectRect(GO_TO_MINES).containsPoint(curPos))
            return MapType::MINES;

        if (getObjectRect(GO_TO_BARN).containsPoint(curPos))
            return MapType::BARN;
    }

    // 向右移动进入城镇
    if (direction == Direction::RIGHT) {
        if (getObjectRect(GO_TO_TOWN).containsPoint(curPos))
            return MapType::TOWN;
    }

    return MapType::NONE;
}

// 进入 Farm 地图时的处理
void Farm::IntoMap(MapType lastMap)
{
    const Vec2 visibleSize = Director::getInstance()->getVisibleSize();

    // 设置地图缩放
    _map->setScale(TILED_MAP_SCALE);

    // Farm 地图使用左下角原点对齐
    _map->setPosition(Vec2::ZERO);
}

// 每帧更新（目前未使用）
void Farm::update(float dt)
{
}

// 根据来源地图设置玩家出生点
Vec2 Farm::getPlayerStartPosition(MapType lastMap)
{
    // 从房屋出来
    if (lastMap == MapType::FARM_HOUSE) {
        const Rect rect = getObjectRect(GO_TO_HOUSE);
        if (!rect.equals(Rect::ZERO))
            return rect.origin + rect.size / 2;
    }

    // 从矿洞出来
    if (lastMap == MapType::MINES) {
        const Rect rect = getObjectRect(GO_TO_MINES);
        if (!rect.equals(Rect::ZERO))
            return rect.origin + rect.size / 2;
    }

    // 从谷仓出来
    if (lastMap == MapType::BARN) {
        const Rect rect = getObjectRect(GO_TO_BARN);
        if (!rect.equals(Rect::ZERO))
            return rect.origin + rect.size / 2;
    }

    // 从城镇回来
    if (lastMap == MapType::TOWN) {
        const Rect rect = getObjectRect(GO_TO_TOWN);
        if (!rect.equals(Rect::ZERO))
            return rect.origin + rect.size / 2;
    }

    // 兜底出生点
    return Vec2(100, 100);
}

// 判断世界坐标是否可碰撞
bool Farm::isCollidable(Vec2 worldPos)
{
    if (!_map) return false;

    // 转换为瓦片坐标
    const Vec2 tilePos = calMapPos(worldPos);
    const int x = static_cast<int>(tilePos.x);
    const int y = static_cast<int>(tilePos.y);

    const Size mapSize = _map->getMapSize();

    // 超出地图范围视为不可通行
    if (x < 0 || x >= mapSize.width || y < 0 || y >= mapSize.height)
        return true;

    // 农场环境物品（木头、杂草等）碰撞检测
    if (_farmItemManager && _farmItemManager->isCollidable(tilePos))
        return true;

    // event 层中的 Collidable 属性检测
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

// 鼠标左键：工具使用 / 种植 / 砍伐 / 钓鱼
MouseEvent Farm::onLeftClick(const Vec2& playerPos, const Direction direction, ItemType objects)
{
    Vec2 basePos = this->calMapPos(playerPos);

    // 钓鱼竿优先检测钓鱼区域
    if (objects == ItemType::FISHINGROD) {
        auto layer = _map->getLayer("event");
        if (layer) {
            const int tileGID = layer->getTileGIDAt(basePos);
            if (tileGID) {
                auto properties = _map->getPropertiesForGID(tileGID).asValueMap();
                if (properties.find("Fishing") != properties.end() &&
                    properties.at("Fishing").asBool()) {
                    return MouseEvent::FISHING;
                }
            }
        }
    }

    // 计算玩家正前方交互位置
    ApplyDirectionOffset(basePos, direction);

    // 允许上下三格检测，解决高度/遮挡问题
    const int yOffsets[] = { 1, 0, -1 };

    for (int offset : yOffsets) {
        Vec2 checkPos = basePos;
        checkPos.y += offset;

        // 砍树 / 割草
        if (_farmItemManager) {
            EnvironmentItem* item = _farmItemManager->getItem(checkPos);
            if (item) {
                auto type = item->getType();

                if (type == EnvironmentItemType::WOOD && objects == ItemType::AXE) {
                    _farmItemManager->removeItem(checkPos);
                    if (auto inv = InventoryScene::getInstance()) {
                        inv->ToolUseAnimation();
                        inv->addItemCount(ItemType::WOOD, 1);
                    }
                    return MouseEvent::NONE;
                }

                if (type == EnvironmentItemType::GRASS && objects == ItemType::SCYTHE) {
                    _farmItemManager->removeItem(checkPos);
                    if (auto inv = InventoryScene::getInstance()) {
                        inv->ToolUseAnimation();
                        inv->addItemCount(ItemType::FIBER, 1);
                    }
                    return MouseEvent::NONE;
                }
            }
        }

        // 耕种系统交互
        if (_cultivationManager) {
            switch (objects) {
            case ItemType::HOE:
                _cultivationManager->attemptCultivate(checkPos);
                return MouseEvent::NONE;
            
            case ItemType::WATERING_CAN:
                _cultivationManager->waterSoil(checkPos);
                return MouseEvent::NONE;
            
            case ItemType::PARSNIP_SEED:
                if (_cultivationManager->plantCrop(checkPos, ItemType::PARSNIP))
                    if (auto inv = InventoryScene::getInstance()) {
                        inv->removeItemCount(ItemType::PARSNIP_SEED, 1);
                    }
                return MouseEvent::NONE;

            case ItemType::POTATO_SEED:
                if (_cultivationManager->plantCrop(checkPos, ItemType::POTATO))
                    if (auto inv = InventoryScene::getInstance()) {
                        inv->removeItemCount(ItemType::POTATO_SEED, 1);
                    }
                return MouseEvent::NONE;

            case ItemType::CAULIFLOWER_SEED:
                if (_cultivationManager->plantCrop(checkPos, ItemType::CAULIFLOWER))
                    if (auto inv = InventoryScene::getInstance()) {
                        inv->removeItemCount(ItemType::CAULIFLOWER_SEED, 1);
                    }
                return MouseEvent::NONE;

            default:
                break;
            }

            if (auto inv = InventoryScene::getInstance())
                inv->ToolUseAnimation();

        }
    }

    return MouseEvent::USE_TOOL;
}

// 鼠标右键：采集 / 商店
MouseEvent Farm::onRightClick(const Vec2& playerPos, const Direction direction)
{
    // 检测是否点击出售区域
    const Rect saleRect = getObjectRect("sale");
    if (saleRect.containsPoint(playerPos)) {
        openShopForNPC();
        return MouseEvent::NONE;
    }

    Vec2 basePos = calMapPos(playerPos);
    ApplyDirectionOffset(basePos, direction);

    const int yOffsets[] = { 1, 0, -1 };

    for (int offset : yOffsets) {
        Vec2 checkPos = basePos;
        checkPos.y += offset;

        // 采集地面物品
        if (_farmItemManager) {
            const EnvironmentItem* item = _farmItemManager->getItem(checkPos);
            if (item) {
                auto type = item->getType();
                if (type == EnvironmentItemType::LEEK ||
                    type == EnvironmentItemType::DAFFODILS) {

                    _farmItemManager->removeItem(checkPos);
                    if (auto inv = InventoryScene::getInstance()) {
                        inv->addItemCount(
                            type == EnvironmentItemType::LEEK ? ItemType::LEEK : ItemType::DAFFODILS,
                            1
                        );
                    }
                }
            }
        }

        // 检查是否在采集农作物 
        const ItemType crop = _cultivationManager ? _cultivationManager->harvestCrop(checkPos) : ItemType::NONE;
        switch (crop) {
        case ItemType::PARSNIP:
            if (auto inv = InventoryScene::getInstance()) {
                inv->addItemCount(ItemType::PARSNIP, 1);
            }
            return MouseEvent::NONE;
        case ItemType::POTATO:
            if (auto inv = InventoryScene::getInstance()) {
                inv->addItemCount(ItemType::POTATO, 1);
            }
            return MouseEvent::NONE;
        case ItemType::CAULIFLOWER:
            if (auto inv = InventoryScene::getInstance()) {
                inv->addItemCount(ItemType::CAULIFLOWER, 1);
            }
            return MouseEvent::NONE;
        default:
            break;
        }
    }

    return MouseEvent::NONE;
}

// 打开 NPC 商店界面
void Farm::openShopForNPC()
{
    std::vector<Item*> itemsToSell;

    // 定义可出售物品列表
    std::vector<ItemType> acceptedSellItems = {
        ItemType::STONE, ItemType::WOOD, ItemType::COPPER_ORE,
        ItemType::PARSNIP, ItemType::CAULIFLOWER, ItemType::POTATO,
        ItemType::FIBER, ItemType::DAFFODILS, ItemType::LEEK,
        ItemType::HAY, ItemType::EGG, ItemType::FRIED_EGG,
        ItemType::CARP, ItemType::MILK, ItemType::SALAD
    };

    auto runningScene = Director::getInstance()->getRunningScene();
    if (!runningScene) return;

    // 防止重复打开商店
    const int SHOP_MENU_TAG = 9999;
    if (runningScene->getChildByTag(SHOP_MENU_TAG)) {
        CCLOG("商店菜单已打开，不再重复创建");
        return;
    }

    // 创建并显示商店界面
    auto shopMenu = ShopMenuLayer::create(itemsToSell, acceptedSellItems);
    if (shopMenu) {
        shopMenu->setTag(SHOP_MENU_TAG);
        runningScene->addChild(shopMenu, 999);
        shopMenu->setCameraMask((unsigned short)CameraFlag::DEFAULT);
        CCLOG("成功打开商店菜单");
    }
}
