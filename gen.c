#include <stdio.h>
#include <stdlib.h>
#include "L1.h"
#include "L2.h"
#include "gen.h"
#include "configlib.h"

int printCacheL1(){
    int i;

    printf("\n");
    printf("Valid Blocks in L1 Instruction Cache\n");
    printf("\n");
    for(i = 0; i < ((L1_cache_size/2)/L1_block_size); i++){
    	if(dmL1i[i].addr != 0)
    		printf("Block: %d 	Address: 0x%llx 			Dirty: %d \n", i, dmL1i[i].addr, dmL1i[i].dirty);
    }
	printf("\n");
	printf("Valid Blocks in L1 Data Cache\n");
	printf("\n");
    for(i = 0; i < ((L1_cache_size/2)/L2_block_size); i++){
    	if(dmL1d[i].addr != 0)
    		printf("Block: %d 	Address: 0x%llx 		Dirty: %d \n", i, dmL1d[i].addr, dmL1d[i].dirty);
    }

    printf("\n");
    printf("All Blocks in L1 Instruction Cache\n");
    printf("\n");
    for(i = 0; i < ((L1_cache_size/2)/L1_block_size); i++){
    	printf("Block: %d 	Address: 0x%llx 			Dirty: %d \n", i, dmL1i[i].addr, dmL1i[i].dirty);
    }
	printf("\n");
	printf("All Blocks in L1 Data Cache\n");
	printf("\n");
    for(i = 0; i < ((L1_cache_size/2)/L2_block_size); i++){
    	printf("Block: %d 	Address: 0x%llx 		Dirty: %d \n", i, dmL1d[i].addr, dmL1d[i].dirty);
    }
}

int printCacheL2(){
    int i;
    printf("\n");
    printf("Valid Blocks in L2 Cache\n");
    printf("\n");
    for(i = 0; i < (L2_cache_size/L1_block_size); i++){
    	if(dmL2[i].addr != 0)
    		printf("Block: %d 	Address: 0x%llx 			Dirty: %d \n", i, dmL2[i].addr, dmL2[i].dirty);
    }
    printf("\n");
    printf("All Blocks in L2 Cache\n");
    printf("\n");
    for(i = 0; i < (L2_cache_size/L1_block_size); i++){
    	printf("Block: %d 	Address: 0x%llx 			Dirty: %d \n", i, dmL2[i].addr, dmL2[i].dirty);
    }
}

int cacheFlush(){
    int i;
    for(i =0; i < (L2_cache_size/L1_block_size); i++){

    }
    return 0; 
}; 

int access(char op, unsigned long long int address){
    unsigned long long int cycles = 0;
    
    if(op == 'R'){ // read
        if (inL1d(address)){
            cycles+=L1_hit_time;
        }
        else{
            cycles+=L1_miss_time;
            if(inL2(address)){ // not in L1 cache but in L2 
                cycles+=L2_hit_time+L1_hit_time+(L2_transfer_time*( ((float) L1_block_size) / ((float ) L2_bus_width) ));
                cycles+=loadL1d(address); // put it in the L1 cache after accessing it from teh L2 cache. 
            }
            else{
                    cycles+=L2_miss_time+mem_sendaddr+mem_ready+ (((float)(mem_chunktime*L2_block_size))/((float) mem_chunksize))+L2_hit_time+L1_hit_time+(L2_transfer_time*( ((float) L1_block_size) / ((float ) L2_bus_width) ));
                    cycles+=loadL2(address);
                    cycles+=loadL1d(address);
            }
        }
    }
    else if(op == 'I'){ // instruction
        if (inL1i(address))
            cycles+=L1_hit_time;
        else{
            cycles+=L1_miss_time;
            if(inL2(address)){ // not in L1 cache but in L2 
                cycles+=L2_hit_time+L1_hit_time+(L2_transfer_time*( ((float) L1_block_size) / ((float ) L2_bus_width) ));
                loadL1i(address); // put it in the L1 cache after accessing it from the L2 cache. Note: don't care about writebacks since instructions won't.
            }
            else{
                cycles+=L2_miss_time+mem_sendaddr+mem_ready+ (((float)(mem_chunktime*L2_block_size))/((float) mem_chunksize))+L2_hit_time+L1_hit_time+(L2_transfer_time*( ((float) L1_block_size) / ((float ) L2_bus_width) ));
                cycles+=loadL2(address);
                loadL1i(address);
            }
        }
    }
    else if(op == 'W'){ // write
        if (inL1d(address)){
            cycles+=L1_hit_time;
        }
        else{
            cycles+=L1_miss_time;
            if(inL2(address)){ // not in L1 cache but in L2 
                cycles+=L2_hit_time+L1_hit_time+(L2_transfer_time*( ((float) L1_block_size) / ((float ) L2_bus_width) ));
                cycles+=loadL1d(address); // put it in the L1 cache after accessing it from teh L2 cache.
            }
            else{
                cycles+=L2_miss_time+mem_sendaddr+mem_ready+ (((float)(mem_chunktime*L2_block_size))/((float) mem_chunksize))+L2_hit_time+L1_hit_time+(L2_transfer_time*( ((float) L1_block_size) / ((float ) L2_bus_width) ));
                cycles+=loadL2(address);
                cycles+=loadL1d(address);
            }
        }
        dirtyL1d(1,address); // we mark the L1 block dirty no matter what. L2 is left clean as per the instructions.
    }
    else{
        return -1; 
    }
    return cycles;
}