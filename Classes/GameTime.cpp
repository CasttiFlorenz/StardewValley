/****************************************************************
 * Project Name:  StardewValley
 * File Name:     GameTime.cpp
 ****************************************************************/

#include "GameTime.h"

USING_NS_CC;

// ==================== 构造函数 ====================

GameTime::GameTime()
    : year(1), season(Season::Spring), dayOfMonth(1), hour(6), minute(0) {
}

GameTime::GameTime(int y, Season s, int d, int h, int m)
    : year(y), season(s), dayOfMonth(d), hour(h), minute(m) {
}

// ==================== 核心进位逻辑 ====================

void GameTime::addMinutes(int minutesToAdd) {
    minute += minutesToAdd;

    // 分钟进位
    while (minute >= 60) {
        minute -= 60;
        hour++;
    }

    // 小时进位 (仅做基础防溢出处理)
    // 注意：具体的“凌晨2点强制昏迷”逻辑由 TimeManager 在 Update 中控制
    // 这里允许 hour 达到 24, 25, 26, 27 等，方便计算持续时间
    if (hour >= 28) {
        hour -= 24;
        addDays(1);
    }
}

void GameTime::addDays(int daysToAdd) {
    dayOfMonth += daysToAdd;
    handleDayRollover();
}

void GameTime::handleDayRollover() {
    // 星露谷核心规则：每个月固定 28 天
    // 如果超过 28，进入下一个季节
    while (dayOfMonth > 28) {
        dayOfMonth -= 28;

        int s = (int)season + 1;
        if (s > 3) {
            s = 0;      // 回到春天
            year++;     // 新年快乐
        }
        season = (Season)s;
    }
}

// ==================== 字符串格式化 (UI专用) ====================

std::string GameTime::getTimeString() const {
    std::string period = "am";
    int displayHour = hour;

    // 处理跨天小时 (24点=0点, 25点=1点, 26点=2点)
    if (displayHour >= 24) displayHour -= 24;

    // 12小时制转换
    if (displayHour >= 12) {
        period = "pm";
        if (displayHour > 12) displayHour -= 12;
    }

    // 0点显示为 12 am
    if (displayHour == 0) displayHour = 12;

    // 格式化: "6:10 am"
    return StringUtils::format("%d:%02d %s", displayHour, minute, period.c_str());
}

std::string GameTime::getDateString() const {
    int dayIndex = (int)getDayOfWeek();
    // 星期缩写数组
    const char* days[] = { "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun" };

    // 格式化: "Mon. 1"
    return StringUtils::format("%s. %d", days[dayIndex], dayOfMonth);
}

std::string GameTime::getSeasonString() const {
    switch (season) {
    case Season::Spring: return "Spring";
    case Season::Summer: return "Summer";
    case Season::Fall:   return "Fall";
    case Season::Winter: return "Winter";
    default: return "Unknown";
    }
}

std::string GameTime::getFullString() const {
    // 调试用完整字符串
    return StringUtils::format("Year %d %s Day %d - %s",
        year, getSeasonString().c_str(), dayOfMonth, getTimeString().c_str());
}

// ==================== 辅助计算 ====================

DayOfWeek GameTime::getDayOfWeek() const {
    // 星露谷规则：每个月1号永远是周一 (Mon=0)
    // 所以 (日期-1) % 7 即为星期几
    return (DayOfWeek)((dayOfMonth - 1) % 7);
}

bool GameTime::isSameDay(const GameTime& other) const {
    return year == other.year &&
        season == other.season &&
        dayOfMonth == other.dayOfMonth;
}

bool GameTime::isSameHour(const GameTime& other) const {
    return isSameDay(other) && hour == other.hour;
}