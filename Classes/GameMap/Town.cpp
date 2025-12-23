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
    _map = TMXTiledMap::create("TiledMap/Town/Town.tmx");
    if (!_map) return false;

    // 隐藏事件层
    if (auto eventLayer = _map->getLayer("event")) {
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
    _map->setScale(TILED_MAP_SCALE);
    _map->setPosition(Vec2::ZERO);
}

// 每帧更新（当前未使用）
void Town::update(float dt)
{
}

// 根据来源地图确定玩家出生点
Vec2 Town::getPlayerStartPosition(MapType lastMap)
{
    if (lastMap == MapType::FARM) {
        const Rect rect = getObjectRect(GO_TO_FARM);
        if (!rect.equals(Rect::ZERO))
            return Vec2(rect.getMidX(), rect.getMidY());
    }
    return Vec2(100, 100);
}

// 左键交互（当前无城镇特殊逻辑）
MouseEvent Town::onLeftClick(const Vec2& playerPos,
    const Direction direction,
    ItemType objects)
{
    return MouseEvent::USE_TOOL;
}

// 右键交互（检测 NPC 并打开商店）
MouseEvent Town::onRightClick(const Vec2& playerPos,
    const Direction direction)
{
    // NPC 名称列表
    static const std::vector<std::string> npcNames = {
        "Evelyn", "Haley", "Sam"
    };

    // Shop 名称列表
    static const std::vector<std::string> shopNames = {
        "Pierre", "Marnie"
    };

    // 检测是否点击 NPC
    for (const auto& name : npcNames) {
        if (getObjectRect(name).containsPoint(playerPos)) {
            interactWithNPC(name,InventoryScene::getInstance()->getTap());
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
    std::vector<Item*> itemsToSell;
    std::vector<ItemType > acceptedSellItems;

    // ==========================================
    // 1. 皮埃尔的杂货店 (Pierre's General Store)
    // ==========================================
    if (npcName == "Pierre") {
        // --- 进货列表 (玩家买) ---
        // 种子类
        itemsToSell.push_back(new Item(ItemType::PARSNIP_SEED, 3, 1.0f, 0.0f, "Items/parsnip seed.png", 20, "Parsnip Seeds"));
        itemsToSell.push_back(new Item(ItemType::POTATO_SEED, 3, 1.0f, 0.0f, "Items/potato seed.png", 50, "Potato Seeds"));
        itemsToSell.push_back(new Item(ItemType::CAULIFLOWER_SEED, 3, 1.0f, 0.0f, "Items/cauliflower seed.png", 80, "Cauliflower Seeds")); // 新增花椰菜

        // 农业用品
        itemsToSell.push_back(new Item(ItemType::FERTILIZER, 1, 1.0f, 0.0f, "Items/fertilizer.png", 100, "Fertilizer"));

        // 食品 (皮埃尔也会卖一些现成的食物)
        itemsToSell.push_back(new Item(ItemType::SALAD, 1, 1.0f, 0.0f, "Items/salad.png", 220, "Salad"));

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
    // ==========================================
    // 2. 玛尼的牧场 (Marnie's Ranch)
    // ==========================================
    else if (npcName == "Marnie") {
        // --- 进货列表 (玩家买) ---
        // 玛尼专卖牧场用品
        itemsToSell.push_back(new Item (ItemType::HAY, 1, 1.0f, 0.0f, "Items/hay.png", 50, "Hay"));

        // --- 回收列表 (玩家卖) ---
        // 逻辑上玛尼更愿意收购动物制品
        acceptedSellItems.push_back(ItemType::EGG);
        acceptedSellItems.push_back(ItemType::MILK);
        acceptedSellItems.push_back(ItemType::HAY); // 也可以把多余的干草卖回去
        itemsToSell.push_back(new Item(
           ItemType::ANIMAL_CHICKEN_TAG,
            1,
            1.0f, 0.0f,
            "Items/chicken.png",
            300,
            "Chicken"
        ));

        // 3. 伪造物品 (牛)
        itemsToSell.push_back(new Item(
            ItemType::ANIMAL_COW_TAG,    
            1,
            1.0f, 0.0f,
            "Items/cow.png",
            500,
            "Cow"
        ));
    }
    if (!itemsToSell.empty()) {
        auto runningScene = Director::getInstance()->getRunningScene();
        if (runningScene) {
            const int SHOP_MENU_TAG = 9999;

            // 防止重复打开
            auto existingShop = runningScene->getChildByTag(SHOP_MENU_TAG);
            if (existingShop) {                                                             
                return;
            }

            auto shopMenu = ShopMenuLayer::create(npcName, itemsToSell, acceptedSellItems);
            if (shopMenu) {
                shopMenu->setTag(SHOP_MENU_TAG);
                runningScene->addChild(shopMenu, 999);
                shopMenu->setCameraMask((unsigned short)CameraFlag::DEFAULT);
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

void Town::interactWithNPC(const std::string& npcName, ItemType  heldItem)
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
    bool isGifting = (heldItem != ItemType::NONE && heldItem > ItemType::FISHINGROD);

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

