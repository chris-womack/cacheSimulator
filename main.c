#include <stdio.h>
#include <stdlib.h>
#include "L1.h"
#include "L2.h"
#include "gen.h"
#include "configlib.h"

//**********************************************************//
//**************** Notes for next time *********************//
//**********************************************************//

// Also note: For cache flush use loadL1i and loadL1d and fill with 0 entries. 
// This way the load logic can handle all of the dirty stuff since load is going to 
// return whether we wrote back/ how many cycles it took, this should work for timing 
// the cache flush. The cache flush also is going to return how many cycles it took.

int main(int argc, char *argv[]){
    unsigned long long int totalexec = 0;

    char op;
    unsigned long long int address;
    unsigned int bytesize;

    int i = 0;
    int numwords = 0;
    unsigned long long int wordstart;
    
    if(argv[1])
        configRead(argv[1]);
    else
        configRead(NULL);
    
    // initialize cache arrays from config
    dmL1i = malloc(((L1_cache_size)/L1_block_size)*sizeof(dmEntry));
    dmL1d = malloc(((L1_cache_size)/L1_block_size)*sizeof(dmEntry));
    dmL2  = malloc(((L2_cache_size)/L2_block_size)*sizeof(dmEntry));

    faL1i = malloc(((L1_cache_size)/L1_block_size)*sizeof(faEntry));
    faL1d = malloc(((L1_cache_size)/L1_block_size)*sizeof(faEntry));
    faL2  = malloc(((L2_cache_size)/L2_block_size)*sizeof(faEntry));

    saL1i = malloc(((L1_cache_size)/L1_block_size)*sizeof(saEntry));
    saL1d = malloc(((L1_cache_size)/L1_block_size)*sizeof(saEntry));
    saL2  = malloc(((L2_cache_size)/L2_block_size)*sizeof(saEntry));

    // 
    while(scanf("%c %Lx %d\n",&op,&address,&bytesize)==3){
        if(op=='I'){
            totalexec++; //do this out here because of alignment: each aligned reference is not an instruction execution
        }
        numwords =(bytesize+(address%4))/4; 
        if((bytesize+(address%4))%4 != 0){ //this logic seems right, but may be questionable. Double check if it doens't work.
            numwords ++;
        }
       //printf("Number of words: %d\n",numwords);
        wordstart = (address-(address%4));
        for (i = 0; i<numwords; i++){
            int temp;
            temp = access(op,wordstart+(4*i));
            totalexec+=temp; //access each necessary word.
          //  printf("Access time: %d \n", temp);
        }
        //if((totalexec % 380000) == 0 && totalexec != 0 )
        //    cache_flush(); 
    }

    printf("%Ld\n",totalexec);

    printCacheL1();
    printCacheL2();

    // dealloc cache arrays
    free(dmL1i);
    free(dmL1d);
    free(dmL2);

    free(faL1i);
    free(faL1d);
    free(faL2);

    free(saL1i);
    free(saL1d);
    free(saL2);

    return 0;
}
