/****************************************************************
 * Project Name:  StardewValley
 * File Name:     Town.cpp
 * File Function: Town类的实现
 * Author:        郭芷烟
 * Update Date:   2025/12/28
 * License:       MIT License
 ****************************************************************/
#include "Town.h"
#include "../Inventory/InventoryScene.h"

// 单例实例
GameMap* Town::_instance = nullptr;

// 创建实例
Town* Town::create() {
    auto p = new (std::nothrow) Town();
    if (p && p->init()) {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

// 获取单例
GameMap* Town::getInstance() {
    if (!_instance) {
        _instance = Town::create();
        CC_SAFE_RETAIN(_instance);
    }
    return _instance;
}

// 销毁单例
void Town::destroyInstance() {
    CC_SAFE_RELEASE_NULL(_instance);
}

// 初始化
bool Town::init()
{
    if (!Scene::init()) return false;

    _mapName = MapType::TOWN;

    // 加载地图
    _map = TMXTiledMap::create(TILED_MAP_TOWN_PATH);
    if (!_map) return false;

    // 隐藏事件层
    if (auto eventLayer = _map->getLayer(EVENT_LAYER_NAME)) {
        eventLayer->setVisible(false);
    }

    // 初始化 NPC
    addChild(_map);
    initNPCs();

    return true;
}

// 离开地图逻辑
MapType Town::leaveMap(const Vec2& curPos, bool isStart, const Direction& direction)
{
    // 向左移动返回农场
    if (direction == Direction::LEFT) {
        if (getObjectRect(GO_TO_FARM).containsPoint(curPos)) {
            if (auto npc = NPCManager::getInstance())
                npc->stopAllNPCAnimations();
            return MapType::FARM;
        }
    }
    return MapType::NONE;
}

// 进入地图逻辑
void Town::IntoMap(MapType lastMap)
{
    if(auto music=MusicManager::getInstance())
        music->playMusicForMap("Town");

    if (auto npc = NPCManager::getInstance())
        npc->resumeAllNPCAnimations();

    _map->setScale(TILED_MAP_SCALE);
    _map->setPosition(Vec2::ZERO);
}

// 获取玩家初始位置
Vec2 Town::getPlayerStartPosition(MapType lastMap)
{
    if (lastMap == MapType::FARM) {
        const Rect rect = getObjectRect(GO_TO_FARM);
        if (!rect.equals(Rect::ZERO))
            return Vec2(rect.getMidX(), rect.getMidY());
    }
    return Vec2(PLAYER_DEFAULT_POS_X, PLAYER_DEFAULT_POS_Y);
}

// 右键点击处理
MouseEvent Town::onRightClick(const Vec2& playerPos,
    const Direction direction)
{
    // NPC 名称列表
    static const std::vector<std::string> npcNames = {
        NPC_NAME_EVELYN, NPC_NAME_HALEY, NPC_NAME_SAM
    };

    // 商店名称列表
    static const std::vector<std::string> shopNames = {
        SHOP_NAME_PIERRE, SHOP_NAME_MARNIE
    };

    // 检查是否与 NPC 交互
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

    // 检查是否打开商店
    for (const auto& name : shopNames) {
        if (getObjectRect(name).containsPoint(playerPos)) {
            openShopForNPC(name);
            break;
        }
    }

    return MouseEvent::NONE;
}

// 打开商店逻辑
void Town::openShopForNPC(const std::string& npcName)
{
    auto runningScene = Director::getInstance()->getRunningScene();
    if (!runningScene || runningScene->getChildByTag(SHOP_MENU_TAG)) return;

    std::vector<Item*> itemsToSell;
    std::vector<ItemType> acceptedSellItems;

    // 皮埃尔商店
    if (npcName == SHOP_NAME_PIERRE) {
        // 种子
        itemsToSell.push_back(new Item(ItemType::PARSNIP_SEED, ITEM_STACK_SIZE_3, ITEM_SCALE_1_0, ITEM_SCALE_0_0, ITEM_TEXTURE_PARSNIP_SEED, ITEM_PRICE_PARSNIP_SEED, ITEM_NAME_PARSNIP_SEEDS));
        itemsToSell.push_back(new Item(ItemType::POTATO_SEED, ITEM_STACK_SIZE_3, ITEM_SCALE_1_0, ITEM_SCALE_0_0, ITEM_TEXTURE_POTATO_SEED, ITEM_PRICE_POTATO_SEED, ITEM_NAME_POTATO_SEEDS));
        itemsToSell.push_back(new Item(ItemType::CAULIFLOWER_SEED, ITEM_STACK_SIZE_3, ITEM_SCALE_1_0, ITEM_SCALE_0_0, ITEM_TEXTURE_CAULIFLOWER_SEED, ITEM_PRICE_CAULIFLOWER_SEED, ITEM_NAME_CAULIFLOWER_SEEDS));

        // 肥料与食物
        itemsToSell.push_back(new Item(ItemType::FERTILIZER, ITEM_STACK_SIZE_1, ITEM_SCALE_1_0, ITEM_SCALE_0_0, ITEM_TEXTURE_FERTILIZER, ITEM_PRICE_FERTILIZER, ITEM_NAME_FERTILIZER));
        itemsToSell.push_back(new Item(ItemType::SALAD, ITEM_STACK_SIZE_1, ITEM_SCALE_1_0, ITEM_SCALE_0_0, ITEM_TEXTURE_SALAD, ITEM_PRICE_SALAD, ITEM_NAME_SALAD));

        // 收购列表
        acceptedSellItems = {
            ItemType::PARSNIP, ItemType::POTATO, ItemType::CAULIFLOWER,
            ItemType::DAFFODILS, ItemType::LEEK, ItemType::FIBER,
            ItemType::EGG, ItemType::MILK,
            ItemType::FRIED_EGG, ItemType::SALAD, ItemType::CARP
        };
    }
    // 玛妮商店
    else if (npcName == SHOP_NAME_MARNIE) {
        itemsToSell.push_back(new Item(ItemType::HAY, ITEM_STACK_SIZE_1, ITEM_SCALE_0_0, ITEM_SCALE_0_0, ITEM_TEXTURE_HAY, ITEM_PRICE_HAY, ITEM_NAME_HAY));

        // 动物购买凭证
        itemsToSell.push_back(new Item(ItemType::ANIMAL_CHICKEN_TAG, ITEM_STACK_SIZE_1, ITEM_SCALE_1_0, ITEM_SCALE_0_0, ITEM_TEXTURE_CHICKEN, ITEM_PRICE_CHICKEN, ITEM_NAME_CHICKEN));
        itemsToSell.push_back(new Item(ItemType::ANIMAL_COW_TAG, ITEM_STACK_SIZE_1, ITEM_SCALE_1_0, ITEM_SCALE_0_0, ITEM_TEXTURE_COW, ITEM_PRICE_COW, ITEM_NAME_COW));

        // 收购列表
        acceptedSellItems = { ItemType::EGG, ItemType::MILK, ItemType::HAY };
    }

    // 创建商店界面
    if (!itemsToSell.empty()) {
        auto shopLayer = ShopMenuLayer::create(npcName, itemsToSell, acceptedSellItems);
        if (shopLayer) {
            shopLayer->setTag(SHOP_MENU_TAG);
            shopLayer->setCameraMask(static_cast<unsigned short>(CameraFlag::DEFAULT));
            runningScene->addChild(shopLayer, ZORDER_SHOP_MENU);
        }
        else {
            // 创建失败清理内存
            for (auto item : itemsToSell) delete item;
        }
    }
}

// NPC 交互逻辑
void Town::interactWithNPC(const std::string& npcName, ItemType heldItem)
{
    NPCBase* npc = getNPCByName(npcName);
    if (!npc) return;

    std::vector<std::string> contentList;
    bool isGifting = (heldItem != ItemType::NONE && heldItem > ItemType::FISHINGROD);

    if (isGifting) {
        // 送礼
        std::string reply = npc->receiveGift(heldItem);
        contentList.push_back(reply);

        if (auto inv = InventoryScene::getInstance()) {
            inv->removeItemCount(heldItem, ITEM_COUNT_1);
        }
    }
    else {
        // 对话
        contentList = npc->getConversation(false);
    }

    // 显示对话框
    auto runningScene = Director::getInstance()->getRunningScene();
    if (runningScene) {
        auto dialog = DialogueLayer::create();
        if (dialog) {
            dialog->setTag(TAG_DIALOGUE_LAYER);
            dialog->showText(npcName, contentList);
            dialog->setCameraMask(static_cast<unsigned short>(CameraFlag::DEFAULT));
            runningScene->addChild(dialog, ZORDER_UI_CONTAINER);
        }
    }
}

// 初始化 NPC
void Town::initNPCs()
{
    if (!_npcMap.empty()) return;

    std::vector<std::string> spawnList = { NPC_NAME_SAM, NPC_NAME_HALEY, NPC_NAME_EVELYN };

    for (const auto& name : spawnList) {
        Rect spawnRect = getObjectRect(name);

        if (spawnRect.equals(Rect::ZERO)) {
            CCLOG("Warning: Spawn point for NPC '%s' not found in TMX.", name.c_str());
            continue;
        }

        NPCBase* npc = NPCManager::getInstance()->createNPC(name);
        if (npc) {
            npc->setPosition(Vec2(spawnRect.getMidX(), spawnRect.getMidY()));
            _map->addChild(npc, ZORDER_MAP_OBJECTS);
            _map->setCameraMask(static_cast<unsigned short>(CameraFlag::USER1), true);
            _npcMap[name] = npc;
        }
    }
}

// 获取 NPC 对象
NPCBase* Town::getNPCByName(const std::string& name)
{
    auto it = _npcMap.find(name);
    if (it != _npcMap.end()) {
        return it->second;
    }
    return nullptr;

}
