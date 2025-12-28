/****************************************************************
 * Project Name:  StardewValley
 * File Name:     Haley.cpp
 * File Function: Haley类的实现
 * Author:        赵睿妍、蔡锦慧
 * Update Date:   2025/12/24
 * License:       MIT License
 ****************************************************************/
#include "Haley.h"

USING_NS_CC;

bool Haley::init() {
    if (!NPCBase::init()) return false;
    _totalRows = 12;     
    _totalCols = 4;    
    _currentAnimRow = 10; 
    _frameDelay = 0.15f;

    if (!loadTexture(PATH_NPC_HALEY)) return false;
    playAnimation();
    return true;
}

void Haley::playAnimation() {
    this->startWalkingAnimation(_currentAnimRow);
}
std::vector<std::string> Haley::getConversation(bool isFirstMet)
{
    // 增加好感度
    this->increaseFriendship(50);

    std::vector<std::string> dialogue;

    if (isFirstMet) {
        dialogue.push_back("Oh... aren't you the new farmer?");
        dialogue.push_back("I'm Haley.");
        dialogue.push_back("If it were me, I would hate getting dirty.");
    }
    else {
        int random = cocos2d::RandomHelper::random_int(0, 2);

        if (random == 0) {
            dialogue.push_back("This town is so small.");
            dialogue.push_back("I wish I could go to the mall.");
        }
        else if (random == 1) {
            dialogue.push_back("Do you like my dress?");
            dialogue.push_back("I bought it on sale last week.");
        }
        else {
            dialogue.push_back("Being a farmer sounds like so much work.");
            dialogue.push_back("Make sure you don't smell like dirt.");
        }
    }

    return dialogue;
}

int Haley::checkGiftTaste(ItemType itemTag)
{
    // 最爱 (+80)
    if (itemTag == ItemType::DAFFODILS) return 80;
    if (itemTag == ItemType::SALAD) return 80;

    // 喜欢 (+45)
    if (itemTag == ItemType::FRIED_EGG) return 45;


    // 讨厌 (-20)
    if (itemTag == ItemType::PARSNIP) return -20;
    if (itemTag == ItemType::POTATO) return -20;
    if (itemTag == ItemType::CARP) return -20;

    // 默认值
    return NPCBase::checkGiftTaste(itemTag);

}
