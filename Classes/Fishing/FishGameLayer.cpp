#include "FishGameLayer.h"
#include "../Inventory/InventoryScene.h"
#include "../Player/SkillLevel.h" 

USING_NS_CC;

// 创建实例
FishGameLayer* FishGameLayer::create()
{
    FishGameLayer* layer = new (std::nothrow) FishGameLayer();
    if (layer && layer->init())
    {
        layer->autorelease();
        return layer;
    }
    else
    {
        CC_SAFE_DELETE(layer);
        return nullptr;
    }
}

// 初始化
bool FishGameLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }

    // 初始化游戏参数
    _currentValue = 50.0f;
    _isGameActive = true;

    // 初始化布局参数
    _progressBarRect = Rect(93, 20, 5, 272);
    _fishBasePosition = Vec2(73, 22);
    _progressBarHeight = 400.0f;

    // 设置界面与事件
    setupUI();
    setupEvents();

    // 开启每帧更新
    this->scheduleUpdate();

    return true;
}

// 设置UI
void FishGameLayer::setupUI()
{
    const auto visibleSize = Director::getInstance()->getVisibleSize();

    // 1. 加载背景图片
    _background = Sprite::create("Fishing/Fishingicons.png");
    if (_background) {
        _background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
        _background->setScale(2.0f); // 放大2倍
        this->addChild(_background, 0);
    }

    if (!_background) return;

    // 2. 计算进度条在屏幕上的位置
    const Vec2 backgroundPos = _background->getPosition();
    const Size backgroundSize = _background->getContentSize();
    const Vec2 backgroundAnchor = _background->getAnchorPoint();
    const float backgroundScale = _background->getScale();

    const float progressBarScreenX = backgroundPos.x - backgroundSize.width * backgroundAnchor.x * backgroundScale
        + _progressBarRect.origin.x * backgroundScale;
    const float progressBarScreenY = backgroundPos.y - backgroundSize.height * backgroundAnchor.y * backgroundScale
        + _progressBarRect.origin.y * backgroundScale;

    // 创建进度条绘制节点
    _progressBar = DrawNode::create();

    // 初始绘制
    const float greenHeight = (_currentValue / 1000.0f) * _progressBarRect.size.height;

    Vec2 greenPoints[4] = {
        Vec2(progressBarScreenX, progressBarScreenY),  // 左下
        Vec2(progressBarScreenX + _progressBarRect.size.width * backgroundScale, progressBarScreenY),  // 右下
        Vec2(progressBarScreenX + _progressBarRect.size.width * backgroundScale, progressBarScreenY + greenHeight * backgroundScale),  // 右上
        Vec2(progressBarScreenX, progressBarScreenY + greenHeight * backgroundScale)  // 左上
    };

    _progressBar->drawSolidPoly(greenPoints, 4, Color4F(0.0f, 1.0f, 0.0f, 1.0f));
    this->addChild(_progressBar, 1);

    // 3. 加载鱼的精灵
    _fishSprite = Sprite::create("Fishing/Tuna.png");
    if (_fishSprite) {
        const float fishScreenX = backgroundPos.x - backgroundSize.width * backgroundAnchor.x * backgroundScale
            + _fishBasePosition.x * backgroundScale;
        const float fishScreenY = backgroundPos.y - backgroundSize.height * backgroundAnchor.y * backgroundScale
            + _fishBasePosition.y * backgroundScale
            + (_currentValue / 1000.0f) * _progressBarRect.size.height * backgroundScale;

        _fishSprite->setPosition(Vec2(fishScreenX, fishScreenY));
        _fishSprite->setScale(1.5f);
        this->addChild(_fishSprite, 2);
    }

    // 4. 创建结果标签
    _resultLabel = Label::createWithSystemFont("", "Arial", 48);
    if (_resultLabel) {
        _resultLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 50));
        _resultLabel->setVisible(false);
        this->addChild(_resultLabel, 3);
    }
}

// 设置事件监听
void FishGameLayer::setupEvents()
{
    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseDown = [this](EventMouse* event) {
        if (_isGameActive && event->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT) {
            this->onMouseClick();
        }
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
}

// 鼠标点击处理
void FishGameLayer::onMouseClick()
{
    if (!_isGameActive) return;

    // 点击增加进度
    _currentValue += 30.0f;
    if (_currentValue > 1000.0f) {
        _currentValue = 1000.0f;
    }

    // 更新视图
    updateProgressBar();
    updateFishPosition();

    // 播放鱼的跳跃动画
    auto jumpUp = ScaleTo::create(0.1f, 1.8f);
    auto jumpDown = ScaleTo::create(0.1f, 1.5f);
    auto sequence = Sequence::create(jumpUp, jumpDown, nullptr);

    if (_fishSprite) {
        _fishSprite->stopAllActions();
        _fishSprite->runAction(sequence);
    }

    // 检查胜利条件
    if (_currentValue >= 1000.0f) {
        endGame(true);
    }
}

// 帧更新
void FishGameLayer::update(float delta)
{
    if (!_isGameActive) return;

    // 进度自然衰减
    _currentValue -= 60.0f * delta;
    if (_currentValue < 0.0f) {
        _currentValue = 0.0f;
    }

    // 更新视图
    updateProgressBar();
    updateFishPosition();

    // 鱼的悬浮动画（正弦波运动）
    static float floatTime = 0;
    floatTime += delta;
    const float floatOffset = std::sin(floatTime * 3.0f) * 12.0f;

    if (_fishSprite) {
        _fishSprite->setPositionY(_fishSprite->getPositionY() + floatOffset);
    }

    // 检查失败条件
    if (_currentValue <= 0.0f) {
        endGame(false);
    }
}

// 更新进度条
void FishGameLayer::updateProgressBar()
{
    if (!_progressBar || !_background) return;

    // 清除旧的绘制
    _progressBar->clear();

    // 重新计算绘制参数
    const Vec2 backgroundPos = _background->getPosition();
    const Size backgroundSize = _background->getContentSize();
    const Vec2 backgroundAnchor = _background->getAnchorPoint();
    const float backgroundScale = _background->getScale();

    const float progressBarScreenX = backgroundPos.x - backgroundSize.width * backgroundAnchor.x * backgroundScale
        + _progressBarRect.origin.x * backgroundScale;
    const float progressBarScreenY = backgroundPos.y - backgroundSize.height * backgroundAnchor.y * backgroundScale
        + _progressBarRect.origin.y * backgroundScale;

    const float greenHeight = (_currentValue / 1000.0f) * _progressBarRect.size.height * backgroundScale;

    Vec2 greenPoints[4] = {
        Vec2(progressBarScreenX, progressBarScreenY),
        Vec2(progressBarScreenX + _progressBarRect.size.width * backgroundScale, progressBarScreenY),
        Vec2(progressBarScreenX + _progressBarRect.size.width * backgroundScale, progressBarScreenY + greenHeight),
        Vec2(progressBarScreenX, progressBarScreenY + greenHeight)
    };

    // 根据进度值改变颜色
    const float percentage = _currentValue / 1000.0f;
    Color4F barColor;
    if (percentage > 0.7f) {
        barColor = Color4F(0.0f, 1.0f, 0.0f, 1.0f); // 绿色
    }
    else if (percentage > 0.3f) {
        barColor = Color4F(1.0f, 1.0f, 0.0f, 1.0f); // 黄色
    }
    else {
        barColor = Color4F(1.0f, 0.0f, 0.0f, 1.0f); // 红色
    }

    _progressBar->drawSolidPoly(greenPoints, 4, barColor);
}

// 更新鱼的位置
void FishGameLayer::updateFishPosition()
{
    if (!_fishSprite || !_background) return;

    const Vec2 backgroundPos = _background->getPosition();
    const Size backgroundSize = _background->getContentSize();
    const Vec2 backgroundAnchor = _background->getAnchorPoint();
    const float backgroundScale = _background->getScale();

    const float fishScreenX = backgroundPos.x - backgroundSize.width * backgroundAnchor.x * backgroundScale
        + _fishBasePosition.x * backgroundScale;
    const float fishScreenY = backgroundPos.y - backgroundSize.height * backgroundAnchor.y * backgroundScale
        + _fishBasePosition.y * backgroundScale
        + (_currentValue / 1000.0f) * _progressBarRect.size.height * backgroundScale;

    _fishSprite->setPosition(Vec2(fishScreenX, fishScreenY));
}

// 结束游戏
void FishGameLayer::endGame(bool isSuccess)
{
    _isGameActive = false;

    if (!_resultLabel) return;

    if (isSuccess) {
        _resultLabel->setString("Success!");
        _resultLabel->setColor(Color3B::GREEN);

        // 增加物品
        if (auto inv = InventoryScene::getInstance()) {
            inv->addItemCount(ItemType::CARP, 1);
        }

        // 增加技能经验
        if (auto skill = SkillLevel::getInstance()) {
            skill->increaseSkillLevel(SkillType::FISHING);
        }
    }
    else {
        _resultLabel->setString("Failed!");
        _resultLabel->setColor(Color3B::RED);
    }

    _resultLabel->setVisible(true);
    this->unscheduleUpdate();

    // 2秒后移除自身
    this->scheduleOnce([this](float dt) {
        this->removeFromParent();
        }, 2.0f, "return_to_farm");
}