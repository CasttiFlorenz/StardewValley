#include "ShopMenuLayer.h"
#include "ShopLayer.h"
#include "../Inventory/PlayerState.h"
#include "../Inventory/Item.h"

USING_NS_CC;
using namespace ui;

// 辅助函数：创建按钮
Button* createFallbackButton(const std::string& imagePath, const std::string& text, const Size& size, Color3B color) {
    auto btn = Button::create(imagePath);
    if (btn->getContentSize().width == 0) {
        btn = Button::create();
        btn->ignoreContentAdaptWithSize(false);
        btn->setContentSize(size);
        auto bg = LayerColor::create(Color4B(color), size.width, size.height);
        bg->setPosition(0, 0);
        Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(bg);
        btn->addChild(bg, -1);
        btn->setTitleText(text);
        btn->setTitleFontSize(20);
        btn->setTitleColor(Color3B::WHITE);
    }
    return btn;
}

ShopMenuLayer* ShopMenuLayer::create(const std::vector<Item*>& shopItems, const std::vector<Objects>& acceptedTags) {
    ShopMenuLayer* pRet = new(std::nothrow) ShopMenuLayer();
    if (pRet && pRet->init(shopItems, acceptedTags)) {
        pRet->autorelease();
        return pRet;
    }
    delete pRet;
    return nullptr;
}

bool ShopMenuLayer::init(const std::vector<Item*>& shopItems, const std::vector<Objects>& acceptedTags) {
    if (!Layer::init()) return false;

    // 1. 保存允许售卖的列表
    _acceptedSellTags = acceptedTags;

    Size visibleSize = Director::getInstance()->getVisibleSize();

    // 2. 遮罩层
    auto darkLayer = LayerColor::create(Color4B(0, 0, 0, 180));
    this->addChild(darkLayer);
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [](Touch*, Event*) { return true; };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, darkLayer);

    // 3. 背景板 (保持原有尺寸和位置)
    auto bg = LayerColor::create(Color4B(210, 180, 140, 255));
    bg->setContentSize(Size(800, 650));
    bg->ignoreAnchorPointForPosition(false);
    bg->setAnchorPoint(Vec2(0.5, 0.5));
    bg->setPosition(visibleSize.width / 2, visibleSize.height / 2);
    this->addChild(bg);

    // 4. 标题 & 金钱
    auto title = Label::createWithTTF("Shop", "fonts/arial.ttf", 32);
    title->setPosition(bg->getContentSize().width / 2, bg->getContentSize().height - 35);
    title->setColor(Color3B::BLACK);
    bg->addChild(title);

    _moneyLabel = Label::createWithTTF("0 G", "fonts/arial.ttf", 26);
    _moneyLabel->setAnchorPoint(Vec2(1, 0.5));
    _moneyLabel->setPosition(bg->getContentSize().width - 60, bg->getContentSize().height - 35);
    _moneyLabel->setColor(Color3B(139, 69, 19));
    bg->addChild(_moneyLabel);

    // 5. 关闭按钮
    auto closeBtn = createFallbackButton("ui/close.png", "[X]", Size(50, 50), Color3B::RED);
    closeBtn->setPosition(Vec2(bg->getContentSize().width - 30, bg->getContentSize().height - 30));
    closeBtn->addClickEventListener([this](Ref*) { this->removeFromParent(); });
    bg->addChild(closeBtn, 100);

    // ============================================================
    // 6. 商品列表 (ListView) - 上半部分
    // ============================================================
    _listView = ListView::create();
    _listView->setDirection(ui::ScrollView::Direction::VERTICAL);
    _listView->setBounceEnabled(true);
    _listView->setScrollBarEnabled(true);
    // 保持原有尺寸
    _listView->setContentSize(Size(720, 320));
    _listView->setAnchorPoint(Vec2(0.5, 1));
    _listView->setPosition(Vec2(bg->getContentSize().width / 2, bg->getContentSize().height - 70));
    _listView->setTouchEnabled(true);

    // 列表背景底色
    auto listBg = LayerColor::create(Color4B(0, 0, 0, 30), 720, 320);
    listBg->setPosition(0, 0);
    _listView->addChild(listBg, -1);
    bg->addChild(_listView);

    // --- 填充商品 ---
    int index = 0; // 用于计算奇偶行
    for (const auto& item : shopItems) {
        if (!item) continue;

        auto itemLayout = Layout::create();
        itemLayout->setContentSize(Size(700, 80));
        itemLayout->setTouchEnabled(true);

        
        Color4B rowColor = (index % 2 == 0) ? Color4B(255, 255, 255, 50) : Color4B(0, 0, 0, 20);
        auto rowBg = LayerColor::create(rowColor, 700, 80);
        rowBg->setPosition(0, 0);
        Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(rowBg);
        itemLayout->addChild(rowBg, -1);
        index++;
      

        // 图标
        auto icon = Sprite::create(item->getPos());
        if (icon) {
            icon->setPosition(Vec2(50, 40));
            icon->setScale(0.8f);
            itemLayout->addChild(icon);
        }

        // 名字
        auto nameLabel = Label::createWithTTF(item->getName(), "fonts/arial.ttf", 22);
        nameLabel->setAnchorPoint(Vec2(0, 0.5));
        nameLabel->setPosition(Vec2(100, 55));
        nameLabel->setColor(Color3B::BLACK);
        itemLayout->addChild(nameLabel);

        // 价格
        auto priceLabel = Label::createWithTTF(StringUtils::format("%d G", item->getPrice()), "fonts/arial.ttf", 18);
        priceLabel->setAnchorPoint(Vec2(0, 0.5));
        priceLabel->setPosition(Vec2(100, 25));
        priceLabel->setColor(Color3B::GRAY);
        itemLayout->addChild(priceLabel);

        // 购买按钮
        auto buyBtn = createFallbackButton("ui/btn_buy.png", "BUY", Size(80, 40), Color3B::BLUE);
        buyBtn->setPosition(Vec2(620, 40));
        buyBtn->addClickEventListener([=](Ref*) {
            auto detailLayer = ShopLayer::create(item);
            if (detailLayer) {
                detailLayer->onPurchaseSuccess = [this]() { this->refreshUI(); };
                Director::getInstance()->getRunningScene()->addChild(detailLayer, 9999);
            }
            });
        itemLayout->addChild(buyBtn);

        _listView->pushBackCustomItem(itemLayout);
    }

    // 7. 初始化背包容器 - 下半部分
    _inventoryNode = Node::create();
    _inventoryNode->setPosition(40, 10); // 保持在底部
    bg->addChild(_inventoryNode);

    // 8. 滚轮支持
    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseScroll = [this](Event* event) {
        auto mouseEvent = static_cast<EventMouse*>(event);
        float scrollY = mouseEvent->getScrollY();
        if (scrollY != 0 && _listView->getInnerContainerSize().height > _listView->getContentSize().height) {
            Vec2 pos = _listView->getInnerContainerPosition();
            pos.y += scrollY * 20.0f;
            float minY = _listView->getContentSize().height - _listView->getInnerContainerSize().height;
            float maxY = 0.0f;
            if (pos.y < minY) pos.y = minY;
            if (pos.y > maxY) pos.y = maxY;
            _listView->setInnerContainerPosition(pos);
        }
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

    refreshUI();
    return true;
}

void ShopMenuLayer::refreshUI() {
    int money = PlayerState::getInstance()->money;
    _moneyLabel->setString(StringUtils::format("%d G", money));

    _inventoryNode->removeAllChildren();

    // >>>>>>>>>>>>>>>>> 恢复原有背包绘制逻辑 <<<<<<<<<<<<<<<<<
    auto& items = PlayerState::getInstance()->getInventoryItems();

    // 参数保持你习惯的设置
    const int COLUMNS = 10;
    const float SLOT_SIZE = 64.0f;
    const float GAP = 8.0f;

    // 绘制 3 行 x 10 列 (共30格)
    for (int i = 0; i < 30; i++) {
        int row = i / COLUMNS;
        int col = i % COLUMNS;

        // 这里的坐标计算恢复为标准的网格排列
        float x = col * (SLOT_SIZE + GAP);
        float y = 140 - row * (SLOT_SIZE + GAP); // 从Y=200开始向下排

        // 格子按钮
        auto slotBtn = createFallbackButton("/Items/box.png", "", Size(SLOT_SIZE, SLOT_SIZE), Color3B(240, 240, 240));
        slotBtn->setScale(0.75);
        if (slotBtn->getChildrenCount() > 0) dynamic_cast<Node*>(slotBtn->getChildren().at(0))->setOpacity(150);

        slotBtn->setPosition(Vec2(x + SLOT_SIZE / 2, y + SLOT_SIZE / 2));

        // 物品内容
        if (i < items.size()) {
            Item* item = items[i];

            // 图标
            auto icon = Sprite::create(item->getPos());
            if (icon) {
                float scale = std::min(SLOT_SIZE / icon->getContentSize().width, SLOT_SIZE / icon->getContentSize().height) * 0.8f;
                icon->setScale(scale);
                icon->setPosition(slotBtn->getContentSize() / 2);
                slotBtn->addChild(icon);
            }
            else {
                // 容错红点
                auto dot = DrawNode::create();
                dot->drawDot(Vec2::ZERO, 10, Color4F::RED);
                dot->setPosition(slotBtn->getContentSize() / 2);
                slotBtn->addChild(dot);
            }

            // 数量
            if (item->getCount() > 1) {
                auto countLabel = Label::createWithTTF(std::to_string(item->getCount()), "fonts/arial.ttf", 16);
                countLabel->setAnchorPoint(Vec2(1, 0));
                countLabel->setPosition(Vec2(SLOT_SIZE - 5, 5));
                countLabel->setColor(Color3B::WHITE);
                countLabel->enableOutline(Color4B::BLACK, 2);
                slotBtn->addChild(countLabel);
            }

            // 点击卖出
            slotBtn->addClickEventListener([=](Ref*) {
                this->sellItem(item);
                });
        }

        _inventoryNode->addChild(slotBtn);
    }
}

void ShopMenuLayer::sellItem(Item* item) {
    if (!item) return;

    // >>>>>>>>>>>>>>>>> 核心新增：售卖检查逻辑 <<<<<<<<<<<<<<<<<
    bool isAcceptable = false;

    // 如果允许列表为空，默认为“全收”（如出货箱）
    if (_acceptedSellTags.empty()) {
        isAcceptable = true;
    }
    else {
        // 检查物品 Tag 是否在允许列表里
        for (Objects tag : _acceptedSellTags) {
            if (item->getTag() == tag) {
                isAcceptable = true;
                break;
            }
        }
    }

    // 如果不可卖，显示提示
    if (!isAcceptable) {
        showTip("This item cannot be sold here!", Color3B::RED);
        return;
    }
    // >>>>>>>>>>>>>>>>>>>>>>>>>> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    // 正常售卖流程
    int sellPrice = item->getPrice() / 2;
    if (sellPrice < 1) sellPrice = 1;

    PlayerState::getInstance()->addMoney(sellPrice);
    PlayerState::getInstance()->removeItem(item, 1);

    showTip(StringUtils::format("+%d G", sellPrice), Color3B::YELLOW);

    refreshUI();
}

void ShopMenuLayer::showTip(const std::string& message, const Color3B& color) {
    auto label = Label::createWithTTF(message, "fonts/arial.ttf", 26);
    label->setColor(color);
    label->enableOutline(Color4B::BLACK, 1);

    Size winSize = Director::getInstance()->getVisibleSize();
    label->setPosition(winSize / 2);

    // 加到场景最上层 (Z=10000)
    Director::getInstance()->getRunningScene()->addChild(label, 10000);

    label->runAction(Sequence::create(
        Spawn::create(
            MoveBy::create(1.0f, Vec2(0, 60)),
            FadeOut::create(1.0f),
            nullptr
        ),
        RemoveSelf::create(),
        nullptr
    ));
}