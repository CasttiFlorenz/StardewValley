#include "Farm.h"
#include "../Inventory/InventoryScene.h"
#include "../Scene/GameScene.h"

// 单例实例
GameMap* Farm::_instance = nullptr;

// 创建实例
Farm* Farm::create() {
    auto p = new (std::nothrow) Farm();
    if (p && p->init()) {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

// 获取单例
GameMap* Farm::getInstance() {
    if (!_instance) {
        _instance = Farm::create();
        CC_SAFE_RETAIN(_instance);
    }
    return _instance;
}

// 销毁单例
void Farm::destroyInstance() {
    CC_SAFE_RELEASE_NULL(_instance);
}

// 初始化
bool Farm::init()
{
    if (!Scene::init()) return false;

    _mapName = MapType::FARM;

    // 加载地图
    _map = TMXTiledMap::create(TILED_MAP_FARM_PATH);
    if (_map == nullptr)
    {
        CCLOG("Failed to load map: %s", TILED_MAP_FARM_PATH.c_str());
        return false;
    }

    // 隐藏事件层
    auto eventLayer = _map->getLayer(EVENT_LAYER_NAME);
    if (eventLayer) {
        eventLayer->setVisible(false);
    }

    // 初始化管理器
    _farmItemManager = FarmItemManager::getInstance(this);
    if (_farmItemManager == nullptr)
    {
        CCLOG("Failed to init FarmItemManager");
        return false;
    }

    _cultivationManager = CultivationManager::getInstance(_farmItemManager, this);
    if (_cultivationManager == nullptr)
    {
        CCLOG("Failed to init CultivationManager");
        return false;
    }

    this->addChild(_map, 0);

    return true;
}

// 离开地图逻辑
MapType Farm::leaveMap(const Vec2& curPos, bool isStart, const Direction& direction)
{
    // 向上移动
    if (direction == Direction::UP) {
        if (getObjectRect(GO_TO_HOUSE).containsPoint(curPos))
            return MapType::FARM_HOUSE;

        else if (getObjectRect(GO_TO_MINES).containsPoint(curPos))
            return MapType::MINES;

        if (getObjectRect(GO_TO_BARN).containsPoint(curPos))
            return MapType::BARN;
    }

    // 向右移动
    if (direction == Direction::RIGHT) {
        if (getObjectRect(GO_TO_TOWN).containsPoint(curPos))
            return MapType::TOWN;
    }

    return MapType::NONE;
}

// 进入地图逻辑
void Farm::IntoMap(MapType lastMap)
{
    const Vec2 visibleSize = Director::getInstance()->getVisibleSize();
    MusicManager::getInstance()->playMusicForMap("Farm");

    _map->setScale(TILED_MAP_SCALE);
    _map->setPosition(Vec2::ZERO);
}

// 获取玩家初始位置
Vec2 Farm::getPlayerStartPosition(MapType lastMap)
{
    // 从农舍出来
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

    // 从畜棚出来
    if (lastMap == MapType::BARN) {
        const Rect rect = getObjectRect(GO_TO_BARN);
        if (!rect.equals(Rect::ZERO))
            return rect.origin + rect.size / 2;
    }

    // 从小镇出来
    if (lastMap == MapType::TOWN) {
        const Rect rect = getObjectRect(GO_TO_TOWN);
        if (!rect.equals(Rect::ZERO))
            return rect.origin + rect.size / 2;
    }

    return Vec2(PLAYER_DEFAULT_POS_X, PLAYER_DEFAULT_POS_Y);
}

// 碰撞检测
bool Farm::isCollidable(Vec2 worldPos)
{
    if (!_map) return false;

    const Vec2 tilePos = calMapPos(worldPos);
    const int x = static_cast<int>(tilePos.x);
    const int y = static_cast<int>(tilePos.y);

    const Size mapSize = _map->getMapSize();

    // 边界检测
    if (x < 0 || x >= mapSize.width || y < 0 || y >= mapSize.height)
        return true;

    // 物品碰撞
    if (_farmItemManager && _farmItemManager->isCollidable(tilePos))
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

// 左键点击处理
MouseEvent Farm::onLeftClick(const Vec2& playerPos, const Direction direction, ItemType objects)
{
    Vec2 basePos = this->calMapPos(playerPos);
    ApplyDirectionOffset(basePos, direction);

    const int yOffsets[] = { Y_OFFSET_1, Y_OFFSET_0, Y_OFFSET_NEG_1 };

    for (const int offset : yOffsets) {
        Vec2 checkPos = basePos;
        checkPos.y += static_cast<float>(offset);

        // 钓鱼检测
        if (objects == ItemType::FISHINGROD) {
            auto layer = _map->getLayer(EVENT_LAYER_NAME);
            if (layer) {
                const int tileGID = layer->getTileGIDAt(checkPos);
                if (tileGID) {
                    auto properties = _map->getPropertiesForGID(tileGID).asValueMap();
                    if (properties.find(FISHING_PROPERTY_NAME) != properties.end() &&
                        properties.at(FISHING_PROPERTY_NAME).asBool()) {
                        return MouseEvent::FISHING;
                    }
                }
            }
        }

        // 农场物品交互
        if (_farmItemManager) {
            EnvironmentItem* item = _farmItemManager->getItem(checkPos);
            if (item) {
                auto type = item->getType();

                // 砍树
                if (type == EnvironmentItemType::WOOD && objects == ItemType::AXE) {
                    _farmItemManager->removeItem(checkPos);
                    if (auto inv = InventoryScene::getInstance()) {
                        inv->addItemCount(ItemType::WOOD, static_cast<int>(ITEM_COUNT_1));
                        if (auto skill = SkillLevel::getInstance()) {
                            skill->increaseSkillLevel(SkillType::FORAGING);
                        }
                    }
                    return MouseEvent::USE_TOOL;
                }

                // 割草
                if (type == EnvironmentItemType::GRASS && objects == ItemType::SCYTHE) {
                    _farmItemManager->removeItem(checkPos);
                    if (auto inv = InventoryScene::getInstance()) {
                        inv->addItemCount(ItemType::FIBER, static_cast<int>(ITEM_COUNT_1));
                        if (auto skill = SkillLevel::getInstance()) {
                            skill->increaseSkillLevel(SkillType::FORAGING);
                        }
                    }
                    return MouseEvent::USE_TOOL;
                }
            }
        }

        // 耕种系统交互
        if (_cultivationManager) {
            switch (objects) {
            case ItemType::HOE:
                _cultivationManager->attemptCultivate(checkPos);
                return MouseEvent::USE_TOOL;

            case ItemType::WATERING_CAN:
                _cultivationManager->waterSoil(checkPos);
                return MouseEvent::USE_TOOL;

            case ItemType::PARSNIP_SEED:
                if (_cultivationManager->plantCrop(checkPos, ItemType::PARSNIP))
                    if (auto inv = InventoryScene::getInstance()) {
                        inv->removeItemCount(ItemType::PARSNIP_SEED, static_cast<int>(ITEM_COUNT_1));
                    }
                return MouseEvent::USE_TOOL;

            case ItemType::POTATO_SEED:
                if (_cultivationManager->plantCrop(checkPos, ItemType::POTATO))
                    if (auto inv = InventoryScene::getInstance()) {
                        inv->removeItemCount(ItemType::POTATO_SEED, static_cast<int>(ITEM_COUNT_1));
                    }
                return MouseEvent::USE_TOOL;

            case ItemType::CAULIFLOWER_SEED:
                if (_cultivationManager->plantCrop(checkPos, ItemType::CAULIFLOWER))
                    if (auto inv = InventoryScene::getInstance()) {
                        inv->removeItemCount(ItemType::CAULIFLOWER_SEED, static_cast<int>(ITEM_COUNT_1));
                    }
                return MouseEvent::USE_TOOL;

            default:
                break;
            }
        }
    }

    return MouseEvent::USE_TOOL;
}

// 右键点击处理
MouseEvent Farm::onRightClick(const Vec2& playerPos, const Direction direction)
{
    // 检查是否点击了出货箱
    const Rect saleRect = getObjectRect(SALE_OBJECT_NAME);
    if (saleRect.containsPoint(playerPos)) {
        openShopForNPC();
        return MouseEvent::OPEN_SHOP;
    }

    Vec2 basePos = calMapPos(playerPos);
    ApplyDirectionOffset(basePos, direction);

    const int yOffsets[] = { Y_OFFSET_1, Y_OFFSET_0, Y_OFFSET_NEG_1 };

    for (int offset : yOffsets) {
        Vec2 checkPos = basePos;
        checkPos.y += static_cast<float>(offset);

        // 采集野生作物
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
                            static_cast<int>(ITEM_COUNT_1)
                        );
                        if (auto skill = SkillLevel::getInstance()) {
                            skill->increaseSkillLevel(SkillType::FORAGING);
                        }
                    }
                    return MouseEvent::NONE;
                }
            }
        }

        // 收获农作物
        const ItemType crop = _cultivationManager ? _cultivationManager->harvestCrop(checkPos) : ItemType::NONE;

        if (crop != ItemType::NONE) {
            if (auto inv = InventoryScene::getInstance()) {
                inv->addItemCount(crop, static_cast<int>(ITEM_COUNT_1));
                if (auto skill = SkillLevel::getInstance()) {
                    skill->increaseSkillLevel(SkillType::FARMING);
                }
            }
            return MouseEvent::NONE;
        }
    }

    return MouseEvent::NONE;
}

// 打开出货箱界面
void Farm::openShopForNPC()
{
    // 出货箱只接受特定物品
    std::vector<Item*> itemsToSell;
    std::vector<ItemType> acceptedSellItems;

    // 农作物
    acceptedSellItems.push_back(ItemType::PARSNIP);
    acceptedSellItems.push_back(ItemType::CAULIFLOWER);
    acceptedSellItems.push_back(ItemType::POTATO);

    // 种子
    acceptedSellItems.push_back(ItemType::PARSNIP_SEED);
    acceptedSellItems.push_back(ItemType::CAULIFLOWER_SEED);
    acceptedSellItems.push_back(ItemType::POTATO_SEED);

    // 采集品
    acceptedSellItems.push_back(ItemType::DAFFODILS);
    acceptedSellItems.push_back(ItemType::LEEK);

    // 畜牧产品
    acceptedSellItems.push_back(ItemType::HAY);
    acceptedSellItems.push_back(ItemType::EGG);
    acceptedSellItems.push_back(ItemType::MILK);

    // 料理与鱼
    acceptedSellItems.push_back(ItemType::FRIED_EGG);
    acceptedSellItems.push_back(ItemType::SALAD);
    acceptedSellItems.push_back(ItemType::CARP);

    // 创建界面
    auto runningScene = Director::getInstance()->getRunningScene();
    if (runningScene) {
        auto existingShop = runningScene->getChildByTag(SHOP_MENU_TAG);
        if (existingShop) return;

        auto shopMenu = ShopMenuLayer::create(SHOP_TITLE_SALE, itemsToSell, acceptedSellItems);
        if (shopMenu) {
            shopMenu->setTag(SHOP_MENU_TAG);
            runningScene->addChild(shopMenu, SHOP_MENU_Z_ORDER);
            shopMenu->setCameraMask((unsigned short)CameraFlag::DEFAULT);
        }
    }
}