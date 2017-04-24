
#ifndef _RATEBAR_H_
#define _RATEBAR_H_

#include	"ctrlop.h"
//#include	"text8.h"
#include	"../driver/ysosd/ysosd.h"
#include 	"midware.h"
//#include "../ui/ui.h"//ÑÕÉ«´úÂë


void RateBarInit (void);


void RateBarCircle (Rect_t *prect, int CircleRadius, COLOR crFgSet, COLOR crBgSet);


void RateBarText (Rect_t *prect, char *pText, UI16 nFontSize,
					UI16 nFontStyle,int nStyle,COLOR crFgSet, COLOR crBgSet);

#endif

