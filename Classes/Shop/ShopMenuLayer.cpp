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

/**
 * 工厂方法：创建 ShopMenuLayer 实例
 * 分配内存并初始化对象，设置商店名称、出售物品列表和允许收购的物品类型
 * @param shopName      商店名称（用于加载背景图）
 * @param shopItems     商店出售的物品列表指针
 * @param acceptedTags  商店愿意收购的物品类型列表
 * @return 初始化成功的 ShopMenuLayer 指针，若失败则返回 nullptr
 */
ShopMenuLayer* ShopMenuLayer::create(const std::string& shopName, const std::vector<Item*>& shopItems, const std::vector<ItemType>& acceptedTags) {
    ShopMenuLayer* pRet = new(std::nothrow) ShopMenuLayer();
    if (pRet && pRet->init(shopName, shopItems, acceptedTags)) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
}



/**
 * 初始化商店菜单层
 * 构建模态遮罩、背景板、金钱显示、关闭按钮，初始化商品 ListView 和玩家背包格子显示区域
 * 设置鼠标滚轮事件监听以支持列表滚动
 * @param shopName      商店名称
 * @param shopItems     商店出售的物品列表
 * @param acceptedTags  允许收购的物品类型
 * @return 初始化成功返回 true
 */
bool ShopMenuLayer::init(const std::string& shopName, const std::vector<Item*>& shopItems, const std::vector<ItemType>& acceptedTags) {
    if (!Layer::init()) return false;

    // 获取屏幕尺寸
    Size visibleSize = Director::getInstance()->getVisibleSize();
    _acceptedSellTags = acceptedTags;

    // 1. 创建黑色半透明遮罩层 (模态)
    auto darkLayer = LayerColor::create(COLOR_MASK_DARK);
    if (darkLayer) {
        this->addChild(darkLayer);
        auto listener = EventListenerTouchOneByOne::create();
        if (listener) {
            listener->setSwallowTouches(true);
            listener->onTouchBegan = [](Touch*, Event*) { return true; };
            _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, darkLayer);
        }
    }

    // 2. 创建背景板 
    std::string bgPath = PATH_SHOP_MENU_PREFIX + shopName + ".png";
    auto bg = Sprite::create(bgPath);

    // 如果背景图加载失败，创建保底 Layer
    if (bg == nullptr) {
        // 创建一个临时的空节点作为父容器，或者直接报错返回
        bg = Sprite::create();
    }

    if (bg) {
        bg->setContentSize(Size(SHOP_MENU_WIDTH, SHOP_MENU_HEIGHT));
        bg->setAnchorPoint(Vec2(0.5, 0.4));
        bg->setPosition(visibleSize.width / 2, visibleSize.height / 2);
        this->addChild(bg);
    }
    else {
        return false;
    }

    // 3. 金钱显示 Label
    _moneyLabel = Label::createWithTTF("0 G", PATH_FONT_PIXEL, 26);
    if (_moneyLabel) {
        _moneyLabel->setAnchorPoint(Vec2(1, 0.5));
        _moneyLabel->setPosition(bg->getContentSize().width - 80, bg->getContentSize().height - 50);
        _moneyLabel->setColor(Color3B(139, 69, 19)); // 深褐色
        bg->addChild(_moneyLabel);
    }

    // 4. 关闭按钮
    auto closeBtn = createFallbackButton(PATH_SHOP_CLOSE, "[X]", Size(50, 50), Color3B::RED);
    if (closeBtn) {
        closeBtn->setPosition(Vec2(bg->getContentSize().width, bg->getContentSize().height - 30));
        closeBtn->addClickEventListener([this](Ref*) { this->removeFromParent(); });
        closeBtn->setScale(5.0f);
        bg->addChild(closeBtn, 100);
    }

    // ============================================================
    // 5. 商品列表 (ListView)
    // ============================================================
    _listView = ListView::create();
    if (_listView) {
        _listView->setDirection(ui::ScrollView::Direction::VERTICAL);
        _listView->setBounceEnabled(true);
        _listView->setScrollBarEnabled(true);
        _listView->setScrollBarPositionFromCorner(Vec2(10, 10));
        _listView->setScrollBarColor(Color3B::GRAY);

        _listView->setContentSize(Size(700, 240));
        _listView->setAnchorPoint(Vec2(0.5, 1));
        _listView->setPosition(Vec2(bg->getContentSize().width / 2, bg->getContentSize().height - 80));
        _listView->setTouchEnabled(true);
        _listView->setClippingEnabled(true);

        bg->addChild(_listView);
    }

    // 填充商品数据
    int index = 0;
    for (const auto& item : shopItems) {
        if (!item) continue;

        auto itemLayout = Layout::create();
        itemLayout->setContentSize(Size(700, 80));
        itemLayout->setTouchEnabled(true);

        // 行背景交替颜色
        Color4B rowColor = (index % 2 == 0) ? COLOR_ROW_EVEN : COLOR_ROW_ODD;
        auto rowBg = LayerColor::create(rowColor, 700, 80);
        if (rowBg) {
            rowBg->setPosition(0, 0);
            Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(rowBg);
            itemLayout->addChild(rowBg, -1);
        }
        index++;

        // 商品图标
        std::string path = item->getPath();
        if (FileUtils::getInstance()->isFileExist(path)) {
            auto icon = Sprite::create(path);
            if (icon) {
                icon->setPosition(Vec2(50, 40));
                float maxIconSize = 60.0f;
                // 计算缩放比例，保持图标在框内
                float scale = maxIconSize / std::max(icon->getContentSize().width /1.0f, icon->getContentSize().height / 1.0f);
                icon->setScale(scale);
                itemLayout->addChild(icon);
            }
        }

        // 商品名字
        auto nameLabel = Label::createWithTTF(item->getName(), PATH_FONT_PIXEL, 22);
        if (nameLabel) {
            nameLabel->setAnchorPoint(Vec2(0, 0.5));
            nameLabel->setPosition(Vec2(100, 55));
            nameLabel->setColor(Color3B::BLACK);
            itemLayout->addChild(nameLabel);
        }

        // 商品价格
        auto priceLabel = Label::createWithTTF(StringUtils::format("%d G", item->getPrice()), PATH_FONT_PIXEL, 18);
        if (priceLabel) {
            priceLabel->setAnchorPoint(Vec2(0, 0.5));
            priceLabel->setPosition(Vec2(100, 25));
            priceLabel->setColor(Color3B::GRAY);
            itemLayout->addChild(priceLabel);
        }

        // 购买按钮
        auto buyBtn = createFallbackButton(PATH_SHOP_BUY, "BUY", Size(80, 40), Color3B::BLUE);
        if (buyBtn) {
            buyBtn->setPosition(Vec2(620, 40));
            buyBtn->setScale(3.0f);

            // 购买点击事件
            buyBtn->addClickEventListener([=](Ref*) {
                // 获取当前点击物品的 Tag
                ItemType tag = static_cast<ItemType>(item->getTag());

                // 判断是否为动物 (需要 BarnManager 处理)
                bool isAnimal = (tag == ItemType::ANIMAL_CHICKEN_TAG || tag == ItemType::ANIMAL_COW_TAG);

                if (isAnimal) {
                    // === 动物购买逻辑 ===
                    auto moneySystem = Money::getInstance();
                    auto barnSystem = BarnManager::getInstance();

                    if (!moneySystem || !barnSystem) return;

                    int price = item->getPrice();
                    if (moneySystem->getMoney() < price) {
                        showTip("Not enough money!", Color3B::RED);
                        return;
                    }

                    AnimalType animalType = (tag == ItemType::ANIMAL_CHICKEN_TAG)
                        ? AnimalType::CHICKEN
                        : AnimalType::COW;

                    bool success = barnSystem->addAnimal(animalType);

                    if (success) {
                        moneySystem->spendMoney(price);
                        this->refreshUI();
                        showTip("Sent to Barn!", Color3B::GREEN);
                    }
                    else {
                        showTip("Barn is Full!", Color3B::RED);
                    }
                }
                else {
                    // === 普通物品购买逻辑 (弹窗 ShopLayer) ===
                    auto detailLayer = ShopLayer::create(item);
                    if (detailLayer) {
                        detailLayer->setOnPurchaseSuccess([this]() { this->refreshUI(); });
                        Director::getInstance()->getRunningScene()->addChild(detailLayer, 9999);
                    }
                }
                });
            itemLayout->addChild(buyBtn);
        }

        if (_listView) {
            _listView->pushBackCustomItem(itemLayout);
        }
    }

    // 6. 初始化玩家背包显示区域 (下半部分)
    _inventoryNode = Node::create();
    if (_inventoryNode) {
        _inventoryNode->setPosition(0, 0);
        bg->addChild(_inventoryNode);
    }

    _selectedSlotIndex = -1;
    _highlightNode = DrawNode::create();
    if (_highlightNode && _inventoryNode) {
        _highlightNode->setLineWidth(4.0f);
        _inventoryNode->addChild(_highlightNode, 10);
    }

    // 绘制 3 行背包背景图
    for (int i = 0; i < INVENTORY_ROWS; i++) {
        auto backpackBg = Sprite::create(PATH_INVENTORY_SLOT_BG);
        if (backpackBg && _inventoryNode) {
            backpackBg->setScale(INV_BG_SCALE);
            // 计算背景图纵向位置
            backpackBg->setPosition(Vec2(INV_GRID_CENTER_X, INV_GRID_START_Y - (i * INV_GRID_GAP_Y)));
            _inventoryNode->addChild(backpackBg);
        }
    }

    // ============================================================
    // 7. 鼠标滚轮监听 (用于 ListView 滚动)
    // ============================================================
    auto mouseListener = EventListenerMouse::create();
    if (mouseListener) {
        mouseListener->onMouseScroll = [this](Event* event) {
            if (this->_listView == nullptr) return;

            auto mouseEvent = static_cast<EventMouse*>(event);
            if (mouseEvent == nullptr) return;

            Vec2 mousePos = _listView->convertToNodeSpace(mouseEvent->getLocationInView());
            Size listSize = _listView->getContentSize();
            Rect listRect = Rect(0, 0, listSize.width, listSize.height);

            // 仅当鼠标在列表范围内时响应
            if (listRect.containsPoint(mousePos)) {
                float scrollY = mouseEvent->getScrollY();

                if (scrollY != 0 && _listView->getInnerContainerSize().height > _listView->getContentSize().height) {
                    Vec2 pos = _listView->getInnerContainerPosition();
                    pos.y += scrollY * SCROLL_SENSITIVITY;

                    float minY = _listView->getContentSize().height - _listView->getInnerContainerSize().height;
                    float maxY = 0.0f;

                    if (pos.y < minY) pos.y = minY;
                    if (pos.y > maxY) pos.y = maxY;

                    _listView->setInnerContainerPosition(pos);
                }
            }
            };
        _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
    }

    refreshUI();
    return true;
}



/**
 * 刷新 UI 界面
 * 更新当前持有的金钱数值，重绘背包格子内的物品图标及数量，并清理无效的高亮显示
 */
void ShopMenuLayer::refreshUI() {
    auto inventoryScene = InventoryScene::getInstance();
    if (!inventoryScene) return;

    auto gridLayer = inventoryScene->getInventoryLayer();
    if (!gridLayer) return;

    // 刷新金钱显示
    if (Money::getInstance() && _moneyLabel) {
        int money = Money::getInstance()->getMoney();
        _moneyLabel->setString(StringUtils::format("%d G", money));
    }

    // 清理背包区域的旧物品 (Tag 999)
    if (_inventoryNode) {
        Vector<Node*> children = _inventoryNode->getChildren();
        for (auto node : children) {
            if (node && node->getTag() == 999) {
                node->removeFromParent();
            }
        }
    }

    // 如果没有选中，清理高亮
    if (_selectedSlotIndex == -1 && _highlightNode) {
        _highlightNode->clear();
    }

    // 计算网格布局参数
    float slotStep = INV_SLOT_BASE_SIZE * INV_SLOT_SCALE;
    float startX = INV_GRID_CENTER_X - (slotStep * INVENTORY_COLS / 2) + (slotStep / 2);

    // 遍历背包格子
    for (int i = 0; i < INVENTORY_TOTAL_SLOTS; i++) {
        int row = i / INVENTORY_COLS;
        int col = i % INVENTORY_COLS;

        float x = startX + col * slotStep;
        float y = INV_GRID_START_Y - row * INV_GRID_GAP_Y;

        // 如果选中，绘制高亮框
        if (i == _selectedSlotIndex) {
            drawHighlight(x, y, INV_SLOT_TOUCH_SIZE);
        }

        Item item = gridLayer->getItemAt(i);

        // 创建不可见的点击按钮覆盖在格子上方
        auto slotBtn = ui::Button::create();
        if (slotBtn) {
            slotBtn->ignoreContentAdaptWithSize(false);
            slotBtn->setContentSize(Size(INV_SLOT_TOUCH_SIZE, INV_SLOT_TOUCH_SIZE));
            slotBtn->setPosition(Vec2(x, y));
            slotBtn->setTag(999); // 标记为物品节点

            if (_inventoryNode) {
                _inventoryNode->addChild(slotBtn);
            }

            // 如果该格子有物品
            if (item.getCount() > 0) {
                auto icon = Sprite::create(item.getPath());
                if (icon) {
                    float scale = 40.0f / icon->getContentSize().width;
                    icon->setScale(scale);
                    icon->setPosition(slotBtn->getContentSize() / 2);
                    slotBtn->addChild(icon);
                }

                // 显示数量
                if (item.getCount() > 1) {
                    auto countLabel = Label::createWithTTF(std::to_string(item.getCount()), PATH_FONT_PIXEL, 16);
                    if (countLabel) {
                        countLabel->setAnchorPoint(Vec2(1, 0));
                        float padding = 1.0f;
                        countLabel->setPosition(Vec2(slotBtn->getContentSize().width - padding, padding));
                        countLabel->setScale(1.0f);
                        countLabel->enableOutline(Color4B::BLACK, 1);
                        slotBtn->addChild(countLabel);
                    }
                }

                // 点击事件：选中并尝试卖出
                slotBtn->addClickEventListener([=](Ref*) {
                    _selectedSlotIndex = i;
                    this->drawHighlight(x, y, INV_SLOT_TOUCH_SIZE);
                    this->sellItem(item);
                    });
            }
            else {
                // 空格子点击事件：仅选中
                slotBtn->addClickEventListener([=](Ref*) {
                    _selectedSlotIndex = i;
                    this->drawHighlight(x, y, INV_SLOT_TOUCH_SIZE);
                    });
            }
        }
    }
}



/**
 * 尝试出售选中的物品
 * 验证物品是否在商店收购列表内，若允许则增加玩家金钱、扣除背包物品数量，并显示收益提示
 * @param item 要出售的物品对象
 */
void ShopMenuLayer::sellItem(Item item) {
    bool isAcceptable = false;

    // 检查是否在商店允许收购的列表内
    if (_acceptedSellTags.empty()) {
        isAcceptable = false;
    }
    else {
        for (ItemType tag : _acceptedSellTags) {
            if (item.getTag() == tag) {
                isAcceptable = true;
                break;
            }
        }
    }

    if (!isAcceptable) {
        showTip("This item cannot be sold here!", Color3B::RED);
        return;
    }

    // 执行售卖
    int sellPrice = item.getPrice();
    auto moneySystem = Money::getInstance();
    auto inventorySystem = InventoryScene::getInstance();

    if (moneySystem && inventorySystem) {
        moneySystem->addMoney(sellPrice);
        inventorySystem->removeItemCount(item.getTag(), 1);

        showTip(StringUtils::format("+%d G", sellPrice), Color3B::YELLOW);

        // 刷新界面和通知事件
        refreshUI();
        EventCustom event("INVENTORY_COUNT_CHANGED");
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
    }
}



/**
 * 显示浮动提示文本
 * 在屏幕中央生成一段文本，执行向上飘动并淡出的动画，随后自动销毁
 * @param message 提示内容字符串
 * @param color   提示文本颜色
 */
void ShopMenuLayer::showTip(const std::string& message, const Color3B& color) {
    auto label = Label::createWithTTF(message, PATH_FONT_PIXEL, 26);
    if (!label) return;

    label->setColor(color);
    label->enableOutline(Color4B::BLACK, 1);

    Size winSize = Director::getInstance()->getVisibleSize();
    label->setPosition(winSize / 2);

    // 添加到场景最上层，确保提示可见
    auto runningScene = Director::getInstance()->getRunningScene();
    if (runningScene) {
        runningScene->addChild(label, 10000);

        // 动画：向上飘并淡出
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
}


/**
 * 绘制选中高亮框
 * 在指定的坐标位置绘制一个红色的加粗边框，用于标识当前选中的背包格子
 * @param x    中心 X 坐标
 * @param y    中心 Y 坐标
 * @param size 格子尺寸
 */
void ShopMenuLayer::drawHighlight(float x, float y, float size) {
    if (!_highlightNode) return;

    _highlightNode->clear();
    _highlightNode->setVisible(true);

    float halfSize = size / 2 + 4.0f; // 外扩一点

    float left = x - halfSize;
    float right = x + halfSize;
    float top = y + halfSize;
    float bottom = y - halfSize;

    Color4F highlightColor = COLOR_HIGHLIGHT_RED;

    // 绘制加粗边框 (通过绘制双重线条模拟)
    _highlightNode->drawLine(Vec2(left, top), Vec2(right, top), highlightColor);
    _highlightNode->drawLine(Vec2(left, top - 2), Vec2(right, top - 2), highlightColor);

    _highlightNode->drawLine(Vec2(left, bottom), Vec2(right, bottom), highlightColor);
    _highlightNode->drawLine(Vec2(left, bottom + 2), Vec2(right, bottom + 2), highlightColor);

    _highlightNode->drawLine(Vec2(left, top), Vec2(left, bottom), highlightColor);
    _highlightNode->drawLine(Vec2(left + 2, top), Vec2(left + 2, bottom), highlightColor);

    _highlightNode->drawLine(Vec2(right, top), Vec2(right, bottom), highlightColor);
    _highlightNode->drawLine(Vec2(right - 2, top), Vec2(right - 2, bottom), highlightColor);
}


/**
 * 内部辅助函数：创建按钮（带资源加载失败保底）
 * 尝试使用图片路径创建按钮，若失败则创建一个纯色背景的文本按钮作为替代
 * @param imagePath 图片资源路径
 * @param text      保底文本内容
 * @param size      按钮尺寸
 * @param color     保底背景颜色
 * @return 创建好的 Button 对象指针
 */
Button* ShopMenuLayer::createFallbackButton(const std::string& imagePath, const std::string& text, const Size& size, Color3B color) {
    auto btn = Button::create(imagePath);
    // 检查按钮纹理是否加载成功 (若宽度为0通常意味着加载失败)
    if (btn->getContentSize().width == 0) {
        btn = Button::create();
        btn->ignoreContentAdaptWithSize(false);
        btn->setContentSize(size);

        auto bg = LayerColor::create(Color4B(color), size.width, size.height);
        if (bg) {
            bg->setPosition(0, 0);
            Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(bg);
            btn->addChild(bg, -1);
        }

        btn->setTitleText(text);
        btn->setTitleFontSize(20);
        btn->setTitleColor(Color3B::WHITE);
    }
    return btn;
}