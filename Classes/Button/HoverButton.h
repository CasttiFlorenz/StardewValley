#pragma once
/****************************************************************
 * Project Name:  StardewValley
 * File Name:     HoverButton.h
 * File Function: HoverButton类的实现
 * Author:        郭芷烟
 * Update Date:   2025/12/07
 * License:       MIT License
 ****************************************************************/

#ifndef _HOVER_BUTTON_H_
#define _HOVER_BUTTON_H_

#include <string>
#include <functional>
#include "cocos2d.h"
#include "ui/CocosGUI.h"

// 悬停按钮类
class HoverButton : public cocos2d::ui::Button {
public:
    static HoverButton* create(const std::string& normalImage,
        const std::string& selectedImage,
        const std::string& disabledImage);

    bool init(const std::string& normalImage,
        const std::string& selectedImage,
        const std::string& disabledImage);

private:

    // 图片路径
    std::string _normalImage;
    std::string _selectedImage;
    std::string _disabledImage;

    // 缩放比例
    const float _hoverScale = 1.03f;
    const float _baseScaleX = 1.0f;
    const float _baseScaleY = 1.0f;

    // 鼠标悬停事件
    void onMouseMove(cocos2d::Event* event);

    // 鼠标抬起事件
    void onMouseUp(cocos2d::Event* event);
};

#endif // _HOVER_BUTTON_H_