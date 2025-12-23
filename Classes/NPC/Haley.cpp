#include "Haley.h"

bool Haley::init()
{
    if (!NPCBase::init()) {
        return false;
    }

    // 加载纹理
    if (!loadTexture("NPC/Haley..png")) {
        return false;
    }

    // 创建并播放动画：12行4列，使用第11行（索引10）
    createAnimation(12, 4, 10, 0.15f);

    return true;
}

void Haley::playAnimation()
{

}
std::vector<std::string> Haley::getConversation(bool isFirstMet)
{
    this->increaseFriendship(50);

    std::vector<std::string> dialogue;

    if (isFirstMet) {
        // 初次见面 
        dialogue.push_back("Oh... aren't you the new farmer?");
        dialogue.push_back("I'm Haley.");
    }
    else {
        // 日常对话
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


int Haley::checkGiftTaste(ItemType  itemTag)
{
    // --- 最爱 (+80) ---
    if (itemTag == ItemType::DAFFODILS) return 80;
    if (itemTag == ItemType::SALAD) return 80;

    // --- 喜欢 (+45) ---
    if (itemTag == ItemType::FRIED_EGG) return 45;

    // --- 讨厌 (-20) ---
    if (itemTag == ItemType::PARSNIP) return -20;
    if (itemTag == ItemType::POTATO) return -20;

    // 讨厌鱼
    if (itemTag == ItemType::CARP) return -20;
    return NPCBase::checkGiftTaste(itemTag);
}