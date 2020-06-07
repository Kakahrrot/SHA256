#include "SHA.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define chunk_bits 512 
#define INT_MAX ((long long int)1 << 32)
// test for demo
/*
#define rightrotate(i,n) (((i) >> (n)) | ((i) << (32-(n))))
#define rightshift(i,n) ((i) >> (n))
*/
   unsigned int rightrotate(unsigned int i, unsigned int n)
   {
   return (i >> n) | (i << (32-n));
   }
   unsigned int rightshift(unsigned int i, unsigned int n)
   {
   return i >> n;
   }
void print_bit_pattern(int c, int n)
{	
	for(int i = 0; i < n; i++)
		if(c & (long long int)1 << (n - 1 - i))
			printf("1");
		else
			printf("0");
	printf("%c",n == 32?'\n':' ');
}

unsigned int* sha256(char* msg)
{
	union {
		unsigned char m[chunk_bits/8];//magic!!
		unsigned int w[chunk_bits/32 * 4];
	}sixteen_32_bits;
	for(int i = 0; i < chunk_bits/8; i++)
		sixteen_32_bits.m[i] = 0;
	unsigned int hh[8] = {
		0x6a09e667,0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 
		0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
	};
	unsigned int *hash = malloc(8 * sizeof(unsigned int));
	for(int i = 0; i < 8; i++)
		hash[i] = hh[i];
	const unsigned int k[64] = {
		0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
		0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
		0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
		0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
		0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
		0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
		0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
		0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
		0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
		0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
		0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
		0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
		0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
		0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
		0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
		0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
	};
	unsigned char *m = sixteen_32_bits.m;
	strcpy(m, msg);
	m[strlen(m)] |= 1 << 7;
	long long int llt = (long long int)(strlen(m) - 1) * 8;
	m[chunk_bits/8 - 8] = llt;
	unsigned char* cp = &m[chunk_bits/8-8];
	unsigned char* chptr[8];
	for(int i = 0; i < 8; i++)
		chptr[i] = cp++;
	for(int i = 0; i < 4; i++)
	{
		unsigned char t = *chptr[i];
		*chptr[i] = *chptr[7-i];
		*chptr[7-i] = t;
	}
	/*
	printf("8 bits char:\n");
	for(int i = 0; i < chunk_bits/8; i++)
		print_bit_pattern(m[i], 8);
	printf("\n32 bits int:\n");
	for(int i = 0; i < chunk_bits/32; i++)
		print_bit_pattern(sixteen_32_bits.w[i], 32);
	printf("\nHex:\n");
	for(int i = 0; i < chunk_bits/8; i++)
		printf("%02x ", m[i]);
	printf("\n");
	*/
	unsigned int s0, s1;
	unsigned int* w = sixteen_32_bits.w;
	for(int i = 16; i < 64; i++)
	{
		s0 = rightrotate(w[i-15],7) ^ rightrotate(w[i-15],18)
			^ rightshift(w[i-15], 3);
		s1 = rightrotate(w[i-2],17) ^ rightrotate(w[i-2],19)
			^ rightshift(w[i-2], 10);
		w[i] = w[i-16] + s0 + w[i-7] + s1;
	}
	unsigned int a, b, c, d, e, f, g, h, maj, t1, t2, ch;
	a = hash[0];
	b = hash[1];
	c = hash[2];
	d = hash[3];
	e = hash[4];
	f = hash[5];
	g = hash[6];
	h = hash[7];
	for(int i = 0; i < 64;i++)
	{
		//printf("%12u %12u %12u %12u\n%12u %12u %12u %12u", a, b, c, d, e, f, g, h);
		s0 = rightrotate(a,2) ^ rightrotate(a,13) ^ rightrotate(a,22);
		maj = (a&b) ^ (b&c) ^ (a&c);
		t2 = ((long long int)s0 + maj) %  INT_MAX;
		s1 = rightrotate(e, 6) ^ rightrotate(e, 11) ^ rightrotate(e, 25);
		ch = (e&f) ^ ((~e)&g);
		t1 = ((long long int)h + s1 + ch + k[i] + w[i]) % INT_MAX;
		h = g;
		g = f;
		f = e;
		e = d + t1;
		d = c;
		c = b;
		b = a;
		a = ((long long int)t1 + t2) % INT_MAX;
		b %= INT_MAX;
		c %= INT_MAX;
		d %= INT_MAX;
		e %= INT_MAX;
		f %= INT_MAX;
	 	g %= INT_MAX;
		h %= INT_MAX;
	}
	hash[0] += a % INT_MAX;
	hash[1] += b % INT_MAX;
	hash[2] += c % INT_MAX;
	hash[3] += d % INT_MAX;
	hash[4] += e % INT_MAX;
	hash[5] += f % INT_MAX;
	hash[6] += g % INT_MAX;
	hash[7] += h % INT_MAX;
	return hash;
}
