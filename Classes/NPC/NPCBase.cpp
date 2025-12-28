/***************************************************************
* Project Name : StardewValley
* File Name : NPCBase.cpp
* File Function : NPCBase类的实现
* Author : 赵睿妍、蔡锦慧
* Update Date : 2025 / 12 / 20
* License : MIT License
* ***************************************************************/
#include "NPCBase.h"

USING_NS_CC;

// 初始化：设置 NPC 初始状态
bool NPCBase::init()
{
    // 调用父类初始化
    if (!Sprite::init()) {
        return false;
    }

    _isTextureLoaded = false;
    _friendship = 0; // 初始好感度为0
    return true;
}

// 接收礼物：计算好感度并返回对话
std::string NPCBase::receiveGift(ItemType itemTag)
{
    // 1. 计算礼物分值
    int points = checkGiftTaste(itemTag);

    // 2. 增加好感度
    increaseFriendship(points);

    // 3. 根据分值返回不同对话反馈
    if (points >= 80) return "Oh my god! I love this! (Love)";
    if (points >= 45) return "Thank you! (Like)";
    if (points < 0)   return "Ew... (Hate)";
    return "Thanks. (Neutral)";
}

// 检查礼物喜好（基类默认返回普通，子类需重写）
int NPCBase::checkGiftTaste(ItemType itemTag)
{
    return 40; // 默认普通礼物
}

// 增加好感度，包含数值范围限制
void NPCBase::increaseFriendship(int amount)
{
    _friendship += amount;

    // 限制范围 [0, 1500] (假设每颗心250点)
    if (_friendship > 1500) _friendship = 1500;
    if (_friendship < 0) _friendship = 0;

    CCLOG("NPC %s Friendship changed by %d. Total: %d", _npcName.c_str(), amount, _friendship);
}

// 加载纹理图片
bool NPCBase::loadTexture(const std::string& path)
{
    auto textureCache = Director::getInstance()->getTextureCache();
    if (textureCache == nullptr) return false;

    auto texture = textureCache->addImage(path);
    if (texture == nullptr) {
        CCLOG("Error: Failed to load texture %s", path.c_str());
        return false;
    }

    this->setTexture(texture);
    _isTextureLoaded = true;
    return true;
}

// 创建并播放行走动画 (基于 SpriteSheet 切割)
// totalRows: 总行数, totalCols: 总列数, animationRow: 当前播放第几行
void NPCBase::createAnimation(int totalRows, int totalCols, int animationRow, float frameDelay)
{
    if (!_isTextureLoaded) return;

    auto texture = this->getTexture();
    if (texture == nullptr) return;

    // 计算单帧尺寸
    float frameWidth = texture->getContentSize().width / totalCols;
    float frameHeight = texture->getContentSize().height / totalRows;

    // 设置初始静态帧 (防止动画未播放时显示错误)
    this->setTextureRect(Rect(0, animationRow * frameHeight, frameWidth, frameHeight));

    // 切割每一帧
    Vector<SpriteFrame*> frames;
    for (int col = 0; col < totalCols; col++) {
        float x = col * frameWidth;
        float y = animationRow * frameHeight;

        auto frame = SpriteFrame::createWithTexture(texture, Rect(x, y, frameWidth, frameHeight));
        if (frame) {
            frames.pushBack(frame);
        }
    }

    if (frames.empty()) return;

    // 创建无限循环动画
    auto animation = Animation::createWithSpriteFrames(frames, frameDelay);
    animation->setRestoreOriginalFrame(false);
    animation->setLoops(-1);

    auto animate = Animate::create(animation);
    this->runAction(RepeatForever::create(animate));
}

void NPCBase::setNPCPosition(const Vec2& position)
{
    this->setPosition(position);
}

void NPCBase::setNPCScale(float scale)
{
    this->setScale(scale);
}

void NPCBase::onEnter() {
    Sprite::onEnter();
    if (this->getNumberOfRunningActions() == 0 && _isTextureLoaded) {
        this->startWalkingAnimation(_currentAnimRow);
    }
}
void NPCBase::startWalkingAnimation(int animationRow)
{
    if (!_isTextureLoaded) return;
    _currentAnimRow = animationRow;

    this->stopAllActions(); // 确保开始新动画前清理旧的

    auto texture = this->getTexture();
    // 使用子类自己设置的 _totalCols 和 _totalRows
    float frameWidth = texture->getContentSize().width / _totalCols;
    float frameHeight = texture->getContentSize().height / _totalRows;

    Vector<SpriteFrame*> frames;
    for (int col = 0; col < _totalCols; col++) {
        auto frame = SpriteFrame::createWithTexture(texture,
            Rect(col * frameWidth, animationRow * frameHeight, frameWidth, frameHeight));
        if (frame) frames.pushBack(frame);
    }

    if (frames.empty()) return;

    auto animation = Animation::createWithSpriteFrames(frames, _frameDelay);
    animation->setLoops(-1);
    this->runAction(RepeatForever::create(Animate::create(animation)));
}

void NPCBase::stopWalkingAnimation()
{
    this->stopAllActions();
    // 停在当前朝向的第一帧
    auto texture = this->getTexture();
    if (texture) {
        float frameWidth = texture->getContentSize().width / _totalCols;
        float frameHeight = texture->getContentSize().height / _totalRows;
        this->setTextureRect(Rect(0, _currentAnimRow * frameHeight, frameWidth, frameHeight));
    }
}