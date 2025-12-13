/****************************************************************
 * Project Name:  StardewValley
 * File Name:     Player.h
 * File Function: Player类的实现
 * Author:        郭芷烟
 * Update Date:   2025/12/11
 * License:       MIT License
 ****************************************************************/

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include "Constant.h"
#include"GameMap.h"


USING_NS_CC;

// 玩家类
class Player : public Sprite
{
public:
    static Player* create();
    virtual bool init();
    virtual void update(float dt);

    virtual void onEnter() override;
    virtual void onExit() override;

    // 设置当前所在地图
    void setGameMap(GameMap* map);

    // 设置移动速度
    void setSpeed(float speed);

    // 获取方向的接口
    Direction getPlayerDirection() const { return _direction; }

private:
    float _speed;           // 速度
    cocos2d::Vec2 _velocity; // 速度向量
    GameMap* _map;          // 当前地图引用



    // 键盘按键状态
    bool _upPressed;
    bool _downPressed;
    bool _leftPressed;
    bool _rightPressed;

    // 玩家当前方向变量
    Direction _direction;

    // 更新速度向量
    void updateVelocity();
};

#endif // __PLAYER_H__
