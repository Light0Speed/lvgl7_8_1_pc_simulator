
/**
 * @file main
 *
 * Demo: Tabview with a drop-down bar panel (slides from top).
 * Pattern follows launcher_drop_down_bar_create from lv_watch.
 *
 * - Main screen: 3-tab tabview (Controls / Visuals / Info)
 * - Trigger: click the top button to show the drop-down panel
 * - Drop-down panel slides in from y=-LV_VER_RES, can be dismissed by
 *   swiping up or clicking close button
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
#define DROP_DOWN_ANIM_TIME     300
#define DROP_DOWN_HIDE_THRESHOLD 30

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void hal_init(void);
static int tick_thread(void *data);
static void memory_monitor(lv_task_t *param);

static void create_main_ui(void);
static void trigger_btn_event_cb(lv_obj_t * btn, lv_event_t e);

static void drop_down_bar_create(void);
static void drop_down_bar_fill_content(lv_obj_t * parent);
static void drop_down_show_anim_cb(void * var, lv_anim_value_t val);
static void drop_down_hide_anim_cb(void * var, lv_anim_value_t val);
static void drop_down_destroy_anim_end(lv_anim_t * a);
static void drop_down_close_btn_cb(lv_obj_t * btn, lv_event_t e);
static lv_res_t drop_down_scrl_signal_cb(lv_obj_t * scrl, lv_signal_t sign, void * param);

/**********************
 *  STATIC VARIABLES
 **********************/
static lv_signal_cb_t ancestor_scrl_signal = NULL;
static lv_obj_t * drop_down_panel = NULL;

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    lv_init();
    hal_init();

    create_main_ui();

    while (1) {
        lv_task_handler();
        usleep(5 * 1000);
    }

    return 0;
}

/**********************
 *   MAIN UI
 **********************/

static void create_main_ui(void)
{
    lv_obj_t * tv = lv_tabview_create(lv_scr_act(), NULL);
    lv_tabview_set_anim_time(tv, 200);

    /* ---- Tab 1: Controls ---- */
    lv_obj_t * tab1 = lv_tabview_add_tab(tv, "Controls");

    lv_obj_t * open_btn = lv_btn_create(tab1, NULL);
    lv_obj_set_size(open_btn, 250, 50);
    lv_obj_align(open_btn, NULL, LV_ALIGN_IN_TOP_MID, 0, 10);
    lv_obj_set_event_cb(open_btn, trigger_btn_event_cb);
    lv_obj_t * open_lbl = lv_label_create(open_btn, NULL);
    lv_label_set_text(open_lbl, LV_SYMBOL_DOWN " Open Drop-Down");

    lv_obj_t * sw = lv_switch_create(tab1, NULL);
    lv_obj_align(sw, open_btn, LV_ALIGN_OUT_BOTTOM_MID, 0, 30);

    lv_obj_t * sw_lbl = lv_label_create(tab1, NULL);
    lv_label_set_text(sw_lbl, "Toggle Switch");
    lv_obj_align(sw_lbl, sw, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

    lv_obj_t * slider = lv_slider_create(tab1, NULL);
    lv_obj_set_width(slider, LV_HOR_RES / 2);
    lv_slider_set_value(slider, 50, LV_ANIM_OFF);
    lv_obj_align(slider, sw_lbl, LV_ALIGN_OUT_BOTTOM_MID, 0, 30);

    /* ---- Tab 2: Visuals ---- */
    lv_obj_t * tab2 = lv_tabview_add_tab(tv, "Visuals");

    lv_obj_t * spinner = lv_spinner_create(tab2, NULL);
    lv_obj_set_size(spinner, 80, 80);
    lv_obj_align(spinner, NULL, LV_ALIGN_IN_TOP_MID, 0, 20);

    lv_obj_t * spin_lbl = lv_label_create(tab2, NULL);
    lv_label_set_text(spin_lbl, "Loading...");
    lv_obj_align(spin_lbl, spinner, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

    lv_obj_t * bar = lv_bar_create(tab2, NULL);
    lv_obj_set_size(bar, LV_HOR_RES * 2 / 3, 20);
    lv_bar_set_value(bar, 65, LV_ANIM_ON);
    lv_obj_align(bar, spin_lbl, LV_ALIGN_OUT_BOTTOM_MID, 0, 25);

    /* ---- Tab 3: Info ---- */
    lv_obj_t * tab3 = lv_tabview_add_tab(tv, "Info");

    lv_obj_t * info_lbl = lv_label_create(tab3, NULL);
    lv_label_set_text(info_lbl,
        "LVGL Drop-Down Bar Demo\n\n"
        "Click the button on Tab 1\n"
        "to open the drop-down panel.\n\n"
        "Swipe up inside the panel\n"
        "to dismiss it, or click\n"
        "the close button.");
    lv_label_set_align(info_lbl, LV_LABEL_ALIGN_CENTER);
    lv_obj_align(info_lbl, NULL, LV_ALIGN_CENTER, 0, 0);

    lv_tabview_set_tab_act(tv, 0, LV_ANIM_OFF);
}

static void trigger_btn_event_cb(lv_obj_t * btn, lv_event_t e)
{
    if(e == LV_EVENT_CLICKED) {
        if(drop_down_panel == NULL) {
            drop_down_bar_create();
        }
    }
}

/**********************
 *   DROP-DOWN BAR
 *   Pattern: launcher_drop_down_bar_create
 **********************/

/**
 * Create the drop-down panel. It starts at y = -LV_VER_RES (above screen)
 * and animates down to y = 0.
 */
static void drop_down_bar_create(void)
{
    if(drop_down_panel != NULL) return;

    /* Create a full-screen container positioned above the visible area */
    drop_down_panel = lv_obj_create(lv_scr_act(), NULL);
    lv_obj_set_size(drop_down_panel, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_y(drop_down_panel, -LV_VER_RES);

    lv_obj_set_style_local_bg_color(drop_down_panel, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT,
                                    LV_COLOR_MAKE(0x30, 0x30, 0x40));
    lv_obj_set_style_local_bg_opa(drop_down_panel, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_COVER);
    lv_obj_set_style_local_border_width(drop_down_panel, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_radius(drop_down_panel, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    /* Create a page inside for scrollable content + signal interception */
    lv_obj_t * page = lv_page_create(drop_down_panel, NULL);
    lv_obj_set_size(page, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_pos(page, 0, 0);
    lv_page_set_scrollbar_mode(page, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_local_bg_opa(page, LV_PAGE_PART_BG, LV_STATE_DEFAULT, LV_OPA_TRANSP);
    lv_obj_set_style_local_border_width(page, LV_PAGE_PART_BG, LV_STATE_DEFAULT, 0);

    /* Hook the scrollable signal for swipe-to-dismiss */
    ancestor_scrl_signal = lv_obj_get_signal_cb(lv_page_get_scrollable(page));
    lv_obj_set_signal_cb(lv_page_get_scrollable(page), drop_down_scrl_signal_cb);

    /* Fill the panel content */
    drop_down_bar_fill_content(page);

    /* Animate: slide from y = -LV_VER_RES to y = 0 */
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, drop_down_panel);
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)drop_down_show_anim_cb);
    lv_anim_set_values(&a, -LV_VER_RES, 0);
    lv_anim_set_time(&a, DROP_DOWN_ANIM_TIME);
    lv_anim_set_path(&a, lv_anim_path_ease_out);
    lv_anim_start(&a);

    printf("Drop-down panel created\n");
}

/**
 * Fill the drop-down panel with status/setting-like content.
 * Similar to status_and_setting_create() in the watch framework.
 */
static void drop_down_bar_fill_content(lv_obj_t * parent)
{
    lv_obj_t * scrl = lv_page_get_scrollable(parent);

    /* ---- Title bar ---- */
    lv_obj_t * title_bar = lv_obj_create(scrl, NULL);
    lv_obj_set_size(title_bar, LV_HOR_RES, 50);
    lv_obj_set_pos(title_bar, 0, 0);
    lv_obj_set_style_local_bg_color(title_bar, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT,
                                    LV_COLOR_MAKE(0x00, 0x96, 0x88));
    lv_obj_set_style_local_bg_opa(title_bar, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_COVER);
    lv_obj_set_style_local_border_width(title_bar, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_radius(title_bar, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_click(title_bar, false);

    lv_obj_t * title = lv_label_create(title_bar, NULL);
    lv_obj_set_style_local_text_color(title, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_label_set_text(title, LV_SYMBOL_SETTINGS "  Status & Settings");
    lv_obj_align(title, title_bar, LV_ALIGN_CENTER, 0, 0);

    /* ---- Close button ---- */
    lv_obj_t * close_btn = lv_btn_create(title_bar, NULL);
    lv_obj_set_size(close_btn, 40, 35);
    lv_obj_align(close_btn, title_bar, LV_ALIGN_IN_RIGHT_MID, -10, 0);
    lv_obj_set_style_local_bg_color(close_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT,
                                    LV_COLOR_MAKE(0xE0, 0x40, 0x40));
    lv_obj_set_event_cb(close_btn, drop_down_close_btn_cb);
    lv_obj_t * close_lbl = lv_label_create(close_btn, NULL);
    lv_label_set_text(close_lbl, LV_SYMBOL_CLOSE);

    /* ---- Row 1: WiFi & Bluetooth ---- */
    lv_coord_t row_y = 70;
    lv_coord_t col_w = LV_HOR_RES / 2 - 30;

    lv_obj_t * wifi_cont = lv_obj_create(scrl, NULL);
    lv_obj_set_size(wifi_cont, col_w, 80);
    lv_obj_set_pos(wifi_cont, 20, row_y);
    lv_obj_set_style_local_bg_color(wifi_cont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT,
                                    LV_COLOR_MAKE(0x40, 0x40, 0x55));
    lv_obj_set_style_local_bg_opa(wifi_cont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_COVER);
    lv_obj_set_style_local_radius(wifi_cont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 10);
    lv_obj_set_style_local_border_width(wifi_cont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_click(wifi_cont, false);

    lv_obj_t * wifi_icon = lv_label_create(wifi_cont, NULL);
    lv_obj_set_style_local_text_color(wifi_icon, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT,
                                      LV_COLOR_MAKE(0x00, 0xBC, 0xD4));
    lv_label_set_text(wifi_icon, LV_SYMBOL_WIFI);
    lv_obj_align(wifi_icon, wifi_cont, LV_ALIGN_IN_TOP_MID, 0, 10);

    lv_obj_t * wifi_sw = lv_switch_create(wifi_cont, NULL);
    lv_obj_set_size(wifi_sw, 50, 22);
    lv_switch_on(wifi_sw, LV_ANIM_OFF);
    lv_obj_align(wifi_sw, wifi_cont, LV_ALIGN_IN_BOTTOM_MID, 0, -10);

    lv_obj_t * bt_cont = lv_obj_create(scrl, NULL);
    lv_obj_set_size(bt_cont, col_w, 80);
    lv_obj_set_pos(bt_cont, LV_HOR_RES / 2 + 10, row_y);
    lv_obj_set_style_local_bg_color(bt_cont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT,
                                    LV_COLOR_MAKE(0x40, 0x40, 0x55));
    lv_obj_set_style_local_bg_opa(bt_cont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_COVER);
    lv_obj_set_style_local_radius(bt_cont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 10);
    lv_obj_set_style_local_border_width(bt_cont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_click(bt_cont, false);

    lv_obj_t * bt_icon = lv_label_create(bt_cont, NULL);
    lv_obj_set_style_local_text_color(bt_icon, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT,
                                      LV_COLOR_MAKE(0x42, 0xA5, 0xF5));
    lv_label_set_text(bt_icon, LV_SYMBOL_BLUETOOTH);
    lv_obj_align(bt_icon, bt_cont, LV_ALIGN_IN_TOP_MID, 0, 10);

    lv_obj_t * bt_sw = lv_switch_create(bt_cont, NULL);
    lv_obj_set_size(bt_sw, 50, 22);
    lv_obj_align(bt_sw, bt_cont, LV_ALIGN_IN_BOTTOM_MID, 0, -10);

    /* ---- Row 2: Brightness slider ---- */
    row_y += 100;

    lv_obj_t * bright_lbl = lv_label_create(scrl, NULL);
    lv_obj_set_style_local_text_color(bright_lbl, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_label_set_text(bright_lbl, LV_SYMBOL_IMAGE "  Brightness");
    lv_obj_set_pos(bright_lbl, 20, row_y);

    lv_obj_t * bright_slider = lv_slider_create(scrl, NULL);
    lv_obj_set_width(bright_slider, LV_HOR_RES - 60);
    lv_slider_set_value(bright_slider, 70, LV_ANIM_OFF);
    lv_obj_set_pos(bright_slider, 30, row_y + 30);

    /* ---- Row 3: Volume slider ---- */
    row_y += 70;

    lv_obj_t * vol_lbl = lv_label_create(scrl, NULL);
    lv_obj_set_style_local_text_color(vol_lbl, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_label_set_text(vol_lbl, LV_SYMBOL_VOLUME_MAX "  Volume");
    lv_obj_set_pos(vol_lbl, 20, row_y);

    lv_obj_t * vol_slider = lv_slider_create(scrl, NULL);
    lv_obj_set_width(vol_slider, LV_HOR_RES - 60);
    lv_slider_set_value(vol_slider, 45, LV_ANIM_OFF);
    lv_obj_set_pos(vol_slider, 30, row_y + 30);

    /* ---- Row 4: Battery & hint ---- */
    row_y += 80;

    lv_obj_t * bat_lbl = lv_label_create(scrl, NULL);
    lv_obj_set_style_local_text_color(bat_lbl, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT,
                                      LV_COLOR_MAKE(0x66, 0xBB, 0x6A));
    lv_label_set_text(bat_lbl, LV_SYMBOL_BATTERY_FULL "  Battery: 85%");
    lv_obj_set_pos(bat_lbl, 20, row_y);

    lv_obj_t * hint_lbl = lv_label_create(scrl, NULL);
    lv_obj_set_style_local_text_color(hint_lbl, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT,
                                      LV_COLOR_MAKE(0x90, 0x90, 0x90));
    lv_label_set_text(hint_lbl, LV_SYMBOL_UP "  Swipe up to close this panel");
    lv_label_set_align(hint_lbl, LV_LABEL_ALIGN_CENTER);
    lv_obj_set_pos(hint_lbl, (LV_HOR_RES - 300) / 2, row_y + 40);
}

/**********************
 *   DROP-DOWN ANIMATIONS
 **********************/

static void drop_down_show_anim_cb(void * var, lv_anim_value_t val)
{
    lv_obj_set_y((lv_obj_t *)var, val);
}

static void drop_down_hide_anim_cb(void * var, lv_anim_value_t val)
{
    lv_obj_set_y((lv_obj_t *)var, val);
}

static void drop_down_destroy_anim_end(lv_anim_t * a)
{
    lv_obj_t * panel = (lv_obj_t *)a->var;
    if(panel) {
        lv_obj_del(panel);
    }
    drop_down_panel = NULL;
    printf("Drop-down panel destroyed\n");
}

static void drop_down_dismiss(void)
{
    if(drop_down_panel == NULL) return;

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, drop_down_panel);
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)drop_down_hide_anim_cb);
    lv_anim_set_values(&a, lv_obj_get_y(drop_down_panel), -LV_VER_RES);
    lv_anim_set_time(&a, DROP_DOWN_ANIM_TIME);
    lv_anim_set_path(&a, lv_anim_path_ease_in);
    lv_anim_set_ready_cb(&a, drop_down_destroy_anim_end);
    lv_anim_start(&a);
}

static void drop_down_snap_back(void)
{
    if(drop_down_panel == NULL) return;

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, drop_down_panel);
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)drop_down_show_anim_cb);
    lv_anim_set_values(&a, lv_obj_get_y(drop_down_panel), 0);
    lv_anim_set_time(&a, DROP_DOWN_ANIM_TIME / 2);
    lv_anim_set_path(&a, lv_anim_path_ease_out);
    lv_anim_start(&a);
}

static void drop_down_close_btn_cb(lv_obj_t * btn, lv_event_t e)
{
    if(e == LV_EVENT_CLICKED) {
        drop_down_dismiss();
    }
}

/**********************
 *   DROP-DOWN SIGNAL HANDLER
 *   Pattern: launcher_drop_down_tileview_scrl_signal
 **********************/

/**
 * Intercept scroll signals to implement swipe-to-dismiss.
 * On upward swipe past threshold -> dismiss with animation.
 * On release without enough swipe -> snap back to y=0.
 */
static lv_res_t drop_down_scrl_signal_cb(lv_obj_t * scrl, lv_signal_t sign, void * param)
{
    static lv_point_t ori_pt;
    lv_point_t act_pt;

    /* Delegate non-input signals to ancestor */
    if(sign != LV_SIGNAL_PRESSED &&
       sign != LV_SIGNAL_PRESSING &&
       sign != LV_SIGNAL_COORD_CHG &&
       sign != LV_SIGNAL_PRESS_LOST &&
       sign != LV_SIGNAL_RELEASED &&
       sign != LV_SIGNAL_DRAG_END) {
        return ancestor_scrl_signal(scrl, sign, param);
    }

    ancestor_scrl_signal(scrl, sign, param);

    if(drop_down_panel == NULL) return LV_RES_OK;

    if(LV_SIGNAL_PRESSED == sign) {
        lv_indev_t * indev = lv_indev_get_act();
        if(indev == NULL) return LV_RES_OK;
        lv_indev_get_point(indev, &ori_pt);

    } else if(LV_SIGNAL_PRESSING == sign || LV_SIGNAL_COORD_CHG == sign) {
        lv_indev_t * indev = lv_indev_get_act();
        if(indev == NULL) return LV_RES_OK;
        lv_drag_dir_t drag_dir = indev->proc.types.pointer.drag_dir;

        if(drag_dir & LV_DRAG_DIR_VER) {
            lv_indev_get_point(indev, &act_pt);
            lv_coord_t dy = act_pt.y - ori_pt.y;
            if(lv_indev_get_gesture_dir(indev) == LV_GESTURE_DIR_TOP &&
               dy <= -DROP_DOWN_HIDE_THRESHOLD) {
                lv_obj_set_y(drop_down_panel, dy);
            }
        }
        return LV_RES_OK;

    } else if(LV_SIGNAL_PRESS_LOST == sign ||
              LV_SIGNAL_RELEASED == sign ||
              LV_SIGNAL_DRAG_END == sign) {
        lv_indev_t * indev = lv_indev_get_act();
        if(indev == NULL) return LV_RES_OK;
        lv_drag_dir_t drag_dir = indev->proc.types.pointer.drag_dir;

        if(drag_dir & LV_DRAG_DIR_VER) {
            lv_indev_get_point(indev, &act_pt);
            lv_coord_t dy = act_pt.y - ori_pt.y;

            if(act_pt.y < LV_VER_RES / 2 && dy <= -DROP_DOWN_HIDE_THRESHOLD) {
                drop_down_dismiss();
            } else {
                drop_down_snap_back();
            }
        }
        return LV_RES_OK;
    }

    return LV_RES_OK;
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
