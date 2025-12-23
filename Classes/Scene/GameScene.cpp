✅调整事件触发方式，改为直接在地图类内调用相关方法|
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
    _npcManager = NPCManager::getInstance();
    
    this->addChild(_player, 4);
    this->addChild(_weatherManager, 5);
    this->addChild(_timeManager, 6);
    this->addChild(_inventory, 7);

    _player->changeUpdateStatus();
    _timeManager->changeUpdateStatus();
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

    _map = _mapCache[MapType::FARM_HOUSE];
}

void GameScene::update(float dt)
{
    switchMap();

    if (_map && _map->isCameraFollow()) {
        updateCamera();
    }

    _timeManager->onDayStartCallback = [this]() {
        this->onNewDay();
        };
}

void GameScene::switchMap()
{
    if (!_player || !_map) return;

    MapType _newMap;
    MapType _lastMap;

    _newMap = _map->leaveMap(_player->getPosition(), _isStart, _player->getPlayerDirection());
    _lastMap = _map->getMapName();

    if (_newMap != MapType::NONE) {
        if (_mapCache.find(_newMap) != _mapCache.end()) {
            _isStart = false;

            _map->unscheduleUpdate();
            _map->removeFromParent();

            _map = _mapCache[_newMap];
            _map->scheduleUpdate();
            this->addChild(_map, 0);

            _map->IntoMap(_lastMap);
            _player->setPosition(_map->getPlayerStartPosition(_lastMap));
            _player->setGameMap(_map);

            if (_map->isCameraFollow()) {
                if (!_followCamera) {
                    _followCamera = Camera::createOrthographic(Director::getInstance()->getVisibleSize().width, Director::getInstance()->getVisibleSize().height, 1, 1000);
                    _followCamera->setCameraFlag(CameraFlag::USER1);
                    this->addChild(_followCamera, 6);
                }

                _player->setCameraMask((unsigned short)CameraFlag::USER1, true);
                _map->setCameraMask((unsigned short)CameraFlag::USER1, true);

                if (_inventory) {
                    _inventory->setCameraMask((unsigned short)CameraFlag::DEFAULT, true);
                }

                Vec3 currentPos = _followCamera->getPosition3D();
                currentPos.z = CAMERA_POSZ;
                _followCamera->setPosition3D(currentPos);
                updateCamera();

            }
            else {
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
    if (_map->getMapName() != MapType::FARM_HOUSE) {
        _map->unscheduleUpdate();
        _map->removeFromParent();
        _map = _mapCache[MapType::FARM_HOUSE];
        this->addChild(_map, 0);
    }
    resetCamera();
    _map->setCameraMask((unsigned short)CameraFlag::DEFAULT, true);
    _player->setCameraMask((unsigned short)CameraFlag::DEFAULT, true);

    _map->IntoMap(MapType::FARM_HOUSE);
    _map->scheduleUpdate();
    _player->setPosition(_map->getPlayerStartPosition(MapType::FARM_HOUSE));
    _player->setGameMap(_map);

}

void GameScene::setupMouseListener()
{
    _mouseListener = EventListenerMouse::create();
    _mouseListener->onMouseDown = [this](EventMouse* e) {
        if (e->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT) {
            if (_map && _player && !_mouseLeftPressed) {
                _mouseLeftPressed = true;
                _player->changeUpdateStatus();
                carryMouseEvent(_map->onLeftClick(_player->getPosition(), _player->getPlayerDirection(), _inventory->getTap()));
            }
        }
        else if (e->getMouseButton() == EventMouse::MouseButton::BUTTON_RIGHT) {
            if (_map && _player && !_mouseRightPressed) {
                _mouseRightPressed = true;
                _player->changeUpdateStatus();
                carryMouseEvent(_map->onRightClick(_player->getPosition(), _player->getPlayerDirection()));
            }
        }
        };
    _mouseListener->onMouseUp = [this](EventMouse* e) {
        if (e->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT) {
            _mouseLeftPressed = false;
        }
        if (e->getMouseButton() == EventMouse::MouseButton::BUTTON_RIGHT) {
            _mouseRightPressed = false;
        }
        };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListener, this);
}

void GameScene::setupKeyboardListener()
{
    _keyboardListener = EventListenerKeyboard::create();
    _keyboardListener->onKeyPressed = [this](EventKeyboard::KeyCode key, Event* e) {
        if (key == EventKeyboard::KeyCode::KEY_E) {
            _keyEPressed = true;
            _timeManager->changeUpdateStatus();
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
    ItemType currentItem = _inventory->getTap();

    switch (event) {
    case MouseEvent::SLEEP: {
       sleep();
        break;
    }
    case MouseEvent::FISHING: {
        openFishingGame();
        break;
    }
                           
    case MouseEvent::USE_TOOL:
        _inventory->ToolUseAnimation();
        break;
    default:
        break;
    }

    if (_map->isCameraFollow())
        _map->setCameraMask((unsigned short)CameraFlag::USER1, true);
    _player->changeUpdateStatus();
}
void GameScene::carryKeyBoardEvent(const KeyBoardEvent event)
{
    switch (event) {
    case KeyBoardEvent::CHANGE_INVENTORY:
        if (_mouseListener->isEnabled())enableMouse(false);
        else  enableMouse(true);

        _player->changeUpdateStatus();
        _timeManager->changeUpdateStatus();
        _inventory->toggleInventory();
        break;
    default:
        break;
    }
}

void GameScene::openFishingGame()
{
    const int FISHING_GAME_TAG = 8998;

    auto existingFishing = this->getChildByTag(FISHING_GAME_TAG);
    if (existingFishing) {
        return;
    }
    auto fishing = FishGameLayer::create();
    if (fishing) {
        fishing->setTag(FISHING_GAME_TAG);
        this->addChild(fishing, 10);
        fishing->setTag(FISHING_GAME_TAG);
        fishing->setCameraMask((unsigned short)CameraFlag::DEFAULT);
    }
}

void GameScene::onNewDay()
{
    this->setPlayerToStart();
    if (auto instance = CultivationManager::getInstance()) { instance->onNewDay(); }
    if (auto instance = BarnManager::getInstance()) { instance->onNewDay(); }
    if (auto instance = FarmItemManager::getInstance()) { instance->onNewDay(); }
    if (auto instance = MinesItemManager::getInstance()) { instance->onNewDay(); }


}

void GameScene::sleep() {
    onNewDay();
    auto runningScene = Director::getInstance()->getRunningScene();
    if (!runningScene) return;

    // 防止重复打开
    if (runningScene->getChildByName("goToBed")) return;

    auto dialog = Layer::create();
    dialog->setName("goToBed");
    Size winSize = Director::getInstance()->getWinSize();
    auto bg = Sprite::create("Shop/SelectDialogue.png");
    float bgHeight = bg->getContentSize().height;
    bg->setPosition(winSize.width / 2, 150);
    bg->setScale(3.0f);
    dialog->addChild(bg);
    Size bgSize = bg->getContentSize();

    auto label = Label::createWithTTF("Do you want to sleep now?", "fonts/pixel.ttf", 10);
    label->setTextColor(Color4B::BLACK);
    label->setPosition(Vec2(bgSize.width / 2, bgSize.height * 0.6f));
    bg->addChild(label);

    // 添加 OK 按钮
    auto btnOk = ui::Button::create("Shop/ok.png");
    btnOk->setScale(0.45f);
    btnOk->setPosition(Vec2(bgSize.width * 0.3f, bgSize.height * 0.3f));
    btnOk->addClickEventListener([runningScene](Ref*) {
        CCLOG("Sleep: YES clicked");
        runningScene->removeChildByName("goToBed");
        TimeManager::getInstance()->startSleepSequence();
        });
    bg->addChild(btnOk);
    
    // 添加 NO 按钮
    auto btnNo = ui::Button::create("Shop/no.png");
    btnNo->setScale(0.45f);
    btnNo->setPosition(Vec2(bgSize.width * 0.7f, bgSize.height * 0.3f));
    btnNo->addClickEventListener([runningScene](Ref*) {
        CCLOG("Sleep: NO clicked");

        runningScene->removeChildByName("goToBed");
        });
    bg->addChild(btnNo);

    runningScene->addChild(dialog, 9999);
}
