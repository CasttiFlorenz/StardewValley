/****************************************************************
 * Project Name:  StardewValley
 * File Name:     BarnAnimal.h
 * File Function: BarnAnimal类的实现
 * Author:        郭芷烟
 * Update Date:   2025/12/28
 * License:       MIT License
 ****************************************************************/
#pragma once
#ifndef __BARN_ANIMAL_H__
#define __BARN_ANIMAL_H__

#include "cocos2d.h"
#include "Constant.h"
#include <vector>
#include <string>

USING_NS_CC;

// 农场动物基类
class BarnAnimal : public Sprite {
public:
    virtual ~BarnAnimal() = default;

    // 获取产物资源路径
    virtual std::string getProducePath() const = 0;

    // 获取产物类型
    virtual ItemType getProduceObject() const = 0;

    // 获取动物类型
    virtual AnimalType getType() const noexcept = 0;

    // 开始动画
    virtual void startAnimation() = 0;

    // 停止动画
    virtual void stopAnimation() = 0;

    // 辅助函数：批量加载动画帧
    static std::vector<SpriteFrame*> framesFromFiles(const std::string& baseNoExt, int maxFrames = 10);
};


#endif // __BARN_ANIMAL_H__
