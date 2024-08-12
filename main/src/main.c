
/**
 * @file main
 *
 */

/*********************
 *      INCLUDES
 *********************/
#define _DEFAULT_SOURCE /* needed for usleep() */
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include "lvgl/lvgl.h"
#include "lvgl/examples/lv_examples.h"
#include "lvgl/demos/lv_demos.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static lv_display_t * hal_init(int32_t w, int32_t h);
void typing_animation_timer(lv_timer_t *timer);
/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *      VARIABLES
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

int main(int argc, char **argv)
{
  (void)argc; /*Unused*/
  (void)argv; /*Unused*/

  /*Initialize LVGL*/
  lv_init();

  /*Initialize the HAL (display, input devices, tick) for LVGL*/
  hal_init(480, 272);

  while(1) {
    /* Periodically call the lv_task handler.
     * It could be done in a timer interrupt or an OS task too.*/
    lv_timer_handler();
    lv_task_handler();
    usleep(5 * 1000);
  }

  return 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**
 * Initialize the Hardware Abstraction Layer (HAL) for the LVGL graphics
 * library
 */
static int text_pos = 0;

const char *text = "The missile knows where it is at all times. It knows this because it knows where it isn't. By subtracting where it is from where it isn't, or where it isn't from where it is (whichever is greater), it obtains a difference, or deviation. The guidance subsystem uses deviations to generate corrective commands to drive the missile from a position where it is to a position where it isn't, and arriving at a position where it wasn't, it now is. Consequently, the position where it is, is now the position that it wasn't, and it follows that the position that it was, is now the position that it isn't. In the event that the position that it is in is not the position that it wasn't, the system has acquired a variation, the variation being the difference between where the missile is, and where it wasn't. If variation is considered to be a significant factor, it too may be corrected by the GEA. However, the missile must also know where it was. The missile guidance computer scenario works as follows. Because a variation has modified some of the information the missile has obtained, it is not sure just where it is. However, it is sure where it isn't, within reason, and it knows where it was. It now subtracts where it should be from where it wasn't, or vice-versa, and by differentiating this from the algebraic sum of where it shouldn't be, and where it was, it is able to obtain the deviation and its variation, which is called error.";

static lv_display_t * hal_init(int32_t w, int32_t h)
{

  lv_group_set_default(lv_group_create());

  lv_display_t * disp = lv_sdl_window_create(w, h);

  lv_indev_t * mouse = lv_sdl_mouse_create();
  lv_indev_set_group(mouse, lv_group_get_default());
  lv_indev_set_display(mouse, disp);
  lv_display_set_default(disp);

    /*Change the active screen's background color*/
  lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0x000000), LV_PART_MAIN);
  lv_obj_t * text_area = lv_textarea_create(lv_scr_act());
  lv_obj_set_size(text_area, w, h);
  lv_obj_set_style_bg_color(text_area, lv_color_hex(0x000000), LV_PART_MAIN);
  lv_obj_set_style_text_color(text_area, lv_color_hex(0xFFFFFF), LV_PART_MAIN);

  /* Create a style for the text cursor */
  static lv_style_t style_cursor;
  lv_style_init(&style_cursor);
  lv_style_set_text_color(&style_cursor, lv_color_white());  // Set cursor color to white

    /* Apply the cursor style to the text area */
  lv_obj_add_style(text_area, &style_cursor, LV_PART_CURSOR | LV_STATE_FOCUSED);


  lv_obj_align(text_area,LV_ALIGN_CENTER, 0, 0);
  lv_textarea_set_text(text_area, "");
  lv_timer_create(typing_animation_timer, 20, text_area);

  return disp;
}
void typing_animation_timer(lv_timer_t *timer) {
      lv_obj_t *text_area = (lv_obj_t *)lv_timer_get_user_data(timer);

      char current_text[2560];
      strncpy(current_text, text, text_pos); // Copy text up to text_pos
      current_text[text_pos] = '\0'; // Null-terminate the string

      lv_textarea_set_text(text_area, current_text);
      text_pos++;
      if (text[text_pos] == '\0') {
        text_pos = 0;
        lv_timer_del(timer); // Stop the timer when done
    }
}
