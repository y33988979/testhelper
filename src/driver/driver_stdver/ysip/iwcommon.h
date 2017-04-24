/*
 *	Wireless Tools
 *
 *		Jean II - HPLB '99
 *
 * Common header for the wireless tools...
 */

#ifndef IWCOMMON_H
#define IWCOMMON_H

/************************** DOCUMENTATION **************************/
/*
 * None ? Todo...
 */

/* --------------------------- HISTORY --------------------------- */
/*
 * wireless 16 :		(Jean Tourrilhes)
 * -----------
 *	o iwconfig, iwpriv & iwspy
 *
 * wireless 17 :		(Justin Seger)
 * -----------
 *	o Compile under glibc fix
 *	o merge iwpriv in iwconfig
 *	o Add Wavelan roaming support
 *	o Update man page of iwconfig
 *
 * wireless 18 :
 * -----------
 *		(From Andreas Neuhaus <andy@fasta.fh-dortmund.de>)
 *	o Many fix to remove "core dumps" in iwconfig
 *	o Remove useless headers in iwconfig
 *	o CHAR wide private ioctl
 *		(From Jean Tourrilhes)
 *	o Create iwcommon.h and iwcommon.c
 *	o Separate iwpriv again for user interface issues
 *	  The folllowing didn't make sense and crashed :
 *		iwconfig eth0 priv sethisto 12 15 nwid 100
 *	o iwspy no longer depend on net-tools-1.2.0
 *	o Reorganisation of the code, cleanup
 *	o Add ESSID stuff in iwconfig
 *	o Add display of level & noise in dBm (stats in iwconfig)
 *	o Update man page of iwconfig and iwpriv
 *	o Add xwireless (didn't check if it compiles)
 *		(From Dean W. Gehnert <deang@tpi.com>)
 *	o Minor fixes
 *		(Jan Rafaj <rafaj@cedric.vabo.cz>)
 *	o Cosmetic changes (sensitivity relative, freq list)
 *	o Frequency computation on double
 *	o Compile clean on libc5
 *		(From Jean Tourrilhes)
 *	o Move listing of frequencies to iwspy
 *	o Add AP address stuff in iwconfig
 *	o Add AP list stuff in iwspy
 *
 * wireless 19 :
 * -----------
 *		(From Jean Tourrilhes)
 *	o Allow for sensitivity in dBm (if < 0) [iwconfig]
 *	o Formatting changes in displaying ap address in [iwconfig]
 *	o Slightly improved man pages and usage display
 *	o Add channel number for each frequency in list [iwspy]
 *	o Add nickname... [iwconfig]
 *	o Add "port" private ioctl shortcut [iwpriv]
 *	o If signal level = 0, no range or dBms [iwconfig]
 *	o I think I now got set/get char strings right in [iwpriv]
 *		(From Thomas Ekstrom <tomeck@thelogic.com>)
 *	o Fix a very obscure bug in [iwspy]
 */

/* ----------------------------- TODO ----------------------------- */
/*
 * One day, maybe...
 *
 * iwconfig :
 * --------
 *	Use new 802.11 parameters (rate, rts, frag)...
 *
 * iwpriv :
 * ------
 *	?
 *
 * iwspy :
 * -----
 *	?
 *
 * Doc & man pages :
 * ---------------
 *	?
 */

/***************************** INCLUDES *****************************/

/* Standard headers */
#include <sys/types.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <math.h>
#include <errno.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>		/* gethostbyname, getnetbyname */
#include <net/ethernet.h>	/* struct ether_addr */
#include <sys/time.h>		/* struct timeval */
#include <unistd.h>

/* This is our header selection. Try to hide the mess and the misery :-(
 * Don't look, you would go blind ;-)
 * Note : compatibility with *old* distributions has been removed,
 * you will need Glibc 2.2 and older to compile (which means 
 * Mandrake 8.0, Debian 2.3, RH 7.1 or older).
 */

/* Set of headers proposed by Dr. Michael Rietz <rietz@mail.amps.de>, 27.3.2 */
#include <net/if_arp.h>		/* For ARPHRD_ETHER */
#include <sys/socket.h>		/* For AF_INET & struct sockaddr */
#include <netinet/in.h>         /* For struct sockaddr_in */
#include <netinet/if_ether.h>

/* Fixup to be able to include kernel includes in userspace.
 * Basically, kill the sparse annotations... Jean II */
#ifndef __user
#define __user
#endif

#include <linux/types.h>		/* for "caddr_t" et al		*/

#include <net/if.h>			/* for IFNAMSIZ and co... */

#include "compat.h"		/* Don't ask ! */

/* Private copy of Wireless extensions (in this directoty) */
#include "wireless.h"
//#include "iw_cfg.h"

/* Make gcc understant that when we say inline, we mean it.
 * I really hate when the compiler is trying to be more clever than me,
 * because in this case gcc is not able to figure out functions with a
 * single call site, so not only I have to tag those functions inline
 * by hand, but then it refuse to inline them properly.
 * Total saving for iwevent : 150B = 0.7%.
 * Fortunately, in gcc 3.4, they now automatically inline static functions
 * with a single call site. Hurrah !
 * Jean II */
#undef IW_GCC_HAS_BROKEN_INLINE
#if __GNUC__ == 3
#if __GNUC_MINOR__ >= 1 && __GNUC_MINOR__ < 4
#define IW_GCC_HAS_BROKEN_INLINE	1
#endif	/* __GNUC_MINOR__ */
#endif	/* __GNUC__ */
/* However, gcc 4.0 has introduce a new "feature", when compiling with
 * '-Os', it does not want to inline iw_ether_cmp() and friends.
 * So, we need to fix inline again !
 * Jean II */
#if __GNUC__ == 4
#define IW_GCC_HAS_BROKEN_INLINE	1
#endif	/* __GNUC__ */
/* Now, really fix the inline */
#ifdef IW_GCC_HAS_BROKEN_INLINE
#ifdef inline
#undef inline
#endif	/* inline */
#define inline		inline		__attribute__((always_inline))
#endif	/* IW_GCC_HAS_BROKEN_INLINE */

#ifdef __cplusplus
extern "C" {
#endif

#define WE_NOLIBM   1

/****************************** DEBUG ******************************/


/************************ CONSTANTS & MACROS ************************/

/* Various versions information */
/* Recommended Wireless Extension version */
#if (WIRELESS_EXT > 20)
#define WE_VERSION	21
#else
#define WE_VERSION  20
#endif
/* Maximum forward compatibility built in this version of WT */
#define WE_MAX_VERSION	22
/* Version of Wireless Tools */
#if (WIRELESS_EXT > 20)
#define WT_VERSION	29
#else
#define WT_VERSION 28
#endif

/* Paths */
#define PROC_NET_WIRELESS	"/proc/net/wireless"
#define PROC_NET_DEV		"/proc/net/dev"

/* Some usefull constants */
#define KILO	1e3
#define MEGA	1e6
#define GIGA	1e9

/****************************** TYPES ******************************/

/* Shortcuts */
typedef struct iw_statistics	iwstats;
typedef struct iw_range		iwrange;
typedef struct iw_param		iwparam;
typedef struct iw_freq		iwfreq;
typedef struct iw_quality	iwqual;
typedef struct iw_priv_args	iwprivargs;
typedef struct sockaddr		sockaddr;

/* Structure for storing all wireless information for each device
 * This is a cut down version of the one above, containing only
 * the things *truly* needed to configure a card.
 * Don't add other junk, I'll remove it... */
typedef struct wireless_config
{
  char		name[IFNAMSIZ + 1];	/* Wireless/protocol name */
  int		has_nwid;
  iwparam	nwid;			/* Network ID */
  int		has_freq;
  double	freq;			/* Frequency/channel */
  int		freq_flags;
  int		has_key;
  unsigned char	key[IW_ENCODING_TOKEN_MAX];	/* Encoding key used */
  int		key_size;		/* Number of bytes */
  int		key_flags;		/* Various flags */
  int		has_essid;
  int		essid_on;
  char		essid[IW_ESSID_MAX_SIZE + 1];	/* ESSID (extended network) */
  int		has_mode;
  int		mode;			/* Operation mode */
} wireless_config;

/* Structure for storing all wireless information for each device
 * This is pretty exhaustive... */
typedef struct wireless_info
{
  struct wireless_config	b;	/* Basic information */

  int		has_sens;
  iwparam	sens;			/* sensitivity */
  int		has_nickname;
  char		nickname[IW_ESSID_MAX_SIZE + 1]; /* NickName */
  int		has_ap_addr;
  sockaddr	ap_addr;		/* Access point address */
  int		has_bitrate;
  iwparam	bitrate;		/* Bit rate in bps */
  int		has_rts;
  iwparam	rts;			/* RTS threshold in bytes */
  int		has_frag;
  iwparam	frag;			/* Fragmentation threshold in bytes */
  int		has_power;
  iwparam	power;			/* Power management parameters */
  int		has_txpower;
  iwparam	txpower;		/* Transmit Power in dBm */
  int		has_retry;
  iwparam	retry;			/* Retry limit or lifetime */

  /* Stats */
  iwstats	stats;
  int		has_stats;
  iwrange	range;
  int		has_range;

  /* Auth params for WPA/802.1x/802.11i */
  int		auth_key_mgmt;
  int		has_auth_key_mgmt;
  int		auth_cipher_pairwise;
  int		has_auth_cipher_pairwise;
  int		auth_cipher_group;
  int		has_auth_cipher_group;
} wireless_info;

/* Structure for storing an entry of a wireless scan.
 * This is only a subset of all possible information, the flexible
 * structure of scan results make it impossible to capture all
 * information in such a static structure. */
typedef struct wireless_scan
{
  /* Linked list */
  struct wireless_scan *	next;

  /* Cell identifiaction */
  int		has_ap_addr;
  sockaddr	ap_addr;		/* Access point address */

  /* Other information */
  struct wireless_config	b;	/* Basic information */
  iwstats	stats;			/* Signal strength */
  int		has_stats;
#if (WIRELESS_EXT > 20)
  iwparam	maxbitrate;		/* Max bit rate in bps */
  int		has_maxbitrate;
#endif
} wireless_scan;

/*
 * Context used for non-blocking scan.
 */
typedef struct wireless_scan_head
{
  wireless_scan *	result;		/* Result of the scan */
  int			retry;		/* Retry level */
} wireless_scan_head;

/* Structure used for parsing event streams, such as Wireless Events
 * and scan results */
typedef struct stream_descr
{
  char *	end;		/* End of the stream */
  char *	current;	/* Current event in stream of events */
  char *	value;		/* Current value in event */
} stream_descr;

/**************************** VARIABLES ****************************/

/* Modes as human readable strings */
extern const char * const	iw_operation_mode[];
#define IW_NUM_OPER_MODE	7
#if (WIRELESS_EXT > 20)
#define IW_NUM_OPER_MODE_EXT	8
#endif


/**************************** PROTOTYPES ****************************/
/*
 * All the functions in iwcommon.c
 */
/* ---------------------- SOCKET SUBROUTINES -----------------------*/
int
	sockets_open(void);
int
	iw_sockets_open(void);
void
	iw_sockets_close(int	skfd);

/* --------------------- WIRELESS SUBROUTINES ----------------------*/
int
	get_range_info(int		skfd,
		       char *		ifname,
		       iwrange *	range);
int
	get_priv_info(int		skfd,
		      char *		ifname,
		      iwprivargs *	priv);
int
	iw_get_range_info(int		skfd,
			  const char *	ifname,
			  iwrange *	range);
int
	iw_get_priv_info(int		skfd,
			 const char *	ifname,
			 iwprivargs **	ppriv);
int
	iw_get_basic_config(int			skfd,
			    const char *	ifname,
			    wireless_config *	info);
int
	iw_set_basic_config(int			skfd,
			    const char *	ifname,
			    wireless_config *	info);
/* -------------------- FREQUENCY SUBROUTINES --------------------- */
void
	float2freq(double	in,
		   iwfreq *	out);
double
	freq2float(iwfreq *	in);
void
	iw_float2freq(double	in,
		      iwfreq *	out);
double
	iw_freq2float(const iwfreq *	in);
/* -------------------- STATISTICS SUBROUTINES -------------------- */
int 
    iw_getstats(char *	ifname, iwstats *stats);
int
	iw_get_stats(int		skfd,
		     const char *	ifname,
		     iwstats *		stats,
		     const iwrange *	range,
		     int		has_range);
/* --------------------- ADDRESS SUBROUTINES ---------------------- */
int
	check_addr_type(int	skfd,
			char *	ifname);
int
	iw_check_if_addr_type(int		skfd,
			      const char *	ifname);
char *
	pr_ether(unsigned char *ptr);
int
	in_ether(char *bufp, struct sockaddr *sap);
int
	in_inet(char *bufp, struct sockaddr *sap);
int
	in_addr(int		skfd,
		char *		ifname,
		char *		bufp,
		struct sockaddr *sap);
/* ----------------------- MISC SUBROUTINES ------------------------ */
int
	byte_size(int		args);

/*------------------------------------------------------------------*/
/*
 * Wrapper to extract some Wireless Parameter out of the driver
 */
int
iw_get_ext(int			skfd,		/* Socket to the kernel */
	   const char *		ifname,		/* Device name */
	   int			request,	/* WE ID */
	   struct iwreq *	pwrq);		/* Fixed part of the request */

/*------------------------------------------------------------------*/
/*
 * Input an Ethernet Socket Address and convert to binary.
 */
int
iw_saether_aton(const char *bufp, struct sockaddr *sap);

/*------------------------------------------------------------------*/
/*
 * Compare two ethernet addresses
 */
int
iw_ether_cmp(const struct ether_addr* eth1, const struct ether_addr* eth2);

/**************************** VARIABLES ****************************/

#ifdef __cplusplus
}
#endif

#endif	/* IWCOMMON_H */

