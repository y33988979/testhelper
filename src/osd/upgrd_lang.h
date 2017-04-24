/******************************************************************************
                              Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     :
Version       :
Author        : Hisilicon multimedia Hardware group ldx
Created       : 2011/05/19
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/

#ifndef __UPGRADE_LANG_H__
#define __UPGRADE_LANG_H__

#ifdef __cplusplus
extern "C"{
#endif

typedef enum tagLOADER_Content_E
{
	//0-4
    LOADER_CONTENT_MAN_UPGRD,
    LOADER_CONTENT_PROMPT_UPGRD_PWD1,
    LOADER_CONTENT_PROMPT_UPGRD_PWD2,
    LOADER_CONTENT_INPUT_PWD,
    LOADER_CONTENT_PROMPT_NOT_OP1,

	//5-9
    LOADER_CONTENT_PROMPT_NOT_OP2,
    LOADER_CONTENT_PROMPT_NOT_OP3,
    LOADER_CONTENT_BUTTON_CONFIRM,
    LOADER_CONTENT_BUTTON_CANCLE,
    LOADER_CONTENT_PROMPT_OPRTN,

	//10-14
    LOADER_CONTENT_TITLE_FREQ,
    LOADER_CONTENT_TITLE_SYM_RATE,
    LOADER_CONTENT_TITLE_MODULATE,
    LOADER_CONTENT_TITLE_PID,
    LOADER_CONTENT_TITLE_TALEID,    

	//15-19
	LOADER_CONTENT_TITLE_AUTO,
    LOADER_CONTENT_BUTTON_START,
    LOADER_CONTENT_TRANS_YES,
    LOADER_CONTENT_TRANS_NO,
    LOADER_CONTENT_TRANS_EXIT,

	//20-24    
    LOADER_CONTENT_PROMPT_POWER1,
    LOADER_CONTENT_TRANS_E2P,
    LOADER_CONTENT_TRANS_PATITION,
    LOADER_CONTENT_TRANS_UPDATE,
    LOADER_CONTENT_TRANS_ERASE,

	//25-29
    LOADER_CONTENT_PROMPT_POWER2,
    LOADER_CONTENT_PROMPT_SEARCH,
    LOADER_CONTENT_NT_CUR_PRGRSS,
    LOADER_CONTENT_NT_TTL_PRGRSS,
    LOADER_CONTENT_PT_USB_PRGRSS,

	//30-34
	LOADER_CONTENT_USB_UPGRD,
	LOADER_CONTENT_TITLE_CONTENT,
	LOADER_CONTENT_TITLE_FILE,
	LOADER_CONTENT_TITLE_ERASE,
	LOADER_CONTENT_PT_USB_CHECK,

	//35-39	
	LOADER_CONTENT_PT_USB_NO_CONTENT,
	LOADER_CONTENT_PT_USB_NO_FILE,
	LOADER_CONTENT_ERR_USB_READ_FILE,
	LOADER_CONTENT_ERR_USB_MEM_GET,
	LOADER_CONTENT_SUCCESS_USB_ERASE,

	//40-44	
	LOADER_CONTENT_FAIL_USB_ERASE,
	LOADER_CONTENT_PT_USB_ERASEING,	
    LOADER_CONTENT_FAIL_SIG_EXPT,
    LOADER_CONTENT_FAIL_FACTORY,
    LOADER_CONTENT_FAIL_HARD_VER,

	//45-49    
    LOADER_CONTENT_FAIL_SOFT_VER,
    LOADER_CONTENT_FAIL_FILE_RNG,
    LOADER_CONTENT_FAIL_CRC,
    LOADER_CONTENT_FAIL_STREAM,
    LOADER_CONTENT_FAIL_DOWNLOAD,

	//50-54	
    LOADER_CONTENT_FAIL_DATA,
    LOADER_CONTENT_FAIL_FREQUENCY,
	LOADER_CONTENT_FAIL_FLASH,
	LOADER_CONTENT_FAIL_UPDATE_TAG,
	LOADER_CONTENT_FAIL_MEM_ALLOC,

	//55-59
    LOADER_CONTENT_FAIL_SYMBOL,
    LOADER_CONTENT_FAIL_PID,
    LOADER_CONTENT_FAIL_MODUL,
    LOADER_CONTENT_PT_DOWNLOADING,
    LOADER_CONTENT_PT_WRITING,

	//60-64
	LOADER_CONTENT_IP_UPGRD,
    LOADER_CONTENT_PT_SUCC,
    LOADER_CONTENT_WARN_DISCONN,
    LOADER_CONTENT_SET_IP_ERR,
    LOADER_CONTENT_FAIL_NET_EXPT,

	//65-69
    LOADER_CONTENT_FAIL_NET_UNRCH,
	LOADER_CONTENT_IP_PARSE_XML_ERR,
    LOADER_CONTENT_FAIL_NO_RESP,
    LOADER_CONTENT_FAIL_FILE_LEN,
    LOADER_CONTENT_DWNLD_WTING,
    
	//70-74
    LOADER_CONTENT_FAIL_UPGRD,
    LOADER_CONTENT_IP_SERVER_ADDR,
    LOADER_CONTENT_IP_SERVER_PORT,
    LOADER_CONTENT_IP_HOST_SET,
    LOADER_CONTENT_IP_OUR_DHCP,

	//75-79
    LOADER_CONTENT_IP_OUR_HAND,
    LOADER_CONTENT_IP_OUR_ADDR,
    LOADER_CONTENT_IP_OUR_MASK,
    LOADER_CONTENT_IP_OUR_GATEWAY,
    LOADER_CONTENT_IP_CFG_NET,

	//80-84
    LOADER_CONTENT_IP_PARSE_XML,
    LOADER_CONTENT_FILE_PARAM_ERROR,
    
	LOADER_CONTENT_MAX

	
}LOADER_Content_E;

#define FLASH_PATITION_BOOT			"fastboot"
#define FLASH_PATITION_BOOTARGS		"bootargs"
#define FLASH_PATITION_STBINFO		"stbinfo"
#define FLASH_PATITION_LOADER			"loader"
#define FLASH_PATITION_LOADERBAK		"loaderbak"
#define FLASH_PATITION_KERNEL			"kernel"
#define FLASH_PATITION_SYSTEM			"system"
#define FLASH_PATITION_ROOTFS			"rootfs"
#define FLASH_PATITION_APP				"elf"
#define FLASH_PATITION_BASEPARAM		"baseparam"
#define FLASH_PATITION_LOGO			"logo"
#define FLASH_PATITION_FASTPLAY		"fastplay"
#define FLASH_PATITION_USERDATA		"userdata"
#define FLASH_PATITION_FLASHDATA	"flashdata"
#define FLASH_PATITION_CACHE		"cache"
#define FLASH_PATITION_WIFI			"wifi"

#define FLASH_MOUNT_DIR				"/tmp"

//below is chinese cotent
//0-4
#define UPGRD_L_CN_MAN_UPGRD         ("手动升级V")
#define UPGRD_L_CN_PROMPT_UPGRD_PWD1 "请输入系统升级权限密码 "
#define UPGRD_L_CN_PROMPT_UPGRD_PWD2 ""
#define UPGRD_L_CN_INPUT_PWD         "输入密码"
#define UPGRD_L_CN_PROMPT_NOT_OP1    ""

//5-9
#define UPGRD_L_CN_PROMPT_NOT_OP2    "   请出现此界面的用户不要进行任何操作并"
#define UPGRD_L_CN_PROMPT_NOT_OP3    "   及时与运营商联系客服电话:3928888"
#define UPGRD_L_CN_BUTTON_CONFIRM    "  确定  "
#define UPGRD_L_CN_BUTTON_CANCLE     "  取消  "
#define UPGRD_L_CN_PROMPT_OPRTN     "      操作提示:按退出键继续启动应用"

//10-14
#define UPGRD_L_CN_TITLE_FREQ        " 下载频点(MHz)  "
#define UPGRD_L_CN_TITLE_SYM_RATE    " 符号率(KS/s)  "
#define UPGRD_L_CN_TITLE_MODULATE    " 调制方式:      "
#define UPGRD_L_CN_TITLE_PID         " 升级PID    "
#define UPGRD_L_CN_TITLE_TALEID      " 升级TableID"

//15-19
#define UPGRD_L_CN_TITLE_AUTO        " 主动下载"
#define UPGRD_L_CN_BUTTON_START      " 开始 "
#define UPGRD_L_CN_TRANS_YES         "  是   "
#define UPGRD_L_CN_TRANS_NO          "  否   "
#define UPGRD_L_CN_TRANS_EXIT        " 退出 "

//20-24
#define UPGRD_L_CN_PROMPT_POWER1     "升级过程中请不要断电,请稍候..."
#define UPGRD_L_TRANS_E2P		" E2P擦除"
#define UPGRD_L_TRANS_PATITION " 所选分区擦除"
#define UPGRD_L_TRANS_UPDATE      " 升级操作 "
#define UPGRD_L_TRANS_ERASE       " 擦除操作 "

//25-29
#define UPGRD_L_CN_PROMPT_POWER2     ""
#define UPGRD_L_CN_PROMPT_SEARCH     "正在搜索数据  "
#define UPGRD_L_CN_NT_CUR_PRGRSS     "当前进度"
#define UPGRD_L_CN_NT_TTL_PRGRSS     "总进度"
#define UPGRD_L_CN_PT_USB_PRGRSS     "升级过程中请不要断电,请稍候..."

//30-34
#define UPGRD_L_CN_USB_UPGRD         ("USB升级V")
#define UPGRD_L_CN_TITLE_CONTENT     "升级内容"
#define UPGRD_L_CN_TITLE_FILE     	  "升级文件"
#define UPGRD_L_CN_TITLE_ERASE     	"擦除分区"
#define UPGRD_L_CN_PT_USB_CHECK      "正在检测可移动设备,请稍候..."

//35-39
#define UPGRD_L_CN_PT_USB_NO_CONTENT "找不到升级文件目录!" 
#define UPGRD_L_CN_PT_USB_NO_FILE    "无升级文件，升级失败!"
#define UPGRD_L_CN_ERR_USB_READ_FILE "读取文件失败!"
#define UPGRD_L_CN_ERR_USB_MEM_GET	  "内存分配失败!"
#define UPGRD_L_CN_SUCCESS_USB_ERASE      "分区擦除成功!"

//40-44
#define UPGRD_L_CN_FAIL_USB_ERASE      "分区擦除失败!"
#define UPGRD_L_CN_PT_USB_ERASEING      "正在擦除数据..."
#define UPGRD_L_CN_FAIL_SIG_EXPT     "   升级失败:信号异常    "
#define UPGRD_L_CN_FAIL_FACTORY      "   升级失败:厂商标志不匹配"
#define UPGRD_L_CN_FAIL_HARD_VER     "   升级失败:硬件版本不匹配"

//45-49
#define UPGRD_L_CN_FAIL_SOFT_VER     "   升级失败:软件版本不匹配"
#define UPGRD_L_CN_FAIL_FILE_RNG     "   升级失败: 文件范围出错"
#define UPGRD_L_CN_FAIL_CRC          "   升级失败:CRC出错"
#define UPGRD_L_CN_FAIL_STREAM       "   升级失败:找不到升级流"
#define UPGRD_L_CN_FAIL_DOWNLOAD     "   升级失败:下载数据出错"

//50-54
#define UPGRD_L_CN_FAIL_DATA         "   升级失败:数据无效"
#define UPGRD_L_CN_FAIL_FREQUENCY    "    频点设置错误！"
#define UPGRD_L_CN_FAIL_FLASH        "   升级失败:更新Flash错误！"
#define UPGRD_L_CN_FAIL_UPDATE_TAG   "   升级失败:清除标识错误！"
#define UPGRD_L_CN_FAIL_MEM_ALLOC	  "   升级失败:内存分配错误！"

//55-59
#define UPGRD_L_CN_FAIL_SYMBOL       "    Symbol rate设置错误！"
#define UPGRD_L_CN_FAIL_PID          "    PID设置错误！"
#define UPGRD_L_CN_FAIL_MODUL        "    modulation设置错误！"
#define UPGRD_L_CN_PT_DOWNLOADING    "正在下载数据  "
#define UPGRD_L_CN_PT_WRITING        "正在更新数据  "

//60-64
#define UPGRD_L_CN_IP_UPGRD         ("IP升级V")
#define UPGRD_L_CN_PT_SUCC           "升级成功  "
#define UPGRD_L_CN_WARN_DISCONN      "   提示: 网络断开！"
#define UPGRD_L_CN_SET_IP_ERR        "   升级失败:设置IP错误"
#define UPGRD_L_CN_FAIL_NET_EXPT     "   升级失败:网络异常"

//65-69
#define UPGRD_L_CN_FAIL_NET_UNRCH    "   下载失败:网线没有插好"
#define UPGRD_L_CN_IP_PARSE_XML_ERR	  "   下载失败:解析XML文件错误"
#define UPGRD_L_CN_FAIL_NO_RESP      "   下载失败:服务器没有响应"
#define UPGRD_L_CN_FAIL_FILE_LEN     "   下载失败:文件长度不匹配"
#define UPGRD_L_CN_DWNLD_WTING       "正在下载数据中请不要断电,请稍候..."

//70-74
#define UPGRD_L_CN_FAIL_UPGRD        "      升级失败！  "
#define UPGRD_L_CN_IP_SERVER_ADDR    "服务器IP地址"
#define UPGRD_L_CN_IP_SERVER_PORT    "服务器端口号"
#define UPGRD_L_CN_IP_HOST_SET    	  "本机IP设置"
#define UPGRD_L_CN_IP_OUR_DHCP		  "自动获取"	

//75-79
#define UPGRD_L_CN_IP_OUR_HAND		  "手动设置"
#define UPGRD_L_CN_IP_OUR_ADDR		  "ADDR"
#define UPGRD_L_CN_IP_OUR_MASK		  "MASK"
#define UPGRD_L_CN_IP_OUR_GATEWAY	  "GATE"
#define UPGRD_L_CN_IP_CFG_NET			"正在设置网络"

//80-84
#define UPGRD_L_CN_IP_PARSE_XML		"正在解析XML信息"
#define UPGRD_L_CN_FILE_PARAM_ERROR		"升级文件中参数有误! "


////below is english cotent
//0-4
#define UPGRD_L_EN_MAN_UPGRD         "Manual Upgrade"
#define UPGRD_L_EN_PROMPT_UPGRD_PWD1 "Enter password for obtaining"
#define UPGRD_L_EN_PROMPT_UPGRD_PWD2 "system upgrade permission."
#define UPGRD_L_EN_INPUT_PWD         "Password: "
#define UPGRD_L_EN_PROMPT_NOT_OP1    "Do not perform any operation when"

//5-9
#define UPGRD_L_EN_PROMPT_NOT_OP2    "this page appears. Contact the"
#define UPGRD_L_EN_PROMPT_NOT_OP3    "operator by calling 3928888."
#define UPGRD_L_EN_BUTTON_CONFIRM    "Confirm"
#define UPGRD_L_EN_BUTTON_CANCLE     " Cancel "
#define UPGRD_L_EN_PROMPT_OPRTN      "Tip: Press BACK to restart."

//10-14
#define UPGRD_L_EN_TITLE_FREQ        "Frequency (MHz)  "
#define UPGRD_L_EN_TITLE_SYM_RATE    "Symbol rate (KS/s)  "
#define UPGRD_L_EN_TITLE_MODULATE    "Modulation:      "
#define UPGRD_L_EN_TITLE_PID         "PID    "
#define UPGRD_L_EN_TITLE_AUTO        "Auto download"

//15-19
#define UPGRD_L_EN_BUTTON_START      "Start "
#define UPGRD_L_EN_TRANS_YES         " Yes  "
#define UPGRD_L_EN_TRANS_NO          "  No  "
#define UPGRD_L_EN_TRANS_EXIT        " Exit  "
#define UPGRD_L_EN_PROMPT_POWER1     "Do not power off the STB "

//20-24
#define UPGRD_L_EN_PROMPT_POWER2     "during upgrade. Waiting..."
#define UPGRD_L_EN_PROMPT_SEARCH     "Searching"
#define UPGRD_L_EN_NT_CUR_PRGRSS     "Current progress"
#define UPGRD_L_EN_NT_TTL_PRGRSS     "Total progress"
#define UPGRD_L_EN_PT_USB_PRGRSS     "Do not power off. Waiting..."

//25-29
#define UPGRD_L_EN_FAIL_SIG_EXPT     "Failed. Signal exception."
#define UPGRD_L_EN_FAIL_FACTORY      "Failed. Invalid factory ID."
#define UPGRD_L_EN_FAIL_HARD_VER     "Failed. Invalid hardware ver."
#define UPGRD_L_EN_FAIL_SOFT_VER     "Failed. Invalid software ver."
#define UPGRD_L_EN_FAIL_FILE_RNG     "Failed. File is too large."

//30-34
#define UPGRD_L_EN_FAIL_CRC          "Failed. CRC error."
#define UPGRD_L_EN_FAIL_STREAM       "Failed. No upgrade stream." 
#define UPGRD_L_EN_FAIL_DOWNLOAD     "Failed. Error when downloading."
#define UPGRD_L_EN_FAIL_DATA         "Failed. Invalid data."
#define UPGRD_L_EN_FAIL_FREQUENCY    "Failed. Invalid frequency."

//35-39
#define UPGRD_L_EN_FAIL_SYMBOL       "Failed. Invalid symbol rate."
#define UPGRD_L_EN_FAIL_PID          "Failed. Invalid PID."
#define UPGRD_L_EN_FAIL_MODUL        "Failed. Invalid modulation."
#define UPGRD_L_EN_PT_DOWNLOADING    "Downloading"
#define UPGRD_L_EN_PT_WRITING        "Writing to flash"

//40-44
#define UPGRD_L_EN_PT_SUCC           "Upgrade succeeded"
#define UPGRD_L_EN_WARN_DISCONN      "Warning. Disconnected!"
#define UPGRD_L_EN_SET_IP_ERR        "Failed. IP error."
#define UPGRD_L_EN_FAIL_NET_EXPT     "Failed. Network exception."
#define UPGRD_L_EN_FAIL_NET_UNRCH    "Failed. Network disconnected."

//45-49
#define UPGRD_L_EN_FAIL_NO_RESP      "Failed. Server no response."
#define UPGRD_L_EN_FAIL_FILE_LEN     "Failed. Invalid length of file."
#define UPGRD_L_EN_DWNLD_WTING       "Downloading data, Waiting..."
#define UPGRD_L_EN_FAIL_UPGRD        "Failed to upgrade."

#ifdef __cplusplus
}
#endif
#endif

