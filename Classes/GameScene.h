/****************************************************************
 * Project Name:  StardewValley
 * File Name:     GameScene.h
 * File Function: GameScene类的实现
 * Author:        郭芷烟
 * Update Date:   2025/12/13
 * License:       MIT License
 ****************************************************************/

#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include "cocos2d.h"
#include "Constant.h"
#include "FarmHouse.h"
#include "Farm.h"
#include "GameMap.h"
#include "MainScene.h"
#include "Player.h"

USING_NS_CC;

// 游戏主场景，管理地图切换和玩家
class GameScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    ~GameScene();

    virtual bool init();
    // 初始化游戏地图
    virtual void initGameMap();
    // 切换地图逻辑
    virtual void switchMap();
    virtual void update(float dt);


    CREATE_FUNC(GameScene);

private:
    bool _isStart;          // 是否刚开始游戏
    std::string _newMap;    // 目标地图名称
    std::string _lastMap;   // 上一张地图名称
    GameMap* _map;          // 当前地图
    Player* _player;        // 玩家对象
    std::map<std::string, GameMap*> _mapCache; // 地图缓存
    Scene* _ui;

    Camera* _followCamera; // 跟随摄像机

    void updateCamera(); // 更新摄像机位置
    void resetCamera();  // 重置摄像机
};

#endif // _GAME_SCENE_H_
