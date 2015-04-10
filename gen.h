/* General Library for CacheSim */

// Flushes cache every 380,000 cycles
int cache_flush();

// Main function to maths the cycles. Assumes address is on word boundary.
// Returns time it took for access not including fetch execution time.
int access(char op, unsigned long long int address);