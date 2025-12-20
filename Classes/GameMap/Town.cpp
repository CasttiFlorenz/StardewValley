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

MouseEvent Town::onRightClick(const Vec2& playerpos, const Direction direction)
{
    const auto EvelynRect = getObjectRect("Evelyn");
    const auto HaleyRect = getObjectRect("Haley");
    const auto SamRect = getObjectRect("Sam");
    const auto PierreRect = getObjectRect("Pierre");
    const auto MarnieRect = getObjectRect("Marnie");

    if (EvelynRect.containsPoint(playerpos))return MouseEvent::CONVERSATION_EVELYN;
    if (HaleyRect.containsPoint(playerpos))return MouseEvent::CONVERSATION_HALEY;
    if (SamRect.containsPoint(playerpos))return MouseEvent::CONVERSATION_SAM;
    if (MarnieRect.containsPoint(playerpos))return MouseEvent::SHOP_MARNIE;
    if (PierreRect.containsPoint(playerpos))return MouseEvent::SHOP_PIERRE;

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
    std::vector<std::string>nameGroup = { "Sam","Haley","Evelyn" };
    for (const auto& name : nameGroup) {

        // 让管理器创建 NPC
        NPCBase* npc = NPCManager::getInstance()->createNPC(name);

        if (npc) {
            npc->setPosition(getObjectRect(name).getMidX(), getObjectRect(name).getMidY());
            // 加到地图上
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