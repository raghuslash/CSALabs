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
#define MemSize 65536 // memory size, in reality, the memory size should be 2^32, but for this lab, for the space resaon, we keep it as this large number, but the memory is still 32-bit addressable.


class RF
{
    public:
        bitset<32> ReadData1, ReadData2; 
     	RF()
    	{ 
          Registers.resize(32);  
          Registers[0] = bitset<32> (0);  
        }
	
        void ReadWrite(bitset<5> RdReg1, bitset<5> RdReg2, bitset<5> WrtReg, bitset<32> WrtData, bitset<1> WrtEnable)
        {   
            ReadData1 = Registers[RdReg1.to_ulong()];
            ReadData2 = Registers[RdReg2.to_ulong()];
            if ((bool)(WrtEnable.to_ulong()))
                {
                    Registers[WrtReg.to_ulong()]=WrtData;
                    // this->OutputRF();
                }
         }
		 
	void OutputRF() // write RF results to file
             {
               ofstream rfout;
                  rfout.open("RFresult.txt",std::ios_base::app);
                  if (rfout.is_open())
                  {
                    rfout<<"A state of RF:"<<endl;
                  for (int j = 0; j<32; j++)
                      {        
                        rfout << Registers[j]<<endl;
                      }
                     
                  }
                  else cout<<"Unable to open file";
                  rfout.close();
               
               }     
	private:
            vector<bitset<32> >Registers;
	
};

class ALU
{
      public:
             bitset<32> ALUresult;
             bitset<32> ALUOperation (bitset<3> ALUOP, bitset<32> oprand1, bitset<32> oprand2)
             {   
                 switch((int)(ALUOP.to_ulong()))
                 {
                    // case 0: ALUresult = oprand1 + oprand2;
                        // break;
                    case 1: ALUresult = oprand1.to_ulong() + oprand2.to_ulong();
                        break;
                    // case 2: ALUresult = oprand1 - oprand2;
                        // break;
                    case 3: ALUresult = oprand1.to_ulong() - oprand2.to_ulong();
                        break;
                    case 4: ALUresult = oprand1.to_ulong() & oprand2.to_ulong();
                        break;
                    case 5: ALUresult = oprand1.to_ulong() | oprand2.to_ulong();
                        break;
                    case 7: ALUresult = ~(oprand1.to_ulong() | oprand2.to_ulong());
                 }
                 return (bitset<32>)ALUresult;
               }            
};

class INSMem
{
      public:
          bitset<32> Instruction;
          INSMem() // read instruction memory
          {       IMem.resize(MemSize); 
                  ifstream imem;
                  string line;
                  int i=0;
                  imem.open("imem.txt");
                  if (imem.is_open())
                  {
                  while (getline(imem,line))
                     {      
                        IMem[i] = bitset<8>(line);
                        i++;
                     }
                     
                  }
                  else cout<<"Unable to open file";
                  imem.close();
                     
                  }
                  
          bitset<32> ReadMemory (bitset<32> ReadAddress) 
              {    
               // implement by you. (Read the byte at the ReadAddress and the following three byte).
               Instruction = (IMem[ReadAddress.to_ulong()]<<24).to_ulong() + (IMem[ReadAddress.to_ulong()+1]<<16).to_ulong() + (IMem[ReadAddress.to_ulong()+2]<<8).to_ulong() + (IMem[ReadAddress.to_ulong()+3]).to_ulong();
               return Instruction;     
              }     
      
      private:
           vector<bitset<8> > IMem;
      
};
      
class DataMem    
{
      public:
          bitset<32> readdata;  
          DataMem() // read data memory
          {
             DMem.resize(MemSize); 
             ifstream dmem;
                  string line;
                  int i=0;
                  dmem.open("dmem.txt");
                  if (dmem.is_open())
                  {
                  while (getline(dmem,line))
                       {      
                        DMem[i] = bitset<8>(line);
                        i++;
                       }
                  }
                  else cout<<"Unable to open file";
                  dmem.close();
          
          }  
          bitset<32> MemoryAccess (bitset<32> Address, bitset<32> WriteData, bitset<1> readmem, bitset<1> writemem) 
          {    
               
               // implement by you.
              bitset<8> putByte;
              if ((bool)readmem.to_ulong())
                readdata = (DMem[Address.to_ulong()]<<24).to_ulong() + (DMem[Address.to_ulong()+1]<<16).to_ulong() + (DMem[Address.to_ulong()+2]<<8).to_ulong() + (DMem[Address.to_ulong()+3]).to_ulong();
              else if ((bool) writemem.to_ulong())
                for (int i=3; i>=0; i--)
                {
                  putByte = (bitset<8>) (WriteData.to_ulong() >> (i*8));
                  DMem[Address.to_ulong()+(3-i)] = putByte;

                }
              return readdata;     
          }   
                     
          void OutputDataMem()  // write dmem results to file
          {
               ofstream dmemout;
                  dmemout.open("dmemresult.txt");
                  if (dmemout.is_open())
                  {
                  for (int j = 0; j< 1000; j++)
                       {     
                        dmemout << DMem[j]<<endl;
                       }
                     
                  }
                  else cout<<"Unable to open file";
                  dmemout.close();
               
               }             
      
      private:
           vector<bitset<8> > DMem;
      
};  


   
int main()
{
    RF myRF;
    ALU myALU;
    INSMem myInsMem;
    DataMem myDataMem;
    bitset <32> inst;
    bitset <32> PC = (bitset <32>) 0x00000000;
    while (1)
	{
    // Fetch
    inst=myInsMem.ReadMemory(PC);
    PC=PC.to_ulong()+4;
    // If current insturciton is "11111111111111111111111111111111", then break;
    if (inst.to_ulong()==0xFFFFFFFF)
      break;
    else

		// decode(Read RF)
		
		// Execute
		
		// Read/Write Mem
		
		// Write back to RF
		
    myRF.OutputRF(); // dump RF;    
    }
        myDataMem.OutputDataMem(); // dump data mem
      
        return 0;
        
}
