/****************************************************************************/
/*                             Start of crcmodel.c                          */
/****************************************************************************/
/*                                                                          */
/* Author : Ross Williams (ross@guest.adelaide.edu.au.).                    */
/* Date   : 3 June 1993.                                                    */
/* Status : Public domain.                                                  */
/*                                                                          */
/* Description : This is the implementation (.c) file for the reference     */
/* implementation of the Rocksoft^tm Model CRC Algorithm. For more          */
/* information on the Rocksoft^tm Model CRC Algorithm, see the document     */
/* titled "A Painless Guide to CRC Error Detection Algorithms" by Ross      */
/* Williams (ross@guest.adelaide.edu.au.). This document is likely to be in */
/* "ftp.adelaide.edu.au/pub/rocksoft".                                      */
/*                                                                          */
/* Note: Rocksoft is a trademark of Rocksoft Pty Ltd, Adelaide, Australia.  */
/*                                                                          */
/****************************************************************************/
/*                                                                          */
/* Implementation Notes                                                     */
/* --------------------                                                     */
/* To avoid inconsistencies, the specification of each function is not      */
/* echoed here. See the header file for a description of these functions.   */
/* This package is light on checking because I want to keep it short and    */
/* simple and portable (i.e. it would be too messy to distribute my entire  */
/* C culture (e.g. assertions package) with this package.                   */
/*                                                                          */
/****************************************************************************/

#include "Jcrc.h"

/****************************************************************************/
/* The following definitions make the code more readable.                   */

//#define BITMASK(X) (1L << (X))
#define MASK32 0xFFFFFFFFL

/****************************************************************************/

unsigned long RockCRC::reflect(unsigned long v,int b){
	unsigned long t=v;
	for (int i=0; i<b; i++){
		if (t&1L)
			v|=aulBitMask[b-1-i];
		else
			v&=~aulBitMask[b-1-i];
		t>>=1;
		}
	return v;
}

/****************************************************************************/

inline unsigned long RockCRC::widmask (){
	return (((1L<<(cm_width-1))-1L)<<1)|1L;
}

void RockCRC::SetParams(const int nWidth,
					const unsigned long ulPoly,
					const unsigned long ulInit,
					const bool bRefIn,
					const bool bRefOut,
					const unsigned long ulXOR){
	cm_width=nWidth;
	cm_poly=ulPoly;
	cm_init=ulInit;
	cm_refin=bRefIn;
	cm_refot=bRefOut;
	cm_xorot=ulXOR;
	cm_reg=cm_init;
	for (int i=0; i<32; i++) aulBitMask[i]=1UL<<i;
}	

/****************************************************************************/

void RockCRC::Reset(){
	cm_reg=cm_init;
}

/****************************************************************************/

void RockCRC::Next(const int ch){
	unsigned long uch=(unsigned long)ch;
	unsigned long topbit=aulBitMask[cm_width-1];
	if (cm_refin) uch=reflect(uch,8);
	cm_reg^=(uch<<(cm_width-8));
	for (int i=0; i<8; i++){
		if (cm_reg&topbit)
			cm_reg=(cm_reg<<1)^cm_poly;
		else
			cm_reg<<=1;
		cm_reg&=widmask();
		}
}

/****************************************************************************/
   
unsigned long RockCRC::ulGetCRC(){
	if (cm_refot)
		return cm_xorot^reflect(cm_reg,cm_width);
	else
		return cm_xorot^cm_reg;
}


