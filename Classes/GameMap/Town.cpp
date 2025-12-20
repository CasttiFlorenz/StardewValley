/****************************************************************
 * Project Name:  StardewValley
 * File Name:     Town.cpp
 * File Function: Town类的实现
 * Author:        郭芷烟、赵睿妍
 * Update Date:   2025/12/19
 * License:       MIT License
 ****************************************************************/

#include "Town.h"

GameMap* Town::_instance = nullptr;

GameMap* Town::getInstance() {
    if (!_instance) {
        _instance = Town::create();
        CC_SAFE_RETAIN(_instance);
    }
    return _instance;
}

void Town::destroyInstance() {
    CC_SAFE_RELEASE_NULL(_instance);
}

bool Town::init()
{
    if (!Scene::init())
    {
        return false;
    }
    _mapName = "Town";
    _map = TMXTiledMap::create("TiledMap/Town/Town.tmx");
    if (_map == nullptr)
    {
        return false;
    }
    auto eventLayer = _map->getLayer("event");
    if (eventLayer) {
        eventLayer->setVisible(false);
    }
    this->addChild(_map);
    initNPCs();
    this->scheduleUpdate();
    return true;
}

std::string Town::getNewMap(const Vec2& curPos, bool isStart, const Direction& direction)
{
    if (direction == Direction::LEFT) {
        const Rect goToFarm = getObjectRect("goToFarm");
        if (goToFarm.containsPoint(curPos)) {
            return "Farm";
        }
    }

    return "";
}

void Town::setStartPosition(std::string lastMap)
{
    _map->setScale(TILED_MAP_SCALE);
    _map->setPosition(Vec2::ZERO);
}

void Town::update(float dt)
{
}

Vec2 Town::getPlayerStartPosition(std::string lastMap)
{
    if (lastMap == "Farm") {
        const Rect goToFarmRect = getObjectRect("goToFarm");
        if (!goToFarmRect.equals(Rect::ZERO))
            return Vec2(goToFarmRect.getMidX(), goToFarmRect.getMidY());
    }
    return Vec2(100, 100);
}

MouseEvent Town::onLeftClick(const Vec2& playerPos, const Direction direction, Objects objects)
{
    return MouseEvent::USE_TOOL;
}
MouseEvent Town::onRightClick(const Vec2& pos, const Direction direction)
{
    auto objectGroup = _map->getObjectGroup("object");

    if (objectGroup) {
        auto& objects = objectGroup->getObjects();

        for (const auto& obj : objects) {
            ValueMap dict = obj.asValueMap();

            std::string name = dict["name"].asString();
            float x = dict["x"].asFloat();
            float y = dict["y"].asFloat();
            float w = dict["width"].asFloat();
            float h = dict["height"].asFloat();

            Rect rect(x, y, w, h);

            if (rect.containsPoint(pos)) {

                // --- NPC ---
                if (name == "Evelyn") {
                    return MouseEvent::CONVERSATION_EVELYN;
                }
                if (name == "Haley") {
                    return MouseEvent::CONVERSATION_HALEY;
                }
                if (name == "Sam") {
                    return MouseEvent::CONVERSATION_SAM;
                }

                // --- 商店 ---
                if (name == "Pierre") {
                    return MouseEvent::SHOP_PIERRE;
                }
                if (name == "Marnie") {
                    return MouseEvent::SHOP_MARNIE;
                }
            }
        }
    }
    else {
        CCLOG("Town Error: 找不到名为 'object' 的图层！");
    }

    return MouseEvent::NONE;
}

void Town::openShopForNPC(const std::string& npcName)
{
    std::vector<Item*> itemsToSell;
    std::vector<Objects> acceptedSellItems;
    // 根据 NPC 进货
    if (npcName == "Pierre") {
        itemsToSell.push_back(new Item(Objects::FERTILIZER, 1, 1.0f, 0.0f, "Items/fertilizer.png", 100, "Fertilizer"));
        itemsToSell.push_back(new Item(Objects::PARSNIP_SEED, 3, 1.0f, 0.0f, "Items/parsnip seed.png", 20, "Parsnip Seeds"));
        itemsToSell.push_back(new Item(Objects::POTATO_SEED, 3, 1.0f, 0.0f, "Items/potato seed.png", 50, "Potato Seeds"));
        itemsToSell.push_back(new Item(Objects::SALAD, 3, 1.0f, 0.0f, "Items/salad.png", 220, "Salad"));
        itemsToSell.push_back(new Item(Objects::HAY, 1, 1.0f, 0.0f, "Items/hay.png", 50, "Hay"));
        acceptedSellItems.push_back(Objects::PARSNIP);
        acceptedSellItems.push_back(Objects::CAULIFLOWER);
        acceptedSellItems.push_back(Objects::POTATO);
        acceptedSellItems.push_back(Objects::DAFFODILS); // 采集品通常也收
        acceptedSellItems.push_back(Objects::LEEK);
        acceptedSellItems.push_back(Objects::PARSNIP_SEED);
        acceptedSellItems.push_back(Objects::CAULIFLOWER_SEED);
        acceptedSellItems.push_back(Objects::POTATO_SEED);
        acceptedSellItems.push_back(Objects::FERTILIZER);
        acceptedSellItems.push_back(Objects::EGG);       // 也可以收一些农产品
        acceptedSellItems.push_back(Objects::MILK);
        acceptedSellItems.push_back(Objects::SALAD);     // 烹饪
    }
    else if (npcName == "Marnie") {
       itemsToSell.push_back(new Item(Objects::HAY, 1, 1.0f, 0.0f, "Items/hay.png", 50, "Hay"));
    }
    if (!itemsToSell.empty()) {
        auto runningScene = Director::getInstance()->getRunningScene();
        if (runningScene) {
            const int SHOP_MENU_TAG = 9999;

            // 查找是否已有该Tag的子节点
            auto existingShop = runningScene->getChildByTag(SHOP_MENU_TAG);
            if (existingShop) {
                return;
            }
            auto shopMenu = ShopMenuLayer::create(itemsToSell, acceptedSellItems);
            if (shopMenu) {
                shopMenu->setTag(SHOP_MENU_TAG);
                runningScene->addChild(shopMenu, 999);
                shopMenu->setCameraMask((unsigned short)CameraFlag::DEFAULT);

                CCLOG("成功打开商店菜单");
            }
        }
    }
}

void Town::initNPCs()
{
    // 1. 获取 Tiled 地图里的对象层
    auto objectGroup = _map->getObjectGroup("object");
    if (!objectGroup) return;

    for (const auto& obj : objectGroup->getObjects()) {
        ValueMap dict = obj.asValueMap();
        std::string name = dict["name"].asString();
        float x = dict["x"].asFloat();
        float y = dict["y"].asFloat();

        // 2. 让管理器创建 NPC
        NPCBase* npc = NPCManager::getInstance()->createNPC(name);

        if (npc) {
            // 3. 设置位置 (根据 Tiled 坐标)
            npc->setPosition(x + npc->getContentSize().width / 2, y + npc->getContentSize().height / 2);

            // 4. 加到地图上
            _map->addChild(npc, 5);

            _npcMap[name] = npc;
        }
    }
}

NPCBase* Town::getNPCByName(const std::string& name) {
    if (_npcMap.find(name) != _npcMap.end()) return _npcMap[name];
    return nullptr;
}

void Town::interactWithNPC(const std::string& npcName, Objects heldItem)
{
    // 1. 检查是否已经有对话框在显示，如果有，直接跳出，不执行任何逻辑
    auto runningScene = Director::getInstance()->getRunningScene();
    if (!runningScene || runningScene->getChildByName("DialogueLayer")) {
        return;
    }

    // 2. 获取 NPC 对象
    NPCBase* npc = getNPCByName(npcName);
    if (!npc) {
        CCLOG("Error: 找不到 NPC %s", npcName.c_str());
        return;
    }

    std::vector<std::string> dialogContentList;

    // 3. 判断是送礼还是对话
    bool isGifting = (heldItem != Objects::NONE && heldItem > Objects::FISHINGROD);

    if (isGifting) {
        // --- 送礼逻辑 ---
        CCLOG("Gifting logic triggered for: %s", npcName.c_str()); // 调试用

        std::string giftReply = npc->receiveGift(heldItem);
        dialogContentList.push_back(giftReply);

        // 在这里扣除物品是安全的，因为上面已经拦截了重复调用
        InventoryScene::getInstance()->removeItemCount(heldItem, 1);
    }
    else {
        // --- 对话逻辑 ---
        dialogContentList = npc->getConversation(false);
    }

    // 4. 创建并显示 UI
    auto dialog = DialogueLayer::create();
    if (dialog) {
        dialog->setName("DialogueLayer");
        dialog->showText(npcName, dialogContentList);
        runningScene->addChild(dialog, 9999);
        dialog->setCameraMask((unsigned short)CameraFlag::DEFAULT);
    }
}