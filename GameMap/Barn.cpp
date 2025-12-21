
#include "Barn.h"

GameMap* Barn::_instance = nullptr;

GameMap* Barn::getInstance() {
    if (!_instance) {
        _instance = Barn::create();
        CC_SAFE_RETAIN(_instance);
    }
    return _instance;
}

void Barn::destroyInstance() {
    CC_SAFE_RELEASE_NULL(_instance);
}

bool Barn::init()
{
    if (!Scene::init()) return false;
    _mapName = "Barn";
    _map = TMXTiledMap::create("/TiledMap/Barn/Barn.tmx");
    if (!_map) return false;
    auto eventLayer = _map->getLayer("event");
    if (eventLayer) eventLayer->setVisible(false);
    this->addChild(_map);

    _barnManager = BarnManager::getInstance();
    _barnManager->init(this);
    _barnManager->addAnimal(BarnAnimalType::CHICKEN);
    _barnManager->addAnimal(BarnAnimalType::COW);
    _barnManager->addAnimal(BarnAnimalType::COW);
    _barnManager->addAnimal(BarnAnimalType::COW);
    _barnManager->addAnimal(BarnAnimalType::CHICKEN);
    _barnManager->addAnimal(BarnAnimalType::CHICKEN);


    return true;
}

std::string Barn::getNewMap(const Vec2& curPos, bool isStart, const Direction& direction)
{
    if (direction == Direction::DOWN) {
        const Rect goToFarmRect = getObjectRect("goToFarm");
        if (goToFarmRect.containsPoint(curPos)) {
            _barnManager->stopAnimations();
            return "Farm";
        }
    }
    return "";
}

void Barn::setStartPosition(std::string lastMap)
{
    const Vec2 visibleSize = Director::getInstance()->getVisibleSize();
    _map->setScale(TILED_MAP_SCALE);
    _map->setPosition((visibleSize - _map->getContentSize() * _map->getScale()) / 2);
    _barnManager->startAnimations();
}

void Barn::update(float dt)
{
}

Vec2 Barn::getPlayerStartPosition(std::string lastMap)
{
    const Rect goToFarmRect = getObjectRect("goToFarm");
    if (!goToFarmRect.equals(Rect::ZERO))
        return Vec2(goToFarmRect.getMidX(), goToFarmRect.getMidY());
    return Vec2(100, 100);
}


MouseEvent Barn::onLeftClick(const Vec2& playerPos, const Direction direction, Objects objects)
{
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

        switch (objects) {
        case Objects::HAY:
            if (_barnManager->addHayAt(checkPos))return MouseEvent::USE_HAY;
            break;
        default:
            break;
        }

    }

    return MouseEvent::USE_TOOL;
}

MouseEvent Barn::onRightClick(const Vec2& playerPos, const Direction direction)
{

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
        switch (_barnManager->collectProductionAt(checkPos)) {
        case Objects::MILK:
            return MouseEvent::GET_MILK;
        case Objects::EGG:
            return MouseEvent::GET_EGG;
        default:
            break;
        }
    }

    return MouseEvent::NONE;
}
