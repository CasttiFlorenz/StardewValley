

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
#include"../NPC/NPCManager.h"
#include "../Inventory/InventoryScene.h"
#include "../Weather/WeatherManager.h"
#include "../Time/TimeManager.h"
#include "../Shop/ShopLayer.h"
#include "../Fishing/FishGameLayer.h"


USING_NS_CC;

class GameScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    ~GameScene();

    bool init();
    void initGameMap();
    void switchMap();
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
    void openFishingGame();
    void onNewDay();

private:
    bool _isStart;     
    GameMap* _map;      
    Player* _player;    
    std::map<MapType, GameMap*> _mapCache;
    InventoryScene* _inventory;
    WeatherManager* _weatherManager;
    TimeManager* _timeManager;
    NPCManager* _npcManager;
    Camera* _followCamera;
    void updateCamera();
    void resetCamera(); 

    EventListenerMouse* _mouseListener;
    EventListenerKeyboard* _keyboardListener;
    bool _mouseLeftPressed;
    bool _mouseRightPressed;
    bool _keyEPressed;
};


#endif // _GAME_SCENE_H_

