#include <stdio.h>
#include <stdlib.h>
#include "L1.h"
#include "L2.h"
#include "gen.h"
#include "configlib.h"

int inL2(unsigned long long int address){
    int index; 
    if(DIRECT_MAPPED){
        index = (address >> numByteBounL2) % blockNumL2;
        if(dmL2[index].tag  == (address >> numBitsL2)){
            return 1; 
        }
    }
    else if(FULLY_ASSOC){
    }
    else{ // SET_ASSOC
        // set assoc here
    }
    return 0; 
}
    
int loadL2(unsigned long long int address){
   int index; 
    if(DIRECT_MAPPED){
        index = (address >> numByteBounL2) % blockNumL2;
        if(dmL2[index].valid == 0){ // cache spot is empty
            dmL2[index].valid = 1;
            dmL2[index].addr = (address);
            dmL2[index].tag = (address >> numBitsL2);
        }
        else{
            dmL2[index].valid = 1;
            dmL2[index].addr = (address);
            dmL2[index].tag = (address >> numBitsL2);
            if(dmL2[index].dirty == 0){ // we can dump the block
                return 0; // so it didn't take any extra cycles
            }
            else{
                return L2_miss_time+mem_sendaddr+mem_ready+ (((float)(mem_chunktime*L2_block_size))/((float) mem_chunksize))+L2_hit_time+L1_hit_time+(L2_transfer_time*( ((float) L1_block_size) / ((float ) L2_bus_width) )); //number of cycles to write block to MM
            }
        }
        return 0; // no extra cycles
    }
    else if(FULLY_ASSOC){
        // full assoc here
    }
    else{ // SET_ASSOC
        // set assoc here
    }
    return 0; 
}

int dirtyL2(int value, unsigned long long int address){
    if(DIRECT_MAPPED){
        int index; 
        index = (address >> numByteBounL2) % blockNumL2;
        if(dmL2[index].tag != (address >> numBitsL2) || dmL1d[index].valid == 0 ){
            return -1; //We've tried to dirty something not in the cache.
        }
        else{
            dmL2[index].dirty = value; 
        }
    }
    else if(FULLY_ASSOC){
        // full assoc here
    }
    else{ // SET_ASSOC
        // set assoc here
    }
    return 0; 
}