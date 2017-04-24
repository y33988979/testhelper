/*****************************************************************************/
/* COPYRIGHT (C) 2009 STMicroelectronics - All Rights Reserved               */
/* ST makes no warranty express or implied including but not limited to,     */
/* any warranty of                                                           */
/*                                                                           */
/*   (i)  merchantability or fitness for a particular purpose and/or         */
/*   (ii) requirements, for a particular purpose in relation to the LICENSED */
/*        MATERIALS, which is provided AS IS, WITH ALL FAULTS. ST does not   */
/*        represent or warrant that the LICENSED MATERIALS provided here     */
/*        under is free of infringement of any third party patents,          */
/*        copyrights, trade secrets or other intellectual property rights.   */
/*        ALL WARRANTIES, CONDITIONS OR OTHER TERMS IMPLIED BY LAW ARE       */
/*        EXCLUDED TO THE FULLEST EXTENT PERMITTED BY LAW                    */
/*                                                                           */
/*****************************************************************************/
/**
 * @file     clilib.h
 * @brief    This is the command line interpreter file header.
 * @author   STMicroelectronics
 */

#ifndef _CLILIB_H_
#define _CLILIB_H_

/* C++ support */
/* ----------- */
#ifdef __cplusplus
extern "C" {
#endif
#include <testtool.h>
/* Max token and line size */
/* ----------------------- */
#define MAX_TOK_LEN  512
#define MAX_LINE_LEN 512
#define NO_TOKEN    0

/* List of special chars */
/* --------------------- */
#define SPACE_CHAR  0x20
#define TAB_CHAR    0x09
#define ESCAPE_CHAR '\\'
#define NL_CHAR     '\n'
#define CR_CHAR     '\r'
#define COMMENT_CHAR ';'
#define BAD_INT_VAL  (S32) 0x98738216

/* List of conditions */
/* ------------------ */
enum
{
 NO_CONST,
 DEFINE_CONST,
 IF_CONST,
 ELIF_CONST,
 ELSE_CONST,
 WHILE_CONST,
 FOR_CONST
};

/* Symbol table data structure and types */
/* ------------------------------------- */
#define NO_SYMBOL  0    /* No symbol                */
#define INT_SYMBOL 1    /* Integer symbol           */
#define FLT_SYMBOL 2    /* Floating point symbol    */
#define STR_SYMBOL 4    /* String symbol            */
#define COM_SYMBOL 8    /* Command symbol           */
#define MAC_SYMBOL 16   /* Macro symbol             */
#define ANY_SYMBOL 0xff /* Matches all symbol types */


/* Macro store structure */
/* --------------------- */
struct macro
{
 struct macro *line_p;
 char          line[MAX_LINE_LEN+1];
};
typedef struct macro macro_t;

/* Symbol data structure */
/* --------------------- */
struct symtab
{
 char *name_p;     /* Symbol id                               */
 S16   type;       /* Type of symbol                          */
 S16   name_len;   /* Length of symbol name                   */
 union
  {
   S32       int_val;
   double    flt_val;
   char     *str_val;
   BOOL    (*com_val)(parse_t *, char *);
   macro_t  *mac_val;
  }
 value;            /* Value of symbol                         */
 BOOL  fixed;      /* Flag for symbol                         */
 S16   depth;      /* Nesting depth at which declaration made */
 char *info_p;     /* Informational string                    */
};
typedef struct symtab symtab_t;

/* Global variables */
/* ---------------- */
extern symtab_t **symtab_p;   /* Symbol table pointer       */
extern S32        symbol_cnt; /* Number of symbols declared */

/* Prototypes */
/* ---------- */
BOOL read_input         (char *line_p,char *ip_prompt_p);
BOOL is_matched         (char *tested_p,char *definition_p,S16 minlen);
S32  conv_int           (char *token_p,S16 default_base);
BOOL conv_flt           (char *token_p,double *value_p);
void init_pars          (parse_t *pars_p,char *new_line_p);
void cp_pars            (parse_t *dest_p,parse_t *source_p);
S16  get_tok            (parse_t *pars_p,char *delim_p);
void cli_init           (BOOL (*setup_r)(),S32 max_symbols,S16 default_base);
void cli_run            (char *ip_prompt_p,char *file_p);
S32  io_write           (char *buffer);
S32  io_write_uart      (char *buffer);
S32  io_write_console   (char *buffer);
S32  io_putchars_console(char *buffer,U32 len);
S32  io_putchars        (char *buffer,U32 len);
S32  pollkey_uart       (void);
S32  pollkey_console    (void);
S32  io_read            (char *prompt,char *buffer,S32 buflen);
S32  io_read_console    (char *prompt,char *buffer,S32 buflen);
void io_setup           (void);
S32  readkey            (void);
BOOL evaluate_string(char *token_p,char *string_p,S16 max_len);

/* C++ support */
/* ----------- */
#ifdef __cplusplus
}
#endif
#endif
