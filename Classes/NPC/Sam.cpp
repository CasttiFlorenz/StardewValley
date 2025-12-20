// Sam.cpp
#include "Sam.h"

bool Sam::init()
{
    if (!NPCBase::init()) {
        return false;
    }

    // 加载纹理
    if (!loadTexture("NPC/Sam..png")) {
        return false;
    }

    // 创建并播放动画
    createAndPlayAnimation();

    CCLOG("Sam NPC 初始化成功");
    return true;
}

void Sam::playAnimation()
{
    createAndPlayAnimation();
}

void Sam::createAndPlayAnimation()
{
    auto texture = this->getTexture();
    if (!texture) return;

    float frameWidth = texture->getContentSize().width / 4;
    float frameHeight = texture->getContentSize().height / 13;

    // 创建帧容器
    Vector<SpriteFrame*> frames;

    int rowIndex1 = 6; 
    int rowIndex2 = 7; 

    // 添加第7行的4个帧
    for (int col = 0; col < 4; col++) {
        float x = col * frameWidth;
        float y = rowIndex1 * frameHeight;

        auto frame = SpriteFrame::createWithTexture(
            texture,
            Rect(x, y, frameWidth, frameHeight)
        );

        if (frame) {
            frames.pushBack(frame);
        }
    }

    // 添加第8行的4个帧（共8帧）
    for (int col = 0; col < 4; col++) {
        float x = col * frameWidth;
        float y = rowIndex2 * frameHeight;

        auto frame = SpriteFrame::createWithTexture(
            texture,
            Rect(x, y, frameWidth, frameHeight)
        );

        if (frame) {
            frames.pushBack(frame);
        }
    }

    // 检查是否有足够的帧
    if (frames.size() != 8) {
        CCLOG("警告：Sam动画帧数量不足");
        return;
    }

    // 创建动画（每帧0.15秒）
    auto animation = Animation::createWithSpriteFrames(frames, 0.15f);
    animation->setRestoreOriginalFrame(false);
    animation->setDelayPerUnit(0.15f);
    animation->setLoops(-1);  // 无限循环

    this->stopAllActions();

    auto animate = Animate::create(animation);

    auto repeat = RepeatForever::create(animate);

    this->runAction(repeat);

    CCLOG("Sam动画开始播放");
}