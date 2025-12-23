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

std::vector<std::string> Sam::getConversation(bool isFirstMet) {
    this->increaseFriendship(10);

    std::vector<std::string> dialogue;

    if (isFirstMet) {
        dialogue.push_back("Hey, you're the new farmer, right? Nice to meet you.");
        dialogue.push_back("I'm Sam. I live in town with my mom and little brother.");
        dialogue.push_back("I play in a band with Sebastian and Abigail.");
        dialogue.push_back("If you're into music, you should come listen sometime.");
    }
    else {
        // 随机对话
        int randomChoice = rand() % 6;
        switch (randomChoice) {
        case 0:
            dialogue.push_back("I was practicing my skateboard tricks earlier.");
            dialogue.push_back("Almost landed a kickflip!");
            break;
        case 1:
            dialogue.push_back("Sebastian's working on some new music.");
            dialogue.push_back("I can't wait to hear it!");
            break;
        case 2:
            dialogue.push_back("Mom made pancakes this morning. They were amazing!");
            break;
        case 3:
            dialogue.push_back("Vincent keeps trying to play my guitar.");
            dialogue.push_back("He's getting pretty good for a little kid.");
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
    // --- 最爱 (+80) ---
    if (itemTag == ItemType::FRIED_EGG) return 80;

    // --- 喜欢 (+45) ---
    if (itemTag == ItemType::EGG) return 45;
    if (itemTag == ItemType::SALAD) return 45;

    // --- 讨厌 (-20) ---
    if (itemTag == ItemType::CAULIFLOWER) return -20;
    if (itemTag == ItemType::PARSNIP) return -20;
    if (itemTag == ItemType::COPPER_ORE) return -20;

    return NPCBase::checkGiftTaste(itemTag);
}