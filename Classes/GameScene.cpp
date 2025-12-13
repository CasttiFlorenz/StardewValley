/****************************************************************
 * Project Name:  StardewValley
 * File Name:     GameScene.cpp
 * File Function: GameScene类的实现
 * Author:        郭芷烟
 * Update Date:   2025/12/13
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

    initGameMap();
    _isStart = true;

    _player = Player::create();
    _ui = MainScene::createScene();

    _followCamera = nullptr;

    this->scheduleUpdate();
    this->addChild(_player, 4);
    this->addChild(_ui, 5);
    return true;
}

void GameScene::update(float dt)
{
    switchMap();

    if (_map && _map->isCameraFollow()) {
        updateCamera();
    }
}

void GameScene::switchMap()
{
    if (!_player || !_map) return;

    _newMap = _map->getNewMap(_player->getPosition(), _isStart, _player->getPlayerDirection());
    _lastMap = _map->getMapName();

    if (_newMap != "") {
        if (_mapCache.find(_newMap) != _mapCache.end()) {
            _isStart = false;

            _map->removeFromParent();

            _map = _mapCache[_newMap];
            this->addChild(_map, 0);

            _map->setStartPosition(_lastMap);
            _player->setPosition(_map->getPlayerStartPosition(_lastMap));
            _player->setGameMap(_map);

            _map->scheduleUpdate();
             
            // 检查新地图是否需要跟随摄像机
            if (_map->isCameraFollow()) {
                // 如果还没有跟随摄像机，创建一个
                if (!_followCamera) {
                    _followCamera = Camera::createOrthographic(Director::getInstance()->getVisibleSize().width, Director::getInstance()->getVisibleSize().height, 1, 1000);
                    _followCamera->setCameraFlag(CameraFlag::USER1);
                    this->addChild(_followCamera, 6);
                }

                _player->setCameraMask((unsigned short)CameraFlag::USER1, true);
                _map->setCameraMask((unsigned short)CameraFlag::USER1, true);
                
                // 确保 UI 使用默认摄像机
                if (_ui) {
                     _ui->setCameraMask((unsigned short)CameraFlag::DEFAULT, true);
                }

                // 初始化摄像机位置
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

void GameScene::initGameMap()
{
    _map = FarmHouse::createScene();
    if (_map) {
        _map->retain();
        _mapCache[_map->getMapName()] = _map;
    }

    _map = Farm::createScene();
    if (_map) {
        _map->retain();
        _mapCache[_map->getMapName()] = _map;
    }

    _map = _mapCache["FarmHouse"];
}

void GameScene::resetCamera()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();

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
    const float maxY = _map->getTiledMap()->getContentSize().height *TILED_MAP_SCALE - visibleSize.height;

    x = std::min(x, maxX);
    y = std::min(y, maxY);
    x = std::max(x, 0.0f);
    y = std::max(y, 0.0f);


    _followCamera->setPosition3D(Vec3(x, y, CAMERA_POSZ));
}
