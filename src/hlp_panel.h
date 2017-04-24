/*
 * Copyright (C) ychen
 * Copyright (C) Jiuzhou, Inc.
 */
 
#ifndef __HLP_PANEL_H__
#define __HLP_PANEL_H__


#include <hlp_config.h>
#include <hlp_core.h>

#define HLP_KEY_NULL               0xff 
#define HLP_PANEL_KEY_UP           83
#define HLP_PANEL_KEY_DOWN         81
#define HLP_PANEL_KEY_LEFT         82
#define HLP_PANEL_KEY_RIGHT        85
#define HLP_PANEL_KEY_SELECT       87
#define HLP_PANEL_KEY_EXIT         84

#define HLP_MENU_CATEGORY_NUM      3
#define HLP_MENU_ITEM_NAME_LEN_MAX 10

#define MENU_SHOW_STRING           0
#define MENU_SHOW_NU           0

typedef struct hlp_panel_menu_item      hlp_panel_menu_item_t;
typedef struct hlp_panel_menu_category  hlp_panel_menu_category_t;
typedef struct hlp_panel_menu           hlp_panel_menu_t;

typedef void (*hlp_panel_menu_key_callback)(hlp_panel_menu_t *menu, hlp_u8_t key);
typedef void (*hlp_panel_menu_enter_callback)(hlp_panel_menu_t *menu, hlp_u8_t key);

typedef enum
{
	HLP_PANEL_MENU_ITEM,
	HLP_PANEL_MENU_MAIN = 0x11,
	HLP_PANEL_MENU_CATEGORY,
}hlp_panel_menu_type;

typedef enum
{
	ITEM_SHOW_STRING = 0,
	ITEM_SHOW_NUMBER,
	ITEM_SHOW_TIME,
}hlp_panel_item_show_type;

struct hlp_panel_menu_item
{
	hlp_u8_t        name[HLP_MENU_ITEM_NAME_LEN_MAX];
	hlp_u8_t        lever;
	hlp_u8_t        index;
	hlp_u8_t        children_num;
	hlp_panel_menu_type            type;
	hlp_panel_item_show_type       show_type;
	hlp_panel_menu_item_t         *parent;
	hlp_panel_menu_item_t         *prev;
	hlp_panel_menu_item_t         *next;
	hlp_panel_menu_item_t         *chead;
	hlp_panel_menu_item_t         *ctail;
	hlp_panel_menu_key_callback    key_callback;
	hlp_panel_menu_enter_callback  enter_callback;
};

struct hlp_panel_menu_category
{
	hlp_u8_t        name[HLP_MENU_ITEM_NAME_LEN_MAX];
	hlp_u8_t        item_cnt;
	hlp_u8_t        index;
	hlp_panel_menu_t           *parent;
	hlp_panel_menu_category_t  *prev;
	hlp_panel_menu_category_t  *next;
	hlp_panel_menu_item_t      *items;
};

struct hlp_panel_menu
{
	hlp_u8_t        name[HLP_MENU_ITEM_NAME_LEN_MAX];
	hlp_u8_t        item_cnt;
	hlp_u8_t        current_lever;
	hlp_u8_t        current_index;
	hlp_panel_menu_item_t *main_item;
	hlp_panel_menu_item_t *current_item;
};

void hlp_panel_set_key(hlp_u8_t value);
void panel_enter_main_menu();
void panel_show_main_menu();
void panel_led_show_string(char *string);
void panel_led_show_number(hlp_u16_t number);
int hlp_panel_task_start();

#if 0
void DRV_LED_DisplayTime(U32 hour, U32 min);
void DRV_LED_UnDisplayTime();
void DRV_LED_DisplayChar(U8 d1,U8 d2,U8 d3,U8 d4);
#endif

#endif /* __HLP_PANEL_H__ */

