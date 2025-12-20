/****************************************************************
 * Project Name:  StardewValley
 * File Name:     ShopLayer.cpp
 * File Function: ShopLayer类的实现
 * Author:        赵睿妍
 * Update Date:   2025/12/17
 * License:       MIT License
 ****************************************************************/
#include "ShopLayer.h"

USING_NS_CC;
using namespace ui;

ShopLayer* ShopLayer::create(Item* item) {
    ShopLayer* pRet = new(std::nothrow) ShopLayer();
    if (pRet && pRet->init(item)) {
        pRet->autorelease();
        return pRet;
    }
    delete pRet;
    return nullptr;
}

bool ShopLayer::init(Item* item) {
    // 1. 初始化半透明黑色背景 (模态效果)
    if (!LayerColor::initWithColor(Color4B(0, 0, 0, 150))) return false;

    // 吞噬触摸 (防止点穿)
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [](Touch* t, Event* e) { return true; };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    _targetItem = item;
    Size winSize = Director::getInstance()->getVisibleSize(); // 使用 VisibleSize 适配性更好

    // 2. 背景板
    auto bg = Scale9Sprite::create("Items/background.png");
    if (!bg) { // 回退方案：如果没有图片，画个黄色的框
        bg = Scale9Sprite::create();
        bg->setColor(Color3B(210, 180, 140));
    }
    bg->setContentSize(Size(500, 400));
    bg->setPosition(winSize.width / 2, winSize.height / 2);
    this->addChild(bg);

    // 3. 商品图标
    auto icon = Sprite::create(_targetItem->getPath());
    if (icon) {
        icon->setPosition(winSize.width / 2 - 150, winSize.height / 2 + 50);
        icon->setScale(_targetItem->getScale());
        icon->setScale(4.0);
        this->addChild(icon);
    }

    // 4. 商品名字与单价
    auto nameLabel = Label::createWithTTF(_targetItem->getName(), "fonts/arial.ttf", 28);
    nameLabel->setPosition(winSize.width / 2 + 50, winSize.height / 2 + 80);
    nameLabel->setColor(Color3B::BLACK);
    this->addChild(nameLabel);

    auto priceLabel = Label::createWithTTF(StringUtils::format("%d G", _targetItem->getPrice()), "fonts/arial.ttf", 24);
    priceLabel->setPosition(winSize.width / 2 + 50, winSize.height / 2 + 40);
    priceLabel->setColor(Color3B::BLACK); // 深褐色
    this->addChild(priceLabel);

    // 5. 计算最大购买数量
    int playerMoney = PlayerState::getInstance()->money;
    if (_targetItem->getPrice() > 0) {
        _maxQuantity = playerMoney / _targetItem->getPrice();
    }
    else {
        _maxQuantity = 99;
    }
    // 限制购买上限，防止数据溢出或逻辑错误
    if (_maxQuantity > 99) _maxQuantity = 99;
    if (_maxQuantity < 1) _maxQuantity = 1;

    _currentQuantity = 1;

    // 6. 数量显示 (x1)
    _lblQuantity = Label::createWithTTF("x1", "fonts/arial.ttf", 30);
    _lblQuantity->setPosition(Vec2(winSize.width / 2, winSize.height / 2 - 40));
    _lblQuantity->setColor(Color3B::BLACK);
    this->addChild(_lblQuantity);

    // 7. 加减按钮逻辑
    // [ - ] 按钮
    auto btnMinus = Button::create("ui/btn_minus.png");
    if (btnMinus->getContentSize().width == 0) { // 回退：文字按钮
        btnMinus = Button::create();
        btnMinus->setTitleText("[-]");
        btnMinus->setTitleFontSize(40);
        btnMinus->setTitleColor(Color3B::RED);
    }
    btnMinus->setPosition(Vec2(winSize.width / 2 - 100, winSize.height / 2 - 40));
    btnMinus->addClickEventListener([this](Ref*) {
        if (_currentQuantity > 1) {
            _currentQuantity--;
            updateUI(); // 只更新数字和价格，不依赖Slider
        }
        });
    this->addChild(btnMinus);

    // [ + ] 按钮
    auto btnPlus = Button::create("ui/btn_plus.png");
    if (btnPlus->getContentSize().width == 0) { // 回退：文字按钮
        btnPlus = Button::create();
        btnPlus->setTitleText("[+]");
        btnPlus->setTitleFontSize(40);
        btnPlus->setTitleColor(Color3B::GREEN);
    }
    btnPlus->setPosition(Vec2(winSize.width / 2 + 100, winSize.height / 2 - 40));
    btnPlus->addClickEventListener([this](Ref*) {
        if (_currentQuantity < _maxQuantity) {
            _currentQuantity++;
            updateUI();
        }
        });
    this->addChild(btnPlus);

    // 8. 总价显示
    _lblTotalCost = Label::createWithTTF("Total: 0", "fonts/arial.ttf", 26);
    _lblTotalCost->setPosition(Vec2(winSize.width / 2, winSize.height / 2 - 100));
    _lblTotalCost->setColor(Color3B::BLACK);
    this->addChild(_lblTotalCost);

    // 9. 【关键修复】购买确认按钮 (OK Button)
    _btnBuy = Button::create("Items/btn_ok.png");

    // 如果没有图片资源，手动画一个绿色的按钮，保证能点击！
    if (_btnBuy->getContentSize().width == 0) {
        _btnBuy = Button::create();
        _btnBuy->ignoreContentAdaptWithSize(false);
        _btnBuy->setContentSize(Size(120, 50));

        auto btnBg = LayerColor::create(Color4B(34, 139, 34, 255)); // 绿色背景
        btnBg->setContentSize(Size(120, 50));
        btnBg->setPosition(0, 0);
        _eventDispatcher->removeEventListenersForTarget(btnBg); // 移除背景的交互，防止挡住按钮
        _btnBuy->addChild(btnBg, -1);

        _btnBuy->setTitleText("BUY");
        _btnBuy->setTitleFontSize(24);
    }

    // 确保按钮位置在窗口下方
    _btnBuy->setPosition(Vec2(winSize.width / 2, winSize.height / 2 - 160));
    _btnBuy->addClickEventListener(CC_CALLBACK_1(ShopLayer::onBuyClicked, this));
    this->addChild(_btnBuy, 10); // Z轴设为10，确保在最上层

    // 10. 关闭按钮 (X)
    auto btnClose = Button::create("Items/btn_close.png");
    if (btnClose->getContentSize().width == 0) {
        btnClose = Button::create();
        btnClose->setTitleText("[X]");
        btnClose->setTitleFontSize(30);
        btnClose->setTitleColor(Color3B::RED);
    }
    // 放在右上角
    btnClose->setPosition(Vec2(winSize.width / 2 + 250 - 30, winSize.height / 2 + 200 - 30));
    btnClose->addClickEventListener(CC_CALLBACK_1(ShopLayer::onCloseClicked, this));
    this->addChild(btnClose, 10);

    updateUI(); // 初始化界面状态
    return true;
}

// 移除 onSliderEvent，纯靠加减按钮更稳健

void ShopLayer::updateUI() {
    // 1. 更新数量文本
    if (_lblQuantity) {
        _lblQuantity->setString(StringUtils::format("x%d", _currentQuantity));
    }

    // 2. 计算总价
    int totalCost = _currentQuantity * _targetItem->getPrice();
    if (_lblTotalCost) {
        _lblTotalCost->setString(StringUtils::format("Total: %d G", totalCost));
    }

    // 3. 检查钱够不够，控制按钮状态
    bool canAfford = PlayerState::getInstance()->canAfford(totalCost);
    if (_btnBuy) {
        _btnBuy->setEnabled(canAfford);
        _btnBuy->setOpacity(canAfford ? 255 : 128); // 买不起变半透明

        // 如果钱不够，总价显示红色
        if (!canAfford) _lblTotalCost->setColor(Color3B::RED);
        else _lblTotalCost->setColor(Color3B::BLACK);
    }
}

void ShopLayer::onBuyClicked(Ref* sender) {
    if (!_targetItem) return;

    int totalCost = _currentQuantity * _targetItem->getPrice();
    auto playerState = PlayerState::getInstance();

    if (playerState->canAfford(totalCost)) {
        // 1. 扣钱 
        playerState->spendMoney(totalCost);

        // 2. 加背包 
        InventoryScene::getInstance()->addItemCount(_targetItem->getTag(), _currentQuantity, false);

        // 3. 通知上层界面刷新 (主要是刷新金钱显示)
        if (onPurchaseSuccess) {
            onPurchaseSuccess();
        }

        // 4. 关闭弹窗
        this->removeFromParent();
    }
    else {
        CCLOG("金钱不足");
        // 可选：添加一个提示动画
    }
}

void ShopLayer::onCloseClicked(Ref* sender) {
    this->removeFromParent();
}
