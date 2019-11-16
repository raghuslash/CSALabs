#include<iostream>
#include<string>
#include<vector>
#include<bitset>
#include<fstream>
using namespace std;
#define MemSize 1000 // memory size, in reality, the memory size should be 2^32, but for this lab, for the space resaon, we keep it as this large number, but the memory is still 32-bit addressable.

struct IFStruct {
    bitset<32>  PC;
    bool        nop;  
};

struct IDStruct {
    bitset<32>  Instr;
    bool        nop;  
};

struct EXStruct {
    bitset<32>  Read_data1;
    bitset<32>  Read_data2;
    bitset<16>  Imm;
    bitset<5>   Rs;
    bitset<5>   Rt;
    bitset<5>   Wrt_reg_addr;
    bool        is_I_type;
    bool        rd_mem;
    bool        wrt_mem; 
    bool        alu_op;     //1 for addu, lw, sw, 0 for subu 
    bool        wrt_enable;
    bool        nop;  
};

struct MEMStruct {
    bitset<32>  ALUresult;
    bitset<32>  Store_data;
    bitset<5>   Rs;
    bitset<5>   Rt;    
    bitset<5>   Wrt_reg_addr;
    bool        rd_mem;
    bool        wrt_mem; 
    bool        wrt_enable;    
    bool        nop;    
};

struct WBStruct {
    bitset<32>  Wrt_data;
    bitset<5>   Rs;
    bitset<5>   Rt;     
    bitset<5>   Wrt_reg_addr;
    bool        wrt_enable;
    bool        nop;     
};

struct stateStruct {
    IFStruct    IF;
    IDStruct    ID;
    EXStruct    EX;
    MEMStruct   MEM;
    WBStruct    WB;
};

class RF
{
    public: 
        bitset<32> Reg_data;
     	RF()
    	{ 
			Registers.resize(32);  
			Registers[0] = bitset<32> (0);  
        }
	
        bitset<32> readRF(bitset<5> Reg_addr)
        {   
            Reg_data = Registers[Reg_addr.to_ulong()];
            return Reg_data;
        }
    
        void writeRF(bitset<5> Reg_addr, bitset<32> Wrt_reg_data)
        {
            Registers[Reg_addr.to_ulong()] = Wrt_reg_data;
        }
		 
		void outputRF()
		{
			ofstream rfout;
			rfout.open("RFresult.txt",std::ios_base::app);
			if (rfout.is_open())
			{
				rfout<<"State of RF:\t"<<endl;
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

class INSMem
{
	public:
        bitset<32> Instruction;
        INSMem()
        {       
			IMem.resize(MemSize); 
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
                  
		bitset<32> readInstr(bitset<32> ReadAddress) 
		{    
			string insmem;
			insmem.append(IMem[ReadAddress.to_ulong()].to_string());
			insmem.append(IMem[ReadAddress.to_ulong()+1].to_string());
			insmem.append(IMem[ReadAddress.to_ulong()+2].to_string());
			insmem.append(IMem[ReadAddress.to_ulong()+3].to_string());
			Instruction = bitset<32>(insmem);		//read instruction memory
			return Instruction;     
		}     
      
    private:
        vector<bitset<8> > IMem;     
};
      
class DataMem    
{
    public:
        bitset<32> ReadData;  
        DataMem()
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
		
        bitset<32> readDataMem(bitset<32> Address)
        {	
			string datamem;
            datamem.append(DMem[Address.to_ulong()].to_string());
            datamem.append(DMem[Address.to_ulong()+1].to_string());
            datamem.append(DMem[Address.to_ulong()+2].to_string());
            datamem.append(DMem[Address.to_ulong()+3].to_string());
            ReadData = bitset<32>(datamem);		//read data memory
            return ReadData;               
		}
            
        void writeDataMem(bitset<32> Address, bitset<32> WriteData)            
        {
            DMem[Address.to_ulong()] = bitset<8>(WriteData.to_string().substr(0,8));
            DMem[Address.to_ulong()+1] = bitset<8>(WriteData.to_string().substr(8,8));
            DMem[Address.to_ulong()+2] = bitset<8>(WriteData.to_string().substr(16,8));
            DMem[Address.to_ulong()+3] = bitset<8>(WriteData.to_string().substr(24,8));  
        }   
                     
        void outputDataMem()
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

void printState(stateStruct state, int cycle)
{
    ofstream printstate;
    printstate.open("stateresult.txt", std::ios_base::app);
    if (printstate.is_open())
    {
        printstate<<"State after executing cycle:\t"<<cycle<<endl; 
        
        printstate<<"IF.PC:\t"<<state.IF.PC.to_ulong()<<endl;        
        printstate<<"IF.nop:\t"<<state.IF.nop<<endl; 
        
        printstate<<"ID.Instr:\t"<<state.ID.Instr<<endl; 
        printstate<<"ID.nop:\t"<<state.ID.nop<<endl;
        
        printstate<<"EX.Read_data1:\t"<<state.EX.Read_data1<<endl;
        printstate<<"EX.Read_data2:\t"<<state.EX.Read_data2<<endl;
        printstate<<"EX.Imm:\t"<<state.EX.Imm<<endl; 
        printstate<<"EX.Rs:\t"<<state.EX.Rs<<endl;
        printstate<<"EX.Rt:\t"<<state.EX.Rt<<endl;
        printstate<<"EX.Wrt_reg_addr:\t"<<state.EX.Wrt_reg_addr<<endl;
        printstate<<"EX.is_I_type:\t"<<state.EX.is_I_type<<endl; 
        printstate<<"EX.rd_mem:\t"<<state.EX.rd_mem<<endl;
        printstate<<"EX.wrt_mem:\t"<<state.EX.wrt_mem<<endl;        
        printstate<<"EX.alu_op:\t"<<state.EX.alu_op<<endl;
        printstate<<"EX.wrt_enable:\t"<<state.EX.wrt_enable<<endl;
        printstate<<"EX.nop:\t"<<state.EX.nop<<endl;        

        printstate<<"MEM.ALUresult:\t"<<state.MEM.ALUresult<<endl;
        printstate<<"MEM.Store_data:\t"<<state.MEM.Store_data<<endl; 
        printstate<<"MEM.Rs:\t"<<state.MEM.Rs<<endl;
        printstate<<"MEM.Rt:\t"<<state.MEM.Rt<<endl;   
        printstate<<"MEM.Wrt_reg_addr:\t"<<state.MEM.Wrt_reg_addr<<endl;              
        printstate<<"MEM.rd_mem:\t"<<state.MEM.rd_mem<<endl;
        printstate<<"MEM.wrt_mem:\t"<<state.MEM.wrt_mem<<endl; 
        printstate<<"MEM.wrt_enable:\t"<<state.MEM.wrt_enable<<endl;         
        printstate<<"MEM.nop:\t"<<state.MEM.nop<<endl;        

        printstate<<"WB.Wrt_data:\t"<<state.WB.Wrt_data<<endl;
        printstate<<"WB.Rs:\t"<<state.WB.Rs<<endl;
        printstate<<"WB.Rt:\t"<<state.WB.Rt<<endl;        
        printstate<<"WB.Wrt_reg_addr:\t"<<state.WB.Wrt_reg_addr<<endl;
        printstate<<"WB.wrt_enable:\t"<<state.WB.wrt_enable<<endl;        
        printstate<<"WB.nop:\t"<<state.WB.nop<<endl; 
    }
    else cout<<"Unable to open file";
    printstate.close();
}
 

int main()
{
    
    RF myRF;
    INSMem myInsMem;
    DataMem myDataMem;
	stateStruct state, newState;


    //Initializations
    uint cycle=0;
    state.IF.PC=0x00000000;
    state.IF.nop=false;
    newState.IF.nop=false;
    state.ID.nop=true;
    newState.ID.nop=true;
    state.EX.nop=true;
    newState.EX.nop=true;
    state.MEM.nop=true;
    newState.MEM.nop=true;
    state.WB.nop=true;
    newState.WB.nop=true;


    //Reset All Signals
    newState.ID.Instr=0x00000000;

    newState.EX.is_I_type=false;
    newState.EX.rd_mem=false;
    newState.EX.wrt_mem=false;
    newState.EX.alu_op=true;
    newState.EX.wrt_enable=false;

    newState.EX.Read_data1=0x00000000;
    newState.EX.Read_data2=0x00000000;
    newState.EX.Imm=0x0000;
    newState.EX.Rs=0b00000;
    newState.EX.Rt=0b00000;
    newState.EX.Wrt_reg_addr=0b00000;

    newState.MEM.ALUresult=0x00000000;
    newState.MEM.Store_data=0x00000000;
    newState.MEM.Rs=0b00000;
    newState.MEM.Rt=0b00000;
    newState.MEM.Wrt_reg_addr=0b00000;
    newState.MEM.rd_mem=false;
    newState.MEM.wrt_mem=false;
    newState.MEM.wrt_enable=false;

    newState.WB.Wrt_data=0x00000000;
    newState.WB.Rs=0b00000;
    newState.WB.Rt=0b00000;
    newState.WB.Wrt_reg_addr=0b00000;
    newState.WB.wrt_enable=false;

    state.ID.Instr=0x00000000;

    state.EX.is_I_type=false;
    state.EX.rd_mem=false;
    state.EX.wrt_mem=false;
    state.EX.alu_op=true;
    state.EX.wrt_enable=false;

    state.EX.Read_data1=0x00000000;
    state.EX.Read_data2=0x00000000;
    state.EX.Imm=0x0000;
    state.EX.Rs=0b00000;
    state.EX.Rt=0b00000;
    state.EX.Wrt_reg_addr=0b00000;

    state.MEM.ALUresult=0x00000000;
    state.MEM.Store_data=0x00000000;
    state.MEM.Rs=0b00000;
    state.MEM.Rt=0b00000;
    state.MEM.Wrt_reg_addr=0b00000;
    state.MEM.rd_mem=false;
    state.MEM.wrt_mem=false;
    state.MEM.wrt_enable=false;

    state.WB.Wrt_data=0x00000000;
    state.WB.Rs=0b00000;
    state.WB.Rt=0b00000;
    state.WB.Wrt_reg_addr=0b00000;
    state.WB.wrt_enable=false;


    //Temporary variables
    bitset <6> opcode;
    bitset <5> rs, rt, rd;
    bitset <32> immEx, bneqAddr, jaddr;
    // bitset <26> addr;
    bool stall=false;
    bool branch=false;
    bool jump=false;


    uint loop=500;
    uint valid=0;
    uint daccess=0;

    while (loop) {
        cout<<"Cycle "<<cycle<<endl;
        /* --------------------- WB stage --------------------- */
        if (!state.WB.nop){
            if (state.WB.wrt_enable){
                valid++;
                myRF.writeRF(state.WB.Wrt_reg_addr, state.WB.Wrt_data);
                // cout<<"Storing "<<state.WB.Wrt_data<<" in register "<<state.WB.Wrt_reg_addr.to_ulong()<<endl;
            }
        }

        /* --------------------- MEM stage --------------------- */
        //Forward Control and Data Signals
        newState.WB.Rs=state.MEM.Rs;
        newState.WB.Rt=state.MEM.Rt;
        newState.WB.Wrt_reg_addr=state.MEM.Wrt_reg_addr;
        newState.WB.wrt_enable=state.MEM.wrt_enable;
        newState.WB.nop=state.MEM.nop;
        newState.WB.Wrt_data=state.MEM.ALUresult; //Default to ALU result


        if (!state.MEM.nop){

            //Implement Forwarding MEM to MEM
            if (state.MEM.Rt == state.WB.Wrt_reg_addr && state.WB.wrt_enable && !state.WB.nop)
                state.MEM.Store_data=state.WB.Wrt_data;
            
            //Perform Memory Operations
            if (state.MEM.rd_mem){
                daccess++;
                newState.WB.Wrt_data=myDataMem.readDataMem(state.MEM.ALUresult);
                cout<<"Loading from - " <<state.MEM.ALUresult<<endl;
            }
            else if (state.MEM.wrt_mem){
                daccess++;
                // cout<<"Storing "<<state.MEM.Store_data<<" at "<<state.MEM.ALUresult<<endl;
                myDataMem.writeDataMem(state.MEM.ALUresult, state.MEM.Store_data);
                
            }
        }

        /* --------------------- EX stage --------------------- */
        //Implement fowrwarding
        // MEM to EX
        if (state.EX.Rs == state.WB.Wrt_reg_addr && state.WB.wrt_enable && !state.WB.nop)
            state.EX.Read_data1=state.WB.Wrt_data;
        if (state.EX.Rt == state.WB.Wrt_reg_addr && state.WB.wrt_enable && !state.WB.nop)
            state.EX.Read_data2=state.WB.Wrt_data;
        
        // EX to EX
        if (state.EX.Rs == state.MEM.Rt && state.MEM.wrt_enable && !state.MEM.nop)
            state.EX.Read_data1=state.MEM.ALUresult;
        if (state.EX.Rt == state.MEM.Rt && state.MEM.wrt_enable && !state.MEM.nop)
            state.EX.Read_data2=state.MEM.ALUresult;

        if (!state.EX.nop){
            if (state.EX.is_I_type){
                immEx=(bitset<32>)(state.EX.Imm.to_ulong());
                if (state.EX.Imm.to_ulong() & 0x8000 !=0)
                    immEx=(bitset<32>) (immEx.to_ulong() | 0xFFFF0000);
                newState.MEM.ALUresult=(bitset <32>)(state.EX.Read_data1.to_ulong()+(int)immEx.to_ulong());
                // cout<<newState.MEM.ALUresult<<endl;
            }
            else if (state.EX.alu_op){
                // cout<<"Adding "<<state.EX.Read_data1<<" and "<<state.EX.Read_data2<<endl;
                newState.MEM.ALUresult=(bitset <32>)(state.EX.Read_data1.to_ulong()+state.EX.Read_data2.to_ulong());

            }
            else{
                    // cout<<"Subbing "<<state.EX.Read_data1<<" and "<<state.EX.Read_data2<<endl;
                    newState.MEM.ALUresult=(bitset <32>)(state.EX.Read_data1.to_ulong()-state.EX.Read_data2.to_ulong());
                }
        }

        //Forward Control and Data Signals
        newState.MEM.Store_data=state.EX.Read_data2; //IF SW, rt contents are written
        newState.MEM.Rs=state.EX.Rs;
        newState.MEM.Rt=state.EX.Rt;
        newState.MEM.Wrt_reg_addr=state.EX.Wrt_reg_addr;
        newState.MEM.rd_mem=state.EX.rd_mem;
        newState.MEM.wrt_mem=state.EX.wrt_mem;
        newState.MEM.wrt_enable=state.EX.wrt_enable;
        newState.MEM.nop=state.EX.nop;

        /* --------------------- ID stage --------------------- */

        //Set Critical Control bits to safe values
        //Forward Control and Data Signals
        newState.EX.wrt_mem=false;
        newState.EX.wrt_enable=false;
        newState.EX.nop=state.ID.nop;

        if (!state.ID.nop){

            opcode=(bitset <6>) (state.ID.Instr.to_ulong() >> 26);
            //Split  the Instruction
            rs=(bitset <5>) (state.ID.Instr.to_ulong() >> 21);
            newState.EX.Rs=rs;
            rt=(bitset <5>) (state.ID.Instr.to_ulong() >> 16);
            newState.EX.Rt=rt;
            rd=(bitset <5>) (state.ID.Instr.to_ulong() >> 11);

            newState.EX.Imm=(bitset <16>) (state.ID.Instr.to_ulong());

            //Read the Registers
            newState.EX.Read_data1=myRF.readRF(rs);
            newState.EX.Read_data2=myRF.readRF(rt);
            // cout<<rt<<' '<<newState.EX.Read_data2<<endl;

            newState.EX.is_I_type=true; //Default to Itype
            newState.EX.Wrt_reg_addr=rt; //Defalt to rt
            newState.EX.alu_op=true; //Default to add
            newState.EX.rd_mem=false;


            if (opcode.to_ulong()==0x02){
                jump=true;
                jaddr=(bitset <32>)((state.IF.PC.to_ulong() | 0xF0000000) + (state.ID.Instr.to_ulong()<<2));
            }

            //Check for BNEQ
            else if (opcode.to_ulong()==0x04){
                if (newState.EX.Read_data1.to_ulong() != newState.EX.Read_data2.to_ulong()){
                    branch=true;
                    bneqAddr=(bitset<32>)(newState.EX.Imm.to_ulong()<<2);
                    // cout<<"Before: "<<bneqAddr<<endl;
                    if ((bneqAddr.to_ulong() & 0x20000) !=0){
                        bneqAddr=(bitset<32>) (bneqAddr.to_ulong() | 0xFFFE0000);
                    // cout<<"After: "<<(int)bneqAddr.to_ulong()<<endl;
                    }
                    bneqAddr=(bitset <32>)(state.IF.PC.to_ulong() + (int)bneqAddr.to_ulong()); //PC is already PC + 4
                    // cout<<"Need to Take branch... from "<<state.IF.PC.to_ulong()<<endl;
                }
            }


            else if (opcode.to_ulong()==0x00){ //if Rtype
                // cout<<"Rtype"<<endl;
                newState.EX.is_I_type=false;
                if (((bitset <3>) state.ID.Instr.to_ulong()).to_ulong()==3){
                    newState.EX.alu_op=false;
                }
                newState.EX.wrt_enable=true;
                newState.EX.Wrt_reg_addr=rd;
            }
            else if (opcode.to_ulong()==0x23){ //if Load Word
                newState.EX.rd_mem=true;
                newState.EX.wrt_enable=true;
                // cout<<"LoadW"<<endl;
            }
            else if (opcode.to_ulong()==0x2B){//if Store Word
                newState.EX.wrt_mem=true;
                // cout<<"StoreW"<<endl;
            }

            //Check Condition for Stalling
            if (state.EX.rd_mem && !newState.EX.is_I_type){
                if (newState.EX.Rs == state.EX.Wrt_reg_addr || newState.EX.Rt == state.EX.Wrt_reg_addr){
                    stall=true;
                }
            }
        }
        newState.EX.nop=state.ID.nop;
        
        /* --------------------- IF stage --------------------- */
        
        if (!state.IF.nop){

            newState.ID.Instr=myInsMem.readInstr(state.IF.PC);
            // If current insturciton is "11111111111111111111111111111111", then break;
            if (newState.ID.Instr.to_ulong()==0xFFFFFFFF){
                cout<<"oknotok"<<endl;
                newState.IF.nop=true; //Set to terminate program when the nop propogates
                state.IF.nop=true;
            }
            else{
                newState.IF.PC=(bitset<32>)(state.IF.PC.to_ulong()+4);
            }

        }
        newState.ID.nop=state.IF.nop;

        //Implement Stall
        if (stall){

            newState.ID=state.ID;
            newState.IF=state.IF;
            newState.EX.nop=true;
            cout<<"Stalling..."<<endl;
            stall=false;
        }

        //Implement BEQ

        if (branch){
            cout<<"Branching... to "<<bneqAddr.to_ulong()<<endl;
            newState.IF.PC=bneqAddr;
            newState.ID.nop=true;
            branch=false;
        }

        //Implement Jump
        if (jump){
            cout<<"Jumping to "<<jaddr.to_ulong()<<endl;
            newState.IF.PC=jaddr;
            newState.ID.nop=true;
            jump=false;
        }

        printState(newState, cycle); //print states after executing cycle 0, cycle 1, cycle 2 ... 

        if (state.IF.nop && state.ID.nop && state.EX.nop && state.MEM.nop && state.WB.nop){
            cout<<"Ending..."<<endl;
            break;
        }

        state = newState; /*The end of the cycle and updates the current state with the values calculated in this cycle */ 


        
        loop--;
        cycle++;

    }
    
    myRF.outputRF(); // dump RF;	
	myDataMem.outputDataMem(); // dump data mem 
	
    cout<<"STATS - ";
    cout<<"Number of cycles: "<<cycle<<endl;
    cout<<"Total memory access instructions: "<<daccess<<endl;
    cout<<"Fraction of instructions accessing memory: "<<daccess/(float)valid<<endl;
    cout<<"Total number of valid instructions: "<<valid<<endl;
    cout<<"IPC: "<<valid/(float)cycle<<endl;

	return 0;
}