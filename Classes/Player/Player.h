/****************************************************************
 * Project Name:  StardewValley
 * File Name:     Player.h
 * File Function: Player类的实现
 * Author:        郭芷烟、于恩熙
 * Update Date:   2025/12/14
 * License:       MIT License
 ****************************************************************/

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include "Constant.h"
#include"../GameMap/GameMap.h"

USING_NS_CC;

// 玩家类
class Player : public Sprite
{
public:
    CREATE_FUNC(Player);
    static Player* getInstance();
    static void destroyInstance();

    virtual bool init();
    virtual void update(float dt);

    virtual void onEnter() override;
    virtual void onExit() override;

    // 设置当前所在地图
    void setGameMap(GameMap* map);

    // 设置移动速度
    void setSpeed(float speed);

    // 获取方向的接口
    Direction getPlayerDirection() const noexcept { return _direction; }
    void changeUpdateStatus()noexcept { if (isUpdating)unscheduleUpdate(); else scheduleUpdate(); isUpdating = !isUpdating; };

private:
    void createAnimations();       // 创建所有方向动画
    void setAnimation(Direction direction, bool moving);        // 设置当前动画
    void updateAnimation();          // 更新动画状态

    static Player* _instance;

    bool isUpdating;
    float _speed;           // 速度
    cocos2d::Vec2 _velocity; // 速度向量
    GameMap* _map;          // 当前地图引用

    // 动画相关成员变量
    bool _isMoving;                         // 是否正在移动
    std::map<int, Animation*> _walkAnimations;// 存储4个方向的行走动画

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
