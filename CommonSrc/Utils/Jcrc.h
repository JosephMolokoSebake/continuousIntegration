#ifndef __CRC_H
#define __CRC_H

/****************************************************************************/
/*                                                                          */
/* Author : Ross Williams (ross@guest.adelaide.edu.au.).                    */
/* Date   : 3 June 1993.                                                    */
/* Status : Public domain.                                                  */
/*                                                                          */
/* Description : This is the header (.h) file for the reference             */
/* implementation of the Rocksoft^tm Model CRC Algorithm. For more          */
/* information on the Rocksoft^tm Model CRC Algorithm, see the document     */
/* titled "A Painless Guide to CRC Error Detection Algorithms" by Ross      */
/* Williams (ross@guest.adelaide.edu.au.). This document is likely to be in */
/* "ftp.adelaide.edu.au/pub/rocksoft".                                      */
/*                                                                          */
/* Note: Rocksoft is a trademark of Rocksoft Pty Ltd, Adelaide, Australia.  */
/*                                                                          */
/****************************************************************************/
/* Changed to C++ Object format : J G Theron, IMT, 08/12/96                 */
/****************************************************************************/

class RockCRC{

	int   cm_width;   		// Parameter: Width in bits [8,32].
	unsigned long cm_poly;  // Parameter: The algorithm's polynomial.
	unsigned long cm_init;  // Parameter: Initial register value.
	bool  cm_refin;   		// Parameter: Reflect input bytes?
	bool  cm_refot;   		// Parameter: Reflect output CRC?
	unsigned long cm_xorot; // Parameter: XOR this to output CRC.
	unsigned long cm_reg;   // Context: Context during execution.

	unsigned long aulBitMask[32];	

	//Returns the value v with the bottom b [0,32] bits reflected.
	//Example: reflect(0x3e23L,3) == 0x3e26
	unsigned long reflect(unsigned long v, int b);

	//Returns a longword whose value is (2^cm_width)-1.
	//The trick is to do this portably (e.g. without doing <<32).
	unsigned long widmask();

public:

//	Default setup is for "CRC32" as used by Ethernet, PKZip, etc.
	RockCRC(	const int nWidth=32,
				const unsigned long ulPoly=0x04c11db7UL,
				const unsigned long ulInit=0xffffffffUL,
				const bool bRefIn=true,
				const bool bRefOut=true,
				const unsigned long ulXOR=0xffffffffUL){
		SetParams(nWidth,ulPoly,ulInit,bRefIn,bRefOut,ulXOR);
	}

	void SetParams(const int nWidth,
						const unsigned long ulPoly,
						const unsigned long ulInit,
						const bool bRefIn,
						const bool bRefOut,
						const unsigned long ulXOR);

// All parameter fields must be set before calling this.
	void Reset();

// Processes a single message byte [0,255].
	void Next(const int ch);

// Returns the CRC value for the message bytes processed so far.
	unsigned long ulGetCRC();

};

#endif


