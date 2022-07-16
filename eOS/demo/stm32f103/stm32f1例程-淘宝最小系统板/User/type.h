/*深圳宝智达程序  版权所有，未经允许，禁止转载使用*/
#ifndef _TYPE_H_
#define _TYPE_H_
//数据类型  (不同的mcu，数据类型可能不同)

/*-------移植修改部分-------*/
typedef unsigned char ubool;

typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef signed long s64;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long u64;


typedef volatile signed char vs8;
typedef volatile signed short vs16;
typedef volatile signed int vs32;
typedef volatile signed long vs64;

typedef volatile unsigned char vu8;
typedef volatile unsigned short vu16;
typedef volatile unsigned int vu32;
typedef volatile unsigned long vu64;



#define FALSE	0
#define TRUE	1
/*-------------------------*/

#endif
