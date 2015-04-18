/* General Library for CacheSim */

// log2 of number of blocks
unsigned long long log2(unsigned long long num);

// Flushes cache every 380,000 cycles
int cacheFlush();

// Main function to maths the cycles. Assumes address is on word boundary.
// Returns time it took for access not including fetch execution time.
int access(char op, unsigned long long address);

// Prints only Valid blocks in L1 and L2
int printValid();

// Prints all L1 cache blocks
int printL1();

// Prints all L2 cache blocks
int printL2();