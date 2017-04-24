#ifndef __GIFLOAD_H__
#define __GIFLOAD_H__

#include "stddefs.h"
#include "stdio.h"
#include "sttbx.h"
#include "driver.h"

typedef unsigned char   guchar;
typedef void* gpointer;
typedef unsigned int    guint;
typedef int    gint;
typedef gint   gboolean;
typedef size_t gsize;

#ifndef	FALSE
#define	FALSE	(0)
#endif

#ifndef	TRUE
#define	TRUE	(!FALSE)
#endif

//#define USE_GIF_BK_COLOR
#define g_message    Debug_Print
#define MESSAGE_LEVEL PRINT_LEVEL_ALL

#define g_new(x,y) (x*)(gif_allocate(pPartition,(y)*sizeof(x)))
#define g_new0(x,y) (x*)(gif_allocate(pPartition,(y)*sizeof(x)))
#define g_free(x)       gif_deallocate(pPartition,(x))

#ifndef MIN
#define MIN(a, b)  (((a) < (b)) ? (a) : (b))
#endif

#ifndef MAX
#define MAX(a, b)  (((a) > (b)) ? (a) : (b))
#endif

#define g_return_val_if_fail(x,y) if(!(x)) return (y)


/* Color spaces; right now only RGB is supported */
typedef enum {
	GDK_COLORSPACE_RGB
} GdkColorspace;

/* All of these are opaque structures */
typedef struct _GdkPixbuf GdkPixbuf;




/* Handler that must free the pixel array */
typedef void (* GdkPixbufDestroyNotify) (DRVOS_Partition_p pPartition,guchar *pixels, gpointer data);

typedef void (* GdkPixbufLastUnref) (GdkPixbuf *pixbuf, gpointer data);
typedef struct _Animation GdkPixbufAnimation;

typedef struct _GdkPixbufFrame GdkPixbufFrame;


/* GIF-like animation overlay modes for frames */
typedef enum {
	GDK_PIXBUF_FRAME_RETAIN,
	GDK_PIXBUF_FRAME_DISPOSE,
	GDK_PIXBUF_FRAME_REVERT
} GdkPixbufFrameAction;

typedef struct _List List;

struct _List
{
  void* data;
  List *next;
  List *prev;
};

typedef void            (*GFunc)                (gpointer       data,
                                                 gpointer       user_data);


#define MAXCOLORMAPSIZE  256
#define MAX_LZW_BITS     12

typedef unsigned char CMap[3][MAXCOLORMAPSIZE];

typedef struct _Animation {
	/* Reference count */
	int ref_count;

	/* Number of frames */
        int n_frames;

	/* List of Frame structures */
        List *frames;

	/* bounding box size */
	int width, height;
       /* the palette shared by all frames */
      CMap color_map;
} Animation;



/* Private part of the GdkPixbufFrame structure */
struct _GdkPixbufFrame {
	/* The pixbuf with this frame's image data */
	GdkPixbuf *pixbuf;

	/* Offsets for overlaying onto the animation's area */
	int x_offset;
	int y_offset;

	/* Frame duration in ms */
	int delay_time;

	/* Overlay mode */
	GdkPixbufFrameAction action;
};

typedef void (* ModulePreparedNotifyFunc) (GdkPixbuf *pixbuf, gpointer user_data);
typedef void (* ModuleUpdatedNotifyFunc) (GdkPixbuf *pixbuf,
					  guint x, guint y,
					  guint width, guint height,
					  gpointer user_data);
/* Needed only for animated images. */
typedef void (* ModuleFrameDoneNotifyFunc) (GdkPixbufFrame *frame,
					    gpointer user_data);
typedef void (* ModuleAnimationDoneNotifyFunc) (GdkPixbuf *pixbuf,
						gpointer user_data);


/* Private part of the GdkPixbuf structure */
struct _GdkPixbuf {
	/* Reference count */
	int ref_count;

	/* Color space */
	GdkColorspace colorspace;

	/* Number of channels, alpha included */
	int n_channels;

	/* Bits per channel */
	int bpp;

	/* Size */
	int width, height;

	/* Offset between rows */
	int rowstride;//pitch

	/* The pixel array */
	unsigned char *pixels;//data

	/* Destroy notification function; it is supposed to free the pixel array */
	GdkPixbufDestroyNotify destroy_fn;

	/* User data for the destroy notification function */
	gpointer destroy_fn_data;

	/* Last unref handler, determines whether the pixbuf should be finalized */
	GdkPixbufLastUnref last_unref_fn;

	/* User data for the last unref handler */
	gpointer last_unref_fn_data;

	/* Do we have an alpha channel? */
	guint has_alpha : 1;

       DRVOS_Partition_p pPartition;
    
};

typedef struct _Gif89 Gif89;
struct _Gif89
{
	int transparent;
	int delay_time;
	int input_flag;
	int disposal;
};

/* Possible states we can be in. */
enum {
	GIF_START,
	GIF_GET_COLORMAP,
	GIF_GET_NEXT_STEP,
	GIF_GET_FRAME_INFO,
	GIF_GET_EXTENTION,
	GIF_GET_COLORMAP2,
	GIF_PREPARE_LZW,
	GIF_LZW_FILL_BUFFER,
	GIF_LZW_CLEAR_CODE,
	GIF_GET_LZW,
	GIF_DONE,
	GIF_FILE_END,
	GIF_DATA_INVALID/**/
};


typedef struct _GifContext GifContext;

struct _GifContext
{
	int state; /* really only relevant for progressive loading */
       DRVOS_Sem_t *stateMutex;
       DRVOS_Partition_p pPartition;
       OSD_ColorType_t colorType;
       int bpp;
	unsigned int width;
	unsigned int height;
	unsigned int CreateWidth;
	unsigned int CreateHeight;
	CMap color_map;
	unsigned int bit_pixel;
	unsigned int color_resolution;
	unsigned int background;
	unsigned int aspect_ratio;
	GdkPixbuf *pixbuf;
	GdkPixbufAnimation *animation;
	GdkPixbufFrame *frame;
	Gif89 gif89;

	/* stuff per frame.  As we only support the first one, not so
	 * relevant.  But still needed */
	int frame_len;
	int frame_height;
	int frame_interlace;
	int x_offset;
	int y_offset;

	/* Static read only */
	FILE *file;

	/* progressive read, only. */
	ModulePreparedNotifyFunc prepare_func;
	ModuleUpdatedNotifyFunc update_func;
	ModuleFrameDoneNotifyFunc frame_done_func;
	ModuleAnimationDoneNotifyFunc anim_done_func;
	gpointer user_data;
	guchar *buf;
	guint ptr;
	guint size;
	guint amount_needed;

	/* colormap context */
	gint colormap_index;
	gint colormap_flag;

	/* extension context */
	guchar extension_label;
	guchar extension_flag;

	/* get block context */
	guchar block_count;
	guchar block_buf[280];
	gint block_ptr;

	int old_state; /* used by lzw_fill buffer */
	/* get_code context */
	int code_curbit;
	int code_lastbit;
	int code_done;
	int code_last_byte;
	int lzw_code_pending;

	/* lzw context */
	gint lzw_fresh;
	gint lzw_code_size;
	guchar lzw_set_code_size;
	gint lzw_max_code;
	gint lzw_max_code_size;
	gint lzw_firstcode;
	gint lzw_oldcode;
	gint lzw_clear_code;
	gint lzw_end_code;
	gint *lzw_sp;

	gint lzw_table[2][(1 << MAX_LZW_BITS)];
	gint lzw_stack[(1 << (MAX_LZW_BITS)) * 2 + 1];

	/* painting context */
	gint draw_xpos;
	gint draw_ypos;
	gint draw_pass;
};


/*****************************************************************************
  Date&Time    	  : 2012-10-19
  Author          :   wwqing
  Function Name   : gif_allocate
  Description     : gif模块分配内存
  Input           : partition, size
  Outut           :  
  Calls           : 
  Others          : partition未指定(NULL)时，使用SystemPartition
  					gif_allocate_count，记录分配次数，最终需要与gif_deallocate_count一致
*****************************************************************************/ 
void* gif_allocate(DRVOS_Partition_p partition,int size);

/*****************************************************************************
  Date&Time    	  : 2012-10-19
  Author          :   wwqing
  Function Name   : gif_deallocate
  Description     : gif模块释放内存
  Input           : partition, size
  Outut           :  
  Calls           : 
  Others          : partition未指定(NULL)时，使用SystemPartition
  					gif_deallocate_count，记录释放次数，最终需要与gif_allocate_count一致
*****************************************************************************/ 
void gif_deallocate(DRVOS_Partition_p partition,void* block);

/*****************************************************************************
  Date&Time    	  : 2012-10-19
  Author          :   wwqing
  Function Name   : GetGifSize
  Description     : gif模块获取gif的宽高
  Input           : 
  Outut           :  
  Calls           : 
  Others          : 可参看gif头部数据结构
*****************************************************************************/ 
gint  GetGifSize(GifContext *context);

gint gif_get_next_step (GifContext *context);
gint gif_check(GifContext* context);
guint get_bk_color(GifContext* context);
gint gif_main_loop (GifContext *context);
GifContext *new_context (DRVOS_Partition_p pPartition);

#endif

