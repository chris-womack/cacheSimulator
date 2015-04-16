#include <stdio.h>
#include <stdlib.h>
#include "L1.h"
#include "L2.h"
#include "gen.h"
#include "configlib.h"

int inL2(unsigned long long int address){
    int index; 
    if(DIRECT_MAPPED){
        index = (address & (~0xF)) >> 4;
        index = index & ((L2_cache_size/L2_block_size)-1); 
        if(dmL2[index].addr  == (address & (~0xF))){
            return 1; 
        }
    }
    else if(FULLY_ASSOC){
        index = (address & (~0xF)) >> 4;
        int i;
        for(i=0; i<(L1_cache_size/L1_block_size); i++){
            if(faL2[i].addr == index){
                return 1; 
            }
        }
    }
    else{ // SET_ASSOC
        // set assoc here
    }
    return 0; 
}
    
int loadL2(unsigned long long int address){
   int index; 
    if(DIRECT_MAPPED){
        index = (address & (~0xF)) >> 4;
        index = index & ((L2_cache_size/L2_block_size)-1);
        if(dmL2[index].addr == 0){ // cache spot is empty
            dmL2[index].addr=(address & (~0xF));
        }
        else{
            dmL2[index].addr = (address & (~0xF));
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
        index = (address & (~0xF)) >> 4;
        index = index & ((L2_cache_size/L2_block_size)-1); 
        if(dmL2[index].addr != (address & (~0xF)) || dmL1d[index].addr == 0 ){
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