#include <stdio.h>
#include <string.h>
#include <iostream>
#include <iomanip>

using namespace std;

unsigned int M[100][80]={0};
int M_index=0;
static int SHA1(const unsigned char *testStr,
unsigned length);
void SHA1_PAD(const unsigned char *testStr,unsigned length);

unsigned int FT(unsigned int B,unsigned int C,unsigned int D,int t)
{
    if(t<20)return (B&C)|((~B)&D);
    if(t<40)return B^C^D;
    if(t<60)return (B&C)|(B&D)|(C&D);
    else return B^C^D;
}

unsigned int ROTL(unsigned int x,int y)
{
	x=(x<<y)|(x>>(32-y));
         
	return x;
} 

int main(int argc, char* argv[])
{
	char *string;

	int size = 0;

	FILE* In = fopen(argv[1],"r");

	fseek(In,0,SEEK_END);
	size = ftell(In);
	fseek(In,0,SEEK_SET);

	string = new char[size+1];

	if(In != NULL)
	{
		int i = 0;
		
		while(!feof(In))
		{
            	     fgets(string,size+1,In);
            	     i++;
        	}
	}
	else
	{
		printf("\n Error: file not found");
		fclose(In);
		return 0;
	}
	
	SHA1((const unsigned char *) string, strlen(string));

	return 0;
}

void SHA1_PAD(const unsigned char *testStr,unsigned length)
{
	int index;
    
	for(index=0;M_index*16+index<=length/4;index++)
    	{
		if(4*index<length)M[M_index][index]=testStr[4*index]<<24;
            
        	if(4*index+1<length)M[M_index][index]=M[M_index][index]| testStr[4*index+1]<<16;
            
        	if(4*index+2<length)M[M_index][index]=M[M_index][index]| testStr[4*index+2]<<8;
            
        	if(4*index+3<length)M[M_index][index]=M[M_index][index]| testStr[4*index+3];
            
            
        	if(index==16){index=0;M_index++;}
    	}
	
    	index--;
    
    	M[M_index][index]=M[M_index][index]| ((0x80) << (3-length%4)*8);
    
    	long long int bitlen= length*8;
  
    	if(index%16>=14)
               M_index++;
   
	M[M_index][14]= bitlen>>32;
	M[M_index][15]= bitlen;
}

void SHA1_HASH()
{
    unsigned int H0=0x67452301;
    unsigned int H1=0xEFCDAB89;
    unsigned int H2=0x98BADCFE;
    unsigned int H3=0x10325476;
    unsigned int H4=0xC3D2E1F0;
    
    unsigned int K[4]={0x5A827999,0x6ED9EBA1,0x8F1BBCDC,0xCA62C1D6};
    unsigned int A;
    unsigned int B;
    unsigned int C;
    unsigned int D;
    unsigned int E;
	
    for(int i=0;i<=M_index;i++)
    {
	for(int t=16;t<80;t++)
            M[i][t]=ROTL(M[i][t-3]^M[i][t-8]^M[i][t-14]^M[i][t-16],1);
			   
        A=H0;
        B=H1;
        C=H2;
        D=H3;
        E=H4;
            
        for(int t=0;t<80;t++)
        {
	    unsigned int temp=(ROTL(A,5)+FT(B,C,D,t)+E+M[i][t]+K[t/20]);
            E=D;
            D=C;
            C=ROTL(B,30);
            B=A;
            A=temp;
        }
            
        H0+=A;
        H1+=B;
        H2+=C;
        H3+=D;
        H4+=E;
    }
    
    cout << hex << H0<< H1<< H2 <<  H3<< H4 << endl;
}

void SHA_INIT()
{
	for(int i=0;i<100;i++)
		for(int j=0;j<80;j++)
			M[i][j]=0;
}

int SHA1(const unsigned char *testStr,unsigned length)
{
	SHA_INIT();
	SHA1_PAD(testStr,length);
	SHA1_HASH();

	return 0;
}
