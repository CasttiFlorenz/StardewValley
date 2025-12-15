#include "GrassItem.h"

GrassItem* GrassItem::create(const cocos2d::Vec2& tileCoord) {
    auto p = new (std::nothrow) GrassItem();
    if (p && p->init(tileCoord)) {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

bool GrassItem::init(const cocos2d::Vec2& tileCoord) {
    if (!FarmItem::init(FarmItemType::Grass, tileCoord)) {
        return false;
    }

    auto texture = Director::getInstance()->getTextureCache()->addImage("EnvironmentObjects/paths..png");
    if (texture) {
        Size texSize = texture->getContentSize();
        float w = texSize.width / 4;
        float h = texSize.height / 8;
        // 4th row (index 3), 2nd col (index 1)
        // Texture coords (top-down): y = row * h
        Rect rect(1 * w, 3 * h, w, h);
        this->setTexture(texture);
        this->setTextureRect(rect);
    }

    return true;
}
