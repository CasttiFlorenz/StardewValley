#pragma once
#ifndef __BARN_ANIMAL_H__
#define __BARN_ANIMAL_H__

#include "cocos2d.h"
#include "cocos2d.h"
#include "Constant.h"
#include <vector>

USING_NS_CC;

class BarnAnimal : public Sprite {
public:
    virtual std::string getProducePath() const = 0;
    virtual Objects getProduceObject() const = 0;
    virtual void startAnimation() = 0;
    virtual void stopAnimation() = 0;
    static std::vector<SpriteFrame*> framesFromFiles(const std::string& baseNoExt, int maxFrames = 10);
};

#endif // __BARN_ANIMAL_H__
