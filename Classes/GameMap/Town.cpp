/****************************************************************
 * Project Name:  StardewValley
 * File Name:     Town.cpp
 * File Function: Town类的实现
 * Author:        赵睿妍
 * Update Date:   2025/12/14
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

MouseEvent Town::onRightClick(const Vec2& playerPos, const Direction direction)
{
    const Rect EvelynRect = getObjectRect("Evelyn");
    const Rect HaleyRect = getObjectRect("Haley");
    const Rect MarnieRect = getObjectRect("Marnie");
    const Rect PierreRect = getObjectRect("Pierre");
    const Rect SamRect = getObjectRect("Sam");


    if (EvelynRect.containsPoint(playerPos)) {
        return MouseEvent::CONVERSATION_EVELYN;
    }
    if (HaleyRect.containsPoint(playerPos)) {
        return MouseEvent::CONVERSATION_HALEY;
    }
    if (MarnieRect.containsPoint(playerPos)) {
        return MouseEvent::SHOP_MARNIE;
    }
    if (PierreRect.containsPoint(playerPos)) {
        return MouseEvent::SHOP_PIERRE;
    }
    if (SamRect.containsPoint(playerPos)) {
        return MouseEvent::CONVERSATION_HALEY;
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
