/**
 * @file ui.c
 * @brief 
 * @version 0.1
 * @date 2021-10-29
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <time.h>
#include "esp_err.h"
#include "esp_log.h"
#include "lvgl.h"
#include "ui_main.h"

static const char *TAG = "ui";

esp_err_t ui_main_start(void)
{
    if (NULL == lv_scr_act()) {
        ESP_LOGE(TAG, "LVGL not initialized");
        return ESP_ERR_INVALID_STATE;
    }

    ui_mute_init();

    ui_sr_anim_init();

    boot_animate_start(lv_scr_act());

    ui_clock_update_start();

    return ESP_OK;
}

static void clock_blink_cb(lv_timer_t *timer)
{
    static time_t time_val;
    static bool disp = true;
    static char time_str[11];
    static char date_str[32];
    // static int trTime;
    // static char ampm[3];

    time(&time_val);
    struct tm time;

    setenv("TZ", "CST6CDT,M3.2.0/2,M11.1.0", 1);
    tzset();

    localtime_r(&time_val, &time);
    strftime(date_str,sizeof(date_str) - 1, "%A, %B %d", &time);

    // trTime = time.tm_hour;
    // sprintf(ampm,"AM");

    // if(trTime >= 12) {
    //     trTime -= 12;
    //     sprintf(ampm,"PM");
    // }

    disp = !disp;
    if (disp) {
        // sprintf(time_str, "%02d:%02d %s", trTime, time.tm_min, ampm);
        strftime(time_str, sizeof(time_str) - 1, "%I:%M %p", &time);
    } else {
        // sprintf(time_str, "%02d %02d %s", trTime, time.tm_min, ampm);
        strftime(time_str, sizeof(time_str) - 1, "%I %M %p", &time);
    }

    ui_clock_set_time(time_str, date_str);
    ui_status_bar_set_time(time_str);
}

void ui_clock_update_start(void)
{
    static lv_timer_t *timer = NULL;
    if (NULL == timer) {
        timer = lv_timer_create(clock_blink_cb, 1000, NULL);
    }
}
