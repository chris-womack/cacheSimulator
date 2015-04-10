#include <stdio.h>

//Settings
#define L1_block_size 32
#define L1_cache_size 8192
#define L1_assoc 1
#define L1_hit_time 1
#define L1_miss_time 1
#define L2_block_size 64
#define L2_cache_size 32768
#define L2_assoc 1
#define L2_hit_time 5
#define L2_miss_time 7
#define L2_transfer_time 5
#define L2_bus_width 16


#define DIRECT_MAPPED 1
#define FULLY_ASSOC 0
#define mem_sendaddr 10
#define mem_ready 30
#define mem_chunktime 15
#define mem_chunksize 8

unsigned long long int totalexec = 0;

//*** Cache Structs ***//
typedef struct {
    unsigned long long int addr;
    int dirty; 
    } entry; //Direct Mapped

typedef struct {
    unsigned long long int addr;
    int dirty;
    unsigned long long int time; 
} faentry; //Fully Associative

//*** Cache Arrays ***//
//Direct Mapped
entry L1i[L1_cache_size/L1_block_size];
entry L1d[L1_cache_size/L1_block_size];
entry L2[L2_cache_size/L2_block_size];

//Fully Associative
faentry faL1i[L1_cache_size/L1_block_size];
faentry faL1d[L1_cache_size/L1_block_size];
faentry faL2[L2_cache_size/L2_block_size];

//**********************************************************//
//************************** In Cache? *********************//
//**********************************************************//

int inL1i(unsigned long long int address){
    int index; 
    if(DIRECT_MAPPED){
        index = (address & (~0xF)) >> 4;
        index = index & ((L1_cache_size/L1_block_size)-1);
        if(L1i[index].addr  == (address & (~0xF))) // note: we check the entire address including the tag becaues why not 
            return 1;
    }

    if(FULLY_ASSOC){
	    index = (address & (~0xF)) >> 4;
	    int i;
	    for(i=0; i<(L1_cache_size/L1_block_size); i++){
	        if(faL1i[i].addr == index)
	            return 1; 
	    }

    }
    return 0; 
} //returns 0 if false, 1 if true 

int inL1d(unsigned long long int address){
    int index; 
    if(DIRECT_MAPPED){
        index = (address & (~0xF)) >> 4;
        index = index & ((L1_cache_size/L1_block_size)-1); 
        if(L1d[index].addr  == (address & (~0xF)))
            return 1; 
        }
    
    if(FULLY_ASSOC){
	    index = (address & (~0xF)) >> 4;
	    int i;
	    for(i=0; i<(L1_cache_size/L1_block_size); i++){
	        if(faL1d[i].addr == index)
	            return 1; 
	    }

    }
    return 0; 
}

int inL2(unsigned long long int address){
    int index; 
    if(DIRECT_MAPPED){
        index = (address & (~0xF)) >> 4;
        index = index & ((L2_cache_size/L2_block_size)-1); 
        if(L2[index].addr  == (address & (~0xF)))
            return 1; 
        }
    
    if(FULLY_ASSOC){
	    index = (address & (~0xF)) >> 4;
	    int i;
	    for(i=0; i<(L1_cache_size/L1_block_size); i++){
	        if(faL2[i].addr == index)
	            return 1; 
    	}
    }
    return 0; 
}

//**********************************************************//
//************************ Load Cache **********************//
//**********************************************************//

int loadL1i(unsigned long long int address){
    int index;
    int index2; 
    if(DIRECT_MAPPED){
        index = (address & (~0xF)) >> 4;
        index2 = (address & (~0xF)) >> 4;
        index2 = index & ((L2_cache_size/L2_block_size)-1);
	    if(L1i[index].addr == 0){ // cache spot is empty
	        	L1i[index].addr=(address & (~0xF)); 
	        }
	    else {
	        if(L1i[index].dirty == 0) // we can just dump the block
	            L1i[index].addr= (address & (~0xF));
	        else{ // we must write to L2
	            L2[index2].addr = (address & (~0xF)); // is this logic right? I worry we are writing to the wrong l2 block. 
	            L2[index2].dirty = 1; 
	        }
	    }// cache spot is not empty
	}
    if(FULLY_ASSOC){
    }
    return 0; 
} // puts the address in the L1 cache. DOES NOT SPAN BLOCKS!

int loadL1d(unsigned long long int address){
    int index;
    int index2; 
    if(DIRECT_MAPPED){
        index = (address & (~0xF)) >> 4;
        index = index & ((L1_cache_size/L1_block_size)-1);
        index2 = (address & (~0xF)) >> 4;
        index2 = index & ((L2_cache_size/L2_block_size)-1);

	    if(L1d[index].addr == 0){ // cache spot is empty
	        L1d[index].addr=(address & (~0xF)); 
	    }
	    else {
	        if(L1d[index].dirty == 0) // we can just dump the block
	            L1d[index].addr= (address & (~0xF));
	        else{ // we must write to L2
	            L2[index2].addr = (address & (~0xF)); // is this logic right? I worry we are writing to the wrong l2 block. 
	            L2[index2].dirty = 1; 
	        }
	    }// cache spot is not empty
    }
    return 0; 
} 

int loadL2(unsigned long long int address){
   int index; 
    if(DIRECT_MAPPED){
        index = (address & (~0xF)) >> 4;
        index = index & ((L2_cache_size/L2_block_size)-1);
        L2[index].addr=(address & (~0xF)); 
    }
    return 0; 
} //puts the address in the L2 cache. DOES NOT SPAN BLOCKS! Must be a block address!

//**********************************************************//
//************************* Dirty Bits *********************//
//**********************************************************//

int dirtyL1i(int value, unsigned long long int address){
    if(DIRECT_MAPPED){
        int index; 
        index = (address & (~0xF)) >> 4;
        index = index & ((L1_cache_size/L1_block_size)-1); 
        if(L1i[index].addr != (address & (~0xF)) || L1i[index].addr == 0 )
            return -1; //something's gone wrong, we've tried to dirty something not in the cache.
        else
            L1i[index].dirty = value; 
    }
    return 0; 
}

int dirtyL1d(int value, unsigned long long int address){
    if(DIRECT_MAPPED){
        int index; 
        index = (address & (~0xF)) >> 4;
        index = index & ((L1_cache_size/L1_block_size)-1); 
        if(L1d[index].addr != (address & (~0xF)) || L1d[index].addr == 0 )
            return -1; //something's gone wrong, we've tried to dirty something not in the cache.
        else
            L1d[index].dirty = value; 
    }
    return 0; 
}

int dirtyL2(int value, unsigned long long int address){

    if(DIRECT_MAPPED){
        int index; 
        index = (address & (~0xF)) >> 4;
        index = index & ((L2_cache_size/L2_block_size)-1); 
        if(L2[index].addr != (address & (~0xF)) || L1d[index].addr == 0 )
            return -1; //something's gone wrong, we've tried to dirty something not in the cache.
        else
            L2[index].dirty = value;   
    }
    return 0; 
}

//**********************************************************//
//************************ Cache Flush *********************//
//**********************************************************//

int cache_flush(){
    return 0; 
    }; 

//**********************************************************//
//*************************** Access ***********************//
//**********************************************************//

int access(char op, unsigned long long int address) //this assumes address is a word boundary and we're only grabbing a word.
// this function returns the time it took for the access NOTE: this does not include the time to execute a fetched instruction
{

    unsigned long long int cycles = 0;
    
    if(op == 'R'){//read data
	    
	    if (inL1d(address)){
	        cycles+=L1_hit_time;
	    }
	    else{
	        cycles+=L1_miss_time;
	        if(inL2(address)){ // not in L1 cache but in L2 
	            cycles+=L2_hit_time+L1_hit_time+(L2_transfer_time*( ((float) L1_block_size) / ((float ) L2_bus_width) ));
	            loadL1d(address); // put it in the L1 cache after accessing it from teh L2 cache. 
	        }
	        else{
	                cycles+=L2_miss_time+mem_sendaddr+mem_ready+ (((float)(mem_chunktime*L2_block_size))/((float) mem_chunksize))+L2_hit_time+L1_hit_time+(L2_transfer_time*( ((float) L1_block_size) / ((float ) L2_bus_width) ));
	                loadL2(address);
	                loadL1d(address);
	        }
	   	}
    }
    else if(op == 'I'){//read inst
        
        if (inL1i(address)){
            cycles+=L1_hit_time;
        }
	    else{
	        cycles+=L1_miss_time;
	        if(inL2(address)){ // not in L1 cache but in L2 
	            cycles+=L2_hit_time+L1_hit_time+(L2_transfer_time*( ((float) L1_block_size) / ((float ) L2_bus_width) ));
	            loadL1i(address); // put it in the L1 cache after accessing it from teh L2 cache. 
	        }
	        else{
	                cycles+=L2_miss_time+mem_sendaddr+mem_ready+ (((float)(mem_chunktime*L2_block_size))/((float) mem_chunksize))+L2_hit_time+L1_hit_time+(L2_transfer_time*( ((float) L1_block_size) / ((float ) L2_bus_width) ));
	                loadL2(address);
	                loadL1i(address);
	        }
    	}
    }
    else if(op == 'W'){//write
	    if (inL1d(address)){
	        cycles+=L1_hit_time;
	    }
	    else{
	        cycles+=L1_miss_time;
	        if(inL2(address)){ // not in L1 cache but in L2 
	            cycles+=L2_hit_time+L1_hit_time+(L2_transfer_time*( ((float) L1_block_size) / ((float ) L2_bus_width) ));
	            loadL1d(address); // put it in the L1 cache after accessing it from the L2 cache.
	    	}
	        else{
	                cycles+=L2_miss_time+mem_sendaddr+mem_ready+ (((float)(mem_chunktime*L2_block_size))/((float) mem_chunksize))+L2_hit_time+L1_hit_time+(L2_transfer_time*( ((float) L1_block_size) / ((float ) L2_bus_width) ));
	                loadL2(address);
	                loadL1d(address);l
        	}
        }
	}
	else{
		dirtyL1d(1,address); // Mark the L1 block dirty. L2 is left clean as per the instructions.
	    return -1; 
	}

	dirtyL1d(1,address); // Mark the L1 block dirty. L2 is left clean as per the instructions.
	return cycles;
}

int main(){

    char op;
    unsigned long long int address;
    unsigned int bytesize;
    int numwords = 0;
    int i; 
    unsigned long long int wordstart;

    while(scanf("%c %Lx %d\n",&op,&address,&bytesize)==3){
        if(op=='I')
            totalexec++; //do this out here because of alignment: each aligned reference is not an instruction execution
        
        numwords = (bytesize + (address%4))/4; 
        if((bytesize+ (address%4)) % 4 != 0) //this logic seems right, but may be questionable. Double check if it doens't work.
            numwords ++;
       //printf("Number of words: %d\n",numwords);
        wordstart = (address - (address%4));
        for(i = 0; i<numwords; i++){
            int temp;
            temp = access(op,wordstart+(4*i));
            totalexec+=temp; //access each necessary word.
          //  printf("Access time: %d \n", temp);
        }
        if((totalexec % 380000) == 0 && totalexec != 0 )
            cache_flush(); 
        }

    printf("%Ld\n",totalexec);
    return 0;
}
