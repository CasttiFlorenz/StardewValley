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



| 成员               | 主要负责模块   | 具体工作内容                                                 |
| ------------------ | -------------- | ------------------------------------------------------------ |
| **郭芷烟**（组长） | 场景与底层架构 | ✅完成了InitialMenuScene、CreateScene、GameScene类<br />✅实现Farm、Town、Mines、Barn、FarmHouse 五大地图类 |
|                    | 美术资源       | ✅搭建了Farm、Town、Mines、Barn、FarmHouse 五大地图类的瓦片地图 |
|                    | 地图资源生成   | ✅实现了 FarmItemManager、CultivationManager、MinesItemManager、BarnManager 等管理类，并构建了其对应的物品继承体系，包括 EnvironmentItem（如 WoodItem、GrassItem、StoneItem 等）、Crop 及其派生作物类，以及 BarnAnimal及其派生动物类<br />✅负责了以上地图数据的存档与读取 |
|                    | 项目统筹       | ✅负责整体架构设计、任务分配、代码整合、Bug 修复与最终测试     |
| **于恩熙**         | UI状态机       | ✅设计了 InventoryManager 类，实现背包 / 技能 / 社交 / 退出界面的切换 |
|                    | 背包系统       | ✅完成了 Items 和 PlaceItems 类，实现背包内物品增减和放置<br />✅完成了 InventoryGridScene 和 InventoryScene 类，实现背包界面物品选择、高亮和主界面所选物品的预览 |
|                    | 数值可视化     | ✅实现了 SkillLevel 类，显示技能等级<br />✅实现了 SocialLevel 类，显示NPC好感度 |
|                    | 玩家动画       | ✅实现了 Player 类，控制人物移动与工具使用动画，完善了玩家碰撞检测 |
|                    | 系统界面       | ✅完成了 ExitConfirm 类，弹窗退出确认选项                        |
|                    | 存档           | ✅实现了 SaveManage 类，负责背包、时间、天气、金钱、技能等级、NPC 好感度的存档与读取    |
| **赵睿妍**         | 商店系统       | 设计并实现 Pierre、Marnie、Sale 三类商店的差异化买卖逻辑     |
|                    | 经济系统       | 实现 Money 单例，处理金钱变化与交易校验                      |
|                    | 时间系统       | 实现 TimeManager，控制时间流逝、昼夜变化与日期推进           |
|                    | 天气系统       | 实现 WeatherManager，完成天气切换与环境光照变化              |
|                    | NPC 核心逻辑   | 实现 NPC 对话树、送礼判定与好感度计算                        |
|                    | NPC 管理       | 实现 NPCManager，负责 NPC 创建、内存管理与场景恢复           |
| **蔡锦慧**         | NPC 表现       | ✅实现 NPC 的动画创建与播放                                    |
|                    | 音频系统       | ✅实现 MusicManager类，支持 BGM 场景切换与音效播放               |
|                    | 小游戏         | ✅实现了FishGameLayer类，独立开发钓鱼小游戏                                           |
