# StardewValley
同济大学程序设计范式期末项目，基于Cocos2d-x-17.2开发的类星露谷物语生活模拟游戏。


## 🗂 开发日志（Development Log）

| 日期       | 开发者        | 工作内容摘要   | 备注 |
| ---------- | ------------- | -------------- | ---- |
| 2025-12-07 | CasttiFlorenz | ✅使用cocos2d-x-3.17.2创建项目<br>✅完成HoverButton类<br>✅完成InitialMenuScene类<br>✅完成CreateScene类 |类中尚有场景转换、音效音乐配置等未完成，需要完成后续开发后完善   |
| 2025-12-11 | enxi-yu       | ✅初步实现Item类<br>✅初步实现InventoryGridScene类<br> | 其中MainScene仅为实验类，后期需与其他场景合并，Item也有待添加 |
| 2025-12-11 | CasttiFlorenz | ✅初步实现GameScene总体游戏框架<br>✅初步完成GameMap基类<br>✅初步完成FarmHouse地图和可移动Player，具有简单的碰撞逻辑<br>||
| 2025-12-13 | CasttiFlorenz | ✅初步实现Farm地图<br>✅在GameScene中添加了摄像头跟随移动<br>✅将背包类整合加入GameScene<br>||
| 2025-12-13 | ZRY           | ✅实现TimaManager类<br>✅实现GameTime类<br>✅实现WeatherManager类<br>||
| 2025-12-14 | CasttiFlorenz | ✅完成了所有地图的搭建、碰撞检测和地图转换<br>||
| 2025-12-14 | enxi-yu       | ✅实现了人物移动动画、使用工具动画和更精密碰撞检测<br>||
| 2025-12-15 | CasttiFlorenz | ✅整理项目文件结构<br>✅新增了事件处理框架<br>||
| 2025-12-15 | enxi-yu       | ✅新增背包物品，区分不同物品的使用动画<br>|| 
| 2025-12-16 | CasttiFlorenz | ✅新增左键采集物品：木头、杂草、石头、铜矿<br>✅新增右键采集物品：黄水仙、韭葱|| 
| 2025-12-17 | ZRY           | ✅初步实现商店买卖功能<br> | 待整合 | 
| 2025-12-18 | ZRY           | ✅实现商店买卖功能<br>✅同步主界面背包和商店背包<br>||  
| 2025-12-18 | Floloc        | ✅初步实现NPCManager类<br>✅实现NPC动画||
| 2025-12-19 | enxi-yu       | ✅实现背包物品实时更新<br>|| 
| 2025-12-20 | ZRY           | ✅实现NPC对话，送礼功能<br>✅初步实现NPC好感度系统<br>||
| 2025-12-20 | CasttiFlorenz | ✅修复鼠标右键事件位置传入错误问题<br>✅实现了耕种和养殖系统<br>||
| 2025-12-21 | enxi-yu       | ✅实现好感度界面<br>✅实现技能水平界面<br>✅实现退出界面<br>|| 
| 2025-12-21 | Floloc        | ✅实现MusicManager类<br>✅实现FishGameLayer类<br>||
| 2025-12-22 | CasttiFlorenz | ✅修复FishGameLayer类钓鱼小游戏重复触发bug||
| 2025-12-23 | CasttiFlorenz | ✅合并多余enum class到Objects（现更名为ItemType）<br>✅调整事件触发方式，改为直接在地图类内调用相关方法||
| 2025-12-24 | ZRY           | ✅完善商店、npc互动UI界面<br>||
| 2025-12-26 | CasttiFlorenz | ✅修复了玩家在进行钓鱼、对话、商店等活动时玩家仍然可以走动与打开背包的问题<br>||
| 2025-12-27 | enxi-yu       | ✅初步实现存档背包、好感度、时间和技能水平<br>|| 
| 2025-12-28 | CasttiFlorenz | ✅完成了大部分bug的修复、整个项目的整理<br>|| 

