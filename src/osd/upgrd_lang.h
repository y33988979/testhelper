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
#define UPGRD_L_CN_MAN_UPGRD         ("�ֶ�����V")
#define UPGRD_L_CN_PROMPT_UPGRD_PWD1 "������ϵͳ����Ȩ������ "
#define UPGRD_L_CN_PROMPT_UPGRD_PWD2 ""
#define UPGRD_L_CN_INPUT_PWD         "��������"
#define UPGRD_L_CN_PROMPT_NOT_OP1    ""

//5-9
#define UPGRD_L_CN_PROMPT_NOT_OP2    "   ����ִ˽�����û���Ҫ�����κβ�����"
#define UPGRD_L_CN_PROMPT_NOT_OP3    "   ��ʱ����Ӫ����ϵ�ͷ��绰:3928888"
#define UPGRD_L_CN_BUTTON_CONFIRM    "  ȷ��  "
#define UPGRD_L_CN_BUTTON_CANCLE     "  ȡ��  "
#define UPGRD_L_CN_PROMPT_OPRTN     "      ������ʾ:���˳�����������Ӧ��"

//10-14
#define UPGRD_L_CN_TITLE_FREQ        " ����Ƶ��(MHz)  "
#define UPGRD_L_CN_TITLE_SYM_RATE    " ������(KS/s)  "
#define UPGRD_L_CN_TITLE_MODULATE    " ���Ʒ�ʽ:      "
#define UPGRD_L_CN_TITLE_PID         " ����PID    "
#define UPGRD_L_CN_TITLE_TALEID      " ����TableID"

//15-19
#define UPGRD_L_CN_TITLE_AUTO        " ��������"
#define UPGRD_L_CN_BUTTON_START      " ��ʼ "
#define UPGRD_L_CN_TRANS_YES         "  ��   "
#define UPGRD_L_CN_TRANS_NO          "  ��   "
#define UPGRD_L_CN_TRANS_EXIT        " �˳� "

//20-24
#define UPGRD_L_CN_PROMPT_POWER1     "�����������벻Ҫ�ϵ�,���Ժ�..."
#define UPGRD_L_TRANS_E2P		" E2P����"
#define UPGRD_L_TRANS_PATITION " ��ѡ��������"
#define UPGRD_L_TRANS_UPDATE      " �������� "
#define UPGRD_L_TRANS_ERASE       " �������� "

//25-29
#define UPGRD_L_CN_PROMPT_POWER2     ""
#define UPGRD_L_CN_PROMPT_SEARCH     "������������  "
#define UPGRD_L_CN_NT_CUR_PRGRSS     "��ǰ����"
#define UPGRD_L_CN_NT_TTL_PRGRSS     "�ܽ���"
#define UPGRD_L_CN_PT_USB_PRGRSS     "�����������벻Ҫ�ϵ�,���Ժ�..."

//30-34
#define UPGRD_L_CN_USB_UPGRD         ("USB����V")
#define UPGRD_L_CN_TITLE_CONTENT     "��������"
#define UPGRD_L_CN_TITLE_FILE     	  "�����ļ�"
#define UPGRD_L_CN_TITLE_ERASE     	"��������"
#define UPGRD_L_CN_PT_USB_CHECK      "���ڼ����ƶ��豸,���Ժ�..."

//35-39
#define UPGRD_L_CN_PT_USB_NO_CONTENT "�Ҳ��������ļ�Ŀ¼!" 
#define UPGRD_L_CN_PT_USB_NO_FILE    "�������ļ�������ʧ��!"
#define UPGRD_L_CN_ERR_USB_READ_FILE "��ȡ�ļ�ʧ��!"
#define UPGRD_L_CN_ERR_USB_MEM_GET	  "�ڴ����ʧ��!"
#define UPGRD_L_CN_SUCCESS_USB_ERASE      "���������ɹ�!"

//40-44
#define UPGRD_L_CN_FAIL_USB_ERASE      "��������ʧ��!"
#define UPGRD_L_CN_PT_USB_ERASEING      "���ڲ�������..."
#define UPGRD_L_CN_FAIL_SIG_EXPT     "   ����ʧ��:�ź��쳣    "
#define UPGRD_L_CN_FAIL_FACTORY      "   ����ʧ��:���̱�־��ƥ��"
#define UPGRD_L_CN_FAIL_HARD_VER     "   ����ʧ��:Ӳ���汾��ƥ��"

//45-49
#define UPGRD_L_CN_FAIL_SOFT_VER     "   ����ʧ��:����汾��ƥ��"
#define UPGRD_L_CN_FAIL_FILE_RNG     "   ����ʧ��: �ļ���Χ����"
#define UPGRD_L_CN_FAIL_CRC          "   ����ʧ��:CRC����"
#define UPGRD_L_CN_FAIL_STREAM       "   ����ʧ��:�Ҳ���������"
#define UPGRD_L_CN_FAIL_DOWNLOAD     "   ����ʧ��:�������ݳ���"

//50-54
#define UPGRD_L_CN_FAIL_DATA         "   ����ʧ��:������Ч"
#define UPGRD_L_CN_FAIL_FREQUENCY    "    Ƶ�����ô���"
#define UPGRD_L_CN_FAIL_FLASH        "   ����ʧ��:����Flash����"
#define UPGRD_L_CN_FAIL_UPDATE_TAG   "   ����ʧ��:�����ʶ����"
#define UPGRD_L_CN_FAIL_MEM_ALLOC	  "   ����ʧ��:�ڴ�������"

//55-59
#define UPGRD_L_CN_FAIL_SYMBOL       "    Symbol rate���ô���"
#define UPGRD_L_CN_FAIL_PID          "    PID���ô���"
#define UPGRD_L_CN_FAIL_MODUL        "    modulation���ô���"
#define UPGRD_L_CN_PT_DOWNLOADING    "������������  "
#define UPGRD_L_CN_PT_WRITING        "���ڸ�������  "

//60-64
#define UPGRD_L_CN_IP_UPGRD         ("IP����V")
#define UPGRD_L_CN_PT_SUCC           "�����ɹ�  "
#define UPGRD_L_CN_WARN_DISCONN      "   ��ʾ: ����Ͽ���"
#define UPGRD_L_CN_SET_IP_ERR        "   ����ʧ��:����IP����"
#define UPGRD_L_CN_FAIL_NET_EXPT     "   ����ʧ��:�����쳣"

//65-69
#define UPGRD_L_CN_FAIL_NET_UNRCH    "   ����ʧ��:����û�в��"
#define UPGRD_L_CN_IP_PARSE_XML_ERR	  "   ����ʧ��:����XML�ļ�����"
#define UPGRD_L_CN_FAIL_NO_RESP      "   ����ʧ��:������û����Ӧ"
#define UPGRD_L_CN_FAIL_FILE_LEN     "   ����ʧ��:�ļ����Ȳ�ƥ��"
#define UPGRD_L_CN_DWNLD_WTING       "���������������벻Ҫ�ϵ�,���Ժ�..."

//70-74
#define UPGRD_L_CN_FAIL_UPGRD        "      ����ʧ�ܣ�  "
#define UPGRD_L_CN_IP_SERVER_ADDR    "������IP��ַ"
#define UPGRD_L_CN_IP_SERVER_PORT    "�������˿ں�"
#define UPGRD_L_CN_IP_HOST_SET    	  "����IP����"
#define UPGRD_L_CN_IP_OUR_DHCP		  "�Զ���ȡ"	

//75-79
#define UPGRD_L_CN_IP_OUR_HAND		  "�ֶ�����"
#define UPGRD_L_CN_IP_OUR_ADDR		  "ADDR"
#define UPGRD_L_CN_IP_OUR_MASK		  "MASK"
#define UPGRD_L_CN_IP_OUR_GATEWAY	  "GATE"
#define UPGRD_L_CN_IP_CFG_NET			"������������"

//80-84
#define UPGRD_L_CN_IP_PARSE_XML		"���ڽ���XML��Ϣ"
#define UPGRD_L_CN_FILE_PARAM_ERROR		"�����ļ��в�������! "


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

