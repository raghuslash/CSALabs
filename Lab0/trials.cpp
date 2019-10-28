#include<iostream>
#include<string>
#include<vector>
#include<bitset>
#include<fstream>
using namespace std;
#define ADDU 1
#define SUBU 3
#define AND 4
#define OR  5
#define NOR 7
#define MemSize 65536 

bitset<32> IMem[100];
bitset<32> ReadAddress=0x00000001;
bitset<8> Inst[4]={0xC0,0xFF,0xEE,0x00};
          int test()
              {bitset<32> Instruction;

               // implement by you. (Read the byte at the ReadAddress and the following three byte).
               // cout<<Instruction.to_ulong()+1;
               Instruction=(bitset<32>)((Inst[0].to_ulong()<<24) + (Inst[1].to_ulong()<<16) + (Inst[2].to_ulong()<<8) + (Inst[3].to_ulong()));
               cout<<Instruction<<endl;

               Inst[0]=0x00;Inst[1]=0x00;Inst[2]=0x00;Inst[3]=0x00;
               bitset<8> putByte;
               for (int i=3; i>=0; i--)
                {
                  putByte = (bitset<8>)(Instruction.to_ulong() >> ((i)*8));
                  Inst[3-i] = putByte;
 
                }
               for (int i=0; i<4; i++)
               	cout<<Inst[i];
              }     
