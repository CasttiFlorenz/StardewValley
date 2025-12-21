
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

    _farmItemManager = FarmItemManager::getInstance(this);
    _cultivationManager = CultivationManager::getInstance();
    _cultivationManager->init(_farmItemManager, this);

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

    if (_farmItemManager && _farmItemManager->isCollidable(tilePos))
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
    Vec2 basePos = this->calMapPos(playerPos);

    if (objects == Objects::FISHINGROD) {
        auto layer = _map->getLayer("event");
        if (layer) {
            const int tileGID = layer->getTileGIDAt(basePos);
            if (tileGID) {
                auto properties = _map->getPropertiesForGID(tileGID).asValueMap();
                if (!properties.empty() &&
                    properties.find("Fishing") != properties.end() &&
                    properties.at("Fishing").asBool()) {
                    return MouseEvent::FISHING;;
                }
            }
        }
    }

    switch (direction) {
    case Direction::DOWN:  basePos.y++; break;
    case Direction::UP:    basePos.y--; break;
    case Direction::LEFT:  basePos.x--; break;
    case Direction::RIGHT: basePos.x++; break;
    default: break;
    }

    const int yOffsets[] = { 1,0, -1 };

    for (int offset : yOffsets) {
        Vec2 checkPos = basePos;
        checkPos.y += offset;

        EnvironmentItem* item = _farmItemManager->getItem(checkPos);
        if (item) {
            auto type = item->getType();
            if (type == EnvironmentItemType::WOOD && objects == Objects::AXE) {
                _farmItemManager->removeItem(checkPos);
                return MouseEvent::GET_WOOD;
            }
            else if (type == EnvironmentItemType::GRASS && objects == Objects::SCYTHE) {
                _farmItemManager->removeItem(checkPos);
                return MouseEvent::GET_GRASS;
            }
        }

        switch (objects) {
        case Objects::HOE:
            _cultivationManager->attemptCultivate(checkPos);
            return MouseEvent::USE_TOOL;

        case Objects::PICKAXE:
            _cultivationManager->removeSoil(checkPos);
            return MouseEvent::USE_TOOL;

        case  Objects::WATERING_CAN:
            _cultivationManager->waterSoil(checkPos);
            return MouseEvent::USE_TOOL;

        case  Objects::PARSNIP_SEED:
            if (_cultivationManager->plantCrop(checkPos, CropType::PARSNIP))
                return MouseEvent::USE_PARSNIP_SEED;
            break;

        case Objects::POTATO_SEED:
            if (_cultivationManager->plantCrop(checkPos, CropType::POTATO))
                return MouseEvent::USE_POTATO_SEED;
            break;

        case Objects::CAULIFLOWER_SEED:
            if (_cultivationManager->plantCrop(checkPos, CropType::CAULIFLOWER))
                return MouseEvent::USE_CAULIFLOWER_SEED;
            break;
        default:
            break;
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

    Vec2 basePos = this->calMapPos(playerPos);

    switch (direction) {
    case Direction::DOWN:  basePos.y++; break;
    case Direction::UP:    basePos.y--; break;
    case Direction::LEFT:  basePos.x--; break;
    case Direction::RIGHT: basePos.x++; break;
    default: break;
    }

    const int yOffsets[] = { 1,0, -1 };

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

        CropType crop = _cultivationManager->harvestCrop(checkPos);
        switch (crop) {
        case CropType::PARSNIP:
            return MouseEvent::GET_PARSNIP;
        case CropType::POTATO:
            return MouseEvent::GET_POTATO;
        case CropType::CAULIFLOWER:
            return MouseEvent::GET_CAULIFLOWER;
        default:
            break;
        }

    }

    return MouseEvent::NONE;
}

void Farm::openShopForNPC()
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