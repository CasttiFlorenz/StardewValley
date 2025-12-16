/****************************************************************
 * Project Name:  StardewValley
 * File Name:     InventoryGridScene.cpp
 * File Function: InventoryGridScene类的实现
 * Author:        于恩熙
 * Update Date:   2025/12/11
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
    float yPositions[3] = {
        visibleSize.height * 0.75f,  // 第一行：3/4高度
        visibleSize.height * 0.6f,   // 第二行：1/2高度
        visibleSize.height * 0.45f   // 第三行：1/4高度
    };

    for (int i = 0; i < 3; i++) {
        _backpacks[i].x = visibleSize.width / 2;  // X居中
        _backpacks[i].y = yPositions[i];
        _backpacks[i].scale = 1.65f;

        // 创建背包精灵
        auto backpack = Sprite::create("/Items/inventory.jpg");
        backpack->setPosition(_backpacks[i].x, _backpacks[i].y);
        backpack->setScale(_backpacks[i].scale);
        this->addChild(backpack);

        _backpacks[i].backpack = backpack;  // 保存指针
    }
}

//生成背包
void InventoryGridScene::createInvertory()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    // 使用图片背景
    auto background = Sprite::create("/Items/background.png");
    background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 70));
    background->setScaleX(1.5f);  
    background->setScaleY(2.0f);  
    this->addChild(background, -1);

    //三行
    createBackpacks();

    //获取背包的实际显示尺寸
    Size backpackSize = _backpacks[0].backpack->getContentSize();
    _backpackWidth = backpackSize.width * _backpacks[0].scale;
    _backpackHeight = backpackSize.height * _backpacks[0].scale;

    //计算背包格子（一行12个格子）
    int gridCols = 12;   // 12列
    int gridRows = 1;    // 1行

    _cellWidth = _backpackWidth / gridCols;    // 每个格子的宽度
    float cellHeight = _backpackHeight;             // 整个背包高度

}


// ========== 工具 ==========


// 放置工具
void InventoryGridScene::placeTools(int highlightIndex)
{
    // 清除所有已存在的工具精灵
    clearAllTools();

    // 计算背包左边界
    float backpackLeft0 = _backpacks[0].x - _backpackWidth / 2;

    // 遍历工具格
    for (int i = 0; i < INTEM_COUNT; i++) {
        Item& item = _inventory[i];
        float baseScale = item.getScale();;       // 基础大小
        float highlightScale = baseScale + 0.2f;  // 高亮时放大

        float x = backpackLeft0 + _cellWidth * item.getPrintPos(); // 简单位置计算

        float scale = (i == highlightIndex) ? highlightScale : baseScale;

        updateToolSprite(i, item.getPos(), scale, x, _backpacks[i / 12].y);
    }
}

// 清除所有工具精灵
void InventoryGridScene::clearAllTools()
{
    // 遍历所有可能的工具tag
    for (int i = 0; i < INTEM_COUNT; i++) {
        // 计算当前工具的tag
        int tag = TOOL_TAG_BASE + i;

        // 查找并移除该tag的精灵
        auto toolSprite = this->getChildByTag(tag);
        if (toolSprite) {
            toolSprite->removeFromParent();
        }
    }
}

// 显示物品数量
void InventoryGridScene::showItemCount(int itemIndex, int count, float x, float y)
{
    // 先清除该索引原有的数量标签
    int oldCountTag = COUNT_TAG_BASE + itemIndex;
    auto oldCountLabel = this->getChildByTag(oldCountTag);
    if (oldCountLabel) {
        oldCountLabel->removeFromParent();
    }

    // 创建数量标签
    std::string countText = std::to_string(count);
    auto countLabel = Label::createWithTTF(countText, "/fonts/arial.ttf", 14);

    if (countLabel) {
        // 计算右下角位置
        float offsetX = 25.0f;  // 向右偏移
        float offsetY = -25.0f; // 向下偏移

        countLabel->setPosition(Vec2(x + offsetX, y + offsetY));
        countLabel->setColor(Color3B::WHITE);
        countLabel->enableOutline(Color4B::BLACK, 2);  // 黑色描边，更清晰
        
        // 设置唯一tag
        countLabel->setTag(COUNT_TAG_BASE + itemIndex);

        this->addChild(countLabel, 2);  // 比工具精灵更高的层级
    }
}

// 更新单个工具精灵
void InventoryGridScene::updateToolSprite(int toolIndex, const std::string& imagePath,
    float size, float x, float y)
{
    // 计算当前工具的tag
    int tag = TOOL_TAG_BASE + toolIndex;

    // 查找是否已存在相同tag的精灵（理论上应该已被clearAllTools清理）
    auto oldSprite = this->getChildByTag(tag);
    if (oldSprite) {
        oldSprite->removeFromParent();
    }

    // 创建新的工具精灵
    auto sprite = Sprite::create(imagePath);
    if (sprite) {
        Item& item = _inventory[toolIndex];

        sprite->setPosition(Vec2(x, y));
        sprite->setScale(size);
        if (item.getCount() <= 0) {
            sprite->setOpacity(70); // 透明度 
        }

        // 设置唯一tag，便于后续管理
        sprite->setTag(tag);

        // 添加到场景中
        this->addChild(sprite, 1);  

        // 显示数量
        if (item.getTag() > Objects::FISHINGROD)
            showItemCount(toolIndex, item.getCount(), x, y);

        // 添加点击时的轻微动画效果
        if (size > item.getScale()) {  // 如果是高亮状态
            // 轻微上浮动画
            auto floatUp = MoveBy::create(0.1f, Vec2(0, 3));
            sprite->runAction(floatUp);
        }
    }
}

// 初始化工具
void InventoryGridScene::initItem()
{
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        _inventory[i] = Item();  // 创建空物品
    }

    // 放置初始工具
    _inventory[0] = Item(Objects::HOE, 1, 1.5f, 0.7f, "/Items/hoe.png");
    _inventory[1] = Item(Objects::AXE, 1, 1.4f, 1.6f, "/Items/axe.png");
    _inventory[2] = Item(Objects::WATERING_CAN, 1, 1.6f, 2.6f, "/Items/wateringCan.png");
    _inventory[3] = Item(Objects::PICKAXE, 1, 1.4f, 3.6f, "/Items/pickaxe.png");
    _inventory[4] = Item(Objects::SCYTHE, 1, 1.4f, 4.6f, "/Items/scythe.png");
    _inventory[5] = Item(Objects::FISHINGROD, 1, 1.3f, 5.5f, "/Items/fishingRod.png");
    _inventory[6] = Item(Objects::STONE, 1, 4.3f, 6.5f, "/Items/stone.png");
    _inventory[7] = Item(Objects::WOOD, 1, 2.8f, 7.5f, "/Items/wood.png");
    _inventory[8] = Item(Objects::COPPER_ORE, 1, 3.3f, 8.4f, "/Items/copper ore.png");
    _inventory[9] = Item(Objects::FERTILIZER, 1, 4.3f, 9.4f, "/Items/fertilizer.png");
    _inventory[10] = Item(Objects::PARSNIP_SEED, 1, 4.3f, 10.4f, "/Items/parsnip seed.png");
    _inventory[11] = Item(Objects::CAULIFLOWER_SEED, 1, 4.3f, 11.35f, "/Items/cauliflower seed.png");
    _inventory[12] = Item(Objects::POTATO_SEED, 1, 4.3f, 0.65f, "/Items/potato seed.png");
    _inventory[13] = Item(Objects::PARSNIP, 1, 4.3f, 1.6f, "/Items/parsnip.png");
    _inventory[14] = Item(Objects::CAULIFLOWER, 1, 4.3f, 2.6f, "/Items/cauliflower.png");
    _inventory[15] = Item(Objects::POTATO, 1, 4.3f, 3.55f, "/Items/potato.png");
    _inventory[16] = Item(Objects::FIBER, 1, 4.3f, 4.55f, "/Items/fiber.png");
    _inventory[17] = Item(Objects::DAFFODILS, 1, 4.3f, 5.5f, "/Items/daffodils.png");
    _inventory[18] = Item(Objects::LEEK, 1, 4.3f, 6.5f, "/Items/leek.png");
    _inventory[19] = Item(Objects::HAY, 0, 4.3f, 7.5f, "/Items/hay.png");
    _inventory[20] = Item(Objects::EGG, 1, 4.8f, 8.4f, "/Items/egg.png");
    _inventory[21] = Item(Objects::FRIED_EGG, 1, 4.3f, 9.4f, "/Items/fried egg.png");
    _inventory[22] = Item(Objects::CARP, 1, 4.3f, 10.4f, "/Items/carp.png");
    _inventory[23] = Item(Objects::MILK, 1, 4.3f, 11.35f, "/Items/milk.png");
    _inventory[24] = Item(Objects::SALAD, 1, 4.3f, 0.65f, "/Items/salad.png");

}


// ========== 初始化 ==========


bool InventoryGridScene::init()
{
    if (!Layer::init())
    {
        return false;
    }

    createInvertory();

    _selectedGrid = -1;  // 初始没有选中格子
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        _gridHighlights[i] = nullptr;
    }

    // 初始化物品数组
    initItem();

    // 放置工具
    placeTools();

    // 创建可点击格子
    createClickableGrids();

    return true;
}



// ========== 点击高光效果 ==========

// 为指定格子创建触摸监听器
void InventoryGridScene::createTouchListenerForGrid(int gridIndex, cocos2d::Sprite* grid)
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);

    // 触摸开始事件
    listener->onTouchBegan = [this, gridIndex](cocos2d::Touch* touch, cocos2d::Event* event) {
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
    for (int row = 0; row < 3; row++) {
        float backpackLeft = _backpacks[row].x - _backpackWidth / 2;

        float clickHeight = _backpackHeight;
        float clickY = _backpacks[row].y + _backpackHeight * 0.5;

        for (int col = 0; col < 12; col++) {
            int gridIndex = row * 12 + col;
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
    int row = gridIndex / 12;  // 行号：0, 1, 2
    int col = gridIndex % 12;  // 列号：0-11

    // 取消之前格子的高光
    if (_selectedGrid != -1) {
        highlightGrid(_selectedGrid, false);
    }

    // 高光新格子
    highlightGrid(gridIndex, true);
    _selectedGrid = gridIndex;

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

    if (colIndex < 3.5f)
        offset = baseOffset + 0.1f ;  // 左边：向右移动
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
    int row = gridIndex / 12;    // 行号：0, 1, 2
    int col = gridIndex % 12;    // 列号：0-11

    // 检查索引有效性
    if (row < 0 || row >= 3 || col < 0 || col >= 12)
        return;

    auto highlightNode = _gridHighlights[gridIndex];
    if (!highlightNode) 
        return;

    highlightNode->clear();
    highlightNode->setVisible(highlight);

    if (highlight) {
        // 放大动画
        int toolIndexToHighlight = (gridIndex < INTEM_COUNT) ? gridIndex : -1;
        placeTools(toolIndexToHighlight);

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

int InventoryGridScene::getSelectedGrid()
{
    return _selectedGrid;
}

void InventoryGridScene::setGridSelectedCallback(const std::function<void(int)>& callback)
{
    _gridSelectedCallback = callback;
}

// 获取指定格子的物品
Item& InventoryGridScene::getItemAt(int gridIndex) 
{
    assert(gridIndex >= 0 && gridIndex < INVENTORY_SIZE);
    return _inventory[gridIndex];
}
