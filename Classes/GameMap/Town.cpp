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

// 根据 NPC 名称打开对应商店
void Town::openShopForNPC(const std::string& npcName)
{
    std::vector<Item*> itemsToSell;
    std::vector<ItemType> acceptedSellItems;

    // Pierre 商店
    if (npcName == "Pierre") {
        itemsToSell = {
            new Item(ItemType::FERTILIZER, 1, 1.0f, 0.0f, "Items/fertilizer.png", 100, "Fertilizer"),
            new Item(ItemType::PARSNIP_SEED, 3, 1.0f, 0.0f, "Items/parsnip seed.png", 20, "Parsnip Seeds"),
            new Item(ItemType::POTATO_SEED, 3, 1.0f, 0.0f, "Items/potato seed.png", 50, "Potato Seeds"),
            new Item(ItemType::SALAD, 3, 1.0f, 0.0f, "Items/salad.png", 220, "Salad"),
            new Item(ItemType::HAY, 1, 1.0f, 0.0f, "Items/hay.png", 50, "Hay")
        };

        acceptedSellItems = {
            ItemType::PARSNIP, ItemType::CAULIFLOWER, ItemType::POTATO,
            ItemType::DAFFODILS, ItemType::LEEK,
            ItemType::PARSNIP_SEED, ItemType::CAULIFLOWER_SEED, ItemType::POTATO_SEED,
            ItemType::FERTILIZER, ItemType::EGG, ItemType::MILK, ItemType::SALAD
        };
    }
    // Marnie 商店
    else if (npcName == "Marnie") {
        itemsToSell.push_back(
            new Item(ItemType::HAY, 1, 1.0f, 0.0f, "Items/hay.png", 50, "Hay")
        );
    }

    if (itemsToSell.empty()) return;

    // 打开商店 UI（防止重复）
    if (auto runningScene = Director::getInstance()->getRunningScene()) {
        constexpr int SHOP_MENU_TAG = 9999;

        if (runningScene->getChildByTag(SHOP_MENU_TAG)) return;

        if (auto shopMenu = ShopMenuLayer::create(itemsToSell, acceptedSellItems)) {
            shopMenu->setTag(SHOP_MENU_TAG);
            runningScene->addChild(shopMenu, 999);
            shopMenu->setCameraMask((unsigned short)CameraFlag::DEFAULT);
        }
    }
}

// 初始化城镇 NPC
void Town::initNPCs()
{
    const std::vector<std::string> npcNames = { "Sam", "Haley", "Evelyn" };

    for (const auto& name : npcNames) {
        NPCBase* npc = NPCManager::getInstance()->createNPC(name);
        if (!npc) continue;

        const Rect rect = getObjectRect(name);
        npc->setPosition(rect.getMidX(), rect.getMidY());
        _map->addChild(npc, 5);
        _npcMap[name] = npc;
    }
}

// 根据名字获取 NPC
NPCBase* Town::getNPCByName(const std::string& name)
{
    auto it = _npcMap.find(name);
    return (it != _npcMap.end()) ? it->second : nullptr;
}

// 与 NPC 对话或送礼
void Town::interactWithNPC(const std::string& npcName, ItemType heldItem)
{
    // 防止重复弹出对话框
    auto runningScene = Director::getInstance()->getRunningScene();
    if (!runningScene || runningScene->getChildByName("DialogueLayer")) return;

    // 获取 NPC
    NPCBase* npc = getNPCByName(npcName);
    if (!npc) return;

    std::vector<std::string> dialogContent;

    // 判断是否送礼
    const bool isGifting = (heldItem != ItemType::NONE && heldItem > ItemType::FISHINGROD);

    if (isGifting) {
        dialogContent.push_back(npc->receiveGift(heldItem));
        InventoryScene::getInstance()->removeItemCount(heldItem, 1);
    }
    else {
        dialogContent = npc->getConversation(false);
    }

    // 显示对话框
    if (auto dialog = DialogueLayer::create()) {
        dialog->setName("DialogueLayer");
        dialog->showText(npcName, dialogContent);
        runningScene->addChild(dialog, 9999);
        dialog->setCameraMask((unsigned short)CameraFlag::DEFAULT);
    }
}
