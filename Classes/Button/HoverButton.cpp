#include "HoverButton.h"

USING_NS_CC;
using namespace cocos2d::ui;

// 创建按钮实例
HoverButton* HoverButton::create(const std::string& normalImage,
    const std::string& selectedImage,
    const std::string& disabledImage)
{
    HoverButton* button = new (std::nothrow) HoverButton();
    if (button && button->init(normalImage, selectedImage, disabledImage))
    {
        button->autorelease();
        return button;
    }
    CC_SAFE_DELETE(button);
    return nullptr;
}

// 初始化按钮
bool HoverButton::init(const std::string& normalImage,
    const std::string& selectedImage,
    const std::string& disabledImage)
{
    if (!Button::init(normalImage, selectedImage, disabledImage))
    {
        return false;
    }

    _normalImage = normalImage;
    _selectedImage = selectedImage;
    _disabledImage = disabledImage;

    // 注册鼠标事件监听器
    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseMove = CC_CALLBACK_1(HoverButton::onMouseMove, this);
    mouseListener->onMouseUp = CC_CALLBACK_1(HoverButton::onMouseUp, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

    return true;
}

// 鼠标移动事件
void HoverButton::onMouseMove(cocos2d::Event* event)
{
    auto* mouseEvent = static_cast<EventMouse*>(event);

    // 获取父节点并转换坐标
    if (!this->getParent()) return;

    const auto locationInNode = this->getParent()->convertToNodeSpace(mouseEvent->getLocationInView());
    const auto boundingBox = this->getBoundingBox();

    // 判断鼠标是否在按钮范围内
    if (boundingBox.containsPoint(locationInNode))
    {
        // 放大并切换图片
        if (this->getScaleX() == _baseScaleX && this->getScaleY() == _baseScaleY)
        {
            this->stopAllActions();
            this->runAction(ScaleTo::create(0, _hoverScale));
        }

        if (!_selectedImage.empty()) {
            this->loadTextureNormal(_selectedImage);
        }
    }
    else
    {
        // 离开范围，恢复原状
        if (this->getScaleX() != _baseScaleX || this->getScaleY() != _baseScaleY)
        {
            this->stopAllActions();
            this->runAction(ScaleTo::create(0, _baseScaleX, _baseScaleY));
        }
        this->loadTextureNormal(_normalImage);
    }
}

// 鼠标抬起事件
void HoverButton::onMouseUp(cocos2d::Event* event)
{
    auto* mouseEvent = static_cast<EventMouse*>(event);

    if (!this->getParent()) return;

    const auto locationInNode = this->getParent()->convertToNodeSpace(mouseEvent->getLocationInView());
    const auto boundingBox = this->getBoundingBox();

    // 抬起时保持选中状态显示（如果是点击操作的一部分）
    if (boundingBox.containsPoint(locationInNode))
    {
        if (this->getScaleX() == _baseScaleX && this->getScaleY() == _baseScaleY)
        {
            this->stopAllActions();
            this->runAction(ScaleTo::create(0, _hoverScale));
        }

        if (!_selectedImage.empty()) {
            this->loadTextureNormal(_selectedImage);
        }
    }
}