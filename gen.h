/* General Library for CacheSim */

// Prints all valid cache blocks
int printCacheL1();

// Prints all valid cache blocks
int printCacheL2();

// Flushes cache every 380,000 cycles
int cacheFlush();

// Main function to maths the cycles. Assumes address is on word boundary.
// Returns time it took for access not including fetch execution time.
int access(char op, unsigned long long int address);