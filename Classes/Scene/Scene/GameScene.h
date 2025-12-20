/****************************************************************
 * Project Name:  StardewValley
 * File Name:     GameScene.h
 * File Function: GameScene类的实现
 * Author:        郭芷烟
 * Update Date:   2025/12/14
 * License:       MIT License
 ****************************************************************/

#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include "cocos2d.h"
#include "Constant.h"

#include "../Player/Player.h"
#include "../GameMap/GameMap.h"
#include "../GameMap/FarmHouse.h"
#include "../GameMap/Farm.h"
#include "../GameMap/Barn.h"
#include "../GameMap/Mines.h"
#include "../GameMap/Town.h"

#include "../Inventory/InventoryScene.h"
#include "../Inventory/InventoryGridScene.h"
#include "../Weather/WeatherManager.h"
#include "../Time/TimeManager.h"
#include "../Shop/ShopLayer.h"

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
    void setPlayerToStart();

    CREATE_FUNC(GameScene);

    void setupMouseListener();
    void setupKeyboardListener();
    void enableMouse(bool enable);
    void enableKeyboard(bool enable);
    bool isMouseLeftPressed() const { return _mouseLeftPressed; }
    bool isMouseRightPressed() const { return _mouseRightPressed; }
    bool isKeyEPressed() const { return _keyEPressed; }

    void carryMouseEvent(const MouseEvent event);
    void carryKeyBoardEvent(const KeyBoardEvent event);
private:
    bool _isStart;          // 是否刚开始游戏
    GameMap* _map;          // 当前地图
    Player* _player;        // 玩家对象
    std::map<std::string, GameMap*> _mapCache; // 地图缓存
    InventoryScene* _inventory;
    WeatherManager* _weatherManager;
    TimeManager* _timeManager;

    Camera* _followCamera; // 跟随摄像机

    void updateCamera(); // 更新摄像机位置
    void resetCamera();  // 重置摄像机

    EventListenerMouse* _mouseListener;
    EventListenerKeyboard* _keyboardListener;
    bool _mouseLeftPressed;
    bool _mouseRightPressed;
    bool _keyEPressed;
};


#endif // _GAME_SCENE_H_