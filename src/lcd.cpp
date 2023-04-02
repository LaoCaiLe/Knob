#include "lcd.h"
#include "lvgl.h"

static lv_obj_t *bg;
static lv_obj_t *color_bg;
static lv_obj_t *text_obj;
static lv_obj_t *arc;
static lv_obj_t *arc_beyond;

#define MY_DISP_HOR_RES   240
#define MY_DISP_VER_RES   240

TFT_eSPI tft = TFT_eSPI(MY_DISP_HOR_RES, MY_DISP_VER_RES);

void Screen::task_tft(void)
{
	static bool is_init = false;
	if(!is_init)
	{
		lv_knob_test();
		is_init = true;
	}

	int show_angle;
	float angle_persent = (motion.real_angle - motion.m_min_angle) / (motion.m_max_angle - motion.m_min_angle);
    lv_arc_set_angles(arc, 0, motion.real_angle);

	lv_label_set_text_fmt(text_obj, "%d", (int)(100*angle_persent));
	show_angle = 240*angle_persent;
	// 
	if(motion.real_angle>=motion.m_max_angle)
	{
        if(lv_obj_get_y(color_bg)!=0)
		    lv_obj_set_y(color_bg, 0);
		lv_arc_set_angles(arc_beyond, motion.m_max_angle, motion.real_angle);


	}
	else if(motion.real_angle<=motion.m_min_angle)
	{
        if(lv_obj_get_y(color_bg)!=240)
		    lv_obj_set_y(color_bg, 240);
		lv_arc_set_angles(arc_beyond, 360+motion.real_angle, 360+motion.m_min_angle);

	}
	else
	{
        if(lv_obj_get_y(color_bg)!=240-show_angle)
            lv_obj_set_y(color_bg, 240-show_angle);
		lv_arc_set_angles(arc_beyond, motion.real_angle, motion.real_angle);
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

	arc_beyond = lv_arc_create(bg);
    lv_obj_set_size(arc_beyond, 225, 225);
    lv_obj_center(arc_beyond);
	lv_arc_set_rotation(arc_beyond, 270);
	lv_obj_remove_style(arc_beyond, NULL, LV_PART_KNOB);

    arc = lv_arc_create(bg);
    lv_obj_set_size(arc, 230, 230);
    lv_obj_center(arc);
	lv_arc_set_rotation(arc, 270);

	static lv_style_t hide_style;
    lv_style_init(&hide_style);
    lv_style_set_arc_opa(&hide_style, LV_OPA_TRANSP);

	static lv_style_t show_style;
	lv_style_init(&show_style);
    lv_style_set_arc_opa(&show_style, LV_OPA_COVER);
	lv_style_set_arc_color(&show_style, lv_palette_main(LV_PALETTE_RED));
	lv_style_set_arc_width(&show_style, 4);

	static lv_style_t knob_style;
    lv_style_init(&knob_style);
    lv_style_set_bg_color(&knob_style, lv_palette_main(LV_PALETTE_ORANGE));
	lv_style_set_pad_all(&knob_style, -2);

	lv_obj_add_style(arc, &hide_style, LV_PART_MAIN);         
	lv_obj_add_style(arc, &hide_style, LV_PART_INDICATOR);
	lv_obj_add_style(arc, &knob_style, LV_PART_KNOB);

	lv_obj_add_style(arc_beyond, &hide_style, LV_PART_MAIN);         
	lv_obj_add_style(arc_beyond, &show_style, LV_PART_INDICATOR);

	static lv_obj_t* max_limit_line= lv_line_create(arc);
	static lv_obj_t* min_limit_line= lv_line_create(arc);
	static lv_point_t max_line_points[2];
	static lv_point_t min_line_points[2];

	max_line_points[0].x = 115 + sin(motion.m_max_angle * PI / 180 ) * 110;
    max_line_points[0].y = 115 - cos(motion.m_max_angle * PI / 180 ) * 110;
    max_line_points[1].x = 115 + sin(motion.m_max_angle * PI / 180 ) * 120;
    max_line_points[1].y = 115 - cos(motion.m_max_angle * PI / 180 ) * 120;
	
	min_line_points[0].x = 115 + sin(motion.m_min_angle * PI / 180 ) * 120;
    min_line_points[0].y = 115 - cos(motion.m_min_angle * PI / 180 ) * 120;
    min_line_points[1].x = 115 + sin(motion.m_min_angle * PI / 180 ) * 110;
    min_line_points[1].y = 115 - cos(motion.m_min_angle * PI / 180 ) * 110;

    lv_line_set_points(max_limit_line, max_line_points, 2);    
    lv_obj_set_style_line_width(max_limit_line, 2, LV_PART_MAIN);
    lv_obj_set_style_line_rounded(max_limit_line, 1, LV_PART_MAIN);
	lv_obj_set_style_line_color(max_limit_line, lv_color_white(), 0);

	lv_line_set_points(min_limit_line, min_line_points, 2);    
    lv_obj_set_style_line_width(min_limit_line, 2, LV_PART_MAIN);
    lv_obj_set_style_line_rounded(min_limit_line, 1, LV_PART_MAIN);
	lv_obj_set_style_line_color(min_limit_line, lv_color_white(), 0);
}