#pragma once
#ifndef _HOVER_BUTTON_H_
#define _HOVER_BUTTON_H_

#include <string>
#include <functional>
#include "cocos2d.h"
#include "ui/CocosGUI.h"

// 悬停按钮类，继承自 Cocos2d 的 Button
class HoverButton : public cocos2d::ui::Button {
public:
    // 静态创建函数
    static HoverButton* create(const std::string& normalImage,
        const std::string& selectedImage = "",
        const std::string& disabledImage = "");

    // 初始化函数
    virtual bool init(const std::string& normalImage,
        const std::string& selectedImage,
        const std::string& disabledImage);

private:
    // 鼠标移动事件处理
    void onMouseMove(cocos2d::Event* event);

    // 鼠标抬起事件处理
    void onMouseUp(cocos2d::Event* event);

private:
    // 图片路径
    std::string _normalImage;
    std::string _selectedImage;
    std::string _disabledImage;

    // 缩放参数
    const float _hoverScale = 1.03f;
    const float _baseScaleX = 1.0f;
    const float _baseScaleY = 1.0f;
};

#endif // _HOVER_BUTTON_H_