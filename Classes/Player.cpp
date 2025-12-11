/****************************************************************
 * Project Name:  StardewValley
 * File Name:     Player.cpp
 * File Function: Player类的实现
 * Author:        郭芷烟
 * Update Date:   2025/12/11
 * License:       MIT License
 ****************************************************************/

#include "Player.h"

USING_NS_CC;

Player* Player::create()
{
    Player* pRet = new(std::nothrow) Player();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

bool Player::init()
{
    // 加载玩家纹理
    auto texture = Director::getInstance()->getTextureCache()->addImage("Player/Abigail..png");
    if (!texture)
    {
        CCLOG("Failed to load player texture: Player/Abigail..png");
        return false;
    }

    // 计算每帧尺寸
    float w = texture->getContentSize().width / 4;
    float h = texture->getContentSize().height / 13;

    if (!Sprite::initWithTexture(texture, Rect(0, 0, w, h)))
    {
        return false;
    }

    // 初始化成员变量
    _speed = 250.0f;
    _velocity = Vec2::ZERO;
    _upPressed = false;
    _downPressed = false;
    _leftPressed = false;
    _rightPressed = false;
    _map = nullptr;

    this->setScale(TILED_MAP_SCALE);
    this->scheduleUpdate();

    return true;
}

void Player::onEnter()
{
    Sprite::onEnter();

    // 创建键盘监听器
    auto listener = EventListenerKeyboard::create();
    
    // 按下按键
    listener->onKeyPressed = [=](EventKeyboard::KeyCode code, Event* event) {
        switch (code) {
        case EventKeyboard::KeyCode::KEY_W:
        case EventKeyboard::KeyCode::KEY_UP_ARROW:
            _upPressed = true;
            break;
        case EventKeyboard::KeyCode::KEY_S:
        case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
            _downPressed = true;
            break;
        case EventKeyboard::KeyCode::KEY_A:
        case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
            _leftPressed = true;
            break;
        case EventKeyboard::KeyCode::KEY_D:
        case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            _rightPressed = true;
            break;
        default:
            break;
        }
        updateVelocity();
        };

    // 释放按键
    listener->onKeyReleased = [=](EventKeyboard::KeyCode code, Event* event) {
        switch (code) {
        case EventKeyboard::KeyCode::KEY_W:
        case EventKeyboard::KeyCode::KEY_UP_ARROW:
            _upPressed = false;
            break;
        case EventKeyboard::KeyCode::KEY_S:
        case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
            _downPressed = false;
            break;
        case EventKeyboard::KeyCode::KEY_A:
        case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
            _leftPressed = false;
            break;
        case EventKeyboard::KeyCode::KEY_D:
        case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            _rightPressed = false;
            break;
        default:
            break;
        }
        updateVelocity();
        };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void Player::onExit()
{
    _eventDispatcher->removeEventListenersForTarget(this);
    Sprite::onExit();
}

void Player::setGameMap(GameMap* map)
{
    _map = map;
}

void Player::setSpeed(float speed)
{
    _speed = speed;
}

void Player::updateVelocity()
{
    _velocity = Vec2::ZERO;
    if (_upPressed) _velocity.y += 1;
    if (_downPressed) _velocity.y -= 1;
    if (_leftPressed) _velocity.x -= 1;
    if (_rightPressed) _velocity.x += 1;

    // 归一化速度向量
    if (_velocity.lengthSquared() > 0)
    {
        _velocity.normalize();
    }
}

void Player::update(float dt)
{
    if (_velocity != Vec2::ZERO)
    {
        Vec2 currentPos = this->getPosition();
        Vec2 nextPos = currentPos + _velocity * _speed * dt;

        bool collision = false;
        if (_map)
        {
            // 检测碰撞
            collision = _map->isCollidable(nextPos);
        }

        if (!collision)
        {
            this->setPosition(nextPos);
        }
    }
}
