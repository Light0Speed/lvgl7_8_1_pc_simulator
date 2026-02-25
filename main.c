
/**
 * @file main
 *
 * Ported from lv_watch: launcher_create + primary_menu_create + drop-down bar.
 *
 * - Tileview with horizontal loop mode (ghost pages at index 0 and N+1)
 * - launcher_tileview_scrl_signal for loop scrolling + drop-down gesture
 * - LED page indicators
 * - primary_menu pages with button+label per tile
 * - Drop-down bar triggered by downward swipe on the home page
 */

/*********************
 *      INCLUDES
 *********************/
#define _DEFAULT_SOURCE
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include "lvgl/lvgl.h"
#include "lv_drivers/display/monitor.h"
#include "lv_drivers/indev/mouse.h"
#include <stdio.h>

/*********************
 *      DEFINES
 *********************/
#define LAUNCHER_ANIM_TIME              50
#define LAUNCHER_HOR_SLIDING_LOOP_MODE  1
#define LAUNCHER_LED_ENABLE             1
#define LAUNCHER_LED_ON_SIZE            10
#define LAUNCHER_LED_OFF_SIZE           6
#define LAUNCHER_DROP_DOWN_START        (LV_VER_RES / 4)
#define LAUNCHER_HIDE_THRESHOLD         20
#define DROP_DOWN_ANIM_TIME             300
#define PRIMARY_MENU_APP_NUM_PER_TAB    1

/**********************
 *      TYPEDEFS
 **********************/
typedef struct {
    const char * symbol;
    const char * name;
    lv_event_cb_t event_cb;
} sim_app_info_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void hal_init(void);
static int tick_thread(void *data);
static void memory_monitor(lv_task_t *param);

/* launcher_create ported */
static lv_obj_t * launcher_create(lv_obj_t * parent);
static lv_obj_t * launcher_tileview_add_tile(lv_obj_t * tileview, uint16_t id);
static void launcher_tileview_event_cb(lv_obj_t * tileview, lv_event_t e);
static lv_res_t launcher_tileview_scrl_signal(lv_obj_t * tile_scrl, lv_signal_t sign, void * param);
static void launcher_leds_update(lv_obj_t * tileview);

/* primary_menu_create ported */
static uint8_t primary_menu_get_menu_cnt(void);
static lv_obj_t * primary_menu_create_all(lv_obj_t * tileview);
static lv_obj_t * primary_menu_create_one(lv_obj_t * tileview, int8_t menu_id);
static lv_obj_t * menu_create(lv_obj_t * tileview, uint16_t id);
static lv_obj_t * menu_add_btn(lv_obj_t * tab, int8_t menu_index, int8_t btn_id,
                               const char * symbol, const char * name, lv_event_cb_t event_cb);
static void menu_btn_event_cb(lv_obj_t * btn, lv_event_t e);

/* home page (replaces dial_create) */
static lv_obj_t * home_page_create(lv_obj_t * tileview, uint16_t id);

/* drop-down bar */
static void fix_children_coords(lv_obj_t * obj, lv_coord_t dx, lv_coord_t dy);
static void drop_down_bar_create(void);
static void drop_down_bar_fill_content(lv_obj_t * parent);
static void drop_down_show_anim_cb(void * var, lv_anim_value_t val);
static void drop_down_destroy_anim_end(lv_anim_t * a);
static void drop_down_close_btn_cb(lv_obj_t * btn, lv_event_t e);
static void drop_down_dismiss(void);
static void drop_down_snap_back(void);
static lv_res_t drop_down_scrl_signal_cb(lv_obj_t * scrl, lv_signal_t sign, void * param);

/**********************
 *  STATIC VARIABLES
 **********************/
static lv_signal_cb_t ancestor_tileview_scrl_signal = NULL;
static lv_signal_cb_t ancestor_dropdown_scrl_signal = NULL;
static lv_point_t * menu_valid_pos = NULL;
static lv_obj_t * leds_content = NULL;
static lv_obj_t ** leds = NULL;
static lv_obj_t * g_tileview = NULL;
static lv_obj_t * drop_down_panel = NULL;

static sim_app_info_t sim_apps[] = {
    { LV_SYMBOL_SETTINGS,   "Settings",   menu_btn_event_cb },
    { LV_SYMBOL_CALL,       "Phone",      menu_btn_event_cb },
    { LV_SYMBOL_NEW_LINE,   "Message",    menu_btn_event_cb },
    { LV_SYMBOL_IMAGE,      "Gallery",    menu_btn_event_cb },
    { LV_SYMBOL_AUDIO,      "Music",      menu_btn_event_cb },
    { LV_SYMBOL_GPS,        "Map",        menu_btn_event_cb },
};
static uint8_t sim_app_cnt = sizeof(sim_apps) / sizeof(sim_apps[0]);

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    lv_init();
    hal_init();

    launcher_create(lv_scr_act());

    while (1) {
        lv_task_handler();
        usleep(5 * 1000);
    }

    return 0;
}

/**********************
 *  launcher_create
 *  Ported from launcher.c lines 576-627
 **********************/
static lv_obj_t * launcher_create(lv_obj_t * parent)
{
    uint8_t menu_cnt = primary_menu_get_menu_cnt();
    printf("%s: menu_cnt = %d\n", __FUNCTION__, menu_cnt);

    uint8_t valid_pos_cnt = 0;
    uint8_t dial_index = 0;

    if(LAUNCHER_HOR_SLIDING_LOOP_MODE && menu_cnt > 0) {
        valid_pos_cnt = menu_cnt + 2;
        dial_index = 1;
    } else {
        valid_pos_cnt = menu_cnt + 1;
    }

    menu_valid_pos = (lv_point_t *)lv_mem_alloc(sizeof(lv_point_t) * valid_pos_cnt);
    for(uint8_t i = 0; i < valid_pos_cnt; i++) {
        menu_valid_pos[i].x = i;
        menu_valid_pos[i].y = 0;
    }

    lv_obj_t * tileview = lv_tileview_create(parent, NULL);
    g_tileview = tileview;
    lv_tileview_set_valid_positions(tileview, menu_valid_pos, valid_pos_cnt);
    lv_tileview_set_edge_flash(tileview, false);
    lv_tileview_set_anim_time(tileview, LAUNCHER_ANIM_TIME);
    lv_page_set_scrollbar_mode(tileview, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_local_bg_opa(tileview, LV_TILEVIEW_PART_BG, LV_STATE_DEFAULT, LV_OPA_COVER);
    /* Force scrollable bg opaque (needed for 16-bit color + SDL ARGB8888 blend mode) */
    lv_obj_t * scrl = lv_page_get_scrollable(tileview);
    lv_obj_set_style_local_bg_opa(scrl, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_COVER);
    lv_obj_set_event_cb(tileview, launcher_tileview_event_cb);

    /* Hook scrollable signal — core of the loop + gesture logic */
    if(ancestor_tileview_scrl_signal == NULL)
        ancestor_tileview_scrl_signal = lv_obj_get_signal_cb(lv_page_get_scrollable(tileview));
    lv_obj_set_signal_cb(lv_page_get_scrollable(tileview), launcher_tileview_scrl_signal);

    /* Ghost of last menu at index 0 (for loop) */
    if(LAUNCHER_HOR_SLIDING_LOOP_MODE)
        primary_menu_create_one(tileview, menu_cnt - 1);

    /* Home page at dial_index */
    home_page_create(tileview, dial_index);

    /* All menu pages */
    primary_menu_create_all(tileview);

    lv_tileview_set_tile_act(tileview, dial_index, 0, LV_ANIM_OFF);

    /* LED page indicators */
    if(LAUNCHER_LED_ENABLE && valid_pos_cnt > 1) {
        leds_content = lv_cont_create(parent, NULL);
        lv_cont_set_fit(leds_content, LV_FIT_TIGHT);
        lv_cont_set_layout(leds_content, LV_LAYOUT_ROW_MID);
        lv_obj_set_style_local_bg_opa(leds_content, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
        lv_obj_set_style_local_border_opa(leds_content, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
        lv_obj_set_style_local_pad_inner(leds_content, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 6);
        lv_obj_set_click(leds_content, false);

        leds = lv_mem_alloc(sizeof(lv_obj_t *) * valid_pos_cnt);
        for(uint8_t i = 0; i < valid_pos_cnt; i++) {
            leds[i] = lv_led_create(leds_content, NULL);
        }

        /* Hide ghost LED indicators in loop mode */
        if(LAUNCHER_HOR_SLIDING_LOOP_MODE) {
            lv_obj_set_hidden(leds[0], true);
            lv_obj_set_hidden(leds[valid_pos_cnt - 1], true);
        }

        launcher_leds_update(tileview);
    }

    return tileview;
}

/**********************
 *  launcher_tileview_add_tile
 *  Ported from launcher.c lines 136-146
 **********************/
static lv_obj_t * launcher_tileview_add_tile(lv_obj_t * tileview, uint16_t id)
{
    lv_tileview_ext_t * ext = lv_obj_get_ext_attr(tileview);
    lv_obj_t * cont = lv_cont_create(tileview, NULL);
    lv_obj_set_style_local_bg_opa(cont, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
    lv_obj_set_style_local_border_opa(cont, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
    lv_obj_set_size(cont, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_x(cont, LV_HOR_RES * ext->valid_pos[id].x);
    lv_tileview_add_element(tileview, cont);
    lv_obj_set_click(cont, false);
    return cont;
}

/**********************
 *  launcher_tileview_event_cb
 *  Ported from launcher.c lines 464-480
 **********************/
static void launcher_tileview_event_cb(lv_obj_t * tileview, lv_event_t e)
{
    if(LV_EVENT_VALUE_CHANGED == e) {
        launcher_leds_update(tileview);
    }
}

/**********************
 *  launcher_leds_update
 *  Ported from launcher.c lines 815-843
 **********************/
static void launcher_leds_update(lv_obj_t * tileview)
{
    if(!LAUNCHER_LED_ENABLE || !leds || !leds_content) return;
    lv_tileview_ext_t * ext = lv_obj_get_ext_attr(tileview);
    uint8_t cnt = ext->valid_pos_cnt;
    if(cnt <= 1) return;

    /* Hide LEDs when on home page (index 1 in loop mode, index 0 otherwise) */
    if((LAUNCHER_HOR_SLIDING_LOOP_MODE && ext->act_id.x == 1) ||
       (!LAUNCHER_HOR_SLIDING_LOOP_MODE && ext->act_id.x == 0)) {
        lv_obj_set_hidden(leds_content, true);
    } else {
        lv_obj_set_hidden(leds_content, false);
    }

    for(uint8_t i = 0; i < cnt; i++) {
        if(leds[i] == NULL || lv_obj_get_hidden(leds[i])) continue;
        if(i == ext->act_id.x) {
            lv_led_on(leds[i]);
            lv_obj_set_size(leds[i], LAUNCHER_LED_ON_SIZE, LAUNCHER_LED_ON_SIZE);
        } else {
            lv_led_off(leds[i]);
            lv_obj_set_size(leds[i], LAUNCHER_LED_OFF_SIZE, LAUNCHER_LED_OFF_SIZE);
        }
    }

    /* Handle loop mode ghost LED mapping */
    if(LAUNCHER_HOR_SLIDING_LOOP_MODE && ext->act_id.x == 0) {
        uint8_t last_real = cnt - 2;
        if(leds[last_real]) {
            lv_led_on(leds[last_real]);
            lv_obj_set_size(leds[last_real], LAUNCHER_LED_ON_SIZE, LAUNCHER_LED_ON_SIZE);
        }
    }

    lv_obj_set_pos(leds_content,
                   (LV_HOR_RES - lv_obj_get_width(leds_content)) / 2,
                   LV_VER_RES - lv_obj_get_height(leds_content) - 5);
}

/**
 * Recursively fix coords of all descendants without triggering invalidate.
 * Mirrors refresh_children_position in lv_obj.c but silent (no signals, no invalidate).
 */
static void fix_children_coords(lv_obj_t * obj, lv_coord_t dx, lv_coord_t dy)
{
    lv_obj_t * child;
    _LV_LL_READ(obj->child_ll, child) {
        child->coords.x1 += dx;
        child->coords.y1 += dy;
        child->coords.x2 += dx;
        child->coords.y2 += dy;
        fix_children_coords(child, dx, dy);
    }
}

/**********************
 *  launcher_tileview_scrl_signal
 *  Ported from launcher.c lines 325-462
 *  - Horizontal loop scrolling
 *  - Vertical gesture to trigger drop-down bar
 **********************/
static lv_res_t launcher_tileview_scrl_signal(lv_obj_t * tile_scrl, lv_signal_t sign, void * param)
{
    /* Pass non-input signals directly to avoid recursion from LV_ASSERT_OBJ */
    if(sign != LV_SIGNAL_PRESSED &&
       sign != LV_SIGNAL_PRESSING &&
       sign != LV_SIGNAL_COORD_CHG &&
       sign != LV_SIGNAL_PRESS_LOST &&
       sign != LV_SIGNAL_RELEASED &&
       sign != LV_SIGNAL_DRAG_END &&
       sign != LV_SIGNAL_DRAG_THROW_BEGIN) {
        return ancestor_tileview_scrl_signal(tile_scrl, sign, param);
    }

    lv_obj_t * tileview = tile_scrl->parent;
    lv_tileview_ext_t * tileview_ext = lv_obj_get_ext_attr(tileview);
    lv_point_t act_pt;
    lv_drag_dir_t drag_dir;

    if((LV_SIGNAL_PRESSING == sign) || (LV_SIGNAL_COORD_CHG == sign)) {
        lv_indev_t * indev = lv_indev_get_act();
        if(indev == NULL) {
            ancestor_tileview_scrl_signal(tile_scrl, sign, param);
            return LV_RES_OK;
        }
        drag_dir = indev->proc.types.pointer.drag_dir;

        /*
         * ★ 关键修复：当处于下拉模式时，不调用 ancestor_tileview_scrl_signal。
         *
         * 脏块根因链：
         *   ancestor 被调用 → lv_tileview_scrl_signal(COORD_CHG)
         *   → scrollable 被垂直拖拽 → drag_top_en==0 → lv_obj_set_y(scrl, snap_back)
         *   → lv_obj_set_pos() 内部做两次 lv_obj_invalidate (旧+新位置)
         *   → lv_page_start_edge_flash() → 又触发 edge flash 动画 invalidate
         *   → 与 drop_down_panel 的 invalidate 在同一帧交叉 → 脏块
         *
         * 修复：检测到下拉手势 或 panel 已存在时，跳过 ancestor，
         * 让 scrollable 完全静止，只移动 panel。
         */
        bool on_home_page = ((LAUNCHER_HOR_SLIDING_LOOP_MODE && tileview_ext->act_id.x == 1 && tileview_ext->act_id.y == 0) ||
                             (!LAUNCHER_HOR_SLIDING_LOOP_MODE && tileview_ext->act_id.x == 0 && tileview_ext->act_id.y == 0));

        /*
         * Prefer a direct vector check on the home page instead of waiting for
         * gesture classification, otherwise the first few PRESSING frames can
         * still enter ancestor tileview logic and trigger edge-flash invalidates.
         */
        bool downward_pull = (indev->proc.types.pointer.vect.y > 0) &&
                             (LV_MATH_ABS(indev->proc.types.pointer.vect.y) >= LV_MATH_ABS(indev->proc.types.pointer.vect.x));

        bool is_dropdown = (drop_down_panel != NULL) ||
                           (on_home_page &&
                            ((drag_dir & LV_DRAG_DIR_VER) || downward_pull) &&
                            lv_indev_get_gesture_dir(indev) != LV_GESTURE_DIR_TOP);

        if(is_dropdown) {
            /*
             * 不调用 ancestor → 无 edge flash。
             *
             * lv_indev_proc 在发 COORD_CHG 之前已经通过 lv_obj_set_pos 移动了
             * scrollable，该函数内部做了:
             *   invalidate(旧) → 改 coords → refresh_children_position → COORD_CHG → invalidate(新)
             *
             * 如果我们再用 lv_obj_set_pos 锁回去，会再触发同样的 invalidate 链，
             * 每帧 scrollable 被推走又拉回来 = 6 次无效 invalidate → 脏块。
             *
             * 修复：直接写 coords 把 scrollable 静默归位，绕过 lv_obj_set_pos，
             * 零额外 invalidate。scrollable 的子对象 coords 也要同步修正。
             */
            lv_coord_t target_x = tileview->coords.x1 + (-tileview_ext->act_id.x * lv_obj_get_width(tileview));
            lv_coord_t target_y = tileview->coords.y1 + (-tileview_ext->act_id.y * lv_obj_get_height(tileview));
            lv_coord_t dx = target_x - tile_scrl->coords.x1;
            lv_coord_t dy = target_y - tile_scrl->coords.y1;
            if(dx != 0 || dy != 0) {
                tile_scrl->coords.x1 += dx;
                tile_scrl->coords.y1 += dy;
                tile_scrl->coords.x2 += dx;
                tile_scrl->coords.y2 += dy;
                fix_children_coords(tile_scrl, dx, dy);

                /*
                 * We silently reset scrollable coords to avoid tileview edge-flash
                 * side effects, but we still need one explicit invalidate so the
                 * renderer repaints the corrected background in the same frame.
                 */
                lv_obj_invalidate(tileview);
            }

            lv_indev_get_point(indev, &act_pt);
            if(drop_down_panel == NULL) {
                drop_down_bar_create();
            }
            if(drop_down_panel != NULL) {
                lv_obj_set_y(drop_down_panel, act_pt.y - LV_VER_RES);
            }
            return LV_RES_OK;
        }

        /* 非下拉模式 → 正常调用 ancestor 处理 tileview 滑动 */
        ancestor_tileview_scrl_signal(tile_scrl, sign, param);

        /* ---- Horizontal loop logic ---- */
        if((drag_dir & LV_DRAG_DIR_HOR) && LAUNCHER_HOR_SLIDING_LOOP_MODE) {
            lv_coord_t x_coord = -lv_obj_get_width(tileview);
            lv_coord_t x_act = lv_obj_get_x(tile_scrl);

            if(tileview_ext->act_id.x == 0) {
                if(0 == x_act && indev->proc.types.pointer.vect.x > 0) {
                    lv_tileview_set_tile_act(tileview,
                        tileview_ext->valid_pos[tileview_ext->valid_pos_cnt - 1].x, 0, false);
                }
            } else if(tileview_ext->act_id.x == tileview_ext->valid_pos[tileview_ext->valid_pos_cnt - 1].x) {
                if((tileview_ext->act_id.x * x_coord == x_act) && indev->proc.types.pointer.vect.x < 0) {
                    lv_tileview_set_tile_act(tileview, 0, 0, false);
                }
            }
        }
        return LV_RES_OK;

    } else if(LV_SIGNAL_PRESS_LOST == sign || LV_SIGNAL_RELEASED == sign) {
        if(drop_down_panel != NULL) {
            lv_indev_t * indev = lv_indev_get_act();
            if(indev != NULL) {
                lv_indev_get_point(indev, &act_pt);
                if(act_pt.y >= LV_VER_RES / 2) {
                    drop_down_snap_back();
                } else {
                    drop_down_dismiss();
                }
            }
            /* 结束拖拽，让 indev 不再继续抛掷 scrollable */
            lv_indev_finish_drag(lv_indev_get_act());
            return LV_RES_OK;
        }
        ancestor_tileview_scrl_signal(tile_scrl, sign, param);
        return LV_RES_OK;

    } else if(LV_SIGNAL_DRAG_END == sign || LV_SIGNAL_DRAG_THROW_BEGIN == sign) {
        if(drop_down_panel != NULL) {
            /* 下拉模式下吞掉 drag_end/throw，不让 tileview 处理 */
            lv_indev_finish_drag(lv_indev_get_act());
            return LV_RES_OK;
        }
        ancestor_tileview_scrl_signal(tile_scrl, sign, param);
        return LV_RES_OK;

    } else {
        ancestor_tileview_scrl_signal(tile_scrl, sign, param);
    }

    return LV_RES_OK;
}

/**********************
 *  Home page (replaces dial_create)
 **********************/
static lv_obj_t * home_page_create(lv_obj_t * tileview, uint16_t id)
{
    lv_obj_t * tab = launcher_tileview_add_tile(tileview, id);

    lv_obj_t * time_lbl = lv_label_create(tab, NULL);
    lv_obj_set_style_local_text_font(time_lbl, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_48);
    lv_label_set_text(time_lbl, "12:30");
    lv_obj_align(time_lbl, tab, LV_ALIGN_CENTER, 0, -40);
    lv_tileview_add_element(tileview, time_lbl);

    lv_obj_t * date_lbl = lv_label_create(tab, NULL);
    lv_label_set_text(date_lbl, "Wed, Feb 25");
    lv_obj_align(date_lbl, time_lbl, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    lv_tileview_add_element(tileview, date_lbl);

    lv_obj_t * hint = lv_label_create(tab, NULL);
    lv_obj_set_style_local_text_color(hint, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT,
                                      LV_COLOR_MAKE(0x90, 0x90, 0x90));
    lv_label_set_text(hint, LV_SYMBOL_DOWN " swipe down for panel");
    lv_obj_align(hint, tab, LV_ALIGN_IN_BOTTOM_MID, 0, -30);
    lv_tileview_add_element(tileview, hint);

    return tab;
}

/**********************
 *  primary_menu (ported)
 **********************/
static uint8_t primary_menu_get_menu_cnt(void)
{
    return (sim_app_cnt + (PRIMARY_MENU_APP_NUM_PER_TAB - 1)) / PRIMARY_MENU_APP_NUM_PER_TAB;
}

/**
 * primary_menu_create_all — ported from primary_menu.c lines 464-471
 */
static lv_obj_t * primary_menu_create_all(lv_obj_t * tileview)
{
    lv_obj_t * tab = NULL;
    for(uint8_t i = 0; i < sim_app_cnt; i++) {
        int8_t j = i % PRIMARY_MENU_APP_NUM_PER_TAB;
        int8_t menu_index = i / PRIMARY_MENU_APP_NUM_PER_TAB;
        int8_t tab_index;

        if(j == 0) {
            if(LAUNCHER_HOR_SLIDING_LOOP_MODE)
                tab_index = menu_index + 2;
            else
                tab_index = menu_index + 1;
            tab = menu_create(tileview, tab_index);
            if(tab == NULL) return NULL;
        }
        menu_add_btn(tab, menu_index, j,
                     sim_apps[i].symbol, sim_apps[i].name, sim_apps[i].event_cb);
    }
    return tab;
}

/**
 * primary_menu_create_one — ported from primary_menu.c lines 474-477
 * Creates only the menu page for a single menu_id (used for ghost page at index 0).
 */
static lv_obj_t * primary_menu_create_one(lv_obj_t * tileview, int8_t menu_id)
{
    lv_obj_t * tab = NULL;
    for(uint8_t i = 0; i < sim_app_cnt; i++) {
        int8_t j = i % PRIMARY_MENU_APP_NUM_PER_TAB;
        int8_t menu_index = i / PRIMARY_MENU_APP_NUM_PER_TAB;
        if(menu_index < menu_id) continue;
        if(menu_index > menu_id) break;
        if(j == 0) {
            tab = menu_create(tileview, 0);
            if(tab == NULL) return NULL;
        }
        menu_add_btn(tab, menu_index, j,
                     sim_apps[i].symbol, sim_apps[i].name, sim_apps[i].event_cb);
    }
    return tab;
}

/**
 * menu_create — ported from primary_menu.c lines 1082-1117
 */
static lv_obj_t * menu_create(lv_obj_t * tileview, uint16_t id)
{
    lv_obj_t * tab = launcher_tileview_add_tile(tileview, id);
    if(tab == NULL) return NULL;

    lv_obj_t * cont = lv_cont_create(tab, NULL);
    lv_cont_set_layout(cont, LV_LAYOUT_OFF);
    lv_obj_set_style_local_bg_opa(cont, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
    lv_obj_set_style_local_border_opa(cont, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
    lv_obj_set_width(cont, LV_HOR_RES);
    lv_obj_set_height(cont, LV_VER_RES);
    lv_obj_set_click(cont, false);

    return tab;
}

/**
 * menu_add_btn — ported from primary_menu.c lines 1013-1080
 * Adds a button + label to a menu tile.
 */
static lv_obj_t * menu_add_btn(lv_obj_t * tab, int8_t menu_index, int8_t btn_id,
                               const char * symbol, const char * name, lv_event_cb_t event_cb)
{
    lv_obj_t * cont = lv_obj_get_child(tab, NULL);
    if(cont == NULL) return NULL;

    /*
     * tab's parent = scrollable (tileview internal), scrollable's parent = tileview.
     * Original code uses lv_obj_get_parent(tab) but that gives the scrollable,
     * not the tileview. Go up one more level to reach the actual tileview.
     */
    lv_obj_t * tileview = lv_obj_get_parent(lv_obj_get_parent(tab));

    lv_obj_t * btn = lv_btn_create(cont, NULL);
    lv_obj_set_size(btn, 120, 120);
    lv_obj_set_event_cb(btn, event_cb);
    lv_tileview_add_element(tileview, btn);

    lv_obj_t * icon = lv_label_create(btn, NULL);
    lv_obj_set_style_local_text_font(icon, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_28);
    lv_label_set_text(icon, symbol);
    lv_obj_align(icon, btn, LV_ALIGN_CENTER, 0, -8);

    lv_obj_t * label = lv_label_create(cont, NULL);
    lv_label_set_text(label, name);
    lv_label_set_align(label, LV_LABEL_ALIGN_CENTER);
    lv_obj_set_width(label, 120);
    lv_obj_set_click(label, false);

    lv_obj_align(btn, cont, LV_ALIGN_CENTER, 0, -20);
    lv_obj_align(label, btn, LV_ALIGN_OUT_BOTTOM_MID, 0, 8);

    return btn;
}

static void menu_btn_event_cb(lv_obj_t * btn, lv_event_t e)
{
    if(e == LV_EVENT_CLICKED) {
        printf("Menu button clicked!\n");
    }
}

/**********************
 *   DROP-DOWN BAR
 **********************/
static void drop_down_bar_create(void)
{
    if(drop_down_panel != NULL) return;

    /*
     * ★ 创建在 layer_top 上而非 lv_scr_act()。
     * 根因：下拉时 tileview scrollable 也在被拖拽改坐标，两者同时 invalidate
     * 导致脏矩形交叉→残影/不全。layer_top 有独立的渲染层，避免此问题。
     */
    drop_down_panel = lv_obj_create(lv_disp_get_layer_top(NULL), NULL);
    lv_obj_set_size(drop_down_panel, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_y(drop_down_panel, -LV_VER_RES);
    lv_obj_set_style_local_bg_color(drop_down_panel, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT,
                                    LV_COLOR_MAKE(0x30, 0x30, 0x40));
    lv_obj_set_style_local_bg_opa(drop_down_panel, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_COVER);
    lv_obj_set_style_local_border_width(drop_down_panel, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_radius(drop_down_panel, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

    lv_obj_t * page = lv_page_create(drop_down_panel, NULL);
    lv_obj_set_size(page, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_pos(page, 0, 0);
    lv_page_set_scrollbar_mode(page, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_local_bg_opa(page, LV_PAGE_PART_BG, LV_STATE_DEFAULT, LV_OPA_TRANSP);
    lv_obj_set_style_local_border_width(page, LV_PAGE_PART_BG, LV_STATE_DEFAULT, 0);

    ancestor_dropdown_scrl_signal = lv_obj_get_signal_cb(lv_page_get_scrollable(page));
    lv_obj_set_signal_cb(lv_page_get_scrollable(page), drop_down_scrl_signal_cb);

    drop_down_bar_fill_content(page);
    printf("Drop-down panel created\n");
}

static void drop_down_bar_fill_content(lv_obj_t * parent)
{
    lv_obj_t * scrl = lv_page_get_scrollable(parent);

    lv_obj_t * title_bar = lv_obj_create(scrl, NULL);
    lv_obj_set_size(title_bar, LV_HOR_RES, 50);
    lv_obj_set_pos(title_bar, 0, 0);
    lv_obj_set_style_local_bg_color(title_bar, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x00, 0x96, 0x88));
    lv_obj_set_style_local_bg_opa(title_bar, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_COVER);
    lv_obj_set_style_local_border_width(title_bar, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_radius(title_bar, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_click(title_bar, false);

    lv_obj_t * title = lv_label_create(title_bar, NULL);
    lv_obj_set_style_local_text_color(title, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_label_set_text(title, LV_SYMBOL_SETTINGS "  Status & Settings");
    lv_obj_align(title, title_bar, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t * close_btn = lv_btn_create(title_bar, NULL);
    lv_obj_set_size(close_btn, 40, 35);
    lv_obj_align(close_btn, title_bar, LV_ALIGN_IN_RIGHT_MID, -10, 0);
    lv_obj_set_style_local_bg_color(close_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0xE0, 0x40, 0x40));
    lv_obj_set_event_cb(close_btn, drop_down_close_btn_cb);
    lv_obj_t * close_lbl = lv_label_create(close_btn, NULL);
    lv_label_set_text(close_lbl, LV_SYMBOL_CLOSE);

    lv_coord_t y = 70;
    lv_obj_t * lbl;

    lbl = lv_label_create(scrl, NULL);
    lv_obj_set_style_local_text_color(lbl, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_label_set_text(lbl, LV_SYMBOL_WIFI "  WiFi          " LV_SYMBOL_BLUETOOTH "  Bluetooth");
    lv_obj_set_pos(lbl, 30, y);

    y += 40;
    lv_obj_t * sl = lv_slider_create(scrl, NULL);
    lv_obj_set_width(sl, LV_HOR_RES - 60);
    lv_slider_set_value(sl, 70, LV_ANIM_OFF);
    lv_obj_set_pos(sl, 30, y);

    y += 40;
    lbl = lv_label_create(scrl, NULL);
    lv_obj_set_style_local_text_color(lbl, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x90, 0x90, 0x90));
    lv_label_set_text(lbl, LV_SYMBOL_UP "  Swipe up to close");
    lv_obj_set_pos(lbl, (LV_HOR_RES - 200) / 2, y);
}

static void drop_down_show_anim_cb(void * var, lv_anim_value_t val) { lv_obj_set_y((lv_obj_t *)var, val); }

static void drop_down_destroy_anim_end(lv_anim_t * a)
{
    lv_obj_t * p = (lv_obj_t *)a->var;
    if(p) lv_obj_del(p);
    drop_down_panel = NULL;
    printf("Drop-down panel destroyed\n");
}

static void drop_down_dismiss(void)
{
    if(!drop_down_panel) return;
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, drop_down_panel);
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)drop_down_show_anim_cb);
    lv_anim_set_values(&a, lv_obj_get_y(drop_down_panel), -LV_VER_RES);
    lv_anim_set_time(&a, DROP_DOWN_ANIM_TIME);
    static lv_anim_path_t path_ease_in;
    path_ease_in.cb = lv_anim_path_ease_in;
    lv_anim_set_path(&a, &path_ease_in);
    lv_anim_set_ready_cb(&a, drop_down_destroy_anim_end);
    lv_anim_start(&a);
}

static void drop_down_snap_back(void)
{
    if(!drop_down_panel) return;
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, drop_down_panel);
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)drop_down_show_anim_cb);
    lv_anim_set_values(&a, lv_obj_get_y(drop_down_panel), 0);
    lv_anim_set_time(&a, DROP_DOWN_ANIM_TIME / 2);
    static lv_anim_path_t path_snap;
    path_snap.cb = lv_anim_path_ease_out;
    lv_anim_set_path(&a, &path_snap);
    lv_anim_start(&a);
}

static void drop_down_close_btn_cb(lv_obj_t * btn, lv_event_t e)
{
    if(e == LV_EVENT_CLICKED) drop_down_dismiss();
}

static lv_res_t drop_down_scrl_signal_cb(lv_obj_t * scrl, lv_signal_t sign, void * param)
{
    static lv_point_t ori_pt;
    lv_point_t act_pt;

    if(sign != LV_SIGNAL_PRESSED && sign != LV_SIGNAL_PRESSING &&
       sign != LV_SIGNAL_COORD_CHG && sign != LV_SIGNAL_PRESS_LOST &&
       sign != LV_SIGNAL_RELEASED && sign != LV_SIGNAL_DRAG_END) {
        return ancestor_dropdown_scrl_signal(scrl, sign, param);
    }

    ancestor_dropdown_scrl_signal(scrl, sign, param);
    if(!drop_down_panel) return LV_RES_OK;

    if(LV_SIGNAL_PRESSED == sign) {
        lv_indev_t * indev = lv_indev_get_act();
        if(indev) lv_indev_get_point(indev, &ori_pt);
    } else if(LV_SIGNAL_PRESSING == sign || LV_SIGNAL_COORD_CHG == sign) {
        lv_indev_t * indev = lv_indev_get_act();
        if(!indev) return LV_RES_OK;
        if(indev->proc.types.pointer.drag_dir & LV_DRAG_DIR_VER) {
            lv_indev_get_point(indev, &act_pt);
            if(lv_indev_get_gesture_dir(indev) == LV_GESTURE_DIR_TOP &&
               act_pt.y - ori_pt.y <= -LAUNCHER_HIDE_THRESHOLD) {
                lv_obj_set_y(drop_down_panel, act_pt.y - LV_VER_RES);
            }
        }
        return LV_RES_OK;
    } else if(LV_SIGNAL_PRESS_LOST == sign || LV_SIGNAL_RELEASED == sign || LV_SIGNAL_DRAG_END == sign) {
        lv_indev_t * indev = lv_indev_get_act();
        if(!indev) return LV_RES_OK;
        if(indev->proc.types.pointer.drag_dir & LV_DRAG_DIR_VER) {
            lv_indev_get_point(indev, &act_pt);
            if(act_pt.y < LV_VER_RES / 2 && act_pt.y - ori_pt.y <= -LAUNCHER_HIDE_THRESHOLD)
                drop_down_dismiss();
            else
                drop_down_snap_back();
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
    while (1) { SDL_Delay(5); lv_tick_inc(5); }
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
