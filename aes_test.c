#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "poly.h"
#include "div_poly.h"
#include "mod3.h"
#include "div_generic.h"
#include "SubBytes2.h"
#include "ShiftRows2.h"
#include "MixColumns.h"
#include "AddRoundKey3.h"
#include "KeyExpansion.h"
#include "cipher.h"
#include "InvShiftRows.h"
#include "InvSubBytes.h"
#include "InvMixColumns.h"
#include "InvCipher.h"
#include "shifter.h"
#include "bit.h"

#define TEST_EQUAL(x, y) if ((x) != (y)) printf("ERR " #x "!=" #y "\n");
#define TEST_ASSERT(x) if (!(x)) printf("ERR " #x "\n");
int add(int a, int b)
{
	return a + b;
}

int sub(int a, int b)
{
	return a - b;
}


void test_cipher(){
	//int i,k;
	struct key *key = newKey();
	struct expKey *ekey = newekey();

	uint8_t keyarray[] = {0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c};
	setKey(key,keyarray);
	//first four words must be the same as first four in key
	KeyExpansion(key, ekey);
	printekey(ekey,0,43);

	uint8_t in[] = {0x32,0x43,0xf6,0xa8,0x88,0x5a,0x30,0x8d,0x31,0x31,0x98,0xa2,0xe0,0x37,0x07,0x34};
	//uint8_t in[] = {0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff};

	uint8_t out[16]; //clear_generic(out,16);
	uint8_t out2[16];

	cipher(in,out,ekey);
	int i;
	for(i=0; i<16; i++) printf("%x ",out[i]);
	printf("\n");

	invCipher(out,out2,ekey);
	printf("inv: ");
	for(i=0; i<16; i++) printf("%x ",out2[i]);
	printf("\n");
}


void bench_mark(){
	struct key *key = newKey();
	struct expKey *ekey = newekey();

	uint8_t keyarray[] = {0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c};
	setKey(key,keyarray);
	//first four words must be the same as first four in key
	KeyExpansion(key, ekey);
	//printekey(ekey,0,43);

	uint8_t in[] = {0x32,0x43,0xf6,0xa8,0x88,0x5a,0x30,0x8d,0x31,0x31,0x98,0xa2,0xe0,0x37,0x07,0x34};
	//uint8_t in[] = {0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff};

	uint8_t out[16],final[16];

	int N = 1000;
	clock_t begin = clock();
	for(int i=0; i<N; i++){
		cipher(in,out,ekey);
	}
	clock_t end = clock();
	printf("cipher(): %f usec\n", (end-begin)/(double)N/CLOCKS_PER_SEC*1e6);

	clock_t b = clock();
	for(int i=0; i<N; i++)invCipher(out,final,ekey);
	clock_t e = clock();
	printf("invCipher(): %f usec\n", (e-b)/(double)N/CLOCKS_PER_SEC*1e6);



	struct key *key2 = newKey(); //clearKey(key2);
	struct expKey *ekey2 = newekey();

	uint8_t keyarray2[] = {0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c};
	setKey(key2,keyarray2);
	KeyExpansion_b(key2, ekey2);

	uint8_t in2[] = {0x32,0x43,0xf6,0xa8,0x88,0x5a,0x30,0x8d,0x31,0x31,0x98,0xa2,0xe0,0x37,0x07,0x34};
	uint8_t out2[16], final2[16];

	clock_t begin2 = clock();
	for(int i=0; i<N; i++)cipher_b(in2,out2,ekey2);
	clock_t end2 = clock();
	printf("cipher_b(): %f usec\n", (end2-begin2)/(double)N/CLOCKS_PER_SEC*1e6);

	clock_t b2 = clock();
	for(int i=0; i<N; i++)invCipher_b(out2,final2,ekey2);
	clock_t e2 = clock();
	printf("invCipher_b(): %f usec\n", (e2-b2)/(double)N/CLOCKS_PER_SEC*1e6);


}

void test_cipher2(){
	uint8_t keyarray1[] = {0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c};
	uint8_t input1[] = {0x32,0x43,0xf6,0xa8,0x88,0x5a,0x30,0x8d,0x31,0x31,0x98,0xa2,0xe0,0x37,0x07,0x34};
	uint8_t output1[16]; uint8_t temp_output1[16];

	uint8_t keyarray2[] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
	uint8_t input2[] = {0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff};
	uint8_t output2[16]; uint8_t temp_output2[16];

	struct key *key1 = newKey(); setKey(key1,keyarray1);
	struct expKey *ekey1 = newekey(); KeyExpansion(key1,ekey1);
	cipher(input1,temp_output1,ekey1);
	uint8_t exp1[] = {0x39,0x25,0x84,0x1d,0x02,0xdc,0x09,0xfb,0xdc,0x11,0x85,0x97,0x19,0x6a,0x0b,0x32};

	TEST_ASSERT(isEqualMessage(exp1,temp_output1));
	/*
	int i;
	for(i=0; i<16; i++) printf("%x ",temp_output1[i]);
	printf("\n");*/
	invCipher(temp_output1,output1,ekey1);
	uint8_t exp2[] = {0x32,0x43,0xf6,0xa8,0x88,0x5a,0x30,0x8d,0x31,0x31,0x98,0xa2,0xe0,0x37,0x07,0x34};
	TEST_ASSERT(isEqualMessage(exp2,output1));
	/*
	for(i=0; i<16; i++) printf("%x ",output1[i]);
	printf("\n");*/

	struct key *key2 = newKey(); setKey(key2,keyarray2);
	struct expKey *ekey2 = newekey(); KeyExpansion(key2,ekey2);
	cipher(input2,temp_output2,ekey2);
	uint8_t exp3[] = {0x69,0xc4,0xe0,0xd8,0x6a,0x7b,0x04,0x30,0xd8,0xcd,0xb7,0x80,0x70,0xb4,0xc5,0x5a};
	TEST_ASSERT(isEqualMessage(exp3,temp_output2));
	/*
	for(i=0; i<16; i++) printf("%x ",temp_output2[i]);
	printf("\n");*/
	invCipher(temp_output2,output2,ekey2);
	uint8_t exp4[] = {0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff};
	TEST_ASSERT(isEqualMessage(exp4,output2));
	/*
	for(i=0; i<16; i++) printf("%x ",output2[i]);
	printf("\n");*/
}

void test_sb(){
	int x[] = {1,1,1,0,1,1,1,1};
	int y[8]; clear8(y);
	SubBytes(x,y);
	for(int i=0; i<8; i++) printf("%d",y[i]);
	printf("\n\n");

	uint8_t test = 0x02;
	test = mulInverse_b(test);
	printf("test = %x\n\n\n",test);

	uint8_t x1 = 0xf8, y1 = 0xca;
	x1 = crossMul_b(y1,x1);
	printf("final result = %x\n\n\n",x1);

	uint8_t x2 = 0xa9;
	x2 = SubBytes_b(x2);
	printf("sb result: %x\n\n",x2);



	struct state s; clearState(&s);
	setState(0,0,0x19,&s);
	setState(0,1,0xa0,&s);
	setState(0,2,0x9a,&s);
	setState(0,3,0xe9,&s);

	setState(1,0,0x3d,&s);
	setState(1,1,0xf4,&s);
	setState(1,2,0xc6,&s);
	setState(1,3,0xf8,&s);

	setState(2,0,0xe3,&s);
	setState(2,1,0xe2,&s);
	setState(2,2,0x8d,&s);
	setState(2,3,0x48,&s);

	setState(3,0,0xbe,&s);
	setState(3,1,0x2b,&s);
	setState(3,2,0x2a,&s);
	setState(3,3,0x08,&s);

	printState(&s); printf("\n");


	struct state s2; clearState(&s2);
	SubState_b(&s,&s2);
	printState(&s2);
	printf("\n");

	ShiftRows2(&s2); printf("after ShiftRows2:\n");
	printState(&s2);
	printf("\n");

	MixColumns_b(&s2);
	printState(&s2);
	printf("\n");
}

void test_ke2(){
	struct key *key = newKey();
	struct expKey *ekey = newekey();

	uint8_t keyarray[] = {0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c};
	setKey(key,keyarray);
	//first four words must be the same as first four in key
	KeyExpansion_b(key, ekey);
	printekey(ekey,0,43);
}
void test_c(){
	struct key *key = newKey();
	struct expKey *ekey = newekey();

	uint8_t keyarray[] = {0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c};
	setKey(key,keyarray);
	KeyExpansion(key, ekey);
	//printekey(ekey,0,43);

	uint8_t in[] = {0x32,0x43,0xf6,0xa8,0x88,0x5a,0x30,0x8d,0x31,0x31,0x98,0xa2,0xe0,0x37,0x07,0x34};
	//uint8_t in[] = {0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff};

	uint8_t out[16];
	cipher_b(in,out,ekey);


	struct key *key2 = newKey();
	struct expKey *ekey2 = newekey();

	uint8_t keyarray2[] = {0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c};
	setKey(key2,keyarray2);
	KeyExpansion(key2, ekey2);


	uint8_t in2[] = {0x32,0x43,0xf6,0xa8,0x88,0x5a,0x30,0x8d,0x31,0x31,0x98,0xa2,0xe0,0x37,0x07,0x34};
	uint8_t out2[16];
	cipher_b(in2,out2,ekey2);
}

void test_invsb(){
	uint8_t x = 0xc5;
	uint8_t res = InvSubBytes_b(x);
	printf("res = %x\n",res);

	struct state s; clearState(&s);
	setState(0,0,0x7a,&s);
	setState(0,1,0x89,&s);
	setState(0,2,0x2b,&s);
	setState(0,3,0x3d,&s);

	setState(1,0,0x9f,&s);
	setState(1,1,0xd5,&s);
	setState(1,2,0xef,&s);
	setState(1,3,0xca,&s);

	setState(2,0,0x10,&s);
	setState(2,1,0xf5,&s);
	setState(2,2,0xfd,&s);
	setState(2,3,0x4e,&s);

	setState(3,0,0x27,&s);
	setState(3,1,0x0b,&s);
	setState(3,2,0x9f,&s);
	setState(3,3,0xa7,&s);

	printState(&s);printf("\n\n");

	struct state s2; clearState(&s2);
	InvSubState_b(&s,&s2);
	printState(&s2);




	clearState(&s);
	setState(0,0,0x7a,&s);
	setState(0,1,0x89,&s);
	setState(0,2,0x2b,&s);
	setState(0,3,0x3d,&s);

	setState(1,0,0xd5,&s);
	setState(1,1,0xef,&s);
	setState(1,2,0xca,&s);
	setState(1,3,0x9f,&s);

	setState(2,0,0xfd,&s);
	setState(2,1,0x4e,&s);
	setState(2,2,0x10,&s);
	setState(2,3,0xf5,&s);

	setState(3,0,0xa7,&s);
	setState(3,1,0x27,&s);
	setState(3,2,0x0b,&s);
	setState(3,3,0x9f,&s);

	InvShiftRows2(&s);
	printState(&s);

	printf("\n");
	clearState(&s);
	setState(0,0,0x7a,&s);
	setState(0,1,0x89,&s);
	setState(0,2,0x2b,&s);
	setState(0,3,0x3d,&s);

	setState(1,0,0xd5,&s);
	setState(1,1,0xef,&s);
	setState(1,2,0xca,&s);
	setState(1,3,0x9f,&s);

	setState(2,0,0xfd,&s);
	setState(2,1,0x4e,&s);
	setState(2,2,0x10,&s);
	setState(2,3,0xf5,&s);

	setState(3,0,0xa7,&s);
	setState(3,1,0x27,&s);
	setState(3,2,0x0b,&s);
	setState(3,3,0x9f,&s);

	InvMixColumns_b(&s);
	printState(&s);
}
void test_shifter(){
	uint32_t x = 0xffcc0000; //0xff 0xcc 0x00 0x00
	int num = 3;
	x = rshift32(x,num);
	printf("x = %x\n",x); //0xcc 0x00 0x00 0xff
}

void test_sr32(){
	struct state2 s; clearState2(&s);
	s.block[0] = 0xaabbccdd;
	s.block[1] = 0xaabbccdd;
	s.block[2] = 0xaabbccdd;
	s.block[3] = 0xaabbccdd;
	printState32(s);

	ShiftRows32(&s);
	printf("\n");
	printState32(s);
}
void test_sb32(){
	struct state2 s; clearState2(&s);
	s.block[0] = 0xf0f0f0f0;
	s.block[1] = 0xf0f0f0f0;
	s.block[2] = 0xf0f0f0f0;
	s.block[3] = 0xf0f0f0f0;

	struct state2 s2; clearState2(&s2);
	SubState32(&s, &s2);

	printState32(s2);
}
void test_mc(){
	struct state2 s; clearState2(&s);
	s.block[0] = 0xaabbccdd;
	s.block[1] = 0xeeff0011;
	s.block[2] = 0x22334455;
	s.block[3] = 0x66778899;

	uint32_t res = getColumn32(s,1);


	uint32_t x = 0x01010101;
	uint32_t y = 0x02030101;
	res = mul32(x,y);
	printf("res = %x\n",res);
}

int main()
{
	//TEST_EQUAL(add(3, 5), 3 + 5);
	//TEST_EQUAL(sub(3, 5), 3 - 5);

	bench_mark();
	test_shifter();

	test_sr32();
	printf("\n");
	test_sb32();
	printf("\n");
	test_mc();
}
