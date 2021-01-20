#include "buffer.h"
/* File Name: buffer.c
*Comiler : Visual Studio 2019
* Author: Andy Yang  ID#: 040947248
*		  Wenhao Li  ID#: 040941575
* Course: CST 8152 -Compilers, Lab section 012
* Date: 8 Jun 2020
* CST8152-Assignment #1.
* Professor:Svilen Ranev & Paulo Sousa
* Purpose: using dynamic structures(buffer) with C to implement a program running as buffer.
* Function list: b_allocate(), b_addc(), b_clear(), b_free(), b_isfull(), b_limit(), b_capacity(), b_mark(),
				 b_mode(), b_incfactor(), b_load(), b_isempty(), b_getc(), b_eob(), b_print(), b_compact(),
				 b_rflag(), b_retract(), b_reset(), b_getcoffset(), b_rewind(), b_location()
*/
Buffer* b_allocate(short init_capacity, char inc_factor, char o_mode) {

	if (init_capacity < 0 || init_capacity > MaxAllowV)/*check if init_capacity is valid, if invalid return null*/
		return NULL;
	Buffer* temp = calloc(One, sizeof(Buffer));
	if (!temp)
		return NULL;
	if (init_capacity == 0) {
		char* ctemp = (char*)malloc(sizeof(char) * 200); /*initialized a char pointer with size 200*/
		if (!ctemp) {
			free(temp);
			temp = NULL;
			return NULL;
		}
		if (o_mode == 'a' || o_mode == 'm') {
			inc_factor = 15;
			temp->inc_factor = (unsigned char)inc_factor;
		}
		else if (o_mode == 'f')    /*if in mode 'fixed size', set increment factor to 0 */
			temp->inc_factor = 0;
		else {
			free(temp);            /*free pointers and set o null before return*/
			temp = NULL;
			free(ctemp);
			ctemp = NULL;
			return NULL;
		}
		init_capacity = 200;	   /*set init_capacity with malloced size*/
		temp->cb_head = ctemp;
	}
	if (o_mode == 'f') {
		if (init_capacity > Zero) {
			char* ctemp = (char*)malloc(sizeof(char) * init_capacity); /*char temp with initalized size of init_capacity*/
			if (!ctemp) {											   /*if fail to malloc, free temp and return null*/
				free(temp);
				temp = NULL;
				return NULL;
			}
			temp->cb_head = ctemp;
		}
		temp->mode = Fixed;
		temp->inc_factor = 0;

	}
	else if ((unsigned char)inc_factor == 0 && init_capacity > 0) {   /*if inc_factor is 0 and init_capactiy is positive value*/
		temp->mode = Fixed;
		temp->inc_factor = 0;
		char* ctemp = (char*)malloc(sizeof(char) * init_capacity);		/*char temp with initialized size of init_capacity*/
		if (!ctemp) {
			free(temp);
			temp = NULL;
			return NULL;
		}
		temp->cb_head = ctemp;
		temp->capacity = init_capacity;
	}
	else if (o_mode == 'a' && (unsigned char)inc_factor >= One && (unsigned char)inc_factor <= 255) {  /*range 1-255*/
		if (init_capacity > Zero) {
			char* ctemp = (char*)malloc(sizeof(char) * init_capacity);
			if (!ctemp) {
				free(temp);
				temp = NULL;
				return NULL;
			}
			temp->cb_head = ctemp;
		}
		temp->mode = Additive;
		temp->inc_factor = (unsigned char)inc_factor;
	}
	else if (o_mode == 'm' && (unsigned char)inc_factor >= One && (unsigned char)inc_factor <= Hundred) {
		/*if in multiplicative mode and inc_factor is in valid range 1-100*/

		if (init_capacity > 0) {
			char* ctemp = (char*)malloc(sizeof(char) * init_capacity);               /*char temp with initalized size of init_capacity*/
			if (!ctemp) {
				free(temp);
				temp = NULL;
				return NULL;
			}
			temp->cb_head = ctemp;
		}
		temp->mode = Multiplicative;									/*set mode as -1 indicating in mutipl self-increamting*/
		temp->inc_factor = (unsigned char)inc_factor;
	}
	else {
		free(temp);						/*if fail to malloc, free temp and return null*/
		temp = NULL;
		return NULL;
	}
	temp->capacity = init_capacity;
	temp->flags = DEFAULT_FLAGS;        /*initizalize flag to the default flag*/
	return temp;						/*return successfully alloced buffer pointer*/
}


/********************************************************************************
Purpose: add parameter symbol in char type into the buffer
Author: Andy Yang
Called fnctions: realloc()
Parameters: pBuffer const pBD:pointer to a buffer descriptor
			char symbol: character to be added into the buffer descriptor
return value: pBDin type of pBuffer
Algorithm: *check if pBD exists, otherwise return NULL
		   *Resets Rflag
		   *if cd_head(buffer descripto)exists and is not full (addoffset is smaller than capacity),
		   add symbol at index of add_offset and return pBD after increasing add_offset by 1
		   Fixed mode: not appliable to be increased , return NULL
***********************************************************************************/
pBuffer b_addc(pBuffer const pBD, char symbol) {
	if (!pBD)                          /*will return null when runtime error*/
		return NULL;
	pBD->flags &= RESET_R_FLAG;			/*reset rflag before realloc*/

	if (pBD->cb_head) {
		if (pBD->addc_offset >= Zero && pBD->addc_offset < pBD->capacity) {   /*if buffer is not full, addoffset is smaller than capacity*/
			*(pBD->cb_head + pBD->addc_offset) = symbol;
			++pBD->addc_offset;									/*increase add_offset by 1*/
			return pBD;
		}
	}
	int newCapacity = pBD->capacity;			/*set an int varable to initally hold the capacity*/
	if (pBD->mode == Fixed)						/*in fixed mode, buffer didnt need to be increaed*/
		return NULL;
	else if (pBD->mode == Additive) {
		newCapacity += (unsigned char)(pBD->inc_factor);		/*increase newCapacity by adding the increasing factor*/
		if ((short)newCapacity >= Zero && newCapacity <= MaxAllowV) {
			/*if newCapacity(cast to short type to check if it's overflowed) is in valid range after adding ,then increase buffer size by using realloc*/

			char* temp = realloc(pBD->cb_head, newCapacity);
			if (!temp)											/*if unsuccessfully enlarged*/
				return NULL;

			if (temp != pBD->cb_head)							/*check if size has been changed, set rflag on success*/
				pBD->flags |= RESET_R_FLAG;

			pBD->cb_head = temp;								/*let cd_head point back after realloc*/
			pBD->capacity = (short)newCapacity;
			*(pBD->cb_head + pBD->addc_offset) = symbol;
			++pBD->addc_offset;
		}
		else if ((short)newCapacity >= MaxAllowV) {
			newCapacity = MaxAllowV;							/*assign the max allow value to newcapacity in order to increase size later*/
			char* temp = realloc(pBD->cb_head, newCapacity);	/*begin(process to realloc and store new char into buffer*/
			if (!temp)											/*if fail , realloc will return NULL*/
				return NULL;

			if (temp != pBD->cb_head)							/*if realloc did make a change , set rflag*/
				pBD->flags |= RESET_R_FLAG;

			pBD->cb_head = temp;
			pBD->capacity = (short)newCapacity;
			*(pBD->cb_head + pBD->addc_offset) = symbol;
			++pBD->addc_offset;									/*end (process to realloc and store new char into the buffer*/
		}
		else if ((short)newCapacity < Zero)
			return NULL;
	}
	else if (pBD->mode == Multiplicative) {
		if (pBD->capacity >= MaxAllowV)							/*if capacity can not increased*/
			return NULL;
		int availableSpace = MaxAllowV - pBD->capacity;
		int newIncre = availableSpace * (unsigned char)pBD->inc_factor / Hundred;	/*calulate space to increase buffer*/
		newCapacity = newIncre + pBD->capacity;					/*get newCapacity to be increased*/

		if ((short)newCapacity > pBD->capacity && newCapacity <= MaxAllowV) { /*if capacity in pBD can be increaed and after increase the size of it would be in range*/

			char* temp = realloc(pBD->cb_head, newCapacity);
			if (!temp)
				return NULL;

			if (temp != pBD->cb_head)
				pBD->flags |= RESET_R_FLAG;						/*IF REALLOC DID MAKE A change set rflag*/

			pBD->cb_head = temp;
			pBD->capacity = (short)newCapacity;
			*(pBD->cb_head + pBD->addc_offset) = symbol;
			++pBD->addc_offset;
		}
		else if (newCapacity >= MaxAllowV || newCapacity == pBD->capacity) {
			newCapacity = MaxAllowV;
			char* temp = realloc(pBD->cb_head, newCapacity);
			if (!temp)
				return NULL;

			if (temp != pBD->cb_head)
				pBD->flags |= RESET_R_FLAG;						/*if realloc did make a change ,set rflag*/

			pBD->cb_head = temp;
			pBD->capacity = (short)newCapacity;
			*(pBD->cb_head + pBD->addc_offset) = symbol;
			++pBD->addc_offset;									/*end(process to realloc and store new char into the buffer*/
		}
		else
			return NULL;										/*any other unrecognized condition will be returned*/
	}
	return pBD;													/*return pBD after increase*/
}

/********************************************************************************
Purpose: clear al appliable data(reset all properties in the buffer pointer except the buffer descriptor and mode)
Author:  Andy Yang
Called fnctions: none
Parameters : pBuffer const
	   pBD : pointer to a buffer descriptor
return value: pBD in type of pBuffer
Algorithm:   *if pBD doesn't exist, return -1
			 *reset capacity to 0
			 *reset addc_offset, markc_offset, getc_offset to 0
			 *reset flag to default flag
			 *return 0 on success
***********************************************************************************/

int b_clear(Buffer* const pBD) {
	if (!pBD)							/*any reuntime error will return -1*/
		return RT_FAIL_1;
	pBD->addc_offset = Zero;
	pBD->getc_offset = Zero;
	pBD->markc_offset = Zero;
	pBD->flags &= RESET_EOB;
	pBD->flags &= RESET_R_FLAG;
	return Zero;						/*return 0 on success*/
}

/********************************************************************************
Purpose: free pBD and cd_head
Author : Andy Yang
Called fnctions: none
Parameters : pBuffer const
	   pBD : pointer to a buffer descriptor
return value: none
Algorithm: *if pBD and cd_head exist,
		   *free cd_head
		   *assign null to cd_head to avoid dangling pointer
		   *free pBD
***********************************************************************************/

void b_free(Buffer* const pBD) {
	if (pBD) {
		if (pBD->cb_head)
			free(pBD->cb_head);
		pBD->cb_head = NULL;
		free(pBD);								/*avoid dangling pointer*/
	}											/*unable to assggn NULL to constant pointer*/
}

/********************************************************************************
Purpose: check if buffer is full
Author: Andy Yang
Called fnctions: none
Parameters : pBuffer const
	   pBD : pointer to a buffer descriptor
return value: an integer
Algorithm: *if pBD does not exist, return -1
		   *if add_offset reach the end , return 1 to indicate full
		   *otherwise, return 0 to indicate not full
***********************************************************************************/

int b_isfull(Buffer* const pBD) {
	if (!pBD)									/*any runtime error will return -1*/
		return RT_FAIL_1;
	if (pBD->addc_offset == pBD->capacity)		/*if it;s full, return 1*/
		return One;
	else return Zero;							/* not full , return 0*/
}

/***********************************************************************************
Purpose: get current size of the buffer
Author: Andy Yang
Called functions:none
Parameters : pBuffer const
	   pBD : pointer to a buffer descriptor
return:  short integer
Algorithm: *if pBD does not exist, return -1
			*return addc_offset which can be an indicator for the size
***********************************************************************************/
short b_addcoffset(Buffer* const pBD) {
	if (!pBD)
		return RT_FAIL_1;						/*any runtime error will return -1*/
	return pBD->addc_offset;
}


/***********************************************************************************
Purpose: get current capacity of buffer
Author: Andy Yang
Called functions:none
Parameters : pBuffer const
	   pBD : pointer to a buffer descriptor
return:  short integer
Algorithm:  *if pBD does not exist, return -1
			*return capacit in buffer discriptor
***********************************************************************************/

short b_capacity(Buffer* const pBD) {
	if (!pBD)									/*any runtime error will return -1*/
		return RT_FAIL_1;
	return pBD->capacity;
}

/***********************************************************************************
Purpose: get specified index
Author: Andy Yang
Called functions:none
Parameters : pBuffer const
	   pBD : pointer to a buffer descriptor
	   mark: short type integer
return:  short integer
Algorithm:   *if pBD does not exist, return -1
			 *return getc_offset after it was assigned with value of mark
***********************************************************************************/

short b_markc(Buffer* const pBD, short mark) {
	if (!pBD || mark<0 || mark>pBD->addc_offset)		/*any runtime error will return -1*/
		return RT_FAIL_1;
	pBD->markc_offset = mark;							/*assign mark to markc_offset*/
	return pBD->markc_offset;
}

/***********************************************************************************
Purpose: to get current mode
Author: Andy Yang
Called functions:none
Parameters : pBuffer const
	   pBD : pointer to a buffer descriptor

return: an integer
Algorithm:  *if pBD does not exist, return -1
			*return mode value inside buffer discriptor
***********************************************************************************/

int b_mode(Buffer* const pBD) {

	if (!pBD)
		return RT_FAIL_2;								/*any runtime error will return -1*/
	return pBD->mode;
}


/***********************************************************************************
Purpose: to get increment factor
Author: Andy Yang
Called functions:none
Parameters : pBuffer const
	   pBD : pointer to a buffer descriptor

return:  unsigned integer
Algorithm: *if pBD does not exist, return -1
			*return inc_factor cast to unsigned char type
***********************************************************************************/

size_t b_incfactor(Buffer* const pBD) {
	if (!pBD || (unsigned char)pBD->inc_factor < 0)
		return HXV256(0x100);							/*any runtime error will return hex value of 0x100*/
	return (unsigned char)pBD->inc_factor;				/*cast before retunning to make sure it's an positive value*/
}




/***********************************************************************************
Purpose: to read content inside a file and store charactor one by one into the buffer descriptor
Author: Andy Yang
Called functions:none
Parameters : pBuffer const, file pointer
	   pBD : pointer to a buffer descriptor

return:  an integer
Algorithm: *if pBD doesn't exist or file pointer doesn't exist, return -2
		   *set a char vairable with initilized value of 0
		   *using a while loop to read char and store it into the buffer
		   *return the addc_offset value
***********************************************************************************/

int b_load(FILE* const fi, Buffer* const pBD) {
	if (!fi || !pBD) {

		return LOAD_FAIL;
	}
	/*set a temporary char with initilized value of 0*/
	char add = Zero;

	while (feof(fi) == Zero) {						/*loop until reach the end (when feof() return 0*/
		add = (char)fgetc(fi);						/*assign 'add' with newly read character*/
		if (feof(fi) != 0)
			break;									/*if 'add' is not eof*/
		if (b_addc(pBD, add) == NULL) {				/*if character is unsuccessfully added*/
			ungetc(add, fi);						/*skip character which cant be added and return -2*/
			return LOAD_FAIL;
		}
	}
	return pBD->addc_offset;
}


/***********************************************************************************
Purpose: to check if buffer is empty
Author: Andy Yang
Called functions:none
Parameters : pBuffer const
	   pBD : pointer to a buffer descriptor

return:  an  integer
Algorithm: *if pBD doesn't exist or file pointer doesn't exist, return -1
		   *set a char vairable with initilized value of 0
		   *if addc_offset is 0 means the buffer is empty, return 1 to indicate
		   *return 0 if not empty
***********************************************************************************/

int b_isempty(Buffer* const pBD) {
	if (!pBD)
		return RT_FAIL_1;
	if (pBD->addc_offset == Zero)					/*return 1 if buffer is empty*/
		return 1;
	return Zero;									/*return 0 if buffer is not empty*/
}


/***********************************************************************************
Purpose: to get char at specific loaction
Author: Andy Yang
Called functions:none
Parameters : pBuffer const
	   pBD : pointer to a buffer descriptor

return:  a char
Algorithm: *if getc_offset is equal to addc_offset means it reach the end , return 0 after set the eob flag
		   *any other condition will reset the eob flag
		   *assign the char at poisition of getc_offset to a temporary char variable
		   *increase getc_offset by one
		   *return that char in that temporary char variable
***********************************************************************************/

char b_getc(Buffer* const pBD) {
	if (!pBD)
		return RT_FAIL_2;										/*any runtime error will return -2*/
	if (pBD->getc_offset == pBD->addc_offset) {

		pBD->flags |= SET_EOB;
		return Zero;
	}
	else {
		pBD->flags &= RESET_EOB;
	}
	char temp = *(pBD->cb_head + pBD->getc_offset);				/*get char at getc_offset, increase getc_offset by 1 and return that char*/
	pBD->getc_offset++;
	return temp;
}


/***********************************************************************************
Purpose: function to know if eob flag is on or off
Author: Andy Yang
Called functions:none
Parameters : pBuffer const
	   pBD : pointer to a buffer descriptor

return:  an integer
Algorithm: *if pBD doesn't exist , return -1 on fail
		   *store value on eob position to an integer type variable
		   *right shift one bit to that integer variable in order to get the correct value
		   *return that integer vaiable(1 or 0)
***********************************************************************************/

int b_eob(Buffer* const pBD) {
	if (!pBD)
		return RT_FAIL_1;
	int temp = pBD->flags & CHECK_EOB;			/*use an integer type vairable to store value on eob position shift one bit later*/
	temp >>= One;
	return(temp);
}

/***********************************************************************************
Purpose: function to know if eob flag is on or off
Author: Andy Yang
Called functions:none
Parameters : pBuffer const
	   pBD : pointer to a buffer descriptor

return:  an integer
Algorithm: *if pBD doesn't exist , return -1 on fail
		   *store value on eob position to an integer type variable
		   *right shift one bit to that integer variable in order to get the correct value
		   *return that integer vaiable
***********************************************************************************/

int b_print(Buffer* const pBD, char nl) {
	if (!pBD)
		return RT_FAIL_1;
	char temp = Zero;						/*set one temp char with initialized value of 0*/
	while (!b_eob(pBD)) {					/*while not reach the end of the buffer*/
		temp = b_getc(pBD);					/*get one char in sequence and assign it to temp*/
		if (temp)							/*if temp is not eob*/
			printf("%c", temp);
	}
	if (nl != Zero)							/*if parameter nl is 0, print a new line*/
		printf("\n");
	return pBD->getc_offset;				/*return current position after the loop*/
}


/***********************************************************************************
Purpose: function to shrink or enlarge buffer
Author: Andy Yang
Called functions:none
Parameters : pBuffer const
	   pBD : pointer to a buffer descriptor
	   symbol : char to add into the newly reserved one space to the end of the buffer

return:  pBD
Algorithm: *if pBD doesn't exist or addc_oiffset is not in the valid range , return null on fail
		   *realloc currnet buffer to the size of current size plus one space for a char , save the realced one to a temp buffer pointer
		   *return null if fail to realloc
		   *check if realloc did make a change , set rflag on success
		   *add symbol at  the reserved space in the end
		   *re-assign the capacity to the value of addc_offset
		   *return modified pBD
***********************************************************************************/


Buffer* b_compact(Buffer* const pBD, char symbol) {

	char* nb;
	short nc;
	if (pBD == NULL) return NULL;

	nc = (pBD->addc_offset + 1) * sizeof(char);

	if (nc < 0) return NULL;

	nb = realloc(pBD->cb_head, nc);

	if (!nb) return NULL;

	if (pBD->cb_head != nb) {

		pBD->cb_head = nb;

		pBD->flags |= SET_R_FLAG;

	}
	pBD->capacity = nc;//assign new character

	pBD->cb_head = nb;

	pBD->cb_head[pBD->addc_offset] = symbol;

	++pBD->addc_offset;


	return pBD;
	//if (!pBD) return NULL;
	//if (pBD->addc_offset > MaxAllowV)
	//	return NULL;
	//char* temp = realloc(pBD->cb_head, pBD->addc_offset + 1);				/*add one more space for symbol*/
	//if (!temp) return NULL;													/*return NULL on unsuccess realloc*/
	//if (temp != pBD->cb_head)												/*if successfully realloc*/
	//	pBD->flags |= SET_R_FLAG;
	//	pBD->cb_head = temp;
	//	*(pBD->cb_head + pBD->addc_offset) = symbol;						/*add symbol at last position*/
	//	pBD->capacity = pBD->addc_offset;
	//	return pBD;
}


/***********************************************************************************
Purpose: function to check rflag
Author: Andy Yang
Called functions:none
Parameters : pBuffer const
	   pBD : pointer to a buffer descriptor

return:  a char
Algorithm: *if pBD doesn't exist, return -1
		   * use AND operator to get value at the postion of r flag and return the value after addition
***********************************************************************************/

char b_rflag(Buffer* const pBD) {
	if (!pBD)
		return RT_FAIL_1;
	return pBD->flags & CHECK_R_FLAG;							/*return value at rflag position, if 1 mean on , 0 means*/

}


/***********************************************************************************
Purpose: function to decrease getc_offset by one in order to accsee one char at previous position again
Author: Andy Yang
Called functions:none
Parameters : pBuffer const
	   pBD : pointer to a buffer descriptor

return:  a short
Algorithm: *if pBD doesn't exist, return -1
		   *if getc_offset is not accessiable , retun -1
		   *decrease getc_offset by one
		   *check if getc_offset is still in valid range
		   *return getc_offset
***********************************************************************************/
short b_retract(Buffer* const pBD) {
	if (!pBD)
		return RT_FAIL_1;
	if (!pBD->getc_offset)							/*if getc_offset is accessible*/
		return RT_FAIL_1;
	pBD->getc_offset--;
	if (pBD->getc_offset < Zero)					/*decrease getc_offset*/
		return RT_FAIL_1;
	return pBD->getc_offset;

}

/***********************************************************************************
Purpose: function to  sets getc_offset to the value of the current markc_offset
Author: Andy Yang
Called functions:none
Parameters : pBuffer const
	   pBD : pointer to a buffer descriptor

return:  a short
Algorithm: *if pBD doesn't exist, return -1
		   *if getc_offset is not accessiable , return -1
		   *assign calue of markc_offset to getc_offset
		   *if fail to assign will return -1
		   *return getc_offset
***********************************************************************************/

short b_reset(Buffer* const pBD) {
	if (!pBD)
		return RT_FAIL_1;
	if (!pBD->getc_offset)
		return RT_FAIL_1;
	pBD->getc_offset = pBD->markc_offset;							/*assign value getc_offset with value of marc_offset*/
	if (!pBD->getc_offset)
		return RT_FAIL_1;											/*return -1 on unsuccessfuly assignment*/
	else
		return pBD->getc_offset;
}

/***********************************************************************************
Purpose: function to  get value of getc_offset
Author: Andy Yang
Called functions:none
Parameters : pBuffer const
	   pBD : pointer to a buffer descriptor

return:  a short
Algorithm: *if pBD doesn't exist, return -1
		   *if getc_offset is not accessiable , return -1
		   *return getc_offset
***********************************************************************************/

short b_getcoffset(Buffer* const pBD) {
	if (!pBD)
		return RT_FAIL_1;
	if (!pBD->getc_offset)
		return RT_FAIL_1;
	return pBD->getc_offset;
}

/***********************************************************************************
Purpose: function to  set the getc_offset and markc_offset ot 0 in order to make the buffer can be reread again
Author: Andy Yang
Called functions:none
Parameters : pBuffer const
	   pBD : pointer to a buffer descriptor

return:  an integer
Algorithm:  *if pBD doesn't exist, return -1
			*set getc_offset to 0
			*set markc_offset to 0
			*reset eob flag
			*if fail to assign will return -1
			*return 0 on success
***********************************************************************************/

int b_rewind(Buffer* const pBD) {

	if (!pBD)
		return RT_FAIL_1;

	pBD->getc_offset = Zero;
	pBD->markc_offset = Zero;
	pBD->flags &= RESET_EOB;								/*reset eof flag so that it can be read again*/
	if (!pBD->getc_offset || !pBD->markc_offset)			/*any runtime error after assginment will return -1*/
		return RT_FAIL_1;
	return Zero;
}

/***********************************************************************************
Purpose: function to  get a pointer to a location of the character buffer indicted by the current markc_offset
Called functions:none
Parameters : pBuffer const
	   pBD : pointer to a buffer descriptor

return:  a char
***********************************************************************************/

char* b_location(Buffer* const pBD, short loc_offset) {
	if (!pBD)
		return NULL;
	if (loc_offset < 0 || loc_offset >= pBD->addc_offset)
		return NULL;
	return pBD->cb_head + loc_offset;
}

short b_limit(Buffer* const pBD)
{
	return pBD ? pBD->addc_offset : RT_FAIL_1;
}
