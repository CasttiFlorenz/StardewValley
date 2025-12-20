#include "NPCBase.h"

bool NPCBase::init()
{
    if (!Sprite::init()) {
        return false;
    }

    // 设置锚点为中心
    this->setAnchorPoint(Vec2(0.5f, 0.5f));

    _isTextureLoaded = false;
    return true;
}

bool NPCBase::loadTexture(const std::string& path)
{
    auto texture = Director::getInstance()->getTextureCache()->addImage(path);
    if (!texture) {
        CCLOG("错误：无法加载纹理 %s", path.c_str());
        return false;
    }

    this->setTexture(texture);
    _isTextureLoaded = true;

    return true;
}

void NPCBase::createAnimation(int totalRows, int totalCols, int animationRow, float frameDelay)
{
    if (!_isTextureLoaded) return;

    auto texture = this->getTexture();

    // 计算每帧尺寸
    float frameWidth = texture->getContentSize().width / totalCols;
    float frameHeight = texture->getContentSize().height / totalRows;

    // 设置当前显示帧
    float x = 0;
    float y = animationRow * frameHeight;
    this->setTextureRect(Rect(x, y, frameWidth, frameHeight));

    // 创建帧容器
    Vector<SpriteFrame*> frames;

    // 收集该行的所有帧
    for (int col = 0; col < totalCols; col++) {
        float x = col * frameWidth;
        float y = animationRow * frameHeight;

        auto frame = SpriteFrame::createWithTexture(
            texture,
            Rect(x, y, frameWidth, frameHeight)
        );

        if (frame) {
            frames.pushBack(frame);
        }
    }

    // 创建动画
    auto animation = Animation::createWithSpriteFrames(frames, frameDelay);
    animation->setRestoreOriginalFrame(false);
    animation->setLoops(-1);

    // 创建并运行动画动作
    auto animate = Animate::create(animation);
    auto repeat = RepeatForever::create(animate);
    this->runAction(repeat);
}

void NPCBase::setNPCPosition(const Vec2& position)
{
    this->setPosition(position);
}

void NPCBase::setNPCScale(float scale)
{
    this->setScale(scale);
}


std::string NPCBase::getConversation(bool isFirstTalkToday)
{
    if (isFirstTalkToday) {
        _friendship += 20; // 每天第一次对话+20
        CCLOG("NPC %s 好感度提升! 当前: %d", _npcName.c_str(), _friendship);
    }

    // !!!这里可以根据好感度返回不同的话，可以去 Haley.cpp 里重写这个函数实现个性化
    if (_npcName == "Haley") return "Oh... you're the new farmer? My clothes are getting dirty just looking at you.";
    if (_npcName == "Sam") return "Hey! I'm trying to write a new song for my band.";
    if (_npcName == "Evelyn") return "Hello dear. You look just like my grandson.";

    return "Hi there.";
}

std::string NPCBase::receiveGift(Objects itemTag)
{
    if (_hasGiftedToday) {
        return "You already gave me a gift today!";
    }

    int points = checkGiftTaste(itemTag);
    _friendship += points;
    _hasGiftedToday = true;

    if (points >= 80) return "Oh my god! This is my favorite thing!"; // 最爱
    if (points >= 45) return "Thank you! I love this."; // 喜欢
    if (points < 0) return "Ew... what is this garbage?"; // 讨厌

    return "Thanks for the gift."; // 普通
}

int NPCBase::checkGiftTaste(Objects item)
{
    // !!!可以扩展

    if (_npcName == "Haley") {
        if (item == Objects::DAFFODILS) return 80; // Haley最爱黄水仙(假设)
        if (item == Objects::POTATO_SEED) return 80; // Haley最爱黄水仙(假设)
        if (item == Objects::PARSNIP) return -20; // 讨厌防风草
    }

    // 通用逻辑
    if (item == Objects::AXE) return 80;

    return 20; // 默认普通礼物
}

