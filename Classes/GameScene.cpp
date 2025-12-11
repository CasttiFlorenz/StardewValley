/****************************************************************
 * Project Name:  StardewValley
 * File Name:     GameScene.cpp
 * File Function: GameScene类的实现
 * Author:        郭芷烟
 * Update Date:   2025/12/11
 * License:       MIT License
 ****************************************************************/

#include "GameScene.h"

Scene* GameScene::createScene()
{
    return GameScene::create();
}

bool GameScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    // 初始化地图
    initGameMap();
    _isStart = true;

    // 创建玩家
    _player = Player::create();

    this->scheduleUpdate();
    this->addChild(_player, 1);

    return true;
}

void GameScene::update(float dt)
{
    // 每帧检查是否需要切换地图
    switchMap();
}

void GameScene::switchMap()
{
    if (!_player || !_map) return;

    // 获取下一张地图的名称
    _newMap = _map->getNewMap(_map, _player->getPosition(), _isStart);
    _lastMap = _map->getMapName();

    if (_newMap != "") {
        // 如果目标地图在缓存中
        if (_mapCache.find(_newMap) != _mapCache.end()) {
            _isStart = false;

            // 移除当前地图
            _map->removeFromParent();

            // 切换到新地图
            _map = _mapCache[_newMap];
            this->addChild(_map, 0);

            // 设置地图和玩家位置
            _map->setStartPosition(_lastMap);
            _player->setPosition(_map->getPlayerStartPosition(_lastMap));
            _player->setGameMap(_map);

            _map->scheduleUpdate();
        }
    }
}

void GameScene::initGameMap()
{
    // 创建 FarmHouse 地图并加入缓存
    _map = dynamic_cast<GameMap*>(FarmHouse::createScene());
    if (_map) {
        _map->retain(); // 防止被销毁
        _mapCache[_map->getMapName()] = _map;
    }

    // 设置当前地图
    _map = _mapCache["FarmHouse"];
}