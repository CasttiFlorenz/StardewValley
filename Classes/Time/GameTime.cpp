/***************************************************************
 * Project Name : StardewValley
 * File Name : GameTime.cpp
 * File Function : GameTime类的实现
 * Author : 赵睿妍
 * Update Date : 2025 / 12 / 13
 * License : MIT License
* ***************************************************************/
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

    if (hour >= 28) {
        hour -= 24;
    }
}
void GameTime::addDays(int daysToAdd) {
    dayOfMonth += daysToAdd;
    handleDayRollover();
}
void GameTime::handleDayRollover() {
    // 每个月固定 28 天
    while (dayOfMonth > 28) {
        dayOfMonth -= 28;
        int s = (int)season + 1;
        if (s > 3) {
            s = 0;
            year++;
        }
        season = (Season)s;
    }
}
std::string GameTime::getTimeString() const {
    std::string period = "am";
    int displayHour = hour;
    // 处理跨天小时 (24点=0点, 25点=1点, 26点=2点)
    if (displayHour >= 24)
            displayHour -= 24;

    // 12小时制转换
    if (displayHour >= 12) {
        period = "pm";
        if (displayHour > 12)
            displayHour -= 12;
    }

    if (displayHour == 0) 
        displayHour = 12;
    return StringUtils::format("%d:%02d %s", displayHour, minute, period.c_str());
}
std::string GameTime::getDateString() const {
    int dayIndex = (int)getDayOfWeek();
    const char* days[] = { "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun" };
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
