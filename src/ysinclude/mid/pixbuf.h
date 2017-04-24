#ifndef __GDKK_PIXBUF_IO_H__
#define __GDKK_PIXBUF_IO_H__

#include "gif.h"
#include "gifload.h"

guchar *
gdk_pixbuf_get_pixels (const GdkPixbuf *pixbuf);
int
gdk_pixbuf_get_width (const GdkPixbuf *pixbuf);
int
gdk_pixbuf_get_height (const GdkPixbuf *pixbuf);
int
gdk_pixbuf_get_rowstride (const GdkPixbuf *pixbuf);
List*
g_list_append (DRVOS_Partition_p pParttition,List	*list,
	       gpointer	 data);


GdkPixbuf *gdk_pixbuf_new (DRVOS_Partition_p pPartition,GdkColorspace colorspace, gboolean has_alpha, int bits_per_sample,int width, int height);

void gdk_pixbuf_unref (GdkPixbuf *pixbuf);   
#endif
