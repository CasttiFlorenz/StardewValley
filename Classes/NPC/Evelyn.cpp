/****************************************************************
 * Project Name:  StardewValley
 * File Name:     Evelyn.cpp
 * File Function: Evelyn类的实现
 * Author:        赵睿妍、蔡锦慧
 * Update Date:   2025/12/24
 * License:       MIT License
 ****************************************************************/
#include "Evelyn.h"

USING_NS_CC;

bool Evelyn::init()
{
    if (!NPCBase::init()) return false;

    // 1. 设置 Evelyn 特有的规格
    _totalRows = 5;
    _totalCols = 4;
    _frameDelay = 0.2f;

    if (!loadTexture(PATH_NPC_EVELYN)) return false;

    setupStaticFrame();
    return true;
}

void Evelyn::startWalkingAnimation(int animationRow)
{
    // 记录行数，但只设置静态帧，不运行 runAction
    _currentAnimRow = animationRow;
    setupStaticFrame();
}

// 重写：停止动画逻辑
void Evelyn::stopWalkingAnimation()
{
    this->stopAllActions();
    setupStaticFrame();
}

void Evelyn::playAnimation()
{
    // Evelyn 是静态 NPC，不需要播放行走动画
}

void Evelyn::setupStaticFrame()
{
    auto texture = this->getTexture();
    if (texture == nullptr) return;

    // Evelyn 纹理: 5行 4列
    float frameWidth = texture->getContentSize().width / 4.0f;
    float frameHeight = texture->getContentSize().height / 5.0f;

    // 使用第1行第1帧 (索引 0,0)
    this->setTextureRect(Rect(0, 0, frameWidth, frameHeight));
}

std::vector<std::string> Evelyn::getConversation(bool isFirstMet) {
    this->increaseFriendship(50);
    std::vector<std::string> dialogue;

    if (isFirstMet) {
        dialogue.push_back("Oh my! You must be the new farmer.");
        dialogue.push_back("I'm Evelyn. It's so nice to meet you, dear.");
        dialogue.push_back("I have lived here for years.");
        dialogue.push_back("If you ever need anything, ask me.");
    }
    else {
        int randomChoice = rand() % 4;

        switch (randomChoice) {
        case 0:
            dialogue.push_back("I just baked a fresh batch of cookies.");
            dialogue.push_back("Would you like one, dear?");
            break;
        case 1:
            dialogue.push_back("My gardenias are blooming beautifully.");
            dialogue.push_back("The secret is talking to them every morning.");
            break;
        case 2:
            dialogue.push_back("Remember to take care of yourself.");
            dialogue.push_back("Farming is hard work.");
            dialogue.push_back("Don't forget to eat proper meals!");
            break;
        case 3:
            dialogue.push_back("How are you doing, dear?");
            dialogue.push_back("Is the farm keeping you busy?");
            break;
        }
    }
    return dialogue;
}

int Evelyn::checkGiftTaste(ItemType itemTag)
{
    // 最爱
    if (itemTag == ItemType::CAULIFLOWER) return 80;
    if (itemTag == ItemType::DAFFODILS) return 80;

    // 喜欢
    if (itemTag == ItemType::MILK) return 45;
    if (itemTag == ItemType::SALAD) return 45;
    if (itemTag == ItemType::LEEK) return 45;

    // 讨厌
    if (itemTag == ItemType::FISHINGROD) return -20;
    if (itemTag == ItemType::COPPER_ORE) return -20;

    return NPCBase::checkGiftTaste(itemTag);
}