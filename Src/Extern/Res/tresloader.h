#ifndef TRESLOADER_H
#define TRESLOADER_H

#include <stdio.h>
#include <stdarg.h>
#include "ResConv.h"

#define RL_MAX_PATH_LEN		256

#define RL_LOADMODE_XMLV1	1		/*读取嵌套结构体以类型名标识的xml资源文件*/
#define RL_LOADMODE_XMLV2	2		/*读取嵌套结构体以成员名标识的xml资源文件*/
#define RL_LOADMODE_XMLV3	3		/*读取嵌套结构体以属性值标识的xml资源文件*/
#define RL_LOADMODE_BIN		4		/*读取二进制数据格式的资源文件*/

#ifndef RES_ID_ARRAY
#define RES_ID_ARRAY	1
#endif

/** @name TRC_LOADR错误ID
*  @{
*/
#define RL_ERROR_OPEN_FILE_FAILED		-1		/*打开文件读失败*/
#define RL_ERROR_READ_FILE_FAILED		-2		/*从文件读失败*/
#define RL_ERROR_CHECK_FAILE_FAILED		-3		/*检验资源文件失败，可能资源文件的版本，魔数与本接口定义的不一致*/
#define RL_ERROR_NO_MEMORY				-4		/*分配内存失败*/
#define RL_ERROR_UNIT_SIZE_TOO_SMALE		-5		/*指定的单个资源存储空间比预期的要小*/
#define RL_ERROR_BUFFER_SIZE_TO_SMALL	-6		/*加载资源的数据区空间不够*/
#define RL_ERROR_DIFF_HASH 	-7		/*资源文件中记录的DR元数据库hash值与当前指定的hash值不同，说明资源结构体可能已经变更*/
#define RL_ERROR_READ_XML_FILE_FAILED -8	/*读取xml文件失败*/
#define RL_ERROR_FAILED_TO_GET_HEAD_META -9	/*获取TResHead元数据描述句柄失败*/
#define RL_ERROR_FAILED_TO_READ_XRESHEAD -10	/*从xml中读取TResHead失败*/
#define RL_ERROR_INVALID_PARAM -11	/*传入接口的参数无效*/
/**   @}*/

/** @name TRC_LOADR资源加载标识位
*  @{
*/
#define RL_FLAG_INGORE_DIFF_HASH		0x00000001		/*忽略资源文件中元数据描述库的hash值与当前hash值的不同，继续加载资源*/
/**   @}*/




/*表示该参数只是输入参数*/
#ifndef IN
#define IN  
#endif


/*表示该参数只是输出参数*/
#ifndef OUT
#define OUT
#endif


/*表示该参数既是输入参数，又是输出参数*/
#ifndef INOUT
#define INOUT
#endif

/* automatically include the correct library on windows */
#ifdef WIN32

	# ifdef _DEBUG
		#  define LIB_RESLOADER_D    "_d"
	# else
		#  define LIB_RESLOADER_D
	# endif /* _DEBUG */

	# if defined(LIB_RESLOADER_D)
		# pragma comment( lib, "libresloader"  LIB_RESLOADER_D ".lib" )
	# else
		# pragma comment( lib, "libresloader.lib" )
	# endif/*# if defined(LIB_RESLOADER_D)*/
#endif /*#ifdef WIN32*/


/*为兼容老接口而定义的接口转换*/
#define rl_load	trl_load
#define rl_unload trl_unload
#define rl_specail_load	trl_specail_load
#define rl_xload_init	trl_xload_init
#define rl_xload_with_reshead	trl_xload_with_reshead
#define rl_xload	trl_xload
#define rl_find	trl_find
#define rl_find_ll	trl_find_ll
#define rl_find_n	trl_find_n

#ifdef __cplusplus
extern "C"
{
#endif

/** @defgroup TRC_LOADR_VFILE 虚拟文件系统支持接口
*  @{
*/

/*file stream open function*/
typedef FILE* (*LPFOPEN)( const char *path, const char *mode ); 

/*close a file stream*/
typedef int (*LPFCLOSE)(FILE *fp);	

	/*binary stream input function*/
typedef	size_t (*LPFREAD)(void *ptr, size_t size, size_t nmemb, FILE *stream); 	 

	/*function sets the file position indicator for the stream pointed to by stream*/
typedef int (*LPFSEEK)(FILE *stream, long offset, int whence);

	/*obtains the current value of the file position indicator for the stream pointed to by stream*/
typedef long (*LPFTELL)(FILE *stream);	


/*文件系统IOAPI回调函数，open,fread,fwrite等文件系统io接口容许定制*/
struct tagTFileIOInterface
{
	/*file stream open function*/
	LPFOPEN pfopen; 

	/*close a file stream*/
	LPFCLOSE pfclose;	

	/*binary stream input function*/
	LPFREAD pfread; 	 

	/*function sets the file position indicator for the stream pointed to by stream*/
	LPFSEEK pfseek;

	/*obtains the current value of the file position indicator for the stream pointed to by stream*/
	LPFTELL pftell;	
};
typedef struct tagTFileIOInterface TFILEIOINTERFACE;
typedef struct tagTFileIOInterface *LPTFILEIOINTERFACE;


/**设置文件系统I/O操作接口
@param[in] a_pstInterface 包含文件系统IO操作相关接口的数据结构指针
*@retval 0 成功
*@retval <0	失败
@pre \e a_pstInterface 不能为NULL
@pre \e a_pstInterface所指结构体中个接口指针不能为NULL
@note 本接口是非线程安全的，如果在多线程环境调用本接口，调用者必须互斥调用本接口
@note 只有使用虚拟文件系统才需在加载资源之前调用一次，以指定特殊的文件系统io接口。
@note 如果不调用本接口，资源加载库会使用C库提供的文件io操作接口:fopen,fread,fseek等
*/
int trl_set_fileio_interface(LPTFILEIOINTERFACE a_pstInterface);
/**  @}*/ /** @defgroup TRC_LOADR_VFILE 虚拟文件系统支持接口*/


/** @defgroup TRC_LOADR_BIN 二进制格式的资源文件加载接口
*  @{
*/




/** 从资源文件中加载资源到缓冲区中，缓冲区的空间在函数内分配
*@param[in,out] ppvBuff 保存数据的数据区指针的指针
*@param[out] piBuff	获取数据区的字节数
*@param[out] piUnit	获取单个资源信息结构体的在缓冲区中的存储空间大小
*@param[in] pszFilePath	资源文件名
*@param[in] pszMetalibHash 此资源文件中资源结构对应DR元数据描述库的hash值
*@param[in] iFlag	资源加载控制标识位，此参数可以为0或下列标识位的位'与'值:
*	-	RL_FLAG_INGORE_DIFF_HASH	忽略元数据描述库hash的不同
*
*note	保存资源信息的缓冲区必须调用trl_unload进行释放
*note 目前资源文件的头部记录了生成资源文件时此资源结构体对应应DR
*	元数据库的hash值。pszMetalibHash参数传入应用当前使用的资源结构DR 元数据库
*	的hash值，通过检测hash值是否相同，可以判断资源文件中资源结构的版本
*	与当前结构的版本是否相同；如果资源结构版本不同，则加载资源
*	失败，这样能避免因资源结构体变更而加载错误的资源
*note	如果pszMetalibHash参数置为NULL, 则不检测元数据库的hash值，这时不能检测
*	资源文件中结构体与当前使用的资源结构是否一致
*note 	当前资源结构DR 元数据描述hash值的获取途径有:
*	-	通过使用md5sum工具利用当前二进制DR元数据库文件生成
*	-	利用tdr工具，将资源结构描述转换成.h头文件时，会将此资源结构
*		DR元数据库的hash值写道头文件中，
*note 如果使用虚拟文件来存储存储资源文件，则调用本接口之前必须调用trl_set_fileio_interface，以
*	设置文件IO操作接口
*@see trl_set_fileio_interface
*
*@retval 正数  读取的资源个数
*@retval 0	文件中没有资源
*@retval 负数， 加载资源失败，可能返回值和错误原因：
*	－ RL_ERROR_OPEN_FILE_FAILED 打开文件读失败
*	-  RL_ERROR_READ_FILE_FAILED	从文件读失败
*	- RL_ERROR_CHECK_FAILE_FAILED		检验资源文件失败，可能资源文件的版本，魔数与本接口定义的不一致
*	- RL_ERROR_NO_MEMORY				分配内存失败
*	- RL_ERROR_DIFF_HASH	资源结构元数据描述库hash不同
*/
int trl_load(INOUT char** ppvBuff, OUT int *piBuff, OUT int *piUnit, IN const char* pszFilePath, 
	IN const char *pszMetalibHash, IN int iFlag);

/*TRC_LOADR_BIN 二进制资源文件加载接口*/

/** 指定保存资源数据的缓冲大小并指定单个资源数据的内存大小,从资源文件中将资源大小缓冲区中，缓冲区的空间在函数外分配
*@param[in] pszBuff 保存数据的缓冲区首地址
*@param[in] iBuff	缓冲区的可用字节数
*@param[in] iUnit	指定单个资源数据结构体的在内存中的存储空间，通过这个参数调用者可以为每个资源分配比实际需要存储空间更大的空间。
如果此参数的值为0，则单个资源信息结构体的存储空间为实际所需空间
*@param[in] pszFilePath	资源文件名
*@param[in] pszMetalibHash 此资源文件中资源结构对应DR元数据描述库的hash值
*@param[in] iFlag	资源加载控制标识位，此参数可以为0或下列标识位的位'与'值:
*	-	RL_FLAG_INGORE_DIFF_HASH	忽略元数据描述库hash的不同
*
*note	保存资源信息的缓冲区必须调用trl_unload进行释放
*note 目前资源文件的头部记录了生成资源文件时此资源结构体对应应DR
*	元数据库的hash值。pszMetalibHash参数传入应用当前使用的资源结构DR 元数据库
*	的hash值，通过检测hash值是否相同，可以判断资源文件中资源结构的版本
*	与当前结构的版本是否相同；如果资源结构版本不同，则加载资源
*	失败，这样能避免因资源结构体变更而加载错误的资源
*note	如果pszMetalibHash参数置为NULL, 则不检测元数据库的hash值，这时不能检测
*	资源文件中结构体与当前使用的资源结构是否一致
*note 	当前资源结构DR 元数据描述hash值的获取途径有:
*	-	通过使用md5sum工具利用当前二进制DR元数据库文件生成
*	-	利用tdr工具，将资源结构描述转换成.h头文件时，会将此资源结构
*		DR元数据库的hash值写道头文件中，
*
*note 如果使用虚拟文件来存储存储资源文件，则调用本接口之前必须调用trl_set_fileio_interface，以
*	设置文件IO操作接口
*@see trl_set_fileio_interface
*@retval 正数  读取的资源个数
*@retval 0	文件中没有资源
*@retval 负数， 加载资源失败，可能返回值和错误原因：
*	－ RL_ERROR_OPEN_FILE_FAILED 打开文件读失败
*	-  RL_ERROR_READ_FILE_FAILED	从文件读失败
*	- RL_ERROR_CHECK_FAILE_FAILED		检验资源文件失败，可能资源文件的版本，魔数与本接口定义的不一致
*	- RL_ERROR_NO_MEMORY				分配内存失败
*	- RL_ERROR_DIFF_HASH	资源结构元数据描述库hash不同
*/
int trl_specail_load(IN char* pszBuff, IN int iBuff, IN int iUnit, IN const char* pszFilePath, 
					IN const char *pszMetalibHash, IN int iFlag);


/*  trl_get_string_by_id  查找使用字符串紧凑存储的字符串字段的接口


*@param[in]    pszBuff  : 保存资源数据的数据缓冲区指针
*@param  [in]  iBuff   : 保存资源数据的数据缓冲区的长度。
*@param[in]    iIndex  : 字符串索引ID .
*note   当前获取索引ID的途径有：调用trl_load或者trl_specail_load加载资源后, 可以获取到资源中对应字符串的索引ID
*@ param[in]  iUnitDiff : 指单个资源数据结构体的在内存中的存储空间与实际存储空间的差值，如果此参数的值为0，则单个资源信息结构体的存储空间为实际所需空间。
*@ param[in]  iCount : 资源结构体的个数, 如果iUnitDiff为0，即单个资源信息结构体的存储空间为实际所需空间, 则此参数值为0，否则必须指定资源的个数。
*note: 如果资源调用trl_load接口加载的,则iUnitDiff 、iCount都赋值为0. 
*note: 如果资源是trl_special_load加载并且指定的单个资源的内存存储空间大于资源的实际空间, 必须指定iUnitDiff 、iCount两个参数. 否则iUnitDiff 、iCount赋值为0即可
* @retval 成功,返回字符串的起始地址
* @retval 失败,返回NULL
*/

char* trl_get_string_by_id(IN  char * pszBuff, IN int iBuff , IN int iIndex, IN int iUnitDiff, IN int iCount);


/*  trl_get_strdict_by_res   从使用紧凑存储的资源中，获取字符串字典结构.字典的存储空间在函数内部分配。

*@param[in,out] ppvBuff 保存字符串字典的数据区指针的指针
*@param[out] piBuff	获取数据区的字节数
*@param[in]    pszResBuff  : 保存资源数据的数据缓冲区指针
*@param  [in]  iResBuff   : 保存资源数据的数据缓冲区的长度。
*@ param[in]  iUnit : 指单个资源数据结构体的在内存中的存储空间,请注意不是实际存储空间。
*                             通过trl_special_load接口加载的单个资源结构体的内存存储空间有可能大于实际的存储空间。
*@ param[in]  iCount : 资源结构体的个数
*@ param[in]  iUnitDiff : 指单个资源数据结构体的在内存中的存储空间与实际存储空间的差值，
* 如果此参数的值为0，则单个资源信息结构体的存储空间为实际所需空间。
*note: 如果资源调用trl_load接口加载的,则iUnitDiff 赋值为0
*note: 如果资源是trl_special_load加载的，则iUnitDiff 就是单个资源数据结构体的在内存中的存储空间与实际存储空间的差值

* @retval 成功,返回0
* @retval 失败,返回非0
*/

int trl_get_strdict_by_res(INOUT char **ppvBuff,OUT int *piBuff, IN char * pszResBuff, IN int iResBuff, IN int iUnit,IN int iCount,IN int iUnitDiff);

/*  trl_get_string_by_strdict  从字符串字典中通过字符串索引ID查找字符串

*@param[in]    pszStrDict  : 保存字符串字典的数据缓冲区指针
*@param  [in]  iStrDict   : 保存字符串字典的数据缓冲区的长度。
*@param[in]    iIndex  : 字符串索引ID .
*note   当前获取索引ID的途径有：调用trl_load或者trl_specail_load加载资源后, 可以获取到资源中对应字符串的索引ID
* @retval 成功,返回字符串的起始地址
* @retval 失败,返回NULL
*/
char * trl_get_string_by_strdict(IN  char * pszStrDict, IN int iStrDict ,IN int iIndex);

/* 释放资源所占的缓冲区空间
*@param[in,out] ppvBuff 保存数据的缓冲区指针的指针
*
*/
void trl_unload(INOUT char** ppvBuff);


/** 从二进行资源文件中读取出资源信息头部
*@param[in] a_pszFilePath	资源文件名
*@param[in] a_pstResHead 保存资源信息头部信息的结构体指针
*
*note 如果使用虚拟文件来存储存储资源文件，则调用本接口之前必须调用trl_set_fileio_interface，以
*	设置文件IO操作接口
*@see trl_set_fileio_interface
*retval 0	成功
*@retval 负数  失败,可能返回值和错误原因：
*	－ RL_ERROR_OPEN_FILE_FAILED 打开文件读失败
*	-  RL_ERROR_READ_FILE_FAILED	从文件读失败
*	- RL_ERROR_CHECK_FAILE_FAILED		检验资源文件失败，可能资源文件的版本，魔数与本接口定义的不一致
*/
int trl_read_reshead(IN const char* a_pszFilePath, IN LPTRESHEAD a_pstResHead);




/** @defgroup TRC_LOADR_XML XML格式的资源文件加载接口
*  @{
*/

/** 初始化加载资源的接口
*@param[in] pszResDir	资源文件所在目录
*@param[in] pszTDRFile 描述资源结构体的元数据库文件文件名
*@param[in] cLoadMode	资源文件的格式,其取值为：
*	-	 RL_LOADMODE_XMLV1	  1		读取嵌套结构体以类型名标识的xml资源文件
*	-	 RL_LOADMODE_XMLV2	  2		读取嵌套结构体以成员名标识的xml资源文件
*	-	 RL_LOADMODE_XMLV3	  3		读取嵌套结构体以属性值标识的xml资源文件
*	-	 RL_LOADMODE_BIN	  4	       读取二进制数据格式的资源文件
*@return 成功返回0，失败返回负数
*@note 本接口非线程安全,在多线程环境,应用需自行互斥调用
*@note 目前不支持使用虚拟文件系统来存储xml文件格式的资源文件
*/
int trl_xload_init(IN const char *pszResDir, IN const char *pszTDRFile, IN char cLoadMode);

/** 释放加载资源接口的资源
*/
void trl_xload_fini(void);

/** 从XML格式的资源文件中读取出资源信息头部
*@param[in] a_pszFilePath	资源文件名
*@param[in] a_pstResHead 保存资源信息头部信息的结构体指针
*
*retval 0	成功
*@retval 负数  失败,可能返回值和错误原因：
*	- RL_ERROR_CHECK_FAILE_FAILED		检验资源文件失败，可能资源文件的版本，魔数与本接口定义的不一致
*	- RL_ERROR_FAILED_TO_GET_HEAD_META 	获取TResHead元数据描述句柄失败
*	- RL_ERROR_FAILED_TO_READ_XRESHEAD 	从xml中读取TResHead失败
*/
int trl_xread_reshead(IN const char* a_pszFilePath, IN LPTRESHEAD a_pstResHead);


/** 指定保存资源数据的缓冲大小并指定单个资源数据的内存大小,从资源文件中将资源大小缓冲区中，缓冲区的空间在函数外分配
* 如果资源文件为xml文件，则此文件中必须包含完整的资源头部
*@param[in] pszMetaName 资源结构体的元数据名字字符串的指针
*@param[in] pszBuff 保存数据的缓冲区首地址
*@param[in] iBuff	缓冲区的可用字节数
*@param[in] iUnit	单个资源信息结构体的在缓冲区中的存储空间，通过这个参数调用者可以为每个资源分配比实际存储空间更大的空间。
*@param[in] pszFilePath	资源文件名
*@param[in] iFlag	资源加载控制标识位，此参数可以为0或下列标识位的位'与'值:
*	-	RL_FLAG_INGORE_DIFF_HASH	忽略元数据描述库hash的不同
*@note 使用此接口之前，必须调用trl_xload_init进行初始化
*  @code
	如果资源文件是xml文件，则此xml中的资源数据必须是以下格式：
	<ResExperience_Tab>
		<ResHead version="2147483647">
			<Magic>1397052237 </Magic>
			<Version>2 </Version>
			<Unit>12 </Unit>
			<Count>100 </Count>
			<MetalibHash>b97379b24b0aa935f87b6658190325a5 </MetalibHash>
		</ResHead>
		<ResExperience version="2147483647">
			<Level>1 </Level>
			<Value>10 </Value>
			<Total>0 </Total>
		</ResExperience>
		..........
	</Res_ResExperience>
*  @endcode
*@retval 正数  读取的资源个数
*@retval 0	文件中没有资源
*@retval 负数， 加载资源失败
*
*@see trl_xload_init
*/
int trl_xload_with_reshead(IN const char *pszMetaName, IN char* pszBuff, IN int iBuff, IN int iUnit, 
						  IN const char* pszFilePath, IN int iFlag);



/** 指定保存资源数据的缓冲大小并指定单个资源数据的内存大小,从资源文件中将资源大小缓冲区中，缓冲区的空间在函数外分配
*@param[in] pszMetaName 资源结构体的元数据名字字符串的指针
*@param[in] pszBuff 保存数据的缓冲区首地址
*@param[in] iBuff	缓冲区的可用字节数
*@param[in] iUnit	指定单个资源数据结构体的在内存中的存储空间，通过这个参数调用者可以为每个资源分配比实际需要存储空间更大的空间。
如果此参数的值为0，则单个资源信息结构体的存储空间为实际所需空间
*@param[in] pszFilePath	资源文件名
*@param[in] iFlag	资源加载控制标识位，此参数可以为0或下列标识位的位'与'值:
*	-	RL_FLAG_INGORE_DIFF_HASH	忽略元数据描述库hash的不同
*
*@note 使用此接口之前，必须调用trl_xload_init进行初始化
*
*  @code
	如果资源文件是xml文件，则此xml中的资源数据必须是以下格式：
	<ResExperienceTab>
		<Total>100</Total>		
		<ResExperience version="2147483647">
			<Level>1 </Level>
			<Value>10 </Value>
			<Total>0 </Total>
		</ResExperience>
		..........
	</ResExperienceTab>
*  @endcode
*@retval 正数  读取的资源个数
*@retval 0	文件中没有资源
*@retval 负数， 加载资源失败
*
*@see trl_xload_init
*/
int trl_xload(IN const char *pszMetaName, IN char *pszBuff, IN int iBuff, IN int iUnit, IN const char* pszFilePath, IN int iFlag);

/** @} */  /*TRC_LOADR_XML XML格式的资源文件加载接口 */



/** @defgroup TRC_RES_FIND 资源查找
*  @{
*/

/** 根据资源信息的关键字信息，在资源文件查找特定资源信息
*@param[in] pszBuff 保存资源数据的缓冲区首地址
*@param[in] iCount	此缓冲区中保存的资源信息结构体的个数
*@param[in] iUnit	单个资源信息结构体的在缓冲区中的存储空间
*@param[in] iKey	查找关键字
*@return 如果成功找到返回此资源信息的首地址，否则返回NULL
*
*@pre \e 此资源信息结构的第一个成员作为查找关键字，其类型必须是整数类型。
*/
char* trl_find(char* pszBuff, int iCount, int iUnit, int iKey);

/** 根据资源信息的关键字信息，在资源文件查找特定资源信息
*@param[in] pszBuff 保存资源数据的缓冲区首地址
*@param[in] iCount	此缓冲区中保存的资源信息结构体的个数
*@param[in] iUnit	单个资源信息结构体的在缓冲区中的存储空间
*@param[in] llKey	查找关键字
*@return 如果成功找到返回此资源信息的首地址，否则返回NULL
*
*@pre \e 此资源信息结构的第一个成员作为查找关键字，其类型必须是长整数类型。
*/
char* trl_find_ll(char* pszBuff, int iCount, int iUnit, tdr_longlong llKey);


/** 根据资源信息的关键字信息，在资源文件查找特定资源信息
*@param[in] pszBuff 保存资源数据的缓冲区首地址
*@param[in] iCount	此缓冲区中保存的资源信息结构体的个数
*@param[in] iUnit	单个资源信息结构体的在缓冲区中的存储空间
*@param[in] nKey	查找关键字
*@return 如果成功找到返回此资源信息的首地址，否则返回NULL
*
*@pre \e 此资源信息结构的第一个成员作为查找关键字，其类型必须是短整数类型。
*/
char* trl_find_n(char* pszBuff, int iCount, int iUnit, short nKey);

/**  @}*/
#ifdef __cplusplus
}
#endif

#endif /* TRESLOADER_H */
