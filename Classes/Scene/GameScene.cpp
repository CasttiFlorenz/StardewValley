#include "GameScene.h"

// 创建场景
Scene* GameScene::createScene()
{
    return GameScene::create();
}

// 析构：释放缓存的地图
GameScene::~GameScene()
{
    for (auto& pair : _mapCache)
    {
        CC_SAFE_RELEASE(pair.second);
    }
    _mapCache.clear();
}

// 场景初始化
bool GameScene::init()
{
    if (!Scene::init()) return false;

    _isStart = true;

    // 获取玩家单例
    _player = Player::getInstance();
    if (!_player) return false;

    // 初始化所有地图
    initGameMap();

    // 获取系统单例
    _inventory = InventoryScene::getInstance();
    _weatherManager = WeatherManager::getInstance();
    _timeManager = TimeManager::getInstance();
    _npcManager = NPCManager::getInstance();

    if (_inventory) _inventory->setPlayer(_player);

    // 加入场景
    this->addChild(_player, 4);
    if (_weatherManager) this->addChild(_weatherManager, 5);
    if (_timeManager) this->addChild(_timeManager, 6);
    if (_inventory) this->addChild(_inventory, 7);

    this->scheduleUpdate();

    // 初始化输入状态
    setupMouseListener();
    setupKeyboardListener();

    return true;
}

// 初始化并缓存所有地图
void GameScene::initGameMap()
{
    auto addMap = [this](GameMap* map) {
        if (map) _mapCache[map->getMapName()] = map;
        };

    addMap(FarmHouse::getInstance());
    addMap(Farm::getInstance());
    addMap(Mines::getInstance());
    addMap(Town::getInstance());
    addMap(Barn::getInstance());

    _map = _mapCache[MapType::FARM_HOUSE];
    if (_map) this->addChild(_map, 0);
}

// 每帧更新
void GameScene::update(float dt)
{
    switchMap();

    if (_map && _map->isCameraFollow())
        updateCamera();

    // 确保只绑定一次回调
    if (_timeManager)
    {
        _timeManager->onDayStartCallback = [this]() {
            this->onNewDay();
            };
    }

    if (existInterface())this->gamePause();
    else this->gameRestore();
}

// 地图切换检测
void GameScene::switchMap()
{
    if (!_player || !_map) return;

    MapType newMap = _map->leaveMap(
        _player->getPosition(),
        _isStart,
        _player->getPlayerDirection()
    );

    if (newMap == MapType::NONE) return;
    if (_mapCache.find(newMap) == _mapCache.end()) return;

    _isStart = false;

    MapType lastMap = _map->getMapName();

    _map->removeFromParent();

    _map = _mapCache[newMap];
    this->addChild(_map, 0);

    _map->IntoMap(lastMap);
    _player->setPosition(_map->getPlayerStartPosition(lastMap));
    _player->setGameMap(_map);

    // 相机处理
    if (_map->isCameraFollow())
    {
        if (!_followCamera)
        {
            _followCamera = Camera::createOrthographic(
                Director::getInstance()->getVisibleSize().width,
                Director::getInstance()->getVisibleSize().height,
                1, 1000
            );
            _followCamera->setCameraFlag(CameraFlag::USER1);
            this->addChild(_followCamera, 6);
        }

        _player->setCameraMask((unsigned short)CameraFlag::USER1, true);
        _map->setCameraMask((unsigned short)CameraFlag::USER1, true);

        if (_inventory)
            _inventory->setCameraMask((unsigned short)CameraFlag::DEFAULT, true);

        updateCamera();
    }
    else
    {
        resetCamera();
        _map->setCameraMask((unsigned short)CameraFlag::DEFAULT, true);
        _player->setCameraMask((unsigned short)CameraFlag::DEFAULT, true);
    }
}

// 重置为默认相机
void GameScene::resetCamera()
{
    if (_followCamera)
    {
        _followCamera->removeFromParent();
        _followCamera = nullptr;
    }

    const Size visibleSize = Director::getInstance()->getVisibleSize();
    this->getDefaultCamera()->setPosition(
        Vec2(visibleSize.width / 2, visibleSize.height / 2)
    );
}

// 更新跟随相机位置
void GameScene::updateCamera()
{
    if (!_followCamera || !_player || !_map || !_map->getTiledMap()) return;

    const Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 playerPos = _player->getPosition();

    float x = playerPos.x - visibleSize.width / 2;
    float y = playerPos.y - visibleSize.height / 2;

    float maxX = _map->getTiledMap()->getContentSize().width * TILED_MAP_SCALE - visibleSize.width;
    float maxY = _map->getTiledMap()->getContentSize().height * TILED_MAP_SCALE - visibleSize.height;

    x = clampf(x, 0.0f, maxX);
    y = clampf(y, 0.0f, maxY);

    _followCamera->setPosition3D(Vec3(x, y, CAMERA_POSZ));
}

// 玩家回到出生点
void GameScene::setPlayerToStart()
{
    if (!_map || !_player) return;

    if (_map->getMapName() != MapType::FARM_HOUSE)
    {
        _map->unscheduleUpdate();
        _map->removeFromParent();
        _map = _mapCache[MapType::FARM_HOUSE];
        this->addChild(_map, 0);
    }

    resetCamera();

    _map->IntoMap(MapType::FARM_HOUSE);
    _map->scheduleUpdate();
    _player->setPosition(_map->getPlayerStartPosition(MapType::FARM_HOUSE));
    _player->setGameMap(_map);
}

// 设置鼠标监听
void GameScene::setupMouseListener()
{
    _mouseLeftPressed = false;
    _mouseRightPressed = false;

    _mouseListener = EventListenerMouse::create();
    if (!_mouseListener) return;

    // 鼠标按下
    _mouseListener->onMouseDown = [this](EventMouse* e) {
        if (!_map || !_player) return;

        if (e->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT && !_mouseLeftPressed)
        {
            _mouseLeftPressed = true;
            carryMouseEvent(
                _map->onLeftClick(
                    _player->getPosition(),
                    _player->getPlayerDirection(),
                    _inventory ? _inventory->getTap() : ItemType::NONE
                )
            );
        }
        else if (e->getMouseButton() == EventMouse::MouseButton::BUTTON_RIGHT && !_mouseRightPressed)
        {
            _mouseRightPressed = true;
            carryMouseEvent(
                _map->onRightClick(
                    _player->getPosition(),
                    _player->getPlayerDirection()
                )
            );
        }
        };

    // 鼠标抬起
    _mouseListener->onMouseUp = [this](EventMouse* e) {
        if (e->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT)
            _mouseLeftPressed = false;
        else if (e->getMouseButton() == EventMouse::MouseButton::BUTTON_RIGHT)
            _mouseRightPressed = false;
        };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListener, this);
}

// 设置键盘监听
void GameScene::setupKeyboardListener()
{
    _keyEPressed = false;

    _keyboardListener = EventListenerKeyboard::create();
    if (!_keyboardListener) return;

    _keyboardListener->onKeyPressed = [this](EventKeyboard::KeyCode key, Event*) {
        if (key == EventKeyboard::KeyCode::KEY_E && !_keyEPressed)
        {
            _keyEPressed = true;
            carryKeyBoardEvent(KeyBoardEvent::CHANGE_INVENTORY);
        }
        };

    _keyboardListener->onKeyReleased = [this](EventKeyboard::KeyCode key, Event*) {
        if (key == EventKeyboard::KeyCode::KEY_E)
            _keyEPressed = false;
        };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(_keyboardListener, this);
}

// 启用或禁用鼠标
void GameScene::enableMouse(bool enable)
{
    if (_mouseListener)
        _mouseListener->setEnabled(enable);
}

// 启用或禁用键盘
void GameScene::enableKeyboard(bool enable)
{
    if (_keyboardListener)
        _keyboardListener->setEnabled(enable);
}

// 处理地图返回的鼠标事件
void GameScene::carryMouseEvent(const MouseEvent event)
{
    switch (event)
    {
    case MouseEvent::SLEEP:
        sleep();
        break;

    case MouseEvent::FISHING:
        openFishingGame();
        break;

    case MouseEvent::USE_TOOL:
        if (_inventory)
            _inventory->ToolUseAnimation();
        break;
    default:
        break;
    }

    if (_map->isCameraFollow())
        _map->setCameraMask((unsigned short)CameraFlag::USER1, true);
}


// 处理键盘事件
void GameScene::carryKeyBoardEvent(const KeyBoardEvent event)
{
    switch (event)
    {
    case KeyBoardEvent::CHANGE_INVENTORY:
        if (!existInterface() || _inventory->getInventoryVisible()) _inventory->toggleInventory();
        break;

    default:
        break;
    }
}

// 打开钓鱼小游戏（防止重复）
void GameScene::openFishingGame()
{
    const int FISHING_GAME_TAG = 8998;

    if (this->getChildByTag(FISHING_GAME_TAG)) return;

    auto fishing = FishGameLayer::create();
    if (!fishing) return;

    fishing->setTag(FISHING_GAME_TAG);
    fishing->setCameraMask((unsigned short)CameraFlag::DEFAULT);
    this->addChild(fishing, 10);
}

// 新的一天开始
void GameScene::onNewDay()
{
    setPlayerToStart();

    if (auto c = CultivationManager::getInstance())
    {
        c->onNewDay();
        if (_weatherManager &&
            _weatherManager->getCurrentWeather() == WeatherType::Rain)
        {
            c->waterAllSoils();
        }
    }

    if (auto b = BarnManager::getInstance()) b->onNewDay();
    if (auto f = FarmItemManager::getInstance()) f->onNewDay();
    if (auto m = MinesItemManager::getInstance()) m->onNewDay();
}

// 睡觉确认弹窗
void GameScene::sleep()
{

    // 防止重复创建
    if (this->getChildByName("goToBed")) return;

    const auto dialog = Layer::create();
    dialog->setName("goToBed");

    const Size winSize = Director::getInstance()->getWinSize();
    auto bg = Sprite::create("Shop/SelectDialogue.png");
    if (!bg) return;

    bg->setPosition(winSize.width / 2, 150);
    bg->setScale(3.0f);
    dialog->addChild(bg);

    const Size bgSize = bg->getContentSize();

    const auto label = Label::createWithTTF(
        "Do you want to sleep now?",
        "fonts/pixel.ttf",
        10
    );
    label->setTextColor(Color4B::BLACK);
    label->setPosition(bgSize.width / 2, bgSize.height * 0.6f);
    bg->addChild(label);

    // OK
    auto btnOk = ui::Button::create("Shop/ok.png");
    btnOk->setScale(0.45f);
    btnOk->setPosition(Vec2(bgSize.width * 0.3f, bgSize.height * 0.3f));
    btnOk->addClickEventListener([this](Ref*) {
        this->removeChildByName("goToBed");
        TimeManager::getInstance()->startSleepSequence();
        });
    bg->addChild(btnOk);

    // NO
    auto btnNo = ui::Button::create("Shop/no.png");
    btnNo->setScale(0.45f);
    btnNo->setPosition(Vec2(bgSize.width * 0.7f, bgSize.height * 0.3f));
    btnNo->addClickEventListener([this](Ref*) {
        this->removeChildByName("goToBed");
        });
    bg->addChild(btnNo);

    this->addChild(dialog, 9999);
}

bool GameScene::existInterface()
{
    if (_inventory) {
        if (_inventory->getInventoryVisible())return true;
    }
    if (this->getChildByName("goToBed"))return true;
    if (this->getChildByTag(FISHING_GAME_TAG))return true;
    if (this->getChildByTag(SHOP_MENU_TAG))return true;
    if (this->getChildByName("DialogueLayer"))return true;

    return false;
}

void GameScene::gamePause()
{
    if (_player)_player->unscheduleUpdate();
    if (_timeManager)_timeManager->unscheduleUpdate();
    enableMouse(false);
}

void GameScene::gameRestore()
{
    if (_player)_player->scheduleUpdate();
    if (_timeManager)_timeManager->scheduleUpdate();
    enableMouse(true);
}