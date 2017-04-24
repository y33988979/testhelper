#ifndef _TESTTOOL_H_
#define _TESTTOOL_H_
/* C++ support */
/* ----------- */
#ifdef __cplusplus
extern "C" {
#endif
#include <stddefs.h>
#define MAX_TOK_LEN  512

/* Key definition */
/* -------------- */
typedef enum e_key_code
{
 KEY_ESC,
 KEY_BACKSPACE,
 KEY_RETURN,
 KEY_END_OF_LINE,
 KEY_BEGIN_OF_LINE,
 KEY_ARROW_UP,
 KEY_ARROW_DOWN,
 KEY_ARROW_LEFT,
 KEY_ARROW_RIGHT,
 KEY_INSERT,
 KEY_DELETE,
 KEY_TAB,
 KEY_CLEAR_LINE,
 KEY_CLEAR_EOL,
 KEY_CTRL_LEFT,
 KEY_CTRL_RIGHT,
 KEY_NUMBERS
} e_key_code;

/* Action definition */
/* ----------------- */
typedef enum e_action_code
{
 ACTION_BACKSPACE,
 ACTION_RETURN,
 ACTION_MOVE_EOL,
 ACTION_MOVE_BOL,
 ACTION_HISTORY_UP,
 ACTION_HISTORY_DOWN,
 ACTION_MOVE_LEFT,
 ACTION_MOVE_RIGHT,
 ACTION_TOGGLE_INSERT,
 ACTION_DELETE,
 ACTION_COMPLETE,
 ACTION_CLEAR_LINE,
 ACTION_CLEAR_EOL,
 ACTION_ADD_CHAR,
 ACTION_MOVE_BOT,
 ACTION_MOVE_EOT,
 ACTION_NOACTION
} e_action_code;


struct parse
{
    char           *line_p;     /* string under examination  */
    S16             par_pos;    /* index of cur position,  delimiter or EOL */
    S16             par_sta;    /* index start position for last operation  */
    S16             tok_len;    /* length of identified token */
    char            tok_del;    /* delimit of current token */
    char            token[MAX_TOK_LEN];     /* actual token found,uppercase */
};
/* History definition */
/* ------------------ */
#define NLINES 20
typedef struct
{
 U32 write_pointer;
 U32 read_pointer;
 U8  command_history[512][NLINES];
} history_t;

/* Key bind definition */
/* ------------------- */
typedef struct s_key_bind
{
 e_key_code     key_code;
 char          *key_string;
 e_action_code  action_code;
} t_key_bind;

typedef struct parse STTST_Parse_t;
typedef struct parse parse_t;
#define        STTST_Init(a)                   TesttoolInit()
#define        STTST_Start( )                  TesttoolRun()
#define        STTST_Term()
#define        STTST_SetMode(a)
#define        STTST_Print(msg)                print msg
#define        STTST_GetItem(p,d,r,m)          cget_item(p,d,r,m)
#define        STTST_GetTokenCount(p,r)        cget_token_count(p,r)
#define        STTST_GetString(p,d,r,m)        cget_string(p,d,r,m)
#define        STTST_GetInteger(p,d,r)         cget_integer(p,d,r)
#define        STTST_GetFloat(p,d,r)           cget_float(p,d,r)
#define        STTST_AssignString(t,v,c)       assign_string(t,v,c)
#define        STTST_AssignInteger(t,v,c)      assign_integer(t,v,c)
#define        STTST_AssignFloat(t,v,c)        assign_float(t,v,c)
#define        STTST_EvaluateString(t,s,m)     evaluate_string(t,s,m)
#define        STTST_EvaluateInteger(t,v,d)    evaluate_integer(t,v,d)
#define        STTST_EvaluateFloat(t,v)        evaluate_float(t,v)
#define        STTST_EvaluateComparison(t,r,d) evaluate_comparison(t,r,d)
#define        STTST_TagCurrentLine(p,m)       tag_current_line(p,m)
#define        STTST_RegisterCommand(t,a,h)    register_command(t,a,h)



ST_ErrorCode_t TesttoolInit(U32 DeviceId);
ST_ErrorCode_t TesttoolRun(BOOL Synchronous);


/*-------------------------------------------------------------------------
 * Function : FILTER_InitCommands
 *            Definition of the macros
 *            (commands and constants to be used with Testtool)
 * Input    :
 * Output   :
 * Return   : TRUE if error, FALSE if success
 * ----------------------------------------------------------------------*/
BOOL drv_FilterInitCommand(void);

BOOL drv_AvInitCommand(void);

BOOL drv_TunerInitCommand(void);

BOOL drv_AudInitCommand( void );

BOOL drv_OutInitCommand( void );

int drv_vfsRegisterCmd(void);


/* C++ support */
/* ----------- */
#ifdef __cplusplus
}
#endif
#endif

