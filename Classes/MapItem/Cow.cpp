#include "Cow.h"

Cow* Cow::create() {
    auto p = new (std::nothrow) Cow();
    if (p && p->initWithFile("Animal/cow_0.png")) {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

std::string Cow::getProducePath() const {
    return "EnvironmentObjects/milk.png";
}

Objects Cow::getProduceObject() const {
    return Objects::MILK;
}

void Cow::startAnimation() {
    std::vector<std::string> paths = {
        "Animal/cow_0.png",
        "Animal/cow_1.png",
        "Animal/cow_2.png",
        "Animal/cow_3.png",
        "Animal/cow_4.png",
        "Animal/cow_5.png"
    };
    Vector<AnimationFrame*> afs;
    for (size_t i = 0; i < paths.size(); ++i) {
        auto spr = Sprite::create(paths[i]);
        if (!spr) continue;
        auto f = spr->getSpriteFrame();
        if (!f) continue;
        float units = 1.0f;
        if (i == 0) units = 3.0f / 0.3f;
        else if (i == 4) units = 2.0f / 0.3f;
        afs.pushBack(AnimationFrame::create(f, units, ValueMap()));
    }
    if (afs.empty()) return;
    auto anim = Animation::create();
    anim->initWithAnimationFrames(afs, 0.3f, 1);
    runAction(RepeatForever::create(Animate::create(anim)));
}

void Cow::stopAnimation() {
    pause();
}
