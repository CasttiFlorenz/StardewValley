/****************************************************************
 * Project Name:  StardewValley
 * File Name:     GameScene.h
 * File Function: GameScene���ʵ��
 * Author:        ������
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

// ��Ϸ��������������ͼ�л������
class GameScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    ~GameScene();

    virtual bool init();
    // ��ʼ����Ϸ��ͼ
    virtual void initGameMap();
    // �л���ͼ�߼�
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
    bool _isStart;          // �Ƿ�տ�ʼ��Ϸ
    GameMap* _map;          // ��ǰ��ͼ
    Player* _player;        // ��Ҷ���
    std::map<MapType, GameMap*> _mapCache; // ��ͼ����
    InventoryScene* _inventory;
    WeatherManager* _weatherManager;
    TimeManager* _timeManager;

    Camera* _followCamera; // ���������

    void updateCamera(); // ���������λ��
    void resetCamera();  // ���������

    EventListenerMouse* _mouseListener;
    EventListenerKeyboard* _keyboardListener;
    bool _mouseLeftPressed;
    bool _mouseRightPressed;
    bool _keyEPressed;
};


#endif // _GAME_SCENE_H_
