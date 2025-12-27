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

/**
 * 工厂方法：创建 ShopLayer 实例
 * 检查传入的 Item 指针是否有效，创建实例并调用 init() 进行初始化，成功后加入 autorelease 池
 * @param item 商店要出售的物品数据对象
 * @return 创建成功的 ShopLayer 指针，若失败则返回 nullptr
 */
ShopLayer* ShopLayer::create(Item* item) {
    // 参数判空
    if (item == nullptr) {
        return nullptr;
    }

    ShopLayer* pRet = new(std::nothrow) ShopLayer();
    if (pRet && pRet->init(item)) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
}

/**
 * 初始化商店弹窗层
 * 设置全透明背景和触摸吞噬，构建背景、商品图标、价格标签、数量选择器及购买/关闭按钮
 * 同时计算最大可购买数量（受金钱和常量限制）
 * @param item 商店要出售的物品数据对象
 * @return 初始化成功返回 true
 */
bool ShopLayer::init(Item* item) {
    // 初始化父类 LayerColor，全透明背景
    if (!LayerColor::initWithColor(Color4B(0, 0, 0, 0))) return false;

    _targetItem = item;

    // 获取屏幕尺寸
    auto winSize = Director::getInstance()->getVisibleSize();

    // 1. 设置触摸吞噬 (防止点击穿透到下层)
    auto listener = EventListenerTouchOneByOne::create();
    if (listener != nullptr) {
        listener->setSwallowTouches(true);
        listener->onTouchBegan = [](Touch* t, Event* e) { return true; };
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    }

    // 2. 背景板
    auto bg = Sprite::create(PATH_SHOP_BG);
    if (bg != nullptr) {
        bg->setScale(SHOP_BG_SCALE);
        bg->setContentSize(Size(400, 400));
        bg->setPosition(winSize.width / 2, winSize.height / 2);
        this->addChild(bg, ZORDER_SHOP_BG);
    }

    // 3. 商品图标
    if (_targetItem != nullptr) {
        auto icon = Sprite::create(_targetItem->getPath());
        if (icon != nullptr) {
            icon->setPosition(winSize.width / 2 - 150, winSize.height / 2 + 45);
            // 这里覆盖了原有的 getScale，统一使用常量缩放
            icon->setScale(SHOP_ICON_SCALE);
            this->addChild(icon, ZORDER_SHOP_CONTENT);
        }

        // 4. 商品名字与单价
        auto nameLabel = Label::createWithTTF(_targetItem->getName(), PATH_FONT_PIXEL, 28);
        if (nameLabel != nullptr) {
            nameLabel->setPosition(winSize.width / 2 + 50, winSize.height / 2 + 80);
            nameLabel->setColor(Color3B::BLACK);
            this->addChild(nameLabel, ZORDER_SHOP_CONTENT);
        }

        auto priceLabel = Label::createWithTTF(StringUtils::format("%d G", _targetItem->getPrice()), PATH_FONT_PIXEL, 24);
        if (priceLabel != nullptr) {
            priceLabel->setPosition(winSize.width / 2 + 50, winSize.height / 2 + 20);
            priceLabel->setColor(Color3B::BLACK);
            this->addChild(priceLabel, ZORDER_SHOP_CONTENT);
        }

        // 5. 计算最大购买数量
        int playerMoney = 0;
        if (Money::getInstance() != nullptr) {
            playerMoney = Money::getInstance()->getMoney();
        }

        int price = _targetItem->getPrice();
        if (price > 0) {
            _maxQuantity = playerMoney / price;
        }
        else {
            _maxQuantity = SHOP_MAX_QUANTITY_LIMIT;
        }

        // 限制购买上限，防止数据溢出
        if (_maxQuantity > SHOP_MAX_QUANTITY_LIMIT) _maxQuantity = SHOP_MAX_QUANTITY_LIMIT;
        if (_maxQuantity < SHOP_MIN_QUANTITY) _maxQuantity = SHOP_MIN_QUANTITY;
    }

    _currentQuantity = 1;

    // 6. 数量显示 (x1)
    _lblQuantity = Label::createWithTTF("x1", PATH_FONT_PIXEL, 30);
    if (_lblQuantity != nullptr) {
        _lblQuantity->setPosition(Vec2(winSize.width / 2, winSize.height / 2 - 40));
        _lblQuantity->setColor(Color3B::BLACK);
        this->addChild(_lblQuantity, ZORDER_SHOP_CONTENT);
    }

    // 7. 加减按钮逻辑
    // [ - ] 按钮
    auto btnMinus = Button::create(PATH_SHOP_MINUS);
    if (btnMinus != nullptr) {
        btnMinus->setPosition(Vec2(winSize.width / 2 - 100, winSize.height / 2 - 40));
        btnMinus->addClickEventListener([this](Ref*) {
            if (_currentQuantity > 1) {
                _currentQuantity--;
                updateUI();
            }
            });
        this->addChild(btnMinus, ZORDER_SHOP_BUTTONS);
    }

    // [ + ] 按钮
    auto btnPlus = Button::create(PATH_SHOP_PLUS);
    if (btnPlus != nullptr) {
        btnPlus->setPosition(Vec2(winSize.width / 2 + 100, winSize.height / 2 - 40));
        btnPlus->addClickEventListener([this](Ref*) {
            if (_currentQuantity < _maxQuantity) {
                _currentQuantity++;
                updateUI();
            }
            });
        this->addChild(btnPlus, ZORDER_SHOP_BUTTONS);
    }

    // 8. 总价显示
    _lblTotalCost = Label::createWithTTF("Total: 0", PATH_FONT_PIXEL, 26);
    if (_lblTotalCost != nullptr) {
        _lblTotalCost->setPosition(Vec2(winSize.width / 2, winSize.height / 2 - 300));
        _lblTotalCost->setColor(Color3B::BLACK);
        this->addChild(_lblTotalCost, ZORDER_SHOP_CONTENT);
    }

    // 9. 购买确认按钮 (OK Button)
    _btnBuy = Button::create(PATH_SHOP_BUY);
    if (_btnBuy != nullptr) {
        _btnBuy->setScale(SHOP_BTN_SCALE);
        // 确保按钮位置在窗口下方
        _btnBuy->setPosition(Vec2(winSize.width / 2, winSize.height / 2 - 160));
        _btnBuy->addClickEventListener(CC_CALLBACK_1(ShopLayer::onBuyClicked, this));
        this->addChild(_btnBuy, ZORDER_SHOP_BUTTONS);
    }

    // 10. 关闭按钮 (X)
    auto btnClose = Button::create(PATH_SHOP_CLOSE);
    if (btnClose != nullptr) {
        btnClose->setScale(SHOP_BTN_SCALE);
        // 放在右上角
        btnClose->setPosition(Vec2(winSize.width / 2 + 230, winSize.height / 2 + 230));
        btnClose->addClickEventListener(CC_CALLBACK_1(ShopLayer::onCloseClicked, this));
        this->addChild(btnClose, ZORDER_SHOP_BUTTONS);
    }

    updateUI(); // 初始化界面状态
    return true;
}



/**
 * 刷新 UI 状态
 * 根据当前选择的购买数量更新数量文本和总价文本
 * 检查玩家金钱是否足够，并据此设置购买按钮的启用状态和总价文本颜色（红色为钱不够）
 */
void ShopLayer::updateUI() {
    if (_targetItem == nullptr) return;

    // 1. 更新数量文本
    if (_lblQuantity != nullptr) {
        _lblQuantity->setString(StringUtils::format("x%d", _currentQuantity));
    }

    // 2. 计算总价
    int totalCost = _currentQuantity * _targetItem->getPrice();
    if (_lblTotalCost != nullptr) {
        _lblTotalCost->setString(StringUtils::format("Total: %d G", totalCost));
    }

    // 3. 检查钱够不够，控制按钮状态
    bool canAfford = false;
    if (Money::getInstance() != nullptr) {
        canAfford = Money::getInstance()->canAfford(totalCost);
    }

    if (_btnBuy != nullptr) {
        _btnBuy->setEnabled(canAfford);
        // 使用 static_cast 进行类型转换
        _btnBuy->setOpacity(canAfford ? 255 : 128); // 买不起变半透明

        // 如果钱不够，总价显示红色
        if (_lblTotalCost != nullptr) {
            if (!canAfford) _lblTotalCost->setColor(Color3B::RED);
            else _lblTotalCost->setColor(Color3B::BLACK);
        }
    }
}



/**
 * 处理购买按钮点击事件
 * 再次检查余额，若足够则扣除金钱、将物品添加到背包，触发购买成功回调并关闭弹窗
 * @param sender 触发事件的按钮对象
 */
void ShopLayer::onBuyClicked(Ref* sender) {
    if (_targetItem == nullptr) return;

    // 获取单例实例并进行判空
    auto playerState = Money::getInstance();
    auto inventory = InventoryScene::getInstance();

    if (playerState == nullptr || inventory == nullptr) {
        return;
    }

    int totalCost = _currentQuantity * _targetItem->getPrice();

    if (playerState->canAfford(totalCost)) {
        // 1. 扣钱 
        playerState->spendMoney(totalCost);

        // 2. 加背包 
        inventory->addItemCount(_targetItem->getTag(), _currentQuantity, false);

        // 3. 通知上层界面刷新 (主要是刷新金钱显示)
        if (_onPurchaseSuccess) {
            _onPurchaseSuccess();
        }

        // 4. 关闭弹窗
        this->removeFromParent();
    }
    else {
        CCLOG("金钱不足");
    }
}


/**
 * 处理关闭按钮点击事件
 * 将当前商店弹窗层从父节点移除
 * @param sender 触发事件的按钮对象
 */
void ShopLayer::onCloseClicked(Ref* sender) {
    this->removeFromParent();
}
