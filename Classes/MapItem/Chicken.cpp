#include "Chicken.h"

Chicken* Chicken::create() {
    auto p = new (std::nothrow) Chicken();
    if (p && p->initWithFile("Animal/chicken_0.png")) {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

std::string Chicken::getProducePath() const {
    return "EnvironmentObjects/egg.png";
}

Objects Chicken::getProduceObject() const {
    return Objects::EGG;
}

void Chicken::startAnimation() {
    std::vector<std::string> paths = {
        "Animal/chicken_0.png",
        "Animal/chicken_1.png",
        "Animal/chicken_2.png",
        "Animal/chicken_3.png"
    };
    Vector<AnimationFrame*> afs;
    for (size_t i = 0; i < paths.size(); ++i) {
        auto spr = Sprite::create(paths[i]);
        if (!spr) continue;
        auto f = spr->getSpriteFrame();
        if (!f) continue;
        float units = 1.0f;
        if (i == 0) units = 3.0f / 0.3f;
        else if (i == 2) units = 2.0f / 0.3f;
        afs.pushBack(AnimationFrame::create(f, units, ValueMap()));
    }
    if (afs.empty()) return;
    auto anim = Animation::create();
    anim->initWithAnimationFrames(afs, 0.3f, 1);
    runAction(RepeatForever::create(Animate::create(anim)));
}

void Chicken::stopAnimation() {
    pause();
}
