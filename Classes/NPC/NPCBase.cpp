#include "NPCBase.h"
bool NPCBase::init()
{
    if (!Sprite::init()) {
        return false;
    }
    _isTextureLoaded = false;
    _friendship = 0;
    return true;
}

std::string NPCBase::receiveGift(ItemType itemTag)
{
    int points = checkGiftTaste(itemTag);

    // 调用上面写的通用函数
    increaseFriendship(points);

    // 返回对话文本
    if (points >= 80) return "Oh my god! I love this! (Love)";
    if (points >= 45) return "Thank you! (Like)";
    if (points < 0)   return "Ew... (Hate)";
    return "Thanks. (Neutral)";
}

// 默认喜好
int NPCBase::checkGiftTaste(ItemType itemTag)
{
    return 20; // 默认普通礼物加20分
}


void NPCBase::increaseFriendship(int amount)
{
    _friendship += amount;

    // 限制范围
    if (_friendship > 1500) _friendship = 1500; 
    if (_friendship < 0) _friendship = 0;

    CCLOG("NPC %s Friendship changed by %d. Total: %d", _npcName.c_str(), amount, _friendship);
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
