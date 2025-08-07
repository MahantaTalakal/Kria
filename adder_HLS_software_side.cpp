#include "xparameters.h"
#include "xil_io.h"
#include <stdio.h>

#define ADDER_BASE    0xa0000000

#define ADDR_A_OFFSET    0x10
#define ADDR_B_OFFSET    0x18
#define ADDR_SUM_OFFSET  0x20

int main()
{
    u32 a = 123, b = 456;
    u32 sum;

    // Write inputs
    Xil_Out32(ADDER_BASE + ADDR_A_OFFSET, a);
    Xil_Out32(ADDER_BASE + ADDR_B_OFFSET, b);

    // Read result
    sum = Xil_In32(ADDER_BASE + ADDR_SUM_OFFSET);

    // Print to UART
    printf("a = %u, b = %u, sum = %u\n", a, b, sum);

    while (1); 
    return 0;
}
