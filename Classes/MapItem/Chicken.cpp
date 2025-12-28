/****************************************************************
 * Project Name:  StardewValley
 * File Name:     Chicken.cpp
 * File Function: Chicken类的实现
 * Author:        郭芷烟
 * Update Date:   2025/12/16
 * License:       MIT License
 ****************************************************************/
#include "Chicken.h"

Chicken* Chicken::create() {
    auto p = new (std::nothrow) Chicken();
    if (p && p->initWithFile(CHICKEN_DEFAULT_TEXTURE_PATH)) {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

std::string Chicken::getProducePath() const {
    return CHICKEN_PRODUCE_TEXTURE_PATH;
}

ItemType Chicken::getProduceObject() const {
    return ItemType::EGG;
}

void Chicken::startAnimation() {
    std::vector<std::string> paths = {
        CHICKEN_ANIM_FRAME_0,
        CHICKEN_ANIM_FRAME_1,
        CHICKEN_ANIM_FRAME_2,
        CHICKEN_ANIM_FRAME_3
    };
    Vector<AnimationFrame*> afs;
    for (size_t i = 0; i < paths.size(); ++i) {
        auto spr = Sprite::create(paths[i]);
        if (!spr) continue;
        auto f = spr->getSpriteFrame();
        if (!f) continue;
        float units = ANIM_FRAME_OTHER_UNITS;
        if (i == 0) units = ANIM_FRAME_0_UNITS;
        else if (i == 2) units = ANIM_FRAME_2_UNITS;
        afs.pushBack(AnimationFrame::create(f, units, ValueMap()));
    }
    if (afs.empty()) return;
    auto anim = Animation::create();
    anim->initWithAnimationFrames(afs, ANIMATION_INTERVAL, ANIMATION_LOOP_COUNT);
    runAction(RepeatForever::create(Animate::create(anim)));
}

void Chicken::stopAnimation() {
    pause();
}
