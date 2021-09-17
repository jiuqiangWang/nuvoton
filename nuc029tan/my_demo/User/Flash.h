#ifndef __FLASH__H__
#define __FLASH__H__

#include "NUC029XAN.h"

/*��ȡ�ṹ���Ա��ƫ�Ƶ�ַ����*/
#define MemberOffsetByte(type,member)   (unsigned long)(&((type*)0)->member)
/*��ȡ�ṹ���Ա�ĳ���*/
#define MemberSizeByte(type,member)  	  (unsigned int)(sizeof(type.member))


/*
		Falsh�ṹ��
		ע�����Ϊ4�ֽڶ���Ľṹ��
*/
extern int  Init_EEPROM(uint32_t data_amount, uint32_t use_pages);
extern void Search_Valid_Page(void);
extern int  Write_Data(uint8_t index, uint8_t data);
extern int  Read_Data(uint8_t index, uint8_t *data);
extern uint16_t Get_Cycle_Counter(void);
extern int Erase_UserPage(void);
extern int Read_StructFullData(void* Falsh_Data,uint8_t size);
extern int Write_StructFullData(void* Falsh_Data,uint8_t size);
extern int Write_StructData(void* Falsh_Data,uint8_t memberoffset,uint8_t membersize);

#endif





