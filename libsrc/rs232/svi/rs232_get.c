/*
 *	z88dk RS232 Function
 *
 *      Spectravideo SVI-3x8 version
 *
 *	unsigned char rs232_get(char *)
 *
 *	Returns RS_ERROR_OVERFLOW on error (and sets carry)
 *
 *      Thomas Cherryhomes <thom.cherryhomes@gmail.com>
 */


#include <rs232.h>


u8_t __FASTCALL__ rs232_get(i8_t *char)
{	/* fastcall so implicit push */
#asm
	
#endasm
}
