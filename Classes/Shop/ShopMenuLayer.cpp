/****************************************************************
 * Project Name:  StardewValley
 * File Name:     ShopMenuLayer.cpp
 * File Function: ShopMenuLayer类的实现
 * Author:        赵睿妍
 * Update Date:   2025/12/17
 * License:       MIT License
 ****************************************************************/
#include "ShopMenuLayer.h"

USING_NS_CC;
using namespace ui;

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
ShopMenuLayer* ShopMenuLayer::create(const std::string& shopName, const std::vector<Item*>& shopItems, const std::vector<ItemType >& acceptedTags) {
    ShopMenuLayer* pRet = new(std::nothrow) ShopMenuLayer();
    if (pRet && pRet->init(shopName, shopItems, acceptedTags)) { // 传入 shopName
        pRet->autorelease();
        return pRet;
    }
    delete pRet;
    return nullptr;
}

bool ShopMenuLayer::init(const std::string& shopName, const std::vector<Item*>& shopItems, const std::vector<ItemType >& acceptedTags) {
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

    // ... 原有的背景创建代码 ...
    Node* bg = nullptr;
    std::string bgPath = "Shop/Shop_" + shopName + ".png";
    auto spriteBg = Sprite::create(bgPath);

    // 设置背景图
    bg = spriteBg;
    spriteBg->setContentSize(Size(800, 600)); 
    if ( shopName == "sale") {
        int currentHour = TimeManager::getInstance()->getCurrentTime().hour;

        std::string greetingText = "";
        if (currentHour >= 6 && currentHour < 11) {
            greetingText = "早上好，有什么需要出售？";
        }
        else if (currentHour >= 11 && currentHour < 14) {
            greetingText = "中午好，有什么需要出售？";
        }
        else if (currentHour >= 14 && currentHour < 19) {
            greetingText = "下午好，有什么需要出售？.";
        }
        else
            greetingText = "晚上好，有什么需要出售？.";
        // 3. 创建文字标签
        auto label = Label::createWithTTF(greetingText, "fonts/pixel.ttf", 24);
        label->setTextColor(Color4B(65, 30, 10, 255));

        // 4. 设置文字位置
        Size bgSize = spriteBg->getContentSize();

        // 示例位置：背景图宽度的中间，高度的 25% 处（根据实际图片调整这个 0.25f）
        label->setPosition(Vec2(bgSize.width / 2, bgSize.height * 0.25f));

        bg->addChild(label);
    }
    // ==================== 新增逻辑结束 ====================
    // 设置通用的位置属性
    bg->setAnchorPoint(Vec2(0.5, 0.4));
    bg->setPosition(visibleSize.width / 2, visibleSize.height / 2);
    this->addChild(bg);

    // 4. 金钱
    _moneyLabel = Label::createWithTTF("0 G", "fonts/pixel.ttf", 26);
    _moneyLabel->setAnchorPoint(Vec2(1, 0.5));
    _moneyLabel->setPosition(bg->getContentSize().width - 80, bg->getContentSize().height - 50);
    _moneyLabel->setColor(Color3B(139, 69, 19));
    bg->addChild(_moneyLabel);

    // 5. 关闭按钮
    auto closeBtn = createFallbackButton("Shop/close.png", "[X]", Size(50, 50), Color3B::RED);
    closeBtn->setPosition(Vec2(bg->getContentSize().width, bg->getContentSize().height - 30));
    closeBtn->addClickEventListener([this](Ref*) { this->removeFromParent(); });
    closeBtn->setScale(5.0f);
    bg->addChild(closeBtn, 100);

    // ============================================================
      // 6. 商品列表 (ListView) - 修改高度以适配 3 个商品
      // ============================================================
    _listView = ListView::create();
    _listView->setDirection(ui::ScrollView::Direction::VERTICAL);
    _listView->setBounceEnabled(true);
    _listView->setScrollBarEnabled(true); // 确保开启滚动条
    _listView->setScrollBarPositionFromCorner(Vec2(10, 10)); // 调整滚动条位置
    _listView->setScrollBarColor(Color3B::GRAY); // 设置滚动条颜色可见

    _listView->setContentSize(Size(700, 240));

    _listView->setAnchorPoint(Vec2(0.5, 1));
    _listView->setPosition(Vec2(bg->getContentSize().width / 2, bg->getContentSize().height - 80));
    _listView->setTouchEnabled(true);

    _listView->setClippingEnabled(true);

    bg->addChild(_listView);

    // --- 填充商品 ---
    int index = 0;
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
        std::string path = item->getPath();
        if (FileUtils::getInstance()->isFileExist(path)) {
            auto icon = Sprite::create(path);
            if (icon) {
                icon->setPosition(Vec2(50, 40));
                float maxIconSize = 60.0f;
                float scale = maxIconSize / std::max(icon->getContentSize().width, icon->getContentSize().height);
                icon->setScale(scale);
                itemLayout->addChild(icon);
            }
        }

        // 名字
        auto nameLabel = Label::createWithTTF(item->getName(), "fonts/pixel.ttf", 22);
        nameLabel->setAnchorPoint(Vec2(0, 0.5));
        nameLabel->setPosition(Vec2(100, 55));
        nameLabel->setColor(Color3B::BLACK);
        itemLayout->addChild(nameLabel);

        // 价格
        auto priceLabel = Label::createWithTTF(StringUtils::format("%d G", item->getPrice()), "fonts/pixel.ttf", 18);
        priceLabel->setAnchorPoint(Vec2(0, 0.5));
        priceLabel->setPosition(Vec2(100, 25));
        priceLabel->setColor(Color3B::GRAY);
        itemLayout->addChild(priceLabel);

        // 购买按钮
        auto buyBtn = createFallbackButton("Shop/buy.png", "BUY", Size(80, 40), Color3B::BLUE);
        buyBtn->setPosition(Vec2(620, 40));
        buyBtn->setScale(3.0f);
        buyBtn->addClickEventListener([=](Ref*) {

            // 获取当前点击物品的 Tag
            ItemType tag = (ItemType)item->getTag();

            // 1. 判断是不是动物
            bool isAnimal = (tag == ItemType::ANIMAL_CHICKEN_TAG || tag == ItemType::ANIMAL_COW_TAG);

            if (isAnimal) {
                // ============================
                // A. 购买动物逻辑 (走 BarnManager)
                // ============================

                // A1. 检查金钱
                int price = item->getPrice();
                if (Money::getInstance()->getMoney() < price) {
                    showTip("Not enough money!", Color3B::RED);
                    return;
                }

                // A2. 转换类型
                AnimalType animalType = (tag == ItemType::ANIMAL_CHICKEN_TAG)
                    ? AnimalType::CHICKEN
                    : AnimalType::COW;

                // A3. 调用 BarnManager 添加动物
                bool success = BarnManager::getInstance()->addAnimal(animalType);

                if (success) {
                    // A4. 成功：扣钱
                    Money::getInstance()->spendMoney(price);
                    this->refreshUI(); // 刷新金钱显示

                    showTip("Sent to Barn!", Color3B::GREEN);
                }
                else {
                    // A5. 失败：通常是因为畜棚满了
                    showTip("Barn is Full!", Color3B::RED);
                }
            }
            else {
                // ============================
                // B. 购买普通物品逻辑 (走背包)
                // ============================
                auto detailLayer = ShopLayer::create(item);
                if (detailLayer) {
                    detailLayer->onPurchaseSuccess = [this]() { this->refreshUI(); };
                    Director::getInstance()->getRunningScene()->addChild(detailLayer, 9999);
                }
            }
            });
        itemLayout->addChild(buyBtn);

        _listView->pushBackCustomItem(itemLayout);
    }
    // 7. 初始化背包背景区域
    _inventoryNode = Node::create();
    _inventoryNode->setPosition(0, 0);
    bg->addChild(_inventoryNode);
    _selectedSlotIndex = -1;
    _highlightNode = DrawNode::create();
    _highlightNode->setLineWidth(4.0f);
    _inventoryNode->addChild(_highlightNode, 10);
    float centerX = bg->getContentSize().width / 2;
    float startY = 220.0f;
    float gapY = 75.0f;
    float bgScale = 1.45f;

    // 绘制 3 行背景图
    for (int i = 0; i < 3; i++) {
        auto backpackBg = Sprite::create("Items/inventory.jpg");

        if (backpackBg) {
            backpackBg->setScale(bgScale);
            // 从上往下排：Row 0, Row 1, Row 2
            backpackBg->setPosition(Vec2(centerX, startY - (i * gapY)));
            _inventoryNode->addChild(backpackBg);
        }
    }

    // ============================================================
     // 8. 滚轮支持 
     // ============================================================
    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseScroll = [this](Event* event) {
        if (this->_listView == nullptr) return;

        auto mouseEvent = static_cast<EventMouse*>(event);
        if (mouseEvent == nullptr) return;

        // 1. 获取鼠标在 ListView 节点空间内的坐标
        Vec2 mousePos = _listView->convertToNodeSpace(mouseEvent->getLocationInView());
        Size listSize = _listView->getContentSize();
        Rect listRect = Rect(0, 0, listSize.width, listSize.height);

        // 2. 只有鼠标在列表范围内，才响应滚轮
        if (listRect.containsPoint(mousePos)) {
            float scrollY = mouseEvent->getScrollY();

            if (scrollY != 0 && _listView->getInnerContainerSize().height > _listView->getContentSize().height) {
                // 手动计算滚动位置
                Vec2 pos = _listView->getInnerContainerPosition();
                // 增加滚动速度 (原来是 *20，如果你觉得慢可以改成 *40)
                pos.y += scrollY * 30.0f;

                float minY = _listView->getContentSize().height - _listView->getInnerContainerSize().height;
                float maxY = 0.0f;

                if (pos.y < minY) pos.y = minY;
                if (pos.y > maxY) pos.y = maxY;

                _listView->setInnerContainerPosition(pos);
            }
        }
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

    refreshUI();
    return true;
}
void ShopMenuLayer::refreshUI() {
    auto inventoryScene = InventoryScene::getInstance();
    auto gridLayer = inventoryScene->getInventoryLayer();
    if (!gridLayer) return;

    // 刷新金钱
    int money = Money::getInstance()->getMoney();
    _moneyLabel->setString(StringUtils::format("%d G", money));

    // 清理旧物品 (Tag 999)
    Vector<Node*> children = _inventoryNode->getChildren();
    for (auto node : children) {
        if (node->getTag() == 999) {
            node->removeFromParent();
        }
    }

    // 如果当前选中的索引超过了物品总数，重置它
    // if (_selectedSlotIndex >= 36) _selectedSlotIndex = -1; 

    // 如果没有选中任何东西，清理高亮
    if (_selectedSlotIndex == -1) {
        _highlightNode->clear();
    }

    // 参数配置 (与背景图对齐)
    float centerX = 400.0f;
    float startY = 220.0f;
    float gapY = 75.0f;
    float slotStep = 43.0f * 1.35f;
    float startX = centerX - (slotStep * 12 / 2) + (slotStep / 2);

    // 遍历格子
    for (int i = 0; i < 36; i++) {
        int row = i / 12;
        int col = i % 12;

        float x = startX + col * slotStep;
        float y = startY - row * gapY;
        float slotSize = 50.0f; // 点击区域和高亮基准大小

        // 1. 如果当前索引是选中的索引，画框！
        if (i == _selectedSlotIndex) {
            drawHighlight(x, y, slotSize);
        }

        Item item = gridLayer->getItemAt(i);

        // 创建透明按钮
        auto slotBtn = ui::Button::create();
        slotBtn->ignoreContentAdaptWithSize(false);
        slotBtn->setContentSize(Size(slotSize, slotSize));
        slotBtn->setPosition(Vec2(x, y));
        slotBtn->setTag(999);
        _inventoryNode->addChild(slotBtn);

        // 如果有物品
        if (item.getCount() > 0) {
            auto icon = Sprite::create(item.getPath());
            if (icon) {
                float scale = 40.0f / icon->getContentSize().width;
                icon->setScale(scale);
                icon->setPosition(slotBtn->getContentSize() / 2);
                slotBtn->addChild(icon);
            }
            if (item.getCount() > 1) {
                auto countLabel = Label::createWithTTF(std::to_string(item.getCount()), "fonts/pixel.ttf", 16);

                // 1. 设置锚点为 Label 的右下角
                countLabel->setAnchorPoint(Vec2(1, 0));

                // 2. 设置位置到按钮的右下角 (slotSize, 0)
                // 稍微减去/加上一点边距 (padding)，比如 2 像素，防止贴边太紧
                float padding = 1.0f;
                countLabel->setPosition(Vec2(slotBtn->getContentSize().width - padding, padding));
                countLabel->setScale(1.0f);
                countLabel->enableOutline(Color4B::BLACK, 1);
                slotBtn->addChild(countLabel);
            }

            // === 修改点击事件 ===
            slotBtn->addClickEventListener([=](Ref*) {
                // 1. 更新选中索引
                _selectedSlotIndex = i;

                // 2. 立即更新高亮框的位置 (视觉反馈)
                this->drawHighlight(x, y, slotSize);

                // 3. 执行卖出逻辑
                this->sellItem(item);

                });
        }
        else {
            // 如果是空格子，也可以点击选中（虽然不能卖，但可以显示红框）
            slotBtn->addClickEventListener([=](Ref*) {
                _selectedSlotIndex = i;
                this->drawHighlight(x, y, slotSize);
                });
        }
    }
}

void ShopMenuLayer::sellItem(Item item) {
    bool isAcceptable = false;

    // 如果允许列表为空
    if (_acceptedSellTags.empty()) {
        isAcceptable = false;
    }
    else {
        // 检查物品 Tag 是否在允许列表里
        for (ItemType tag : _acceptedSellTags) {
            if (item.getTag() == tag) {
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

    // 正常售卖流程
    int sellPrice = item.getPrice();

    Money::getInstance()->addMoney(sellPrice);
    InventoryScene::getInstance()->removeItemCount(item.getTag(), 1);

    showTip(StringUtils::format("+%d G", sellPrice), Color3B::YELLOW);

    refreshUI();
    EventCustom event("INVENTORY_COUNT_CHANGED");
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void ShopMenuLayer::showTip(const std::string& message, const Color3B& color) {
    auto label = Label::createWithTTF(message, "fonts/pixel.ttf", 26);
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
void ShopMenuLayer::drawHighlight(float x, float y, float size) {
    _highlightNode->clear(); // 清除旧的框
    _highlightNode->setVisible(true);

    float halfSize = size / 2 + 4.0f; // +4 让框稍微比格子大一点

    float left = x - halfSize;
    float right = x + halfSize;
    float top = y + halfSize;
    float bottom = y - halfSize;

    Color4F highlightColor = Color4F(0.85f, 0.18f, 0.18f, 1.0f); // 红色

    // 绘制粗边框 (通过画双线或者多边形来实现粗细)
    // 上边框
    _highlightNode->drawLine(Vec2(left, top), Vec2(right, top), highlightColor);
    _highlightNode->drawLine(Vec2(left, top - 2), Vec2(right, top - 2), highlightColor);

    // 下边框
    _highlightNode->drawLine(Vec2(left, bottom), Vec2(right, bottom), highlightColor);
    _highlightNode->drawLine(Vec2(left, bottom + 2), Vec2(right, bottom + 2), highlightColor);

    // 左边框
    _highlightNode->drawLine(Vec2(left, top), Vec2(left, bottom), highlightColor);
    _highlightNode->drawLine(Vec2(left + 2, top), Vec2(left + 2, bottom), highlightColor);

    // 右边框
    _highlightNode->drawLine(Vec2(right, top), Vec2(right, bottom), highlightColor);
    _highlightNode->drawLine(Vec2(right - 2, top), Vec2(right - 2, bottom), highlightColor);
}