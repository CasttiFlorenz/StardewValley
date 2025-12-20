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
    // 1. 获取名为 "object" 的对象层 (严格匹配你的 Tiled 设置)
    auto objectGroup = _map->getObjectGroup("object");

    if (objectGroup) {
        // 2. 遍历这一层里所有的东西
        auto& objects = objectGroup->getObjects();

        for (const auto& obj : objects) {
            ValueMap dict = obj.asValueMap();

            std::string name = dict["name"].asString();
            float x = dict["x"].asFloat();
            float y = dict["y"].asFloat();
            float w = dict["width"].asFloat();
            float h = dict["height"].asFloat();

            // 直接构建矩形 (这是相对于地图的局部坐标，绝对准确)
            Rect rect(x, y, w, h);

            // 3. 判定点击
            if (rect.containsPoint(pos)) {

                // 打印日志：看看你到底点到了谁？
                // 如果这里打印了名字，说明点击成功了；如果没打印，说明名字或坐标不对。
                CCLOG("Town: 🎯 命中对象 -> '%s'", name.c_str());

                // ====================================================
                // 4. 名字匹配 (注意 Tiled 里首字母通常要大写)
                // ====================================================

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

// 辅助函数：根据名字获取 NPC 指针
NPCBase* Town::getNPCByName(const std::string& name) {
    if (_npcMap.find(name) != _npcMap.end()) return _npcMap[name];
    return nullptr;
}


void Town::interactWithNPC(const std::string& npcName, Objects heldItem)
{
    // 1. 获取 NPC 对象
    NPCBase* npc = getNPCByName(npcName);
    if (!npc) {
        CCLOG("Error: 找不到 NPC %s", npcName.c_str());
        return;
    }

    std::string dialogText = "";

    // 2. 判断是【送礼】还是【对话】
    // 假设 Tag > 5 的物品算作礼物 (排除锄头、斧子等工具)
    bool isGifting = (heldItem != Objects::NONE && heldItem > Objects::FISHINGROD);

    if (isGifting) {
        // --- 送礼逻辑 ---
        // 调用 NPC 的接收礼物函数
        dialogText = npc->receiveGift(heldItem);
        InventoryScene::getInstance()->removeItemCount(heldItem, 1);
    }
    else {
        // --- 普通对话逻辑 ---
        dialogText = npc->getConversation(false);
    }

    // 3. 创建并显示 UI
    auto runningScene = Director::getInstance()->getRunningScene();
    if (runningScene) {
        // 防止重复打开
        if (runningScene->getChildByName("DialogueLayer")) return;

        auto dialog = DialogueLayer::create();
        if (dialog) {
            dialog->setName("DialogueLayer"); // 设个名字防止重复
            dialog->showText(npcName, dialogText);

            // 加到场景最高层
            runningScene->addChild(dialog, 9999);
            // 强制固定在屏幕上
            dialog->setCameraMask((unsigned short)CameraFlag::DEFAULT);
        }
    }
}
