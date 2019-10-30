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
          // vector<bitset<8> > IMem;

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
                  else cout<<"Unable to open Imem file";
                  imem.close();
                     
                  }
                  
          bitset<32> ReadMemory (bitset<32> ReadAddress) 
              {    
               // implement by you. (Read the byte at the ReadAddress and the following three byte).
               // cout<<(IMem[0].to_ulong()<<24);
               Instruction = (bitset <32>) (((IMem[ReadAddress.to_ulong()]).to_ulong()<<24) + ((IMem[ReadAddress.to_ulong()+1]).to_ulong()<<16) + ((IMem[ReadAddress.to_ulong()+2]).to_ulong()<<8) + ((IMem[ReadAddress.to_ulong()+3]).to_ulong()));
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
                  else cout<<"Unable to open Dmem file";
                  dmem.close();
          
          }  
          bitset<32> MemoryAccess (bitset<32> Address, bitset<32> WriteData, bitset<1> readmem, bitset<1> writemem) 
          {    
               
               // implement by you.
              bitset<8> putByte;
              if ((bool)readmem.to_ulong())
                readdata = (bitset <32>) (((DMem[Address.to_ulong()]).to_ulong()<<24) + ((DMem[Address.to_ulong()+1]).to_ulong()<<16) + ((DMem[Address.to_ulong()+2]).to_ulong()<<8) + ((DMem[Address.to_ulong()+3]).to_ulong()));

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
                  else cout<<"Unable to open Dmem file";
                  dmemout.close();
               
               }             
      
      private:
           vector<bitset<8> > DMem;
      
};  

void splitIns (bitset <32> inst, bitset <5> &rs, bitset <5> &rt, bitset <5> &rd, bitset <16> &imm, bitset <26> &addr)
{
  rs=(bitset <5>) (inst.to_ulong() >> 21);
  rt=(bitset <5>) (inst.to_ulong() >> 16);
  rd=(bitset <5>) (inst.to_ulong() >> 11);
  imm=(bitset <16>) (inst.to_ulong());
  addr=(bitset <26>) (inst.to_ulong());

}
void insDecode (bitset <32> inst, bool & rtype, bool & itype, bool &jtype, bool & isstore, bool & isload, bool & isbranch, bool &WE, bitset <6> & funct)
{ rtype = itype = jtype = isstore = isload = isbranch = WE = isbranch = 0;
  bitset <6> opcode = (bitset <6>) (inst.to_ulong() >> 26);
  // cout<<opcode<<endl;
  if (opcode.to_ulong()==0x00)
    {
      rtype=1;
      funct=(bitset <6>) inst.to_ulong();
    }
  else if (opcode.to_ulong()==0x02)
    jtype=1;
  else if (opcode.to_ulong()==0x2B)
    isstore=1;
  else if (opcode.to_ulong()==0x23)
    isload=1;
  if (opcode.to_ulong()==0x04)
    isbranch=1;
  if (!jtype && !rtype)
    {
      itype=1;
      funct=(bitset <6>) 0x21;
    } 
  if (isstore || isbranch || jtype)
    WE=0;
  else
    WE=1;

}

int main()
{
    RF myRF;
    ALU myALU;
    INSMem myInsMem;
    DataMem myDataMem;
    bitset <32> inst, immEx;
    bitset <32> PC = (bitset <32>) 0x00000000;
    bitset <5> rs, rt, rd;
    bitset <16> imm;
    bitset <26> addr;
    bool itype, rtype, jtype, isstore, isload, isbranch, WE;
    bitset <6> funct;
    bitset <32> wrData;

  while (1)
	{
    // Fetch

    inst=myInsMem.ReadMemory(PC);
    PC=PC.to_ulong()+4;
    cout<<inst<<endl;

    // If current insturciton is "11111111111111111111111111111111", then break;
    if (inst.to_ulong()==0xFFFFFFFF)
      {
        cout<<"oknotok";
        break;
      }
    splitIns (inst, rs, rt, rd, imm, addr);
    insDecode (inst, rtype, itype, jtype, isstore, isload, isbranch, WE, funct);
    if (jtype)
      {
        PC=(bitset<32>) (((PC.to_ulong() + 4) | 0xF0000000) + (addr.to_ulong()<<2));
        continue;
      }
    myRF.ReadWrite(rs, rd, rt, wrData, (bitset<1>) 0);
    if (rtype)
      wrData=myALU.ALUOperation((bitset<3>) funct.to_ulong(), myRF.ReadData1, myRF.ReadData2);
    else
    { immEx=(bitset<32>) imm.to_ulong();
      if (imm.to_ulong() & 0x8000 !=0)
        immEx=(bitset<32>) (immEx.to_ulong() | 0xFFFF0000);
      wrData=myALU.ALUOperation((bitset<3>) funct.to_ulong(), myRF.ReadData1, immEx);
    }
    if (isbranch)
    {
      if (myRF.ReadData1 == myRF.ReadData2)
      { 
        // PC=myALU.ALUresult;
        continue;
      }
    }
    if (isstore)
      myDataMem.MemoryAccess (myALU.ALUresult, myRF.ReadData2, (bitset<1>) 1 , (bitset<1>) 0);
    else if (isload)
      wrData=myDataMem.MemoryAccess (myALU.ALUresult, myRF.ReadData2, (bitset<1>) 0 , (bitset<1>) 1);

    myRF.ReadWrite(rs, rd, rt, wrData, (bitset<1>) WE);
		// decode(Read RF)
		
		// Execute
		
		// Read/Write Mem
		
		// Write back to RF
  myRF.OutputRF(); // dump RF;
    
  }
  myDataMem.OutputDataMem(); // dump data mem
  return 0;
        
}
