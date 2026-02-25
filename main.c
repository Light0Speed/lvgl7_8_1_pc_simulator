
/**
 * @file main
 *
 * Demo: Tabview with cycling pages and a dropdown list.
 * Based on the launcher/primary_menu framework pattern from lv_watch.
 */

/*********************
 *      INCLUDES
 *********************/
#define _DEFAULT_SOURCE
#include <stdlib.h>
#include <unistd.h>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include "lvgl/lvgl.h"
#include "lv_drivers/display/monitor.h"
#include "lv_drivers/indev/mouse.h"
#include "lv_examples/lv_examples.h"
#include <stdio.h>

/*********************
 *      DEFINES
 *********************/
#define PAGE_COUNT  3

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void hal_init(void);
static int tick_thread(void *data);
static void memory_monitor(lv_task_t *param);
static void create_tabview_demo(void);
static void dropdown_event_cb(lv_obj_t * dd, lv_event_t e);
static void tab_change_event_cb(lv_obj_t * tv, lv_event_t e);

/**********************
 *  STATIC VARIABLES
 **********************/
static lv_obj_t * result_label = NULL;

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    lv_init();
    hal_init();

    create_tabview_demo();

    while (1) {
        lv_task_handler();
        usleep(5 * 1000);
    }

    return 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void create_tabview_demo(void)
{
    lv_obj_t * tv = lv_tabview_create(lv_scr_act(), NULL);
    lv_tabview_set_anim_time(tv, 200);
    lv_obj_set_event_cb(tv, tab_change_event_cb);

    /* ---- Tab 1: Dropdown ---- */
    lv_obj_t * tab1 = lv_tabview_add_tab(tv, "Dropdown");

    lv_obj_t * prompt = lv_label_create(tab1, NULL);
    lv_label_set_text(prompt, "Select a fruit:");
    lv_obj_align(prompt, NULL, LV_ALIGN_IN_TOP_MID, 0, 20);

    lv_obj_t * dd = lv_dropdown_create(tab1, NULL);
    lv_dropdown_set_options(dd,
        "Apple\n"
        "Banana\n"
        "Cherry\n"
        "Date\n"
        "Elderberry\n"
        "Fig\n"
        "Grape");
    lv_obj_set_width(dd, LV_HOR_RES * 2 / 3);
    lv_obj_align(dd, prompt, LV_ALIGN_OUT_BOTTOM_MID, 0, 15);
    lv_obj_set_event_cb(dd, dropdown_event_cb);

    result_label = lv_label_create(tab1, NULL);
    lv_label_set_text(result_label, "Selected: Apple");
    lv_obj_align(result_label, dd, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);

    lv_obj_t * hint = lv_label_create(tab1, NULL);
    lv_obj_set_style_local_text_color(hint, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT,
                                      LV_COLOR_MAKE(0x90, 0x90, 0x90));
    lv_label_set_text(hint, "Swipe left/right to switch tabs");
    lv_label_set_align(hint, LV_LABEL_ALIGN_CENTER);
    lv_obj_align(hint, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, -20);

    /* ---- Tab 2: Controls ---- */
    lv_obj_t * tab2 = lv_tabview_add_tab(tv, "Controls");

    lv_obj_t * btn = lv_btn_create(tab2, NULL);
    lv_obj_set_size(btn, 160, 50);
    lv_obj_align(btn, NULL, LV_ALIGN_IN_TOP_MID, 0, 20);
    lv_obj_t * btn_label = lv_label_create(btn, NULL);
    lv_label_set_text(btn_label, "Click Me");

    lv_obj_t * sw = lv_switch_create(tab2, NULL);
    lv_obj_align(sw, btn, LV_ALIGN_OUT_BOTTOM_MID, 0, 30);

    lv_obj_t * sw_label = lv_label_create(tab2, NULL);
    lv_label_set_text(sw_label, "Toggle Switch");
    lv_obj_align(sw_label, sw, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

    lv_obj_t * slider = lv_slider_create(tab2, NULL);
    lv_obj_set_width(slider, LV_HOR_RES / 2);
    lv_slider_set_value(slider, 50, LV_ANIM_OFF);
    lv_obj_align(slider, sw_label, LV_ALIGN_OUT_BOTTOM_MID, 0, 30);

    lv_obj_t * slider_label = lv_label_create(tab2, NULL);
    lv_label_set_text(slider_label, "Drag the slider");
    lv_obj_align(slider_label, slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

    /* ---- Tab 3: Visuals ---- */
    lv_obj_t * tab3 = lv_tabview_add_tab(tv, "Visuals");

    lv_obj_t * spinner = lv_spinner_create(tab3, NULL);
    lv_obj_set_size(spinner, 80, 80);
    lv_obj_align(spinner, NULL, LV_ALIGN_IN_TOP_MID, 0, 20);

    lv_obj_t * spin_label = lv_label_create(tab3, NULL);
    lv_label_set_text(spin_label, "Loading...");
    lv_obj_align(spin_label, spinner, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

    lv_obj_t * bar = lv_bar_create(tab3, NULL);
    lv_obj_set_size(bar, LV_HOR_RES * 2 / 3, 20);
    lv_bar_set_value(bar, 65, LV_ANIM_ON);
    lv_obj_align(bar, spin_label, LV_ALIGN_OUT_BOTTOM_MID, 0, 25);

    lv_obj_t * bar_label = lv_label_create(tab3, NULL);
    lv_label_set_text(bar_label, "Progress: 65%");
    lv_obj_align(bar_label, bar, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

    lv_tabview_set_tab_act(tv, 0, LV_ANIM_OFF);
}

static void dropdown_event_cb(lv_obj_t * dd, lv_event_t e)
{
    if(e == LV_EVENT_VALUE_CHANGED) {
        char sel[32];
        lv_dropdown_get_selected_str(dd, sel, sizeof(sel));
        if(result_label) {
            char buf[64];
            lv_snprintf(buf, sizeof(buf), "Selected: %s", sel);
            lv_label_set_text(result_label, buf);
        }
        printf("Dropdown selected: %s\n", sel);
    }
}

static void tab_change_event_cb(lv_obj_t * tv, lv_event_t e)
{
    if(e == LV_EVENT_VALUE_CHANGED) {
        uint16_t tab = lv_tabview_get_tab_act(tv);
        printf("Tab changed to: %d\n", tab);
    }
}

/**********************
 *  HAL INIT
 **********************/
static void hal_init(void)
{
    monitor_init();

    static lv_disp_buf_t disp_buf1;
    static lv_color_t buf1_1[LV_HOR_RES_MAX * 120];
    lv_disp_buf_init(&disp_buf1, buf1_1, NULL, LV_HOR_RES_MAX * 120);

    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.buffer = &disp_buf1;
    disp_drv.flush_cb = monitor_flush;
    lv_disp_drv_register(&disp_drv);

    mouse_init();
    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = mouse_read;
    lv_indev_t *mouse_indev = lv_indev_drv_register(&indev_drv);

    LV_IMG_DECLARE(mouse_cursor_icon);
    lv_obj_t * cursor_obj = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_src(cursor_obj, &mouse_cursor_icon);
    lv_indev_set_cursor(mouse_indev, cursor_obj);

    SDL_CreateThread(tick_thread, "tick", NULL);
    lv_task_create(memory_monitor, 5000, LV_TASK_PRIO_MID, NULL);
}

static int tick_thread(void *data)
{
    (void)data;
    while (1) {
        SDL_Delay(5);
        lv_tick_inc(5);
    }
    return 0;
}

static void memory_monitor(lv_task_t *param)
{
    (void)param;
    lv_mem_monitor_t mon;
    lv_mem_monitor(&mon);
    printf("used: %6d (%3d %%), frag: %3d %%, biggest free: %6d\n",
           (int)mon.total_size - mon.free_size, mon.used_pct, mon.frag_pct,
           (int)mon.free_biggest_size);
}
