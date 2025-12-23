#pragma once
#ifndef __BARN_ANIMAL_H__
#define __BARN_ANIMAL_H__

#include "cocos2d.h"
#include "Constant.h"
#include <vector>

USING_NS_CC;

class BarnAnimal : public Sprite {
public:
    // 获取对应产物图片资源路径
    virtual std::string getProducePath() const = 0;
    // 获取对应产物的物品类型
    virtual ItemType getProduceObject() const = 0;
    // 开始播放该动物的动画
    virtual void startAnimation() = 0;
    // 停止播放该动物的动画
    virtual void stopAnimation() = 0;
    // 根据基础路径批量加载连续编号的帧（baseNoExt + index + ".png"）
    static std::vector<SpriteFrame*> framesFromFiles(const std::string& baseNoExt, int maxFrames = 10);
    virtual ~BarnAnimal() = default;
};

#endif // __BARN_ANIMAL_H__
