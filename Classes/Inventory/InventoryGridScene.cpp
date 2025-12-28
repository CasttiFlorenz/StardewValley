/****************************************************************
 * Project Name:  StardewValley
 * File Name:     InventoryGridScene.cpp
 * File Function: InventoryGridScene类的实现
 * Author:        于恩熙
 * Update Date:   2025/12/21
 * License:       MIT License
 ****************************************************************/

#include "InventoryGridScene.h"

USING_NS_CC;

Scene* InventoryGridScene::createScene()
{
    auto scene = Scene::create();
    auto layer = InventoryGridScene::create();
    scene->addChild(layer);
    return scene;
}


// ========== 背包背景 ==========


void InventoryGridScene::createBackpacks()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();

    // 计算每行背包的Y坐标
    float yPositions[INVENTORY_ROWS] = {
        visibleSize.height * 0.75f,  // 第一行：3/4高度
        visibleSize.height * 0.6f,   // 第二行：1/2高度
        visibleSize.height * 0.45f   // 第三行：1/4高度
    };

    for (int i = 0; i < INVENTORY_ROWS; i++) {
        _backpacks[i].x = visibleSize.width / 2;  // X居中
        _backpacks[i].y = yPositions[i];
        _backpacks[i].scale = 1.65f;

        // 创建背包精灵
        auto backpack = Sprite::create(PATH_INVENTORY_SLOT_BG);
        backpack->setPosition(_backpacks[i].x, _backpacks[i].y);
        backpack->setScale(_backpacks[i].scale);
        this->addChild(backpack);

        _backpacks[i].backpack = backpack;  // 保存指针
    }
}

//生成背包
void InventoryGridScene::createInvertory()
{
    // 定义需要的变量
    std::array<InventoryManager::BackpackInfo, 3> backpackInfos;
    std::array<Sprite*, 4> buttons;

    // 使用InventoryManager创建整个界面
    InventoryManager::createInterface(
        this,                    // 父节点
        backpackInfos,           // 返回背包信息数组
        buttons,                 // 返回按钮数组
        _background,             // 返回背景精灵
        _backpackWidth,          // 返回背包宽度
        _backpackHeight,         // 返回背包高度
        _cellWidth               // 返回格子宽度
    );

    // 将背包信息转换到原来的_backpacks数组
    for (int i = 0; i < INVENTORY_ROWS; i++) {
        _backpacks[i].backpack = backpackInfos[i].sprite;
        _backpacks[i].x = backpackInfos[i].position.x;
        _backpacks[i].y = backpackInfos[i].position.y;
        _backpacks[i].scale = backpackInfos[i].scale;
    }

    // 保存按钮引用
    _buttons = buttons;
    // 设置按钮回调
    InventoryManager::setButtonCallback([this](int buttonIndex) {
        this->onButtonClicked(buttonIndex);
        });

}

void InventoryGridScene::onButtonClicked(int buttonIndex)
{

    if (buttonIndex == 0) {  // 背包按钮
        // 显示背包
        for (int i = 0; i < INVENTORY_ROWS; i++) {
            if (_backpacks[i].backpack) {
                _backpacks[i].backpack->setVisible(true);
            }
        }
        // 重新放置工具
        refreshInventory();

        // 启用格子触摸
        setGridsTouchEnabled(true);
    }

    else {
        // 隐藏背包
        for (int i = 0; i < INVENTORY_ROWS; i++) {
            if (_backpacks[i].backpack) {
                _backpacks[i].backpack->setVisible(false);
            }
        }
        // 清除所有工具
        if (_placeItems)
            _placeItems->clearAllItems();

        // 禁用格子触摸
        setGridsTouchEnabled(false);
        // 取消高亮
        if (_selectedGrid != -1) {
            highlightGrid(_selectedGrid, false);
        }
    }

}

// ========== 初始化 ==========


bool InventoryGridScene::init()
{
    if (!Layer::init())
    {
        return false;
    }

    createInvertory();
    // 检查背包是否创建成功
    for (int i = 0; i < INVENTORY_ROWS; i++) {
        if (!_backpacks[i].backpack) {
            CCLOG("Failed to create backpack %d", i);
            return false;
        }
    }

    _selectedGrid = -1;  // 初始没有选中格子
    for (int i = 0; i < INVENTORY_TOTAL_SLOTS; i++)
        _gridHighlights[i] = nullptr;

    // 创建PlaceItems
    _placeItems = PlaceItems::getInstance();
    if (!_placeItems) {  // 添加检查
        CCLOG("Failed to get PlaceItems instance");
        return false;
    }

    // 准备背包位置数组
    float backpackX[INVENTORY_ROWS], backpackY[INVENTORY_ROWS];
    for (int i = 0; i < INVENTORY_ROWS; i++) {
        backpackX[i] = _backpacks[i].backpack->getPosition().x;
        backpackY[i] = _backpacks[i].backpack->getPosition().y;
    }

    // 设置PlaceItems的上下文
    _placeItems->setContext(this, _items, ItemCount,
        backpackX, backpackY, _backpackWidth, _cellWidth);

    // 初始化物品
    _placeItems->initItem();

    _placeItems->placeAllItems();

    // 创建可点击格子             
    createClickableGrids();

    // 设置事件监听器
    setupEventListeners();

    return true;
}



// ========== 事件监听 ==========

void InventoryGridScene::setupEventListeners()
{
    // 监听物品数量改变事件
    auto listener = EventListenerCustom::create("INVENTORY_COUNT_CHANGED",
        [this](EventCustom* event) {
            // 收到通知时刷新背包
            this->refreshInventory();
        });
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void InventoryGridScene::refreshInventory()
{
    if (!_placeItems) return;

    InventoryManager::setInitPress();

    // 准备背包位置
    float backpackX[INVENTORY_ROWS], backpackY[INVENTORY_ROWS];
    for (int i = 0; i < INVENTORY_ROWS; i++) {
        backpackX[i] = _backpacks[i].x;
        backpackY[i] = _backpacks[i].y;
    }

    // 设置PlaceItems的上下文
    _placeItems->setContext(this, _items, ItemCount,
        backpackX, backpackY, _backpackWidth, _cellWidth);

    // 重新放置所有工具，保持当前高亮状态
    if (_placeItems)
        _placeItems->placeAllItems(_selectedGrid);
    if (_selectedGrid != -1) {
        highlightGrid(_selectedGrid, true);
    }
}



// ========== 点击高光效果 ==========

void InventoryGridScene::createTouchListenerForGrid(int gridIndex, cocos2d::Sprite* grid)
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);

    // 使用 std::function 包装
    std::function<bool(cocos2d::Touch*, cocos2d::Event*)> callback =
        [this, gridIndex](cocos2d::Touch* touch, cocos2d::Event* event) -> bool {
        auto target = event->getCurrentTarget();
        cocos2d::Vec2 locationInNode = target->convertToNodeSpace(touch->getLocation());
        cocos2d::Size size = target->getContentSize();
        cocos2d::Rect rect = cocos2d::Rect(-size.width / 2, -size.height / 2, size.width, size.height);

        if (rect.containsPoint(locationInNode)) {
            this->onGridClicked(gridIndex);
            return true;
        }
        return false;
        };

    listener->onTouchBegan = callback;

    // 保存监听器到映射表
    _gridListeners[gridIndex] = listener;

    // 将监听器添加到事件分发器
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, grid);
}


// 启用或禁用所有格子的触摸监听
void InventoryGridScene::setGridsTouchEnabled(bool enabled)
{
    for (auto& pair : _gridListeners) {
        auto listener = pair.second;
        if (listener) {
            if (enabled) {
                // 恢复监听器
                listener->setEnabled(true);
            }
            else {
                // 禁用监听器
                listener->setEnabled(false);
            }
        }
    }
}

// 创建可点击格子
void InventoryGridScene::createClickableGrids()
{
    for (int row = 0; row < INVENTORY_ROWS; row++) {
        float backpackLeft = _backpacks[row].x - _backpackWidth / 2;

        float clickHeight = _backpackHeight;
        float clickY = _backpacks[row].y + _backpackHeight * 0.5;

        for (int col = 0; col < INVENTORY_COLS; col++) {
            int gridIndex = row * INVENTORY_COLS + col;
            float gridX = backpackLeft + _cellWidth * (col + 0.82f);

            // 创建格子精灵
            auto grid = Sprite::create();
            grid->setTextureRect(cocos2d::Rect(0, 0, _cellWidth, clickHeight));
            grid->setOpacity(0);
            grid->setPosition(cocos2d::Vec2(gridX, clickY));
            grid->setTag(gridIndex);

            // 创建触摸监听器
            createTouchListenerForGrid(gridIndex, grid);

            this->addChild(grid, 2);

            // 高光边框
            auto highlight = DrawNode::create();
            highlight->setVisible(false);
            this->addChild(highlight, 3);
            _gridHighlights[gridIndex] = highlight;
        }
    }
}

// 点击格子处理
void InventoryGridScene::onGridClicked(int gridIndex)
{
    // 计算行号和列号
    int row = gridIndex / INVENTORY_COLS;  // 行号：0, 1, 2
    int col = gridIndex % INVENTORY_COLS;  // 列号：0-11

    // 取消之前格子的高光
    if (_selectedGrid != -1) {
        highlightGrid(_selectedGrid, false);
    }

    // 高光新格子
    _selectedGrid = gridIndex;
    refreshInventory();
    highlightGrid(gridIndex, true);

    // 触发回调通知主界面 
    if (_gridSelectedCallback) {
        _gridSelectedCallback(gridIndex);
    }
}

// 偏移计算函数
float InventoryGridScene::getGridOffset(int colIndex)
{
    float baseOffset = 0.5f;
    float offset = 0.0f;
    float correction = 0.01f;

    if (colIndex < 2.5f)
        offset = baseOffset + 0.1f;  // 左边：向右移动
    else if (colIndex == 3)
        offset = baseOffset + 0.05f;
    else if (colIndex == 7)
        offset = baseOffset - 0.05f;
    else if (colIndex > 7.5f)
        offset = baseOffset - 0.1f - (colIndex - 7.0) * correction;  // 右边：向左移动
    else
        offset = baseOffset;

    return offset;
}

// 高光格子
void InventoryGridScene::highlightGrid(int gridIndex, bool highlight)
{
    // 计算行号和列号
    int row = gridIndex / INVENTORY_COLS;    // 行号：0, 1, 2
    int col = gridIndex % INVENTORY_COLS;    // 列号：0-11

    // 检查索引有效性
    if (row < 0 || row >= INVENTORY_ROWS || col < 0 || col >= INVENTORY_COLS)
        return;

    auto highlightNode = _gridHighlights[gridIndex];
    if (!highlightNode)
        return;

    highlightNode->clear();
    highlightNode->setVisible(highlight);

    if (highlight) {
        // 放大动画
        int toolIndexToHighlight = (gridIndex < ItemCount) ? gridIndex : -1;

        float backpackLeft = _backpacks[row].x - _backpackWidth / 2;
        float offset = getGridOffset(col);
        float gridX = backpackLeft + _cellWidth * (col + offset);

        // 计算边框位置（留出更多内边距给粗边框）
        float halfCell = _cellWidth / 2 - 1.0f;
        float halfHeight = _backpackHeight * 0.5f - 14.0f;

        // 边框四个角
        float left = gridX - halfCell;
        float right = gridX + halfCell;
        float top = _backpacks[row].y + halfHeight;
        float bottom = _backpacks[row].y - halfHeight;

        // 颜色
        Color4F highlightColor = Color4F(0.85f, 0.18f, 0.18f, 1.0f);

        // 边框厚度
        float borderThickness = 4.0f;

        // 简化的粗边框绘制：每个边框画2条线
        // 上边框（两条线）
        highlightNode->drawLine(Vec2(left, top), Vec2(right, top), highlightColor);
        highlightNode->drawLine(Vec2(left, top - 2), Vec2(right, top - 2), highlightColor);

        // 下边框（两条线）
        highlightNode->drawLine(Vec2(left, bottom), Vec2(right, bottom), highlightColor);
        highlightNode->drawLine(Vec2(left, bottom + 2), Vec2(right, bottom + 2), highlightColor);

        // 左边框（两条线）
        highlightNode->drawLine(Vec2(left, top), Vec2(left, bottom), highlightColor);
        highlightNode->drawLine(Vec2(left + 2, top), Vec2(left + 2, bottom), highlightColor);

        // 右边框（两条线）
        highlightNode->drawLine(Vec2(right, top), Vec2(right, bottom), highlightColor);
        highlightNode->drawLine(Vec2(right - 2, top), Vec2(right - 2, bottom), highlightColor);

        if (_gridSelectedCallback) {
            _gridSelectedCallback(gridIndex);
        }
    }
}

// ========== 主页面显示辅助函数 ==========

// 增加数量
bool InventoryGridScene::addItem(ItemType object, int amount) {
    if (_placeItems) {
        return _placeItems->addItem(object, amount);
    }
    return false;
}

// 减少数量
bool InventoryGridScene::removeItem(ItemType object, int amount) {
    if (_placeItems) {
        return _placeItems->removeItem(object, amount);
    }
    return false;
}

// 返回选定的格子数
int InventoryGridScene::getSelectedGrid()
{
    return _selectedGrid;
}

// 返回当前物品在数组中的索引
int InventoryGridScene::findItemGridIndex(ItemType tag)
{
    if (!_placeItems)
        return -1;

    return _placeItems->findItemIndex(tag);

}

void InventoryGridScene::setGridSelectedCallback(const std::function<void(int)>& callback)
{
    _gridSelectedCallback = callback;
}

// 获取指定格子的物品
Item& InventoryGridScene::getItemAt(int gridIndex)
{
    static Item defaultItem;  // 静态默认对象

    if (gridIndex >= 0 && gridIndex < ItemCount)
        return _items[gridIndex];

    return defaultItem;  // 返回默认值
}