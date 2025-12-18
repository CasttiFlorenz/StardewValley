/****************************************************************
 * Project Name:  StardewValley
 * File Name:     Farm.cpp
 * File Function: Farm类的实现
 * Author:        郭芷烟
 * Update Date:   2025/12/13
 * License:       MIT License
 ****************************************************************/

#include "Farm.h"

GameMap* Farm::_instance = nullptr;

GameMap* Farm::getInstance() {
    if (!_instance) {
        _instance = Farm::create();
        CC_SAFE_RETAIN(_instance);
    }
    return _instance;
}

void Farm::destroyInstance() {
    CC_SAFE_RELEASE_NULL(_instance);
}

Farm::~Farm() {
    CC_SAFE_RELEASE(_farmItemManager);
}

bool Farm::init()
{
    if (!Scene::init())
    {
        return false;
    }

    _mapName = "Farm";

    _map = TMXTiledMap::create("TiledMap/Farm/Farm.tmx");

    if (_map == nullptr)
    {
        CCLOG("Failed to load map: TiledMap/Farm/Farm.tmx");
        return false;
    }

    auto eventLayer = _map->getLayer("event");
    if (eventLayer) {
        eventLayer->setVisible(false);
    }

    _farmItemManager = FarmItemManager::create(this);
    CC_SAFE_RETAIN(_farmItemManager);

    this->addChild(_map, 0);
    this->scheduleUpdate();

    return true;
}

std::string Farm::getNewMap(const Vec2& curPos, bool isStart, const Direction& direction)
{
    if (direction == Direction::UP) {
        const Rect goToHouse = getObjectRect("goToHouse");
        if (goToHouse.containsPoint(curPos)) {
            return "FarmHouse";
        }
        const Rect goToMines = getObjectRect("goToMines");
        if (goToMines.containsPoint(curPos)) {
            return "Mines";
        }
        const Rect goToBarn = getObjectRect("goToBarn");
        if (goToBarn.containsPoint(curPos)) {
            return "Barn";
        }
    }
    if (direction == Direction::RIGHT) {
        const Rect goToTown = getObjectRect("goToTown");
        if (goToTown.containsPoint(curPos)) {
            return "Town";
        }
    }


    return "";
}

void Farm::setStartPosition(std::string lastMap)
{
    const Vec2 visibleSize = Director::getInstance()->getVisibleSize();
    _map->setScale(TILED_MAP_SCALE);

    _map->setPosition(Vec2::ZERO);
}

void Farm::update(float dt)
{
}

Vec2 Farm::getPlayerStartPosition(std::string lastMap)
{
    if (lastMap == "FarmHouse") {
        const Rect startRect = this->getObjectRect("goToHouse");
        if (!startRect.equals(Rect::ZERO)) {

            return Vec2(startRect.getMidX(), startRect.getMidY());
        }
    }
    if (lastMap == "Mines") {
        const Rect goToMinesRect = this->getObjectRect("goToMines");
        if (!goToMinesRect.equals(Rect::ZERO)) {

            return Vec2(goToMinesRect.getMidX(), goToMinesRect.getMidY());
        }
    }
    if (lastMap == "Barn") {
        const Rect goToBarnRect = this->getObjectRect("goToBarn");
        if (!goToBarnRect.equals(Rect::ZERO)) {

            return Vec2(goToBarnRect.getMidX(), goToBarnRect.getMidY());
        }
    }
    if (lastMap == "Town") {
        const Rect goToTownRect = this->getObjectRect("goToTown");
        if (!goToTownRect.equals(Rect::ZERO)) {

            return Vec2(goToTownRect.getMidX(), goToTownRect.getMidY());
        }
    }

    return Vec2(100, 100);
}

bool Farm::isCollidable(Vec2 worldPos)
{
    if (!_map) return false;

    const Vec2 tilePos = calMapPos(worldPos);
    const int x = static_cast<int>(tilePos.x);
    const int y = static_cast<int>(tilePos.y);

    const Size mapSize = _map->getMapSize();

    if (x < 0 || x >= mapSize.width || y < 0 || y >= mapSize.height) {
        return true;
    }

    if (_farmItemManager && _farmItemManager->hasItem(tilePos))
        return true;

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

MouseEvent Farm::onLeftClick(const Vec2& playerPos, const Direction direction, Objects objects)
{
    // 1. 计算基准瓦片坐标
    Vec2 basePos = this->calMapPos(playerPos);

    // 2. 根据朝向调整基准坐标
    switch (direction) {
    case Direction::DOWN:  basePos.y++; break;
    case Direction::UP:    basePos.y--; break;
    case Direction::LEFT:  basePos.x--; break;
    case Direction::RIGHT: basePos.x++; break;
    default: break;
    }

    // 3. 定义检测偏移量顺序：原位置(0)，上方(-1)，下方(+1)
    // 对应原代码逻辑：tiledPos -> tiledPos.y-- -> tiledPos.y+=2
    const int yOffsets[] = { 1,0, -1 };

    // 4. 遍历检测
    for (int offset : yOffsets) {
        Vec2 checkPos = basePos;
        checkPos.y += offset;

        EnvironmentItem* item = _farmItemManager->getItem(checkPos);
        if (item) {
            auto type = item->getType();
            if (type == EnvironmentItemType::WOOD&&objects==Objects::AXE) {
                _farmItemManager->removeItem(checkPos);
                return MouseEvent::GET_WOOD;
            }
            else if (type == EnvironmentItemType::GRASS&& objects == Objects::SCYTHE) {
                _farmItemManager->removeItem(checkPos);
                return MouseEvent::GET_GRASS;
            }
        }
    }

    return MouseEvent::USE_TOOL;
}

MouseEvent Farm::onRightClick(const Vec2& playerPos, const Direction direction)
{
    const Rect saleRect = getObjectRect("sale");

    if (saleRect.containsPoint(playerPos)) {
        return MouseEvent::SHOP_SALE;
    }

    // 1. 计算基准瓦片坐标
    Vec2 basePos = this->calMapPos(playerPos);

    // 2. 根据朝向调整基准坐标
    switch (direction) {
    case Direction::DOWN:  basePos.y++; break;
    case Direction::UP:    basePos.y--; break;
    case Direction::LEFT:  basePos.x--; break;
    case Direction::RIGHT: basePos.x++; break;
    default: break;
    }

    // 3. 定义检测偏移量顺序：原位置(0)，上方(-1)，下方(+1)
    // 对应原代码逻辑：tiledPos -> tiledPos.y-- -> tiledPos.y+=2
    const int yOffsets[] = { 1,0, -1 };

    // 4. 遍历检测
    for (int offset : yOffsets) {
        Vec2 checkPos = basePos;
        checkPos.y += offset;

        EnvironmentItem* item = _farmItemManager->getItem(checkPos);
        if (item) {
            auto type = item->getType();
            if (type == EnvironmentItemType::LEEK) {
                _farmItemManager->removeItem(checkPos);
                return MouseEvent::GET_LEEK;
            }
            else if (type == EnvironmentItemType::DAFFODILS) {
                _farmItemManager->removeItem(checkPos);
                return MouseEvent::GET_DAFFODILS;
            }
        }
    }

    return MouseEvent::NONE;
}


void Farm::openShopForNPC(const std::string& npcName)
{
    std::vector<Item*> itemsToSell;
    std::vector<Objects> acceptedSellItems;
    acceptedSellItems.push_back(Objects::STONE);
    acceptedSellItems.push_back(Objects::WOOD);
    acceptedSellItems.push_back(Objects::COPPER_ORE);
    acceptedSellItems.push_back(Objects::PARSNIP);
    acceptedSellItems.push_back(Objects::CAULIFLOWER);
    acceptedSellItems.push_back(Objects::POTATO);
    acceptedSellItems.push_back(Objects::FIBER);
    acceptedSellItems.push_back(Objects::DAFFODILS);
    acceptedSellItems.push_back(Objects::LEEK);
    acceptedSellItems.push_back(Objects::HAY);
    acceptedSellItems.push_back(Objects::EGG);
    acceptedSellItems.push_back(Objects::FRIED_EGG);
    acceptedSellItems.push_back(Objects::CARP);
    acceptedSellItems.push_back(Objects::MILK);
    acceptedSellItems.push_back(Objects::SALAD);


    auto runningScene = Director::getInstance()->getRunningScene();
    if (runningScene) {
        // 定义一个唯一的TAG，比如 9999
        const int SHOP_MENU_TAG = 9999;

        // 查找是否已有该Tag的子节点
        auto existingShop = runningScene->getChildByTag(SHOP_MENU_TAG);
        if (existingShop) {
            CCLOG("商店菜单已打开，不再重复创建");
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
