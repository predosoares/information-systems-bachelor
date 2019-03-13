#include <stdio.h>
#define makefloat(s,e,f) ((s & 1)<<31 | (((e) & 0xff) << 23) | ((f) & 0x7fffff))
#define getsig(x)  ((x)>>31 & 1)
#define getexp(x)  ((x)>>23 & 0xff)
#define getfrac(x) ((x) & 0x7fffff)

typedef union { 
  float f;
  unsigned int i;
 } U;

float float2(float f){
    U u;
    unsigned int u1;
    
    u.f = f;
    u1 = u.i;
    
    u1 = makefloat(getsig(u1), getexp(u1) + 1, getfrac(u1));
    
    u.i = u1;
    f = u.f;
    
    return f;
}

float int2float(int i){
    U u;
    unsigned int u1, exponent, signal;
    int shifts = 0;
    float f;
    
    signal = i & 0x80000000;
    u1 = i;
    
    if (i == 0){
        u1 = 0x00000000;
    }
    else{
        while((u1 & (1<<23)) == 0){
            u1 <<= 1;
            shifts++;
        }
        u1 = u1>>8;
        exponent = 127 + 31 - shifts;
        u1 = makefloat(getsig(signal), exponent, getfrac(u1));
    }
    
    u.i = u1;
    f = u.f;
    return f;
}

float int2float2(int i)
{
	unsigned int sin=0x00,exp,mant,iabs;
	int casas=31;
	unsigned int mask = 0x80000000;
	U u;
	if(i==0)
		return 0;
	if(i<0){
		sin = 1;
		i=-i;
		}
	iabs=i;
	while((mask & iabs)==0x00)
	{
		iabs=iabs<<1;
		casas--;
	}
		
	iabs=iabs<<1;
	mant= iabs>>9;

	exp = casas+127;
	
	u.i=makefloat(sin,exp,mant);
	return	u.f;
}



int main(void){
    int i;
    float f1 = 0.625;
    
    printf("%f\n", float2(f1));
    
    
   printf("\n******** int2float ****************\n");
   i = 0;
   printf(" %d -> %+10.4f\n", i, int2float(i));
   i = 1;  
   printf(" %d -> %+10.4f\n", i, int2float(i));
   i = -1;  
   printf(" %d -> %10.4f\n", i, int2float(i));
   i = 0x7fffffff;  
   printf(" %d -> %+10.4f\n", i, int2float(i));
   i = -i;
   printf(" %d -> %+10.4f\n", i, int2float(i));
   i = 12345;
   printf(" %d -> %+10.4f\n", i, int2float(i));
   i = -12345;
   printf(" %d -> %+10.4f\n", i, int2float(i));
   
   printf("\n******** int2float2 ****************\n");
   i = 0;
   printf(" %d -> %+10.4f\n", i, int2float2(i));
   i = 1;  
   printf(" %d -> %+10.4f\n", i, int2float2(i));
   i = -1;  
   printf(" %d -> %10.4f\n", i, int2float2(i));
   i = 0x7fffffff;  
   printf(" %d -> %+10.4f\n", i, int2float2(i));
   i = -i;
   printf(" %d -> %+10.4f\n", i, int2float2(i));
   i = 12345;
   printf(" %d -> %+10.4f\n", i, int2float2(i));
   i = -12345;
   printf(" %d -> %+10.4f\n", i, int2float2(i));
   return 0;
}

