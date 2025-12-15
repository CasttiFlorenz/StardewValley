/****************************************************************
 * Project Name:  StardewValley
 * File Name:     GameScene.cpp
 * File Function: GameScene类的实现
 * Author:        郭芷烟
 * Update Date:   2025/12/14
 * License:       MIT License
 ****************************************************************/

#include "GameScene.h"

Scene* GameScene::createScene()
{
    return GameScene::create();
}

GameScene::~GameScene()
{
    for (auto& pair : _mapCache)
    {
        if (pair.second)
        {
            CC_SAFE_RELEASE(pair.second);
        }
    }
    _mapCache.clear();
}

bool GameScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    _isStart = true;
    _followCamera = nullptr;

    _player = Player::getInstance();

    initGameMap();

   _inventory = InventoryScene::getInstance();
   _weatherManager = WeatherManager::getInstance();  
   _timeManager = TimeManager::getInstance();
   _inventory->setPlayer(_player);

    this->addChild(_player, 4);
    this->addChild(_weatherManager, 5);
    this->addChild(_timeManager, 6);
    this->addChild(_inventory, 7);
  
    _player->changeUpdateStatus();
    _weatherManager->scheduleUpdate();
    this->scheduleUpdate();
  

    _mouseListener = nullptr;
    _keyboardListener = nullptr;
    _mouseLeftPressed = false;
    _mouseRightPressed = false;
    _keyEPressed = false;
    setupMouseListener();
    setupKeyboardListener();

    return true;
}

void GameScene::initGameMap()
{
    _map = FarmHouse::getInstance();
    if (_map) {
        _mapCache[_map->getMapName()] = _map;
    }

    _map = Farm::getInstance();
    if (_map) {
        _mapCache[_map->getMapName()] = _map;
    }

    _map = Mines::getInstance();
    if (_map) {
        _mapCache[_map->getMapName()] = _map;
    }

    _map = Town::getInstance();
    if (_map) {
        _mapCache[_map->getMapName()] = _map;
    }

    _map = Barn::getInstance();
    if (_map) {
        _mapCache[_map->getMapName()] = _map;
    }


    _map = _mapCache["FarmHouse"];
}

void GameScene::update(float dt)
{
    switchMap();

    if (_map && _map->isCameraFollow()) {
        updateCamera();
    }
    _timeManager = TimeManager::getInstance();

    _timeManager->onDayStartCallback = [this]() {
     this->setPlayerToStart(); 
     };
}

void GameScene::switchMap()
{
    if (!_player || !_map) return;

    std::string _newMap;    // 目标地图名称
    std::string _lastMap;   // 上一张地图名称

    _newMap = _map->getNewMap(_player->getPosition(), _isStart, _player->getPlayerDirection());
    _lastMap = _map->getMapName();

    if (_newMap != "") {
        if (_mapCache.find(_newMap) != _mapCache.end()) {
            _isStart = false;

            _map->unscheduleUpdate();
            _map->removeFromParent();

            _map = _mapCache[_newMap];
            _map->scheduleUpdate();
            this->addChild(_map, 0);

            _map->setStartPosition(_lastMap);
            _player->setPosition(_map->getPlayerStartPosition(_lastMap));
            _player->setGameMap(_map);


            // 检查新地图是否需要跟随摄像机
            if (_map->isCameraFollow()) {
                // 如果还没有跟随摄像机，创建一个
                if (!_followCamera) {
                    _followCamera = Camera::createOrthographic(Director::getInstance()->getVisibleSize().width, Director::getInstance()->getVisibleSize().height, 1, 1000);
                    _followCamera->setCameraFlag(CameraFlag::USER1);
                    this->addChild(_followCamera, 6);
                }

                // 设置掩码
                _player->setCameraMask((unsigned short)CameraFlag::USER1, true);
                _map->setCameraMask((unsigned short)CameraFlag::USER1, true);

                // 确保 UI 使用默认摄像机
                if (_inventory) {
                    _inventory->setCameraMask((unsigned short)CameraFlag::DEFAULT, true);
                }

                // 设置摄像机位置
                Vec3 currentPos = _followCamera->getPosition3D();
                currentPos.z = CAMERA_POSZ;
                _followCamera->setPosition3D(currentPos);
                updateCamera();

            }
            else {
                // 如果新地图不需要摄像机，设置不同的掩码
                resetCamera();
                _map->setCameraMask((unsigned short)CameraFlag::DEFAULT, true);
                _player->setCameraMask((unsigned short)CameraFlag::DEFAULT, true);
            }

        }
    }
}

void GameScene::resetCamera()
{
    const Size visibleSize = Director::getInstance()->getVisibleSize();

    if (_followCamera) {
        _followCamera->removeFromParent();
        _followCamera = nullptr;
    }

    this->getDefaultCamera()->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
}


void GameScene::updateCamera()
{

    if (!_followCamera || !_player || !_map) return;

    const Size visibleSize = Director::getInstance()->getVisibleSize();
    const Vec2 playerPos = _player->getPosition();

    float x = playerPos.x - visibleSize.width / 2;
    float y = playerPos.y - visibleSize.height / 2;

    const float maxX = _map->getTiledMap()->getContentSize().width * TILED_MAP_SCALE - visibleSize.width;
    const float maxY = _map->getTiledMap()->getContentSize().height * TILED_MAP_SCALE - visibleSize.height;

    x = std::min(x, maxX);
    y = std::min(y, maxY);
    x = std::max(x, 0.0f);
    y = std::max(y, 0.0f);


    _followCamera->setPosition3D(Vec3(x, y, CAMERA_POSZ));
}

void GameScene::setPlayerToStart()
{
    if (_map->getMapName() != "FarmHouse") {
        _map->unscheduleUpdate();
        _map->removeFromParent();
        _map = _mapCache["FarmHouse"];
        this->addChild(_map, 0);
    }
    resetCamera();
    _map->setCameraMask((unsigned short)CameraFlag::DEFAULT, true);
    _player->setCameraMask((unsigned short)CameraFlag::DEFAULT, true);


    _map->setStartPosition("FarmHouse");
    _map->scheduleUpdate();
    _player->setPosition(_map->getPlayerStartPosition("FarmHouse"));
    _player->setGameMap(_map);

}


void GameScene::setupMouseListener()
{
    _mouseListener = EventListenerMouse::create();
    _mouseListener->onMouseDown = [this](EventMouse* e) {
        if (e->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT) {
            _mouseLeftPressed = true;
            _inventory->ToolUseAnimation();
            _player->changeUpdateStatus();
        }
        else if (e->getMouseButton() == EventMouse::MouseButton::BUTTON_RIGHT) {
            _mouseRightPressed = true;
        }
        };
    _mouseListener->onMouseUp = [this](EventMouse* e) {
        if (e->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT) {
            _mouseLeftPressed = false;
            _player->changeUpdateStatus();
        }
        else if (e->getMouseButton() == EventMouse::MouseButton::BUTTON_RIGHT) _mouseRightPressed = false;
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListener, this);
}

void GameScene::setupKeyboardListener()
{
    _keyboardListener = EventListenerKeyboard::create();
    _keyboardListener->onKeyPressed = [this](EventKeyboard::KeyCode key, Event* e) {
        if (key == EventKeyboard::KeyCode::KEY_E) {
            _keyEPressed = true;
            carryKeyBoardEvent(KeyBoardEvent::CHANGE_INVENTORY);
        }
        };
    _keyboardListener->onKeyReleased = [this](EventKeyboard::KeyCode key, Event* e) {
        if (key == EventKeyboard::KeyCode::KEY_E) _keyEPressed = false;
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_keyboardListener, this);
}

void GameScene::enableMouse(bool enable)
{
    if (_mouseListener) _mouseListener->setEnabled(enable);
}

void GameScene::enableKeyboard(bool enable)
{
    if (_keyboardListener) _keyboardListener->setEnabled(enable);
}

void GameScene::carryMouseEvent(const MouseEvent event)
{
    switch (event) {
    case MouseEvent::NPC_CONSERVATION:


        break;
    case MouseEvent::USE_TOOL:
        
        
        break;
    default:
        break;
    }
}

void GameScene::carryKeyBoardEvent(const KeyBoardEvent event)
{
    switch (event) {
    case KeyBoardEvent::CHANGE_INVENTORY:
        enableMouse(!(_mouseListener->isEnabled()));

        _player->changeUpdateStatus();
        _timeManager->changeUpdateStatus();

        _inventory->toggleInventory();
     
        break;
    default:
        break;
    }
}
