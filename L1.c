#include <stdio.h>
#include <stdlib.h>
#include "L1.h"
#include "L2.h"
#include "gen.h"
#include "configlib.h"

int inL1i(unsigned long long int address){
    int index; 
    if(DIRECT_MAPPED){
        index = (address & (~0xF)) >> 4;
        index = index & ((L1_cache_size/L1_block_size)-1);
        if(dmL1i[index].addr  == (address & (~0xF))){ // note: we check the entire address including the tag becaues why not 
            return 1;
        }
    }
    else if(FULLY_ASSOC){
        index = (address & (~0xF)) >> 4;
        int i;
        for(i=0; i<(L1_cache_size/L1_block_size); i++){
            if(faL1i[i].addr == index){
                return 1; 
            }
        }
    }
    else{ // SET_ASSOC
        // set assoc here
    }
    return 0; 
}
int inL1d(unsigned long long int address){
    int index; 
    if(DIRECT_MAPPED){
        index = (address & (~0xF)) >> 4;
        index = index & ((L1_cache_size/L1_block_size)-1); 
        if(dmL1d[index].addr  == (address & (~0xF)))
            return 1; 
        } 
    else if(FULLY_ASSOC){
        index = (address & (~0xF)) >> 4;
        int i;
        for(i=0; i<(L1_cache_size/L1_block_size); i++){
            if(faL1d[i].addr == index)
                return 1; 
        }
    }
    else{ // SET_ASSOC
        // set assoc here
    }
    return 0; 
}

int loadL1i(unsigned long long int address){
    int index;
    if(DIRECT_MAPPED){
        index = (address & (~0xF)) >> 4;
        index = index & ((L1_cache_size/(L1_block_size*2))-1); 
        if(dmL1i[index].addr == 0){ // cache spot is empty
            dmL1i[index].addr=(address & (~0xF)); 
            }
        else {
            if(dmL1i[index].dirty == 0){ // we can just dump the block
                dmL1i[index].addr= (address & (~0xF));
            }
            else{ // we must write to L2
                return -10; //we should not ever have a dirty instruction block since instructions are write only.
            }
        }// cache spot is not empty
    }
    else if(FULLY_ASSOC){
        // full assoc here
    }
    else{ // SET_ASSOC
        // set assoc here
    }
    return 0; 
}
int loadL1d(unsigned long long int address){
    int index;
    if(DIRECT_MAPPED){
        index = (address & (~0xF)) >> 4;
        index = index & ((L1_cache_size/(L1_block_size*2))-1);
        if(dmL1d[index].addr == 0){ // cache spot is empty
            dmL1d[index].addr=(address & (~0xF)); 
        }
        else {
            if(dmL1d[index].dirty == 0){ // we can just dump the block
                dmL1d[index].addr= (address & (~0xF));
            }
            else{ // we must write to L2 and return the number of cycles it took us
                int temp = loadL2(address);
                dirtyL2(1,address);
                return temp+L2_hit_time+L1_hit_time+(L2_transfer_time*( ((float) L1_block_size) / ((float ) L2_bus_width) )); //additional time to write block to L2
            }
        }// cache spot is not empty
    }
    else if(FULLY_ASSOC){
    // full assoc here
    }
    else{ // SET_ASSOC
        // set assoc here
    }
    return 0; 
} 



int dirtyL1i(int value, unsigned long long int address){
    if(DIRECT_MAPPED){
        int index; 
        index = (address & (~0xF)) >> 4;
        index = index & ((L1_cache_size/L1_block_size)-1); 
        if(dmL1i[index].addr != (address & (~0xF)) || dmL1i[index].addr == 0 ){
            return -1; //something's gone wrong, we've tried to dirty something not in the cache.
        }
        else{
            dmL1i[index].dirty = value; 
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

int dirtyL1d(int value, unsigned long long int address){
    if(DIRECT_MAPPED){
        int index; 
        index = (address & (~0xF)) >> 4;
        index = index & ((L1_cache_size/L1_block_size)-1); 
        if(dmL1d[index].addr != (address & (~0xF)) || dmL1d[index].addr == 0 ){
            return -1; //something's gone wrong, we've tried to dirty something not in the cache.
        }
        else{
            dmL1d[index].dirty = value; 
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
