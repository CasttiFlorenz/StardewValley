/****************************************************************
 * Project Name:  StardewValley
 * File Name:     Cow.cpp
 * File Function: Cow类的实现
 * Author:        郭芷烟
 * Update Date:   2025/12/28
 * License:       MIT License
 ****************************************************************/
#include "Cow.h"

// 创建实例
Cow* Cow::create() {
    auto p = new (std::nothrow) Cow();
    if (p && p->initWithFile(COW_DEFAULT_TEXTURE_PATH)) {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

// 获取产物图片路径
std::string Cow::getProducePath() const {
    return COW_PRODUCE_TEXTURE_PATH;
}

// 获取产物类型（牛奶）
ItemType Cow::getProduceObject() const {
    return ItemType::MILK;
}

// 开始动画
void Cow::startAnimation() {
    std::vector<std::string> paths = {
        COW_ANIM_FRAME_0,
        COW_ANIM_FRAME_1,
        COW_ANIM_FRAME_2,
        COW_ANIM_FRAME_3,
        COW_ANIM_FRAME_4,
        COW_ANIM_FRAME_5
    };

    Vector<AnimationFrame*> afs;
    for (size_t i = 0; i < paths.size(); ++i) {
        auto spr = Sprite::create(paths[i]);
        if (!spr) continue;

        auto f = spr->getSpriteFrame();
        if (!f) continue;

        float units = ANIM_FRAME_OTHER_UNITS;
        if (i == 0) units = ANIM_FRAME_0_UNITS;
        else if (i == 4) units = ANIM_FRAME_4_UNITS;

        afs.pushBack(AnimationFrame::create(f, units, ValueMap()));
    }

    if (afs.empty()) return;

    auto anim = Animation::create();
    anim->initWithAnimationFrames(afs, ANIMATION_INTERVAL, ANIMATION_LOOP_COUNT);
    runAction(RepeatForever::create(Animate::create(anim)));
}

// 停止动画
void Cow::stopAnimation() {
    pause();
}
