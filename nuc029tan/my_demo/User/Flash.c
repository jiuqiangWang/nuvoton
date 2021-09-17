#include "flash.h"
#include <stdio.h>


/*NUC029SEE
  128K�ֽ�Ӧ�ó���洢�ռ�(APROM)
	�����õ�����FLASH,ҳ������ԪΪ512�ֽ� ,��255ҳ
	�ֽڴ洢��ʽ��С��ģʽ
	���ݴ洢��ʽ:
	  ҳ��ַ        ����       			 SRAM��ַ(char�����±�)      ���� 
		0X00				��д����Count				   	0x00          					a
		0X02		      a��ַ					   			0x01          					b
		0X03				  a							  	 		0x02			    					c
		0X04					b��ַ							
		0X05					b				    	
		0X06					c��ַ				
		0X07					c								
*/
/*
   2�ֽڲ�д����+1�ֽڵ�ַ����+1�ֽ�����
  ����ܳ���512/2-2=254
*/
#define 	Max_Amount_of_Data   254

/*Flash������ʼ��ַ
	
	NUC029TAN��ROMΪ32K
	0x0000 0000 ~0x0000 7fff  ��63��
		��63-55��*512=4K��ʹ��4K��ΪFlash�洢����ʣ��Ϊ����洢��
*/
#define  DataFlash_BaseAddr   	(uint32_t)(FMC_FLASH_PAGE_SIZE *55)
#define  Status_Unwritten				(uint16_t)0xffff

#define  Even_Addr_Mask       	(uint32_t)0x00ff0000
#define  Even_Data_Mask       	(uint32_t)0xff000000
#define  Even_Addr_Pos        	(uint8_t)16
#define  Even_Data_Pos        	(uint8_t)24

#define  Odd_Addr_Mask       		(uint32_t)0x000000ff
#define  Odd_Data_Mask       		(uint32_t)0x0000ff00
#define  Odd_Addr_Pos       	 	(uint8_t)0
#define  Odd_Data_Pos        		(uint8_t)8


/*�����*/
#define Err_OverAmountData 	-1
#define Err_OverPageAmount 	-2
#define Err_ErrorIndex 		  -3

unsigned char Written_Data[200];					 //���ݻ���
static uint32_t  Amount_of_Data=0;
static uint32_t  Amount_Pages=0;					
static uint8_t   Current_Valid_Page=0;    //�ҳ
static uint16_t  Current_Cursor=0;			  //Flashλ�ù��





/**
* @brief Initial Data Flash as EEPROM.
* @param[in] data_size: The amount of user's data, unit in byte. The maximun amount is 
 128.
* @param[in] use_pages: The amount of page which user want to use.
* @retval Err_OverPageSize: The amount of user's data is over than the maximun amount.
* @retval Err_OverPageAmount: The amount of page which user want to use is over than 
 the maximun amount.
* @retval 0: Success
*/


int  Init_EEPROM(uint32_t data_amount, uint32_t use_pages)
{
		uint32_t i;
	
		/* The amount of data includes 1 byte address and 1 byte data */
		Amount_of_Data = data_amount;
		/* The amount of page which user want to use */
		Amount_Pages = use_pages;
		/* Check setting is valid or not */
		/* The amount of user's data is more than the maximun amount or not */
	  /*���ݻ��岻�ܳ���Max_Amount_of_Data*/
		if(Amount_of_Data > Max_Amount_of_Data)
			return Err_OverAmountData;
		/* For M051 Series, the max. amount of Data Flash pages is 8 */
		/*������дҳ���ܳ���8*/
		if(Amount_Pages > 8)
			return Err_OverPageAmount;
		/* Init SRAM for data */
		/*���ö��ڴ棬����Ч�ʵ�*/
			//Written_Data = (uint8_t *)malloc(sizeof(uint8_t) * Amount_of_Data);  
		/* Fill initial data 0xFF*/
		for(i = 0; i < Amount_of_Data; i++)
		{
				Written_Data[i] = 0xFF;
		}
		return 0;
}

/**
* @brief Search which page has valid data and where is current cursor for the next 
data to write.
         ��궨λ���������ݶ���������Written_Data��
*/
void Search_Valid_Page(void)
{
		uint32_t i, temp;
		uint8_t addr, data;
		uint16_t Page_Status_ptr[10]={0,0,0,0,0,0,0,0,0,0};
		/* Enable FMC ISP function */
		SYS_UnlockReg();
		FMC_Open();
	  FMC_EnableAPUpdate();
		/* Set information of each pages to Page_Status */
	  /*��ʹ�ö��ڴ棬Ч�ʵ�*/
		//Page_Status_ptr = (uint16_t *)malloc(sizeof(uint16_t) * Amount_Pages);
		for(i = 0; i < Amount_Pages; i++)
		{
				Page_Status_ptr[i] = (uint16_t)FMC_Read(DataFlash_BaseAddr 
															+ (FMC_FLASH_PAGE_SIZE * i));
		}
		/* Search which page has valid data */
		for(i = 0; i < Amount_Pages; i++)
		{
				if(Page_Status_ptr[i] != Status_Unwritten)
				Current_Valid_Page = i;
		}
		/* If Data Flash is used for first time, set counter = 0 */
		if(Page_Status_ptr[Current_Valid_Page] == Status_Unwritten)
		{
				/* Set counter = 0 */
				FMC_Write(DataFlash_BaseAddr + (FMC_FLASH_PAGE_SIZE * 
				Current_Valid_Page), 0xFFFF0000);
					/* Set cursor to current Data Flash address */
				Current_Cursor = 2;
				
		}
		else
		{
				/* Search where is current cursor for the next data to write and get the 
				data has been written */
				/* Check even value */
				temp = FMC_Read(DataFlash_BaseAddr + (FMC_FLASH_PAGE_SIZE * 
				Current_Valid_Page));
				addr = (temp & Even_Addr_Mask) >> Even_Addr_Pos;
				data = (temp & Even_Data_Mask) >> Even_Data_Pos;
				/* Check Address is 0xFF (un-written) of not */
				if(addr == 0xFF)
				{
						/* If Address is 0xFF, then set cursor to current Data Flash 
						address */
						Current_Cursor = 2;
					
				}
				else
				{
						/* Copy the address and data to SRAM */
						Written_Data[addr] = data;
			
						/* Check the whole Data Flash */
						for(i = 4; i < FMC_FLASH_PAGE_SIZE; i += 4)
						{
								/* Check odd value */
								temp = FMC_Read(DataFlash_BaseAddr
																+ (FMC_FLASH_PAGE_SIZE * Current_Valid_Page) + i);
								addr = (temp & Odd_Addr_Mask) >> Odd_Addr_Pos;
								data = (temp & Odd_Data_Mask) >> Odd_Data_Pos;
		
								/* Check Address is 0xFF (un-written) of not */
								if(addr == 0xFF)
								{
										/* If Address is 0xFF, then set cursor to 
										current Data Flash address */
										Current_Cursor = i;
										break;
								}
								else
								{
										/* Copy the address and data to SRAM */
										Written_Data[addr] = data;
								}
								/* Check even value */
								addr = (temp & Even_Addr_Mask) >> Even_Addr_Pos;
								data = (temp & Even_Data_Mask) >> Even_Data_Pos;
								/* Check Address is 0xFF (un-written) of not */
								if(addr == 0xFF)
								{
										/* If Address is 0xFF, then set cursor to 
										current Data Flash address */
										Current_Cursor = i + 2;
										break;
								}
								else
								{
										/* Copy the address and data to SRAM */
										Written_Data[addr] = data;
								} 
						} 
				} 
		} 
		 /* Disable FMC ISP function */
		FMC_Close();
		FMC_DisableAPUpdate();
		SYS_LockReg();
}

/* Enable FMC ISP function */
/**
* @brief Manage the valid data from SRAM to new page.
*/
void Manage_Next_Page(void)
{
		uint32_t i = 0, j, counter, temp = 0, data_flag = 0, new_page;
		/* Copy the valid data (not 0xFF) from SRAM to new valid page */
		/* Get counter from the first two bytes */
		counter = FMC_Read(DataFlash_BaseAddr + (FMC_FLASH_PAGE_SIZE * 
		Current_Valid_Page));
		/* If current valid page is the last page, choose the first page as valid page */
		if((Current_Valid_Page + 1) == Amount_Pages)
		{
			new_page = 0;
			/* Add counter to record 1 E/W cycle finished for all pages */
			counter++;
		}
		else
		{
			new_page = Current_Valid_Page + 1;
		}
		/* Enable FMC ISP function */
		FMC_Open();
		/* Copy first valid data */
		while(1)
		{
				/* Not a valid data, skip */
				if(Written_Data[i] == 0xFF)
				{
						i++;
				}
				/* Combine counter and first valid data, and write to new page */
				else
				{
						counter &= ~(Even_Addr_Mask | Even_Data_Mask);
						counter |= (i << Even_Addr_Pos) | (Written_Data[i] << 
						Even_Data_Pos);
						FMC_Write(DataFlash_BaseAddr + (FMC_FLASH_PAGE_SIZE * new_page), 
						counter);
						i++;
						break;
				} 
		}
		/* Copy the rest of data */
		for(j = 4; i < Amount_of_Data; i++)
		{
				/* Not a valid data, skip */
				if(Written_Data[i] == 0xFF)
				{
					continue;
				}
				/* Write to new page */
				else
				{
						/* Collect two valid data and write to Data Flash */
						/* First data, won't write to Data Flash immediately */
						if(data_flag == 0)
						{
								temp |= (i << Odd_Addr_Pos) | (Written_Data[i] << 
								Odd_Data_Pos);
								data_flag = 1;
						}
						/* Second data, write to Data Flash after combine with first 
						data */
						else
						{
								temp |= (i << Even_Addr_Pos) | (Written_Data[i] << 
								Even_Data_Pos);
								FMC_Write(DataFlash_BaseAddr + (FMC_FLASH_PAGE_SIZE * 
								new_page) + j, temp);
								temp = 0;
								data_flag = 0;
								j += 4;
						}
		
				}	 
		}
		/* Set cursor to new page */
		Current_Cursor = j;
		/* If there is one valid data left, write to Data Flash */
		if(data_flag == 1)
		{
				temp |= 0xFFFF0000;
				FMC_Write(DataFlash_BaseAddr + (FMC_FLASH_PAGE_SIZE * new_page) + j, 
				temp);
				Current_Cursor += 2;
		}
		/* Erase the old page */
		FMC_Erase(DataFlash_BaseAddr + (FMC_FLASH_PAGE_SIZE * Current_Valid_Page));
		/* Point to new valid page */
		Current_Valid_Page = new_page;
		FMC_Close();
}

/**
* @brief Write one byte data to SRAM and current valid page.
If this index has the same data, it will not changed in SRAM and Data Flash.
If current valid page is full, execute Manage_Next_Page() to copy valid data 
 to next page.
* @param[in] index: The index of data address.
* @param[in] data: The data that will be writeen.
* @retval Err_ErrorIndex: The input index is not valid.
* @retval 0: Success
*/
int  Write_Data(uint8_t index, uint8_t data)
{
		uint32_t temp = 0;
		/* Check the index is valid or not */
		if(index > Amount_of_Data)
		{
	
				return Err_ErrorIndex;
		}
		/* If the writing data equals to current data, the skip the write process */
		if(Written_Data[index] == data)
		{
				
				return 0;
		}
		/* Enable FMC ISP function */
		SYS_UnlockReg();
		FMC_Open();
	  FMC_EnableAPUpdate();
		/* Current cursor points to odd position*/
		if((Current_Cursor & 0x3) == 0)
		{
				/* Write data to Data Flash */
				temp = 0xFFFF0000 | (index << Odd_Addr_Pos) | (data << Odd_Data_Pos);
				FMC_Write(DataFlash_BaseAddr + (FMC_FLASH_PAGE_SIZE * Current_Valid_Page) 
				+ Current_Cursor, temp);
				/* Write data to SRAM */
				Written_Data[index] = data;
				
		}
		/* Current cursor points to even position*/
		else
		{
				/* Read the odd position data */
				temp = FMC_Read(DataFlash_BaseAddr + (FMC_FLASH_PAGE_SIZE * 
				Current_Valid_Page) + (Current_Cursor - 2));
				/* Combine odd position data and even position data */
				temp &= ~(Even_Addr_Mask | Even_Data_Mask);
				temp |= (index << Even_Addr_Pos) | (data << Even_Data_Pos);
				/* Write data to Data Flash */
				FMC_Write(DataFlash_BaseAddr + (FMC_FLASH_PAGE_SIZE * Current_Valid_Page) 
				+ (Current_Cursor - 2), temp);
				/* Write data to SRAM */
				Written_Data[index] = data;
			
		}
		/* If current cursor points to the last position, then execute Manage_Next_Page() 
		*/
		if(Current_Cursor == (FMC_FLASH_PAGE_SIZE - 2))
		{
				/* Copy valid data to next page */
				Manage_Next_Page();
			
		}
		/* Add current cursor */
		else
		{
				/* Set current cursor to next position */
				Current_Cursor += 2;
		}
		 /* Disable FMC ISP function */
	  FMC_Close();
		FMC_DisableAPUpdate();
		SYS_LockReg();
		return 0;
}
/**
* @brief Read one byte data from SRAM.
* @param[in] index: The index of data address.
* @param[in] data: The data in the index of data address from SRAM.
* @retval Err_ErrorIndex: The input index is now valid.
* @retval 0: Success
*/
int  Read_Data(uint8_t index, uint8_t *data)
{
		/* Check the index is valid or not */
		if(index >= Max_Amount_of_Data)
		{
				return Err_ErrorIndex;
		}
		/* Get the data from SRAM */
		*data = Written_Data[index];
		return 0;
}



/**
* @brief Get the cycle counter for how many cycles has page been erased/programmed.
* @retval Cycle_Conter: The cycles that page has been erased/programmed.
*/
uint16_t Get_Cycle_Counter(void)
{
		uint16_t Cycle_Counter;
		/* Get the cycle counter from first two bytes in current Data Flash page */
		Cycle_Counter = (uint16_t)FMC_Read(DataFlash_BaseAddr + (FMC_FLASH_PAGE_SIZE * 
		Current_Valid_Page));
		return Cycle_Counter;
}


/**
* @brief�������û�Flash�����лҳ��������Ƿ�Ϊ0Xffffffff
* @param[in] :None
* @retvald. 0:�ɹ� -1:ʧ��
*/
int Erase_UserPage(void)
{
    unsigned int Data=0;
	  int  i,j;
	  /* Enable FMC ISP function */
		SYS_UnlockReg();
		FMC_Open();
	  FMC_EnableAPUpdate();
	  for(i=0;i<Amount_Pages;++i)
	  {
				FMC_Erase(DataFlash_BaseAddr + (FMC_FLASH_PAGE_SIZE * i)); 
			  for(j=0;j<FMC_FLASH_PAGE_SIZE;j+=4)
			  {
						Data= FMC_Read(DataFlash_BaseAddr + (FMC_FLASH_PAGE_SIZE * i)+j);
					  if(Data!=0xffffffff)
						{
							    /* Disable FMC ISP function */
									FMC_Close();
									SYS_LockReg();
									FMC_DisableAPUpdate();
						     return -1;
						}
				}
		}
		 /* Disable FMC ISP function */
	  FMC_Close();
		FMC_DisableAPUpdate();
		SYS_LockReg();
		return 0;
}


#if 0
/*ֱ�Ӷ�ȡ�ṹ�����ݼ���*/
int Read_StructData(void)
#endif


/**
* @brief����Falsh��ȡ�����ݴ����ɽṹ������,
* @param[in] :Falsh_Data:�洢Falsh�ṹ��ĵ�ַ
							size:�ṹ���С
* @retvald. 0:�ɹ� -1:ʧ��
	@note:�ṹ���С���ܳ���100�ֽ�
*/

int Read_StructFullData(void* Falsh_Data,uint8_t size)
{
	  char * Data=(char *)Falsh_Data;
		uint8_t i;
	  /*����Ҫ�в�����ȫ�ж�
	  */
	  if(size>Amount_of_Data)
			   return -1;
		for(i=0;i<size;++i)
	  {
				*Data=Written_Data[i];
				 Data++;
		}  
    return 0;
}

/**
* @brief��    ��һ���ṹ��ȫ����Աд��FLASH����,
* @param[in] :Falsh_Data:�洢Falsh�ṹ��ĵ�ַ
							size:�ṹ���С
* @retvald. 0:�ɹ� -1:ʧ��
	@note:�ṹ���С���ܳ���100�ֽ�
*/

int Write_StructFullData(void* Falsh_Data,uint8_t size)
{
	  char * Data=(char *)Falsh_Data;
	  uint8_t i;
	  /*����Ҫ�в�����ȫ�ж�
	  */
		if(size>Amount_of_Data)
			   return -1;
	  for(i=0;i<size;++i)
	  {
				Write_Data(i,*Data);
			  Data++;
			
		}  
		return 0;
}

/**
* @brief��    ��һ���ṹ���Աд��FLASH����,
* @param[in] :Falsh_Data:�洢Falsh�ṹ��ĵ�ַ
							memberoffset:�ṹ���Ա�ĵ�ַƫ�Ƴ���
													 ʹ�ô˺궨�壺#define MemberOffsetByte(type,member)   (unsigned long)(&((type*)0)->member)
							membersize���ṹ���Ա��С
* @retvald. 0:�ɹ� -1:ʧ��
	@note:�ṹ���С���ܳ���100�ֽ�  
				�ṹ���Ա��С���ܳ���8�ֽ�
*/
int Write_StructData(void* Falsh_Data,uint8_t memberoffset,uint8_t membersize)
{
	  char * Data=(char *)Falsh_Data;
	  uint8_t  pos;
	  uint8_t i;
	
		if(membersize>8)
			   return -1;
		if(membersize>Amount_of_Data)
			   return -1;
	  for(i=0;i<memberoffset;++i)
		    Data++;
	  for(i=0;i<membersize;++i)
	  {
			 pos=i+memberoffset;
		   Write_Data(pos,*Data);
			 Data++;
		}
		return 0;
}
















