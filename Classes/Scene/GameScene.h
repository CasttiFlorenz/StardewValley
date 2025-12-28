/****************************************************************
 * Project Name:  StardewValley
 * File Name:     GameScene.h
 * File Function: GameScene类的实现
 * Author:        郭芷烟
 * Update Date:   2025/12/28
 * License:       MIT License
 ****************************************************************/
#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include "cocos2d.h"
#include "Constant.h"

// 玩家与游戏地图
#include "../Player/Player.h"
#include "../GameMap/GameMap.h"

// 具体地图
#include "../GameMap/FarmHouse.h"
#include "../GameMap/Farm.h"
#include "../GameMap/Barn.h"
#include "../GameMap/Mines.h"
#include "../GameMap/Town.h"

// 系统模块
#include "../NPC/NPCManager.h"
#include "../Inventory/InventoryScene.h"
#include "../Weather/WeatherManager.h"
#include "../MapItem/CultivationManager.h"
#include "../Time/TimeManager.h"
#include "../Shop/ShopLayer.h"
#include "../Fishing/FishGameLayer.h"
#include "../Save/SaveManage.h"

USING_NS_CC;

// 游戏主场景
// 负责管理所有地图切换、玩家、时间、天气等核心逻辑
class GameScene : public cocos2d::Scene
{
public:
    // 创建场景
    static cocos2d::Scene* createScene();

    // 析构函数
    ~GameScene();

    // 初始化
    virtual bool init() override;

    // 帧更新
    virtual void update(float dt) override;

    // 宏定义 CREATE_FUNC
    CREATE_FUNC(GameScene);

    // --- 游戏逻辑控制 ---

    // 初始化所有地图
    void initGameMap();

    // 切换地图逻辑
    void switchMap();

    // 重置玩家位置到初始点
    void setPlayerToStart();

    // 打开钓鱼小游戏
    void openFishingGame();

    // 新的一天逻辑
    void onNewDay();

    // 睡觉逻辑
    void sleep();

    // 游戏暂停（停止更新和输入）
    void gamePause();

    // 游戏恢复
    void gameRestore();

    // 检查是否存在打开的界面（如对话框、背包等）
    bool existInterface();

    // --- 输入控制 ---

    // 初始化输入监听器
    void setupMouseListener();
    void setupKeyboardListener();

    // 启用/禁用输入
    void enableMouse(bool enable);
    void enableKeyboard(bool enable);

    // 获取输入状态
    bool isMouseLeftPressed() const noexcept { return _mouseLeftPressed; }
    bool isMouseRightPressed() const noexcept { return _mouseRightPressed; }
    bool isKeyEPressed() const noexcept { return _keyEPressed; }

    // 处理输入事件分发
    void carryMouseEvent(const MouseEvent event);
    void carryKeyBoardEvent(const KeyBoardEvent event);

private:
    // --- 内部辅助函数 ---

    // 更新相机位置
    void updateCamera();
    // 重置相机
    void resetCamera();

private:
    // 游戏状态
    bool _isStart = false;

    // 当前地图引用
    GameMap* _map = nullptr;

    // 玩家引用
    Player* _player = nullptr;

    // 地图缓存 (MapType -> GameMap*)
    std::map<MapType, GameMap*> _mapCache;

    // 系统管理器引用
    InventoryScene* _inventory = nullptr;
    WeatherManager* _weatherManager = nullptr;
    TimeManager* _timeManager = nullptr;
    NPCManager* _npcManager = nullptr;

    // 跟随相机
    Camera* _followCamera = nullptr;

    // 输入监听器
    EventListenerMouse* _mouseListener = nullptr;
    EventListenerKeyboard* _keyboardListener = nullptr;

    // 输入状态标志
    bool _mouseLeftPressed = false;
    bool _mouseRightPressed = false;
    bool _keyEPressed = false;
};


#endif // _GAME_SCENE_H_
