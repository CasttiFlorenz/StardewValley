#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include "cocos2d.h"
#include "Constant.h"

// 核心游戏对象
#include "../Player/Player.h"
#include "../GameMap/GameMap.h"

// 各地图类型
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

USING_NS_CC;

/**
 * GameScene
 * 游戏主场景，负责地图切换、玩家、时间、天气等核心流程调度
 */
class GameScene : public cocos2d::Scene
{
public:
    // 创建游戏场景
    static cocos2d::Scene* createScene();

    // 析构时释放所有子系统
    ~GameScene();

    // 初始化场景
    bool init();

    // 初始化当前地图
    void initGameMap();

    // 根据状态切换地图
    void switchMap();

    // 主循环更新
    virtual void update(float dt) override;

    // 将玩家放回出生点
    void setPlayerToStart();

    CREATE_FUNC(GameScene);

    // 输入监听初始化
    void setupMouseListener();
    void setupKeyboardListener();

    // 启用 / 禁用输入
    void enableMouse(bool enable);
    void enableKeyboard(bool enable);

    // 输入状态查询
    bool isMouseLeftPressed() const { return _mouseLeftPressed; }
    bool isMouseRightPressed() const { return _mouseRightPressed; }
    bool isKeyEPressed() const { return _keyEPressed; }

    // 转发输入事件给子系统
    void carryMouseEvent(const MouseEvent event);
    void carryKeyBoardEvent(const KeyBoardEvent event);

    // 打开钓鱼小游戏
    void openFishingGame();

    // 新的一天开始回调
    void onNewDay();

    // 玩家睡觉（推进时间）
    void sleep();

    // 防止商店界面、背包界面、钓鱼界面、对话框界面同时打开
    bool existInterface();
    void gamePause();
    void gameRestore();


private:
    // 游戏是否已开始
    bool _isStart = false;

    // 当前地图
    GameMap* _map = nullptr;

    // 玩家对象
    Player* _player = nullptr;

    // 地图缓存（避免重复创建）
    std::map<MapType, GameMap*> _mapCache;

    // 系统管理器
    InventoryScene* _inventory = nullptr;
    WeatherManager* _weatherManager = nullptr;
    TimeManager* _timeManager = nullptr;
    NPCManager* _npcManager = nullptr;

    // 跟随玩家的相机
    Camera* _followCamera = nullptr;

    // 相机更新与重置
    void updateCamera();
    void resetCamera();

    // 输入监听器
    EventListenerMouse* _mouseListener = nullptr;
    EventListenerKeyboard* _keyboardListener = nullptr;

    // 输入状态缓存
    bool _mouseLeftPressed = false;
    bool _mouseRightPressed = false;
    bool _keyEPressed = false;
};

#endif // _GAME_SCENE_H_
