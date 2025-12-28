/***************************************************************
* Project Name : StardewValley
* File Name : Sam.cpp
* File Function : Sam类的实现
* Author : 赵睿妍、蔡锦慧
* Update Date : 2025 / 12 / 20
* License : MIT License
* ***************************************************************/
#include "Sam.h"

USING_NS_CC;

bool Sam::init()
{
    if (!NPCBase::init()) {
        return false;
    }

    // 使用常量路径
    if (!loadTexture(PATH_NPC_SAM)) {
        return false;
    }

    playAnimation();
    return true;
}

void Sam::playAnimation()
{
    createAndPlayAnimation();
}

void Sam::createAndPlayAnimation()
{
    auto texture = this->getTexture();
    if (texture == nullptr) return;

    // Sam 纹理布局: 13行 4列
    float frameWidth = texture->getContentSize().width / 4.0f;
    float frameHeight = texture->getContentSize().height / 13.0f;

    // 动作位于第7行和第8行 (索引6和7)
    int rowIndex1 = 6;
    int rowIndex2 = 7;

    // 设置初始显示区域
    this->setTextureRect(Rect(0, rowIndex1 * frameHeight, frameWidth, frameHeight));

    Vector<SpriteFrame*> frames;

    // 收集第一行帧
    for (int col = 0; col < 4; col++) {
        auto frame = SpriteFrame::createWithTexture(
            texture,
            Rect(col * frameWidth, rowIndex1 * frameHeight, frameWidth, frameHeight)
        );
        if (frame) frames.pushBack(frame);
    }

    // 收集第二行帧
    for (int col = 0; col < 4; col++) {
        auto frame = SpriteFrame::createWithTexture(
            texture,
            Rect(col * frameWidth, rowIndex2 * frameHeight, frameWidth, frameHeight)
        );
        if (frame) frames.pushBack(frame);
    }

    if (frames.empty()) return;

    // 创建动画
    auto animation = Animation::createWithSpriteFrames(frames, 0.15f);
    animation->setRestoreOriginalFrame(false);
    animation->setLoops(-1);

    auto animate = Animate::create(animation);
    this->runAction(RepeatForever::create(animate));
}

std::vector<std::string> Sam::getConversation(bool isFirstMet) {
    this->increaseFriendship(50);

    std::vector<std::string> dialogue;

    if (isFirstMet) {
        dialogue.push_back("Hey, you're the new farmer, right?");
        dialogue.push_back("I'm Sam. I live in town with my mom.");
        dialogue.push_back("I play in a band with Sebastian.");
        dialogue.push_back("If you're into music, you should come listen.");
    }
    else {
        int randomChoice = rand() % 6;
        switch (randomChoice) {
        case 0:
            dialogue.push_back("I was practicing my skateboard tricks.");
            dialogue.push_back("Almost landed a kickflip!");
            break;
        case 1:
            dialogue.push_back("Sebastian's working on some new music.");
            dialogue.push_back("I can't wait to hear it!");
            break;
        case 2:
            dialogue.push_back("Mom made pancakes this morning.");
            break;
        case 3:
            dialogue.push_back("Vincent keeps trying to play my guitar.");
            break;
        case 4:
            dialogue.push_back("I work part-time at JojaMart.");
            dialogue.push_back("Not my dream job, but it pays the bills.");
            break;
        case 5:
            dialogue.push_back("Nice day for a skate, don't you think?");
            break;
        }
    }
    return dialogue;
}

int Sam::checkGiftTaste(ItemType itemTag)
{
    // 最爱
    if (itemTag == ItemType::FRIED_EGG) return 80;

    // 喜欢
    if (itemTag == ItemType::EGG) return 45;
    if (itemTag == ItemType::SALAD) return 45;

    // 讨厌
    if (itemTag == ItemType::CAULIFLOWER) return -20;
    if (itemTag == ItemType::PARSNIP) return -20;
    if (itemTag == ItemType::COPPER_ORE) return -20;

    return NPCBase::checkGiftTaste(itemTag);
}


void Sam::startWalkingAnimation(int animationRow) {
    this->stopAllActions(); // 同样要先停止旧动作

    auto texture = this->getTexture();
    float fw = texture->getContentSize().width / 4.0f;
    float fh = texture->getContentSize().height / 13.0f;

    // Sam 独特的两行采样逻辑
    cocos2d::Vector<cocos2d::SpriteFrame*> frames;
    for (int r = 6; r <= 7; r++) { // 遍历索引6和7两行
        for (int c = 0; c < 4; c++) {
            frames.pushBack(SpriteFrame::createWithTexture(texture, Rect(c * fw, r * fh, fw, fh)));
        }
    }
    auto animate = Animate::create(Animation::createWithSpriteFrames(frames, 0.15f));
    this->runAction(RepeatForever::create(animate));
}

/**
 * Sam 专属的停止函数
 * 确保他停在第7行（索引6）的第一帧，而不是随机停在第8行
 */
void Sam::stopWalkingAnimation()
{
    // 1. 停止所有的动作（RepeatForever）
    this->stopAllActions();

    // 2. 强制回到第7行（索引6）的第一帧
    auto texture = this->getTexture();
    if (texture) {
        // Sam 是 13行 4列
        float fw = texture->getContentSize().width / 4.0f;
        float fh = texture->getContentSize().height / 13.0f;

        // 索引 6 代表第 7 行（通常是他的默认站立/行走起始行）
        this->setTextureRect(Rect(0, 6 * fh, fw, fh));
    }
}