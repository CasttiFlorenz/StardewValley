/****************************************************************
 * Project Name:  StardewValley
 * File Name:     Player.cpp
 * File Function: Player类的实现
 * Author:        郭芷烟、于恩熙
 * Update Date:   2025/12/14
 * License:       MIT License
 ****************************************************************/

#include "Player.h"

USING_NS_CC;

Player* Player::_instance = nullptr;

Player* Player::getInstance() {
    if (!_instance) {
        _instance = Player::create();
        CC_SAFE_RETAIN(_instance);
    }
    return _instance;
}

void Player::destroyInstance() {
    CC_SAFE_RELEASE_NULL(_instance);
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
    const float w = texture->getContentSize().width / 4;
    const float h = texture->getContentSize().height / 13;

    if (!Sprite::initWithTexture(texture, Rect(0, 0, w, h)))
    {
        return false;
    }

    // 初始化方向为向下
    _direction = Direction::DOWN;
    _isMoving = false;                   // 初始不在移动

    this->setScale(TILED_MAP_SCALE);

    // 创建动画
    createAnimations();

    auto downAnimation = _walkAnimations[(int)Direction::DOWN];

    // 设置初始动画状态
    setAnimation(_direction, false);

    // 初始化成员变量
    isUpdating = false;
    _speed = DEFAULT_SPEED;
    _velocity = Vec2::ZERO;
    _upPressed = false;
    _downPressed = false;
    _leftPressed = false;
    _rightPressed = false;
    _map = nullptr;

    this->setScale(TILED_MAP_SCALE);

    return true;
}

// 创建所有方向的行走动画
void Player::createAnimations()
{
    // 获取玩家纹理
    auto texture = this->getTexture();

    // 计算纹理中每帧的尺寸
    float frameWidth = texture->getContentSize().width / 4;     // 4列，每列一帧
    float frameHeight = texture->getContentSize().height / 13;  // 13行，每行一个动作

    // 定义纹理中每个方向对应的行索引
    int downRow = 0;   // 第0行：向下行走的4帧动画
    int rightRow = 1;  // 第1行：向右行走的4帧动画
    int upRow = 2;     // 第2行：向上行走的4帧动画
    int leftRow = 3;   // 第3行：向左行走的4帧动画

    // 为每个方向创建动画
    for (int direction = 0; direction < 4; direction++)
    {
        // 存储动画帧的容器
        Vector<SpriteFrame*> frames;

        int row = direction; // 当前方向对应的纹理行

        // 取当前行的4帧创建动画序列
        for (int col = 0; col < 4; col++)
        {
            // 从纹理的指定位置切割出一帧
            auto frame = SpriteFrame::createWithTexture(texture,
                Rect(col * frameWidth, row * frameHeight, frameWidth, frameHeight));

            frames.pushBack(frame);
        }

        // 使用帧序列创建动画，每帧持续0.15秒
        auto animation = Animation::createWithSpriteFrames(frames, 0.15f);

        animation->retain();

        // 使用方向作为键，存储对应的动画
        _walkAnimations[direction] = animation;
    }
}

void Player::setAnimation(Direction direction, bool moving)
{
    // 更新状态
    _direction = direction;
    _isMoving = moving;

    // 停止当前所有动作
    this->stopAllActions();

    if (moving)
    {
        // 动画播放
        auto  animation = _walkAnimations[(int)direction];
        if (animation)
        {
            auto animate = Animate::create(animation);
            auto repeat = RepeatForever::create(animate);
            this->runAction(repeat);
        }
        else
        {
            CCLOG("Warning: Animation for direction %d not available", (int)direction);
        }
    }
    else
    {
        // 站立状态设置
        auto texture = this->getTexture();
        if (texture)
        {
            float frameWidth = texture->getContentSize().width / 4;
            float frameHeight = texture->getContentSize().height / 13;

            // direction值直接作为纹理行索引
            int row = (int)direction;

            // 确保行索引在有效范围内
            int totalRows = texture->getContentSize().height / frameHeight;
            if (row >= 0 && row < totalRows)
            {
                this->setTextureRect(Rect(0, row * frameHeight, frameWidth, frameHeight));
            }
        }
    }
}

// 更新动画状态（根据移动状态）
void Player::updateAnimation()
{
    bool moving = (_velocity != Vec2::ZERO);

    Direction newDirection = _direction; // 默认保持当前方向

    if (moving)
    {
        // 根据速度方向确定面向方向
        if (_upPressed)
        {
            newDirection = Direction::UP;
        }
        else if (_downPressed)
        {
            newDirection = Direction::DOWN;
        }
        else if (_rightPressed)
        {
            newDirection = Direction::RIGHT;
        }
        else if (_leftPressed)
        {
            newDirection = Direction::LEFT;
        }
        // 设置行走动画
        setAnimation(newDirection, true);
    }
    else
    {
        // 设置站立动画
        setAnimation(_direction, false);
    }
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
        updateAnimation();
        };

    // 释放按键 (保持原有逻辑即可，通常不需要在松开时改变面向，除非你想让它回弹)
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
        // 如果需要更高级的逻辑（例如松开右键时，如果还在按上键，就转回向上），
        // 可以在 updateVelocity 里根据剩余的按键状态来修正 _direction。
        // 但根据您的要求，修改 onKeyPressed 已经足够。
        updateVelocity();
        updateAnimation();

        };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void Player::onExit()
{
    this->stopAllActions();

    //清理动画资源
    for (auto& pair : _walkAnimations)
    {
        if (pair.second)
        {
            pair.second->release();
            pair.second = nullptr;
        }
    }
    _walkAnimations.clear();

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
        Vec2 moveDelta = _velocity * _speed * dt;

        if (_map)
        {
            Vec2 newPos = currentPos;

            // 获取角色尺寸
            auto texture = this->getTexture();
            if (texture)
            {
                float frameWidth = texture->getContentSize().width / 4 * TILED_MAP_SCALE;
                float frameHeight = texture->getContentSize().height / 13 * TILED_MAP_SCALE;
                float halfWidth = frameWidth / 2;
                float halfHeight = frameHeight / 2;

                // 分离X轴和Y轴检测
                if (moveDelta.y != 0)
                {
                    Vec2 testPointY = Vec2(newPos.x, currentPos.y);
                    float targetY = currentPos.y + moveDelta.y;

                    testPointY.y = targetY - halfHeight * 0.5f; // 检测角色下部

                    if (!_map->isCollidable(testPointY))
                    {
                        newPos.y = targetY;
                    }
                }

                if (moveDelta.x != 0)
                {
                    float targetX = currentPos.x + moveDelta.x;
                    float testY = newPos.y - halfHeight * 0.4f; // 使用新的Y坐标
                    Vec2 testPointX;
                    testY = newPos.y - halfHeight * 0.5f;

                    if (moveDelta.x > 0) // 向右移动
                    {
                        testPointX = Vec2(targetX + halfWidth * 0.5f, testY); // 检测新位置的右边缘
                    }
                    else // 向左移动
                    {
                        testPointX = Vec2(targetX - halfWidth * 0.5f, testY); // 检测新位置的左边缘
                    }

                    if (!_map->isCollidable(testPointX))
                    {
                        newPos.x = targetX;
                    }
                }
            }

            this->setPosition(newPos);
        }
        else
        {
            this->setPosition(currentPos + moveDelta);
        }
    }
}
