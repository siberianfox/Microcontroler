
#include<STC15W.h>
#include "kfm010b11.H"

/************************Ö÷³ÌÐò*******************************************************/

void  main()
{
	init_kfm010b11();
	Print_a_Ascii8x5("hello world",1,12);
	test_sendByte(0,0);
}
