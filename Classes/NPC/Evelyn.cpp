#include "Evelyn.h"

bool Evelyn::init()
{
    if (!NPCBase::init()) {
        return false;
    }

    // 加载Evelyn的纹理
    if (!loadTexture("NPC/Evelyn..png")) { 
        return false;
    }

    // 设置静态帧（不播放动画）
    setupStaticFrame();

    return true;
}

void Evelyn::setupStaticFrame()
{
    auto texture = this->getTexture();
    if (!texture) return;

    // Evelyn纹理是5行4列
    int totalRows = 5;
    int totalCols = 4;

    // 计算每帧尺寸
    float frameWidth = texture->getContentSize().width / totalCols;
    float frameHeight = texture->getContentSize().height / totalRows;

    // 使用第1行第1帧（索引0,0）作为静态显示
    float x = 0;  // 第1列
    float y = 0 * frameHeight;  // 第1行（索引0）

    // 设置纹理显示区域
    this->setTextureRect(Rect(x, y, frameWidth, frameHeight));

    CCLOG("Evelyn设置为静态显示：第1行第1帧");
}


std::vector<std::string> Evelyn::getConversation(bool isFirstMet) {
    this->increaseFriendship(10);

    std::vector<std::string> dialogue;

    if (isFirstMet) {
        dialogue.push_back("Oh my! You must be the new farmer.");
        dialogue.push_back("I'm Evelyn. It's so nice to meet you, dear.");
        dialogue.push_back("My husband George and I have lived here for years.");
        dialogue.push_back("If you ever need anything, don't hesitate to ask.");
    }
    else {
        int randomChoice = rand() % 4;

        switch (randomChoice) {
        case 0:
            dialogue.push_back("I just baked a fresh batch of cookies.");
            dialogue.push_back("Would you like one, dear? They're still warm.");
            break;

        case 1:
            dialogue.push_back("My gardenias are blooming beautifully this year.");
            dialogue.push_back("The secret is talking to them every morning.");
            break;

        case 2: 
            dialogue.push_back("Remember to take care of yourself, dear.");
            dialogue.push_back("Farming is hard work.");
            dialogue.push_back("Don't forget to eat proper meals!");
            break;

        case 4: 
            dialogue.push_back("How are you doing, dear?");
            dialogue.push_back("Is the farm keeping you busy?");
            dialogue.push_back("You look like you've been working hard.");
            break;
        }
    }

    return dialogue;
}
int Evelyn::checkGiftTaste(ItemType itemTag)
{
    // --- 最爱 (+80) ---
 
    if (itemTag == ItemType::CAULIFLOWER) return 80;
    if (itemTag == ItemType::DAFFODILS) return 80;

    // --- 喜欢 (+45) ---
    if (itemTag == ItemType::MILK) return 45; 
    if (itemTag == ItemType::SALAD) return 45;
    if (itemTag == ItemType::LEEK) return 45; 

    // --- 讨厌 (-20) ---
    if (itemTag == ItemType::FISHINGROD) return -20; 
    if (itemTag == ItemType::COPPER_ORE) return -20;

    return NPCBase::checkGiftTaste(itemTag);
}