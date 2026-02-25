
/**
 * @file main
 *
 * Demo: Tileview with looping (circular) pages and a dropdown list.
 * Follows the launcher/primary_menu framework pattern from lv_watch.
 */

/*********************
 *      INCLUDES
 *********************/
#define _DEFAULT_SOURCE /* needed for usleep() */
#include <stdlib.h>
#include <unistd.h>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include "lvgl/lvgl.h"
#include "lv_drivers/display/monitor.h"
#include "lv_drivers/indev/mouse.h"
#include <stdio.h>
#include "lv_examples/lv_examples.h"

/*********************
 *      DEFINES
 *********************/
#define PAGE_COUNT              3
#define LOOP_MODE_ENABLE        1
#define TILEVIEW_ANIM_TIME      200

#define LED_ON_SIZE             10
#define LED_OFF_SIZE            6
#define LED_INDICATOR_PAD       8
#define LED_BOTTOM_OFFSET       15

/**********************
 *      TYPEDEFS
 **********************/
typedef struct {
    lv_obj_t * led_container;
    lv_obj_t * leds[PAGE_COUNT];
    uint8_t    page_count;
    uint8_t    total_positions;
} app_ext_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void hal_init(void);
static int tick_thread(void *data);
static void memory_monitor(lv_task_t *param);

static void create_looping_tabview(lv_obj_t * parent);
static lv_obj_t * add_tile_page(lv_obj_t * tileview, uint16_t index);
static void fill_page_content(lv_obj_t * page_cont, uint8_t page_id, lv_obj_t * tileview);
static void create_page_indicators(lv_obj_t * parent, app_ext_t * app);
static void update_indicators(lv_obj_t * tileview, app_ext_t * app);

static void tileview_event_cb(lv_obj_t * tileview, lv_event_t e);
static lv_res_t tileview_scrl_signal_cb(lv_obj_t * scrl, lv_signal_t sign, void * param);
static void dropdown_event_cb(lv_obj_t * dropdown, lv_event_t e);

/**********************
 *  STATIC VARIABLES
 **********************/
static lv_signal_cb_t ancestor_scrl_signal;
static app_ext_t app_data;
static lv_point_t valid_pos[PAGE_COUNT + 2];

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    lv_init();
    hal_init();

    create_looping_tabview(lv_scr_act());

    while (1) {
        lv_task_handler();
        usleep(5 * 1000);
    }

    return 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void create_looping_tabview(lv_obj_t * parent)
{
    memset(&app_data, 0, sizeof(app_ext_t));
    app_data.page_count = PAGE_COUNT;

    if(LOOP_MODE_ENABLE && PAGE_COUNT > 1) {
        app_data.total_positions = PAGE_COUNT + 2;
    } else {
        app_data.total_positions = PAGE_COUNT;
    }

    for(uint8_t i = 0; i < app_data.total_positions; i++) {
        valid_pos[i].x = i;
        valid_pos[i].y = 0;
    }

    lv_obj_t * tileview = lv_tileview_create(parent, NULL);
    lv_tileview_set_valid_positions(tileview, valid_pos, app_data.total_positions);
    lv_tileview_set_edge_flash(tileview, false);
    lv_tileview_set_anim_time(tileview, TILEVIEW_ANIM_TIME);
    lv_page_set_scrollbar_mode(tileview, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_local_bg_opa(tileview, LV_TILEVIEW_PART_BG, LV_STATE_DEFAULT, LV_OPA_COVER);
    lv_obj_set_style_local_bg_color(tileview, LV_TILEVIEW_PART_BG, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    /* Ensure scrollable child also has opaque bg (needed for 32-bit color with SDL alpha blending) */
    lv_obj_t * scrl = lv_page_get_scrollable(tileview);
    lv_obj_set_style_local_bg_opa(scrl, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_COVER);
    lv_obj_set_style_local_bg_color(scrl, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_event_cb(tileview, tileview_event_cb);

    ancestor_scrl_signal = lv_obj_get_signal_cb(lv_page_get_scrollable(tileview));
    lv_obj_set_signal_cb(lv_page_get_scrollable(tileview), tileview_scrl_signal_cb);

    if(LOOP_MODE_ENABLE && PAGE_COUNT > 1) {
        lv_obj_t * ghost0 = add_tile_page(tileview, 0);
        fill_page_content(ghost0, PAGE_COUNT - 1, tileview);

        for(uint8_t i = 0; i < PAGE_COUNT; i++) {
            lv_obj_t * page = add_tile_page(tileview, i + 1);
            fill_page_content(page, i, tileview);
        }

        lv_obj_t * ghost_last = add_tile_page(tileview, PAGE_COUNT + 1);
        fill_page_content(ghost_last, 0, tileview);

        lv_tileview_set_tile_act(tileview, 1, 0, LV_ANIM_OFF);
    } else {
        for(uint8_t i = 0; i < PAGE_COUNT; i++) {
            lv_obj_t * page = add_tile_page(tileview, i);
            fill_page_content(page, i, tileview);
        }
        lv_tileview_set_tile_act(tileview, 0, 0, LV_ANIM_OFF);
    }

    create_page_indicators(parent, &app_data);
    update_indicators(tileview, &app_data);
}

static lv_obj_t * add_tile_page(lv_obj_t * tileview, uint16_t index)
{
    lv_obj_t * cont = lv_cont_create(tileview, NULL);
    lv_obj_set_size(cont, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_pos(cont, LV_HOR_RES * index, 0);
    lv_cont_set_layout(cont, LV_LAYOUT_OFF);
    lv_obj_set_style_local_bg_opa(cont, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_COVER);
    lv_obj_set_style_local_bg_color(cont, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_style_local_border_opa(cont, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
    lv_tileview_add_element(tileview, cont);
    lv_obj_set_click(cont, false);
    return cont;
}

static void fill_page_content(lv_obj_t * page_cont, uint8_t page_id, lv_obj_t * tileview)
{
    static const lv_color_t header_colors[] = {
        LV_COLOR_MAKE(0x00, 0x96, 0x88),
        LV_COLOR_MAKE(0x3F, 0x51, 0xB5),
        LV_COLOR_MAKE(0xE9, 0x1E, 0x63),
    };

    lv_obj_t * header = lv_obj_create(page_cont, NULL);
    lv_obj_set_size(header, LV_HOR_RES, 50);
    lv_obj_set_pos(header, 0, 0);
    lv_obj_set_style_local_bg_color(header, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT,
                                    header_colors[page_id % 3]);
    lv_obj_set_style_local_border_opa(header, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
    lv_obj_set_style_local_radius(header, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_click(header, false);
    lv_tileview_add_element(tileview, header);

    lv_obj_t * title = lv_label_create(header, NULL);
    lv_obj_set_style_local_text_color(title, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    char buf[32];
    lv_snprintf(buf, sizeof(buf), "Page %d / %d", page_id + 1, PAGE_COUNT);
    lv_label_set_text(title, buf);
    lv_obj_align(title, header, LV_ALIGN_CENTER, 0, 0);

    switch(page_id) {
        case 0: {
            lv_obj_t * prompt = lv_label_create(page_cont, NULL);
            lv_label_set_text(prompt, "Select an item:");
            lv_obj_align(prompt, page_cont, LV_ALIGN_IN_TOP_MID, 0, 70);
            lv_tileview_add_element(tileview, prompt);

            lv_obj_t * dd = lv_dropdown_create(page_cont, NULL);
            lv_dropdown_set_options(dd,
                "Apple\n"
                "Banana\n"
                "Cherry\n"
                "Date\n"
                "Elderberry");
            lv_obj_set_width(dd, LV_HOR_RES * 2 / 3);
            lv_obj_align(dd, prompt, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
            lv_obj_set_event_cb(dd, dropdown_event_cb);
            lv_tileview_add_element(tileview, dd);

            lv_obj_t * result_lbl = lv_label_create(page_cont, NULL);
            lv_label_set_text(result_lbl, "Selected: Apple");
            lv_obj_align(result_lbl, dd, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);
            lv_obj_set_user_data(dd, (lv_obj_user_data_t)result_lbl);
            lv_tileview_add_element(tileview, result_lbl);

            lv_obj_t * hint = lv_label_create(page_cont, NULL);
            lv_obj_set_style_local_text_color(hint, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT,
                                              LV_COLOR_MAKE(0x90, 0x90, 0x90));
            lv_label_set_text(hint, "Swipe left/right to cycle pages");
            lv_label_set_align(hint, LV_LABEL_ALIGN_CENTER);
            lv_obj_align(hint, page_cont, LV_ALIGN_IN_BOTTOM_MID, 0, -35);
            lv_tileview_add_element(tileview, hint);
            break;
        }
        case 1: {
            lv_obj_t * btn = lv_btn_create(page_cont, NULL);
            lv_obj_set_size(btn, 160, 50);
            lv_obj_align(btn, page_cont, LV_ALIGN_CENTER, 0, -30);
            lv_tileview_add_element(tileview, btn);

            lv_obj_t * btn_lbl = lv_label_create(btn, NULL);
            lv_label_set_text(btn_lbl, "Click Me");

            lv_obj_t * sw = lv_switch_create(page_cont, NULL);
            lv_obj_align(sw, btn, LV_ALIGN_OUT_BOTTOM_MID, 0, 25);
            lv_tileview_add_element(tileview, sw);

            lv_obj_t * sw_lbl = lv_label_create(page_cont, NULL);
            lv_label_set_text(sw_lbl, "Toggle Switch");
            lv_obj_align(sw_lbl, sw, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
            lv_tileview_add_element(tileview, sw_lbl);
            break;
        }
        case 2: {
            lv_obj_t * spinner = lv_spinner_create(page_cont, NULL);
            lv_obj_set_size(spinner, 80, 80);
            lv_obj_align(spinner, page_cont, LV_ALIGN_CENTER, 0, -30);

            lv_obj_t * lbl = lv_label_create(page_cont, NULL);
            lv_label_set_text(lbl, "Loading...");
            lv_obj_align(lbl, spinner, LV_ALIGN_OUT_BOTTOM_MID, 0, 15);
            lv_tileview_add_element(tileview, lbl);

            lv_obj_t * bar = lv_bar_create(page_cont, NULL);
            lv_obj_set_size(bar, LV_HOR_RES * 2 / 3, 15);
            lv_bar_set_value(bar, 65, LV_ANIM_ON);
            lv_obj_align(bar, lbl, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);
            lv_tileview_add_element(tileview, bar);
            break;
        }
        default:
            break;
    }
}

static void create_page_indicators(lv_obj_t * parent, app_ext_t * app)
{
    app->led_container = lv_cont_create(parent, NULL);
    lv_cont_set_fit(app->led_container, LV_FIT_TIGHT);
    lv_cont_set_layout(app->led_container, LV_LAYOUT_ROW_MID);
    lv_obj_set_style_local_bg_opa(app->led_container, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
    lv_obj_set_style_local_border_opa(app->led_container, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
    lv_obj_set_style_local_pad_inner(app->led_container, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, LED_INDICATOR_PAD);
    lv_obj_set_click(app->led_container, false);

    for(uint8_t i = 0; i < app->page_count; i++) {
        app->leds[i] = lv_led_create(app->led_container, NULL);
        lv_obj_set_size(app->leds[i], LED_OFF_SIZE, LED_OFF_SIZE);
        lv_led_off(app->leds[i]);
    }

    lv_obj_align(app->led_container, parent, LV_ALIGN_IN_BOTTOM_MID, 0, -LED_BOTTOM_OFFSET);
}

static void update_indicators(lv_obj_t * tileview, app_ext_t * app)
{
    lv_tileview_ext_t * ext = lv_obj_get_ext_attr(tileview);
    uint16_t act_x = ext->act_id.x;
    uint8_t page_idx;

    if(LOOP_MODE_ENABLE && PAGE_COUNT > 1) {
        if(act_x == 0)
            page_idx = PAGE_COUNT - 1;
        else if(act_x == app->total_positions - 1)
            page_idx = 0;
        else
            page_idx = act_x - 1;
    } else {
        page_idx = act_x;
    }

    for(uint8_t i = 0; i < app->page_count; i++) {
        if(i == page_idx) {
            lv_led_on(app->leds[i]);
            lv_obj_set_size(app->leds[i], LED_ON_SIZE, LED_ON_SIZE);
        } else {
            lv_led_off(app->leds[i]);
            lv_obj_set_size(app->leds[i], LED_OFF_SIZE, LED_OFF_SIZE);
        }
    }

    lv_obj_align(app->led_container, lv_obj_get_parent(app->led_container),
                 LV_ALIGN_IN_BOTTOM_MID, 0, -LED_BOTTOM_OFFSET);
}

static void tileview_event_cb(lv_obj_t * tileview, lv_event_t e)
{
    if(LV_EVENT_VALUE_CHANGED == e) {
        update_indicators(tileview, &app_data);
    }
}

/**
 * Signal handler for the tileview scrollable to implement infinite loop.
 * Only intercepts PRESSING/COORD_CHG signals; all others pass through
 * directly to avoid infinite recursion from LV_ASSERT_OBJ type checks.
 */
static lv_res_t tileview_scrl_signal_cb(lv_obj_t * scrl, lv_signal_t sign, void * param)
{
    if(sign != LV_SIGNAL_PRESSING && sign != LV_SIGNAL_COORD_CHG) {
        return ancestor_scrl_signal(scrl, sign, param);
    }

    lv_obj_t * tileview = scrl->parent;
    lv_tileview_ext_t * ext = lv_obj_get_ext_attr(tileview);

    ancestor_scrl_signal(scrl, sign, param);

    if(!LOOP_MODE_ENABLE || PAGE_COUNT <= 1) return LV_RES_OK;

    lv_indev_t * indev = lv_indev_get_act();
    if(indev == NULL) return LV_RES_OK;

    lv_drag_dir_t drag_dir = indev->proc.types.pointer.drag_dir;
    if(drag_dir & LV_DRAG_DIR_HOR) {
        lv_coord_t tile_w = lv_obj_get_width(tileview);
        lv_coord_t x_act = lv_obj_get_x(scrl);

        if(ext->act_id.x == 0) {
            if(x_act == 0 && indev->proc.types.pointer.vect.x > 0) {
                lv_tileview_set_tile_act(tileview,
                    ext->valid_pos[ext->valid_pos_cnt - 2].x, 0, false);
            }
        }
        else if(ext->act_id.x == ext->valid_pos[ext->valid_pos_cnt - 1].x) {
            if((ext->act_id.x * (-tile_w) == x_act) && indev->proc.types.pointer.vect.x < 0) {
                lv_tileview_set_tile_act(tileview, 1, 0, false);
            }
        }
    }

    return LV_RES_OK;
}

static void dropdown_event_cb(lv_obj_t * dropdown, lv_event_t e)
{
    if(e == LV_EVENT_VALUE_CHANGED) {
        char sel_buf[32];
        lv_dropdown_get_selected_str(dropdown, sel_buf, sizeof(sel_buf));

        lv_obj_t * result_lbl = (lv_obj_t *)lv_obj_get_user_data(dropdown);
        if(result_lbl) {
            char text_buf[64];
            lv_snprintf(text_buf, sizeof(text_buf), "Selected: %s", sel_buf);
            lv_label_set_text(result_lbl, text_buf);
        }
        printf("Dropdown selected: %s\n", sel_buf);
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
