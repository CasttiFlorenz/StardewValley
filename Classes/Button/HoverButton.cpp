/****************************************************************
 * Project Name:  StardewValley
 * File Name:     HoverButton.cpp
 * File Function: HoverButton类的实现
 * Author:        郭芷烟
 * Update Date:   2025/12/07
 * License:       MIT License
 ****************************************************************/

#include "HoverButton.h"

USING_NS_CC;
using namespace cocos2d::ui;

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

    // 注册鼠标监听事件
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
    auto boundingBox = this->getBoundingBox();
    auto locationInNode = this->getParent()->convertToNodeSpace(mouseEvent->getLocationInView());

    // 判断鼠标是否在按钮范围内
    if (boundingBox.containsPoint(locationInNode))
    {
        // 放大并切换图片
        if (this->getScaleX() == _baseScaleX && this->getScaleY() == _baseScaleY)
        {
            this->stopAllActions();
            this->runAction(ScaleTo::create(0, _hoverScale));
        }
        this->loadTextureNormal(_selectedImage);

    }
    else
    {
        // 离开恢复原样
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
    auto boundingBox = this->getBoundingBox();
    auto locationInNode = this->getParent()->convertToNodeSpace(mouseEvent->getLocationInView());

    // 抬起时同时显示选中图片
    if (boundingBox.containsPoint(locationInNode))
    {
        if (this->getScaleX() == _baseScaleX && this->getScaleY() == _baseScaleY)
        {
            this->stopAllActions();
            this->runAction(ScaleTo::create(0, _hoverScale));
        }
        this->loadTextureNormal(_selectedImage);

    }
}