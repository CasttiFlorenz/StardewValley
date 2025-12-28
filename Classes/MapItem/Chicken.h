/****************************************************************
 * Project Name:  StardewValley
 * File Name:     Chicken.h
 * File Function: Chicken类的实现
 * Author:        郭芷烟
 * Update Date:   2025/12/28
 * License:       MIT License
 ****************************************************************/
#pragma once
#ifndef __CHICKEN_H__
#define __CHICKEN_H__

#include "BarnAnimal.h"

// 鸡类，继承自农场动物
class Chicken : public BarnAnimal {
public:
    // 静态创建函数
    static Chicken* create();

    // 获取产物路径
    virtual std::string getProducePath() const override;

    // 获取产物类型
    virtual ItemType getProduceObject() const override;

    // 获取动物类型
    virtual AnimalType getType() const noexcept override {
        return AnimalType::CHICKEN;
    }

    // 动画控制
    virtual void startAnimation() override;
    virtual void stopAnimation() override;
};


#endif // __CHICKEN_H__
