#include "Town.h"
#include "../Inventory/InventoryScene.h"

// Town 场景单例指针
GameMap* Town::_instance = nullptr;

// 创建 Town 对象
Town* Town::create() {
    auto p = new (std::nothrow) Town();
    if (p && p->init()) {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

// 获取 Town 单例
GameMap* Town::getInstance() {
    if (!_instance) {
        _instance = Town::create();
        CC_SAFE_RETAIN(_instance);
    }
    return _instance;
}

// 销毁 Town 单例
void Town::destroyInstance() {
    CC_SAFE_RELEASE_NULL(_instance);
}

// 初始化城镇地图与 NPC
bool Town::init()
{
    if (!Scene::init()) return false;

    _mapName = MapType::TOWN;

    // 加载城镇 TMX 地图
    _map = TMXTiledMap::create(TILED_MAP_TOWN_PATH);
    if (!_map) return false;

    // 隐藏事件层
    if (auto eventLayer = _map->getLayer(EVENT_LAYER_NAME)) {
        eventLayer->setVisible(false);
    }

    // 添加地图并初始化 NPC
    addChild(_map);
    initNPCs();

    return true;
}

// 判断是否离开城镇
MapType Town::leaveMap(const Vec2& curPos, bool isStart, const Direction& direction)
{
    // 向左进入农场
    if (direction == Direction::LEFT) {
        if (getObjectRect(GO_TO_FARM).containsPoint(curPos)) {
            return MapType::FARM;
        }
    }
    return MapType::NONE;
}

// 进入城镇时设置地图参数
void Town::IntoMap(MapType lastMap)
{
    MusicManager::getInstance()->playMusicForMap("Town");
    _map->setScale(TILED_MAP_SCALE);
    _map->setPosition(Vec2::ZERO);
}


// 根据来源地图确定玩家出生点
Vec2 Town::getPlayerStartPosition(MapType lastMap)
{
    if (lastMap == MapType::FARM) {
        const Rect rect = getObjectRect(GO_TO_FARM);
        if (!rect.equals(Rect::ZERO))
            return Vec2(rect.getMidX(), rect.getMidY());
    }
    return Vec2(PLAYER_DEFAULT_POS_X, PLAYER_DEFAULT_POS_Y);
}

// 右键交互（检测 NPC 并打开商店）
MouseEvent Town::onRightClick(const Vec2& playerPos,
    const Direction direction)
{
    // NPC 名称列表
    static const std::vector<std::string> npcNames = {
        NPC_NAME_EVELYN, NPC_NAME_HALEY, NPC_NAME_SAM
    };

    // Shop 名称列表
    static const std::vector<std::string> shopNames = {
        SHOP_NAME_PIERRE, SHOP_NAME_MARNIE
    };

    // 检测是否点击 NPC
    for (const auto& name : npcNames) {
        if (getObjectRect(name).containsPoint(playerPos)) {
            ItemType curItem = ItemType::NONE;
            if (auto const inv = InventoryScene::getInstance()) {
                curItem = inv->getTap();
            }
            interactWithNPC(name, curItem);
            break;
        }
    }

    // 检测是否打开商店
    for (const auto& name : shopNames) {
        if (getObjectRect(name).containsPoint(playerPos)) {
            openShopForNPC(name);
            break;
        }
    }

    return MouseEvent::NONE;
}

void Town::openShopForNPC(const std::string& npcName)
{
    // 1. 防重入检查
    auto runningScene = Director::getInstance()->getRunningScene();
    if (runningScene->getChildByTag(SHOP_MENU_TAG)) return;

    // 2. 准备数据
    std::vector<Item*> itemsToSell;
    std::vector<ItemType>  acceptedSellItems;

    // --- 皮埃尔商店配置 ---
    if (npcName == SHOP_NAME_PIERRE) {
        // --- 进货列表 (玩家买) ---
        // 种子类
        itemsToSell.push_back(new Item(ItemType::PARSNIP_SEED, ITEM_STACK_SIZE_3, ITEM_SCALE_1_0, ITEM_SCALE_0_0, ITEM_TEXTURE_PARSNIP_SEED, ITEM_PRICE_PARSNIP_SEED, ITEM_NAME_PARSNIP_SEEDS));
        itemsToSell.push_back(new Item(ItemType::POTATO_SEED, ITEM_STACK_SIZE_3, ITEM_SCALE_1_0, ITEM_SCALE_0_0, ITEM_TEXTURE_POTATO_SEED, ITEM_PRICE_POTATO_SEED, ITEM_NAME_POTATO_SEEDS));
        itemsToSell.push_back(new Item(ItemType::CAULIFLOWER_SEED, ITEM_STACK_SIZE_3, ITEM_SCALE_1_0, ITEM_SCALE_0_0, ITEM_TEXTURE_CAULIFLOWER_SEED, ITEM_PRICE_CAULIFLOWER_SEED, ITEM_NAME_CAULIFLOWER_SEEDS)); // 新增花椰菜

        // 农业用品
        itemsToSell.push_back(new Item(ItemType::FERTILIZER, ITEM_STACK_SIZE_1, ITEM_SCALE_1_0, ITEM_SCALE_0_0, ITEM_TEXTURE_FERTILIZER, ITEM_PRICE_FERTILIZER, ITEM_NAME_FERTILIZER));

        // 食品 (皮埃尔也会卖一些现成的食物)
        itemsToSell.push_back(new Item(ItemType::SALAD, ITEM_STACK_SIZE_1, ITEM_SCALE_1_0, ITEM_SCALE_0_0, ITEM_TEXTURE_SALAD, ITEM_PRICE_SALAD, ITEM_NAME_SALAD));

        // --- 回收列表 (玩家卖) ---
        // 1. 农作物
        acceptedSellItems.push_back(ItemType::PARSNIP);
        acceptedSellItems.push_back(ItemType::POTATO);
        acceptedSellItems.push_back(ItemType::CAULIFLOWER);

        // 2. 采集品
        acceptedSellItems.push_back(ItemType::DAFFODILS);
        acceptedSellItems.push_back(ItemType::LEEK);
        acceptedSellItems.push_back(ItemType::FIBER); // 纤维也可以稍微卖点钱

        // 3. 动物产品 (也可以卖给皮埃尔)
        acceptedSellItems.push_back(ItemType::EGG);
        acceptedSellItems.push_back(ItemType::MILK);

        // 4. 烹饪与渔获
        acceptedSellItems.push_back(ItemType::FRIED_EGG);
        acceptedSellItems.push_back(ItemType::SALAD);
        acceptedSellItems.push_back(ItemType::CARP); // 假设还没有威利，皮埃尔代收鱼
    }
    // --- 玛尼牧场配置 ---
    else if (npcName == SHOP_NAME_MARNIE) {
        itemsToSell.push_back(new Item(ItemType::HAY, ITEM_STACK_SIZE_1, ITEM_SCALE_0_0, ITEM_SCALE_0_0, ITEM_TEXTURE_HAY, ITEM_PRICE_HAY, ITEM_NAME_HAY));
        // 特殊：动物
        itemsToSell.push_back(new Item(
            ItemType::ANIMAL_CHICKEN_TAG,
            ITEM_STACK_SIZE_1,
            ITEM_SCALE_1_0, ITEM_SCALE_0_0,
            ITEM_TEXTURE_CHICKEN,
            ITEM_PRICE_CHICKEN,
            ITEM_NAME_CHICKEN
        ));

        itemsToSell.push_back(new Item(
            ItemType::ANIMAL_COW_TAG,
            ITEM_STACK_SIZE_1,
            ITEM_SCALE_1_0, ITEM_SCALE_0_0,
            ITEM_TEXTURE_COW,
            ITEM_PRICE_COW,
            ITEM_NAME_COW
        ));

        acceptedSellItems = { ItemType::EGG, ItemType::MILK, ItemType::HAY };
    }

    // 3. 创建 UI
    if (!itemsToSell.empty()) {
        auto shopLayer = ShopMenuLayer::create(npcName, itemsToSell, acceptedSellItems);
        if (shopLayer) {
            shopLayer->setTag(SHOP_MENU_TAG);
            shopLayer->setCameraMask((unsigned short)CameraFlag::DEFAULT);
            runningScene->addChild(shopLayer, ZORDER_SHOP_MENU);
        }
        else {
            // 如果创建失败，必须清理内存防止泄漏
            for (auto item : itemsToSell) delete item;
        }
    }
}

void Town::interactWithNPC(const std::string& npcName, ItemType heldItem)
{
    // 1. 获取 NPC
    NPCBase* npc = getNPCByName(npcName);
    if (!npc) return;

    std::vector<std::string> contentList;

    // 2. 判断是否送礼
    bool isGifting = (heldItem != ItemType::NONE && heldItem > ItemType::FISHINGROD);

    if (isGifting) {
        // --- 送礼分支 ---
        std::string reply = npc->receiveGift(heldItem);
        contentList.push_back(reply);

        // 扣除物品
        if (auto inv = InventoryScene::getInstance()) {
            inv->removeItemCount(heldItem, ITEM_COUNT_1);
        }
    }
    else {
        // --- 对话分支 ---
        contentList = npc->getConversation(false);
    }

    // 3. 显示对话框
    auto runningScene = Director::getInstance()->getRunningScene();
    if (runningScene) {
        auto dialog = DialogueLayer::create();
        if (dialog) {
            dialog->setTag(TAG_DIALOGUE_LAYER); // 防止重复打开
            dialog->showText(npcName, contentList);
            dialog->setCameraMask((unsigned short)CameraFlag::DEFAULT);
            runningScene->addChild(dialog, ZORDER_UI_CONTAINER);
        }
    }
}
void Town::initNPCs()
{
    // 防止重复初始化
    if (!_npcMap.empty()) return;

    // 定义本场景包含的 NPC 列表
    std::vector<std::string> spawnList = { NPC_NAME_SAM, NPC_NAME_HALEY, NPC_NAME_EVELYN };

    for (const auto& name : spawnList) {
        // 从对象层获取出生位置
        Rect spawnRect = getObjectRect(name);

        if (spawnRect.equals(Rect::ZERO)) {
            CCLOG("Warning: Spawn point for NPC '%s' not found in TMX.", name.c_str());
            continue;
        }

        // 通过管理器创建 NPC
        NPCBase* npc = NPCManager::getInstance()->createNPC(name);
        if (npc) {
            npc->setPosition(Vec2(spawnRect.getMidX(), spawnRect.getMidY()));
            // 添加到地图 
            _map->addChild(npc, ZORDER_MAP_OBJECTS);
            _map->setCameraMask(static_cast<unsigned short>(CameraFlag::USER1), true);
            // 存入缓存
            _npcMap[name] = npc;
        }
    }
}
NPCBase* Town::getNPCByName(const std::string& name)
{
    auto it = _npcMap.find(name);
    if (it != _npcMap.end()) {
        return it->second;
    }
    return nullptr;
}