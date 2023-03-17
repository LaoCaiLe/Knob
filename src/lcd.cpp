#include "lcd.h"
#include "lvgl.h"

static lv_obj_t *bg;
static lv_obj_t *color_bg;
static lv_obj_t *text_obj;
static lv_obj_t *arc;
#define MY_DISP_HOR_RES   240
#define MY_DISP_VER_RES   240

TFT_eSPI tft = TFT_eSPI(MY_DISP_HOR_RES, MY_DISP_VER_RES);

void Screen::task_tft(void)
{
  	char buf[10];
	static int last_angle = 1;
	int show_angle;
	// while(1)
	{
		// sensor.update();
		// float now_radian = sensor.getSensorAngle();

		// if(motion.real_angle != last_angle)
		{
			lv_arc_set_angles(arc, 0, motion.real_angle);     //设置前景圆弧角度
			sprintf(buf, "%d", motion.real_angle);
			lv_label_set_text(text_obj, buf);
			show_angle = 240*(motion.real_angle-motion.m_min_angle) / (motion.m_max_angle-motion.m_min_angle);
			if(motion.real_angle<=motion.m_max_angle)
				lv_obj_set_y(color_bg, 240-show_angle);
		
			last_angle = motion.real_angle;
		}

		// vTaskDelay(10);

	}
}

static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors(&color_p->full, w * h, true);
    tft.endWrite();

    lv_disp_flush_ready(disp_drv);
}

void Screen::init(void)
{
	lv_init();

	tft.init();
	tft.fillScreen(TFT_BLACK);
	tft.setRotation(0);
	tft.initDMA();

	/*-----------------------------
     * Create a buffer for drawing
     *----------------------------*/
    static lv_disp_draw_buf_t draw_buf_dsc;
    static lv_color_t buf_1[MY_DISP_HOR_RES * 10];  /*A buffer for 10 rows*/
    static lv_color_t buf_2[MY_DISP_HOR_RES * 10];  /*B buffer for 10 rows*/
    lv_disp_draw_buf_init(&draw_buf_dsc, buf_1, buf_2, MY_DISP_HOR_RES * 10);   /*Initialize the display buffer*/

    /*-----------------------------------
     * Register the display in LVGL
     *----------------------------------*/
    static lv_disp_drv_t disp_drv;                  /*Descriptor of a display driver*/
    lv_disp_drv_init(&disp_drv);                    /*Basic initialization*/

    /*Set up the functions to access to your display*/

    /*Set the resolution of the display*/
    disp_drv.hor_res = MY_DISP_HOR_RES;
    disp_drv.ver_res = MY_DISP_VER_RES;

    /*Used to copy the buffer's content to the display*/
    disp_drv.flush_cb = disp_flush;

    /*Set a display buffer*/
    disp_drv.draw_buf = &draw_buf_dsc;

    /*Finally register the driver*/
    lv_disp_drv_register(&disp_drv);

	lv_knob_test();
}

void lv_knob_test()
{
    bg = lv_obj_create(lv_scr_act());
    lv_obj_set_size(bg, 240, 240);
    lv_obj_center(bg);
    lv_obj_clear_flag(bg, LV_OBJ_FLAG_SCROLLABLE);
    

    color_bg = lv_obj_create(bg);
    lv_obj_set_size(color_bg, 240, 240);
    lv_obj_center(color_bg);
    lv_obj_set_style_bg_color(color_bg, lv_palette_main(LV_PALETTE_DEEP_PURPLE), LV_PART_MAIN);

    text_obj = lv_label_create(bg);
    lv_obj_set_style_text_font(text_obj, &lv_font_montserrat_48, 0);
    lv_obj_center(text_obj);

	arc = lv_arc_create(bg);
    lv_obj_set_size(arc, 220, 220);
    lv_obj_center(arc);
	lv_arc_set_rotation(arc, -90);

	static lv_style_t bg_style;
    lv_style_init(&bg_style);
    lv_style_set_arc_opa(&bg_style, LV_OPA_TRANSP);

	static lv_style_t fg_style;
    lv_style_init(&fg_style);
    // lv_style_set_color(&fg_style, lv_palette_main(LV_PALETTE_ORANGE));
    // lv_style_set_arc_rounded(&fg_style, 1);
    // lv_style_set_arc_width(&fg_style, 10);
	lv_style_set_arc_width(&fg_style, 4);


	lv_obj_add_style(arc, &bg_style, LV_PART_INDICATOR);    //添加前景style -> LV_PART_INDICATOR
    lv_obj_add_style(arc, &bg_style, LV_PART_MAIN);         //添加背景style -> LV_PART_MAIN
	// lv_obj_add_style(arc, &fg_style, LV_PART_KNOB); 

}