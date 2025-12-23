#include "FishGameLayer.h"

USING_NS_CC;

FishGameLayer* FishGameLayer::create()
{
    FishGameLayer* layer = new FishGameLayer();
    if (layer && layer->init())
    {
        layer->autorelease();
        return layer;
    }
    else
    {
        delete layer;
        return nullptr;
    }
}

bool FishGameLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }

    _currentValue = 50.0f;  // 初始值
    _isGameActive = true;

    _progressBarRect = Rect(93, 20, 5, 272);

    _fishBasePosition = Vec2(73, 22);

    // 进度条可填充高度
    _progressBarHeight = 400.0f;

    setupUI();

    setupEvents();

    this->scheduleUpdate();

    return true;
}

void FishGameLayer::setupUI()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();

    //1. 加载背景图片
    _background = Sprite::create("Fishingicons.png");

    _background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

    _background->setScale(2.0f); // 放大到2.0倍

    this->addChild(_background, 0);

    //2. 在背景图片上创建进度条
    Vec2 backgroundPos = _background->getPosition();
    Size backgroundSize = _background->getContentSize();
    Vec2 backgroundAnchor = _background->getAnchorPoint();
    float backgroundScale = _background->getScale(); // 获取缩放比例

    float progressBarScreenX = backgroundPos.x - backgroundSize.width * backgroundAnchor.x * backgroundScale
        + _progressBarRect.origin.x * backgroundScale;
    float progressBarScreenY = backgroundPos.y - backgroundSize.height * backgroundAnchor.y * backgroundScale
        + _progressBarRect.origin.y * backgroundScale;

    // 创建绿色进度条
    _progressBar = DrawNode::create();

    float greenHeight = (_currentValue / 1000.0f) * _progressBarRect.size.height;

    float progressBarWidth = _progressBarRect.size.width * backgroundScale;
    float progressBarHeight = greenHeight * backgroundScale;

    Vec2 greenPoints[4] = {
        Vec2(progressBarScreenX, progressBarScreenY),  // 左下角
        Vec2(progressBarScreenX + _progressBarRect.size.width, progressBarScreenY),  // 右下角
        Vec2(progressBarScreenX + _progressBarRect.size.width, progressBarScreenY + greenHeight),  // 右上角
        Vec2(progressBarScreenX, progressBarScreenY + greenHeight)  // 左上角
    };

    _progressBar->drawSolidPoly(greenPoints, 4, Color4F(0.0f, 1.0f, 0.0f, 1.0f));
    this->addChild(_progressBar, 1);

    //3. 在背景图片上创建鱼
    _fishSprite = Sprite::create("Tuna.png");

    float fishScreenX = backgroundPos.x - backgroundSize.width * backgroundAnchor.x + _fishBasePosition.x;
    float fishScreenY = backgroundPos.y - backgroundSize.height * backgroundAnchor.y + _fishBasePosition.y +
        (_currentValue / 1000.0f) * _progressBarRect.size.height;

    _fishSprite->setPosition(Vec2(fishScreenX, fishScreenY));
    _fishSprite->setScale(1.5f);
    this->addChild(_fishSprite, 2);

    //4. 创建结果标签
    _resultLabel = Label::createWithSystemFont("", "Arial", 48);
    _resultLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 50));
    _resultLabel->setVisible(false);
    this->addChild(_resultLabel, 3);
}

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

void FishGameLayer::onMouseClick()
{
    if (!_isGameActive) return;

    _currentValue += 10.0f;
    if (_currentValue > 1000.0f) {
        _currentValue = 1000.0f;
    }

    // 更新进度条和鱼的位置
    updateProgressBar();
    updateFishPosition();

    // 鱼跳动动画
    auto jumpUp = ScaleTo::create(0.1f, 1.8f);
    auto jumpDown = ScaleTo::create(0.1f, 1.5f);
    auto sequence = Sequence::create(jumpUp, jumpDown, nullptr);
    _fishSprite->stopAllActions();
    _fishSprite->runAction(sequence);

    if (_currentValue >= 1000.0f) {
        endGame(true);
    }
}

void FishGameLayer::update(float delta)
{
    if (!_isGameActive) return;

    _currentValue -= 60.0f * delta;
    if (_currentValue < 0.0f) {
        _currentValue = 0.0f;
    }

    // 更新进度条和鱼的位置
    updateProgressBar();
    updateFishPosition();

    // 鱼浮动动画
    static float floatTime = 0;
    floatTime += delta;
    float floatOffset = sin(floatTime * 3.0f) * 12.0f;
    _fishSprite->setPositionY(_fishSprite->getPositionY() + floatOffset);

    if (_currentValue <= 0.0f) {
        endGame(false);
    }
}

void FishGameLayer::updateProgressBar()
{
    // 清除旧的进度条绘制
    _progressBar->clear();

    // 获取背景图片的世界坐标和锚点信息
    Vec2 backgroundPos = _background->getPosition();
    Size backgroundSize = _background->getContentSize();
    Vec2 backgroundAnchor = _background->getAnchorPoint();
    float backgroundScale = _background->getScale(); // 获取缩放比例

    float progressBarScreenX = backgroundPos.x - backgroundSize.width * backgroundAnchor.x * backgroundScale
        + _progressBarRect.origin.x * backgroundScale;
    float progressBarScreenY = backgroundPos.y - backgroundSize.height * backgroundAnchor.y * backgroundScale
        + _progressBarRect.origin.y * backgroundScale;

    float greenHeight = (_currentValue / 1000.0f) * _progressBarRect.size.height * backgroundScale;

    Vec2 greenPoints[4] = {
        Vec2(progressBarScreenX, progressBarScreenY),
        Vec2(progressBarScreenX + _progressBarRect.size.width * backgroundScale, progressBarScreenY),
        Vec2(progressBarScreenX + _progressBarRect.size.width * backgroundScale, progressBarScreenY + greenHeight),
        Vec2(progressBarScreenX, progressBarScreenY + greenHeight)
    };

    // 根据进度改变颜色
    float percentage = _currentValue / 1000.0f;
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

void FishGameLayer::updateFishPosition()
{
    Vec2 backgroundPos = _background->getPosition();
    Size backgroundSize = _background->getContentSize();
    Vec2 backgroundAnchor = _background->getAnchorPoint();
    float backgroundScale = _background->getScale();

    float fishScreenX = backgroundPos.x - backgroundSize.width * backgroundAnchor.x * backgroundScale
        + _fishBasePosition.x * backgroundScale;
    float fishScreenY = backgroundPos.y - backgroundSize.height * backgroundAnchor.y * backgroundScale
        + _fishBasePosition.y * backgroundScale
        + (_currentValue / 1000.0f) * _progressBarRect.size.height * backgroundScale;

    _fishSprite->setPosition(Vec2(fishScreenX, fishScreenY));
}

void FishGameLayer::endGame(bool isSuccess)
{
    _isGameActive = false;

    if (isSuccess) {
        _resultLabel->setString("Success!");
        _resultLabel->setColor(Color3B::GREEN);
    }
    else {
        _resultLabel->setString("Failed!");
        _resultLabel->setColor(Color3B::RED);
    }

    _resultLabel->setVisible(true);
    this->unscheduleUpdate();

    this->scheduleOnce([this](float dt) {
        // 弹出现有场景（返回农场）
        this->removeFromParent();
        // 注意：这里需要在GameScene中恢复时间和玩家移动
        // 我们需要在FishGameScene中传递消息，或者让GameScene自己恢复
        }, 2.0f, "return_to_farm");
}