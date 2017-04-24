#ifndef __LOG_H__
#define __LOG_H__

#define BUILD_DATE ("\n<Build %s %s>\n\n",  __DATE__, __TIME__);


#define LOG0(...)		do{printf("%s():%d |> ", __FUNCTION__, __LINE__); printf(__VA_ARGS__);}while(0)
#define LOG1(...)		do{printf("%s():%d |> ", __FUNCTION__, __LINE__); printf(__VA_ARGS__);}while(0)
//#define LOG2(...)	printf("%s() | > ", __FUNCTION__, __LINE__); printf(__VA_ARGS__)
#define LOG_FUNC_STACK(...)	printf("%s():%d |> ", __FUNCTION__, __LINE__)

//#define LOG0(...)
//#define LOG1(...)
#define LOG2(...)

#define LOGTMP(...)		do{printf("%s():%d |> ", __FUNCTION__, __LINE__); printf(__VA_ARGS__);}while(0)


#endif

