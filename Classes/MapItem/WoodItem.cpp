#include "WoodItem.h"

WoodItem* WoodItem::create(const cocos2d::Vec2& tileCoord) {
    auto p = new (std::nothrow) WoodItem();
    if (p && p->init(tileCoord)) {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

bool WoodItem::init(const cocos2d::Vec2& tileCoord) {
    if (!FarmItem::init(FarmItemType::WOOD, tileCoord)) {
        return false;
    }

    auto texture = Director::getInstance()->getTextureCache()->addImage("EnvironmentObjects/paths..png");
    if (texture) {
        Size texSize = texture->getContentSize();
        float w = texSize.width / 4;
        float h = texSize.height / 8;
        // 5th row (index 4), 3rd col (index 2)
        // Texture coords (bottom-up): y = H - (row+1)*h
        Rect rect(2 * w,4 * h, w, h);
        this->setTexture(texture);
        this->setTextureRect(rect);
    }

    return true;
}
