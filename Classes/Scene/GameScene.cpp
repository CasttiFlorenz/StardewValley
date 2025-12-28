/****************************************************************
 * Project Name:  StardewValley
 * File Name:     GameScene.cpp
 * File Function: GameScene类的实现
 * Author:        郭芷烟
 * Update Date:   2025/12/16
 * License:       MIT License
 ****************************************************************/
#include "GameScene.h"

// 创建场景
Scene* GameScene::createScene()
{
    return GameScene::create();
}

// 析构函数
GameScene::~GameScene()
{
    _mapCache.clear();
}

// 初始化
bool GameScene::init()
{
    if (!Scene::init()) return false;

    _isStart = true;

    // 获取玩家实例
    _player = Player::getInstance();
    if (!_player) return false;

    // 初始化所有地图
    initGameMap();

    // 获取系统管理器
    _inventory = InventoryScene::getInstance();
    _weatherManager = WeatherManager::getInstance();
    _timeManager = TimeManager::getInstance();
    _npcManager = NPCManager::getInstance();

    if (_inventory) _inventory->setPlayer(_player);

    // 添加子节点
    this->addChild(_player, PLAYER_Z_ORDER);
    if (_weatherManager) this->addChild(_weatherManager, WEATHER_MANAGER_Z_ORDER);
    if (_timeManager) this->addChild(_timeManager, TIME_MANAGER_Z_ORDER);
    if (_inventory) this->addChild(_inventory, INVENTORY_Z_ORDER);

    this->scheduleUpdate();

    // 初始化输入监听
    setupMouseListener();
    setupKeyboardListener();

    return true;
}

// 初始化游戏地图
void GameScene::initGameMap()
{
    auto addMap = [this](GameMap* map) {
        if (map) _mapCache[map->getMapName()] = map;
        else return;
        };

    addMap(FarmHouse::getInstance());
    addMap(Farm::getInstance());
    addMap(Mines::getInstance());
    addMap(Town::getInstance());
    addMap(Barn::getInstance());

    _map = _mapCache[MapType::FARM_HOUSE];
    if (_map) this->addChild(_map, MAP_Z_ORDER);
}

// 帧更新
void GameScene::update(float dt)
{
    switchMap();

    if (_map && _map->isCameraFollow())
        updateCamera();

    // 检查界面状态以暂停/恢复游戏
    if (existInterface()) this->gamePause();
    else this->gameRestore();

    // 确保每天只触发一次新的一天回调
    if (_timeManager)
    {
        _timeManager->onDayStartCallback = [this]() {
            this->onNewDay();
            };
    }

    // 雨天自动浇水
    if (auto c = CultivationManager::getInstance())
    {
        if (_weatherManager &&
            _weatherManager->getCurrentWeather() == WeatherType::Rain)
        {
            c->waterAllSoils();
        }
    }

    // 户外显示天气效果
    if (_map && _weatherManager) {
        if (_map->isOutdoor())
            _weatherManager->setVisible(true);
        else
            _weatherManager->setVisible(false);
    }
}

// 切换地图逻辑
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
    this->addChild(_map, MAP_Z_ORDER);

    _map->IntoMap(lastMap);
    _player->setPosition(_map->getPlayerStartPosition(lastMap));
    _player->setGameMap(_map);

    // 相机跟随设置
    if (_map->isCameraFollow())
    {
        if (!_followCamera)
        {
            _followCamera = Camera::createOrthographic(
                Director::getInstance()->getVisibleSize().width,
                Director::getInstance()->getVisibleSize().height,
                CAMERA_NEAR_PLANE, CAMERA_FAR_PLANE
            );
            _followCamera->setCameraFlag(CameraFlag::USER1);
            this->addChild(_followCamera, FOLLOW_CAMERA_Z_ORDER);
        }

        _player->setCameraMask(static_cast<unsigned short>(CameraFlag::USER1), true);
        _map->setCameraMask(static_cast<unsigned short>(CameraFlag::USER1), true);

        if (_inventory)
            _inventory->setCameraMask(static_cast<unsigned short>(CameraFlag::DEFAULT), true);

        updateCamera();
    }
    else
    {
        resetCamera();
        _map->setCameraMask(static_cast<unsigned short>(CameraFlag::DEFAULT), true);
        _player->setCameraMask(static_cast<unsigned short>(CameraFlag::DEFAULT), true);
    }
}

// 重置相机
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

// 更新相机位置
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

// 设置玩家回初始点
void GameScene::setPlayerToStart()
{
    if (!_map || !_player) return;

    if (_map->getMapName() != MapType::FARM_HOUSE)
    {
        _map->unscheduleUpdate();
        _map->removeFromParent();
        _map = _mapCache[MapType::FARM_HOUSE];
        this->addChild(_map, MAP_Z_ORDER);
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

// 启用/禁用鼠标
void GameScene::enableMouse(bool enable)
{
    if (_mouseListener)
        _mouseListener->setEnabled(enable);

    if (enable) _mouseLeftPressed = _mouseRightPressed = false;
}

// 启用/禁用键盘
void GameScene::enableKeyboard(bool enable)
{
    if (_keyboardListener)
        _keyboardListener->setEnabled(enable);
}

// 处理鼠标事件
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
        _map->setCameraMask(static_cast<unsigned short>(CameraFlag::USER1), true);
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

// 打开钓鱼游戏
void GameScene::openFishingGame()
{
    if (this->getChildByTag(FISHING_GAME_TAG)) return;

    auto fishing = FishGameLayer::create();
    if (!fishing) return;

    fishing->setTag(FISHING_GAME_TAG);
    fishing->setCameraMask(static_cast<unsigned short>(CameraFlag::DEFAULT));
    this->addChild(fishing, FISHING_GAME_Z_ORDER);
}

// 新的一天
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

    if (auto save = SaveManage::getInstance()) {
        save->saveAllData();
    }
}

// 睡觉确认对话框
void GameScene::sleep()
{
    if (this->getChildByName(SLEEP_DIALOG_NAME)) return;

    const auto dialog = Layer::create();
    if (dialog == nullptr)return;
    dialog->setName(SLEEP_DIALOG_NAME);

    const Size winSize = Director::getInstance()->getWinSize();
    auto bg = Sprite::create(SELECT_DIALOGUE_TEXTURE_PATH);
    if (!bg) return;

    bg->setPosition(winSize.width / 2, DIALOGUE_POS_Y);
    bg->setScale(DIALOGUE_SCALE);
    dialog->addChild(bg);

    const Size bgSize = bg->getContentSize();

    const auto label = Label::createWithTTF(
        SLEEP_DIALOG_TEXT,
        PIXEL_FONT_PATH,
        FONT_SIZE
    );
    label->setTextColor(Color4B::BLACK);
    label->setPosition(bgSize.width / 2, bgSize.height * LABEL_POS_Y_RATIO);
    bg->addChild(label);

    // 确认按钮
    auto btnOk = ui::Button::create(OK_BUTTON_TEXTURE_PATH);
    btnOk->setScale(BUTTON_SCALE);
    btnOk->setPosition(Vec2(bgSize.width * OK_BUTTON_POS_X_RATIO, bgSize.height * BUTTON_POS_Y_RATIO));
    btnOk->addClickEventListener([this](Ref*) {
        this->removeChildByName(SLEEP_DIALOG_NAME);
        TimeManager::getInstance()->startSleepSequence();
        });
    bg->addChild(btnOk);

    // 取消按钮
    auto btnNo = ui::Button::create(NO_BUTTON_TEXTURE_PATH);
    btnNo->setScale(BUTTON_SCALE);
    btnNo->setPosition(Vec2(bgSize.width * NO_BUTTON_POS_X_RATIO, bgSize.height * BUTTON_POS_Y_RATIO));
    btnNo->addClickEventListener([this](Ref*) {
        this->removeChildByName(SLEEP_DIALOG_NAME);
        });
    bg->addChild(btnNo);

    this->addChild(dialog, SLEEP_DIALOG_Z_ORDER);
}

// 检查界面状态
bool GameScene::existInterface()
{
    if (_inventory) {
        if (_inventory->getInventoryVisible()) return true;
    }
    if (this->getChildByName(SLEEP_DIALOG_NAME)) return true;
    if (this->getChildByTag(FISHING_GAME_TAG)) return true;
    if (this->getChildByTag(SHOP_MENU_TAG)) return true;
    if (this->getChildByTag(TAG_DIALOGUE_LAYER)) return true;
    if (this->getChildByTag(SLEEPING_TAG)) return true;

    return false;
}

// 暂停游戏
void GameScene::gamePause()
{
    if (_player) _player->unscheduleUpdate();
    if (_timeManager) _timeManager->unscheduleUpdate();
    enableMouse(false);
}

// 恢复游戏
void GameScene::gameRestore()
{
    if (_player) _player->scheduleUpdate();
    if (_timeManager) _timeManager->scheduleUpdate();
    enableMouse(true);

}
