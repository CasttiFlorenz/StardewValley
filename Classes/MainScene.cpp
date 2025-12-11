/****************************************************************
 * Project Name:  StardewValley
 * File Name:     MainScene.cpp
 * File Function: MainScene类的实现
 * Author:        于恩熙
 * Update Date:   2025/12/11
 * License:       MIT License
 ****************************************************************/

#include "MainScene.h"

USING_NS_CC;

Scene* MainScene::createScene()
{
    auto scene = Scene::create();
    auto layer = MainScene::create();
    scene->addChild(layer);
    return scene;
}

// 初始化主场景 
bool MainScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    // 创建背景
    _background = Sprite::create("/CreateScene/Background-0.jpg");  // 主背景
    if (!_background) {
        _background = Sprite::create("/tools/background.png");  // 备用背景
    }

    if (_background) {
        // 全屏显示背景
        _background->setPosition(Vec2(visibleSize.width / 2 + origin.x,
            visibleSize.height / 2 + origin.y));
        _background->setScale(visibleSize.width / _background->getContentSize().width,
            visibleSize.height / _background->getContentSize().height);
        this->addChild(_background, 0);  // 最底层
    }

    _previewFrame = Sprite::create("/tools/box.png"); // 预览框图片

    // 放在左下角
    float previewX = origin.x + 100;      // 左边距100
    float previewY = origin.y + 100;      // 下边距100
    _previewFrame->setPosition(cocos2d::Vec2(previewX, previewY));
    this->addChild(_previewFrame, 5);

    // 预览工具图片
    _previewTool = Sprite::create();
    _previewTool->setPosition(_previewFrame->getPosition());
    this->addChild(_previewTool, 6);

    // 创建背包界面
    _inventoryLayer = InventoryGridScene::create();
    if (_inventoryLayer) {
        _inventoryLayer->setVisible(false);
        _inventoryLayer->setGridSelectedCallback([this](int gridIndex) {
            // 当背包中格子被选中时，立即更新预览
            this->updatePreviewTool();
            });
        this->addChild(_inventoryLayer, 10);
    }

    _inventoryVisible = false;

    // 设置键盘监听 
    setupKeyboardListener();

    // 初始更新预览（显示第一个工具）
    updatePreviewTool();
    _previewFrame->setVisible(true);
    _previewTool->setVisible(true);

    return true;
}

// 设置键盘监听器
void MainScene::setupKeyboardListener()
{
    auto keyboardListener = EventListenerKeyboard::create();

    // E键按下事件
    keyboardListener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event* event) {
        if (keyCode == EventKeyboard::KeyCode::KEY_E) {
            this->toggleInventory();
        }
        };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
}

// 切换背包显示/隐藏状态
void MainScene::toggleInventory()
{
    _inventoryVisible = !_inventoryVisible;    // 切换背包可见状态标志

    if (_inventoryLayer) {
        _inventoryLayer->setVisible(_inventoryVisible);

        if (_inventoryVisible) {
            _inventoryLayer->setScale(0.1f);
            auto scaleAction = ScaleTo::create(0.3f, 1.0f);
            _inventoryLayer->runAction(scaleAction);
        }
        else {
            auto scaleAction = ScaleTo::create(0.2f, 0.1f);
            auto hideAction = CallFunc::create([this]() {
                _inventoryLayer->setVisible(false);
                _inventoryLayer->setScale(1.0f);
                });
            auto sequence = Sequence::create(scaleAction, hideAction, nullptr);
            _inventoryLayer->runAction(sequence);

            updatePreviewTool();
        }
    }
}

// 更新预览的工具图片
void MainScene::updatePreviewTool()
{
    if (!_inventoryLayer) {
        return;
    }

    // 获取背包中当前高光的格子索引
    int selectedGrid = _inventoryLayer->getSelectedGrid();

    // 如果没有选中任何格子，直接返回
    if (selectedGrid < 0) {
        return;
    }

    if (selectedGrid >= 0 && selectedGrid < _inventoryLayer->INTEM_COUNT) { // 只显示前INTEM_COUNT个工具
        // 获取工具图片路径
        const Item& item = _inventoryLayer->getItemAt(selectedGrid);

        // 获取物品图片路径
        std::string toolImage = item.getPos();

        if (!toolImage.empty()) {
            auto toolSprite = cocos2d::Sprite::create(toolImage);
            if (toolSprite) {
                _previewTool->setTexture(toolSprite->getTexture());
                _previewTool->setTextureRect(toolSprite->getTextureRect());
                _previewTool->setScale(1.6f); // 放大一点显示
                _previewTool->setVisible(true);

                CCLOG("预览框显示工具: %s", toolImage.c_str());
            }
        }
    }
    else
        _previewTool->setVisible(false);  // 保持可见，显示空框
}