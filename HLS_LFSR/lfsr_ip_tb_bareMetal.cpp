/**
 * LFSR Application for Kria KR260
 */
#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xil_io.h"
#include "sleep.h"
#include "xparameters.h"

// Assuming the LFSR IP is at this base address - update based on your actual address
#define LFSR_IP_BASEADDR XPAR_LFSR_HLS_0_S_AXI_CTRL_BASEADDR

// Register offsets
#define START_REG_OFFSET      0x10  // Address offset for start signal
#define WRITE_SEED_REG_OFFSET 0x18  // Address offset for write_seed signal
#define SEED_IN_REG_OFFSET    0x20  // Address offset for seed_in value
#define RANDOM_OUT_REG_OFFSET 0x28  // Address offset for random_out value
#define VALID_OUT_REG_OFFSET  0x30  // Address offset for valid_out signal

int main()
{
    init_platform();
    
    xil_printf("LFSR Application Started\r\n");
    
    // Initialize with a seed
    u8 seed = 0x42;
    xil_printf("Writing seed: 0x%02x to BRAM\r\n", seed);
    
    // Set seed input value
    Xil_Out32(LFSR_IP_BASEADDR + SEED_IN_REG_OFFSET, seed);
    
    // Set write_seed to true
    Xil_Out32(LFSR_IP_BASEADDR + WRITE_SEED_REG_OFFSET, 1);
    
    // Reset write_seed to false
    Xil_Out32(LFSR_IP_BASEADDR + WRITE_SEED_REG_OFFSET, 0);
    
    // Start the LFSR with seed from BRAM
    xil_printf("Starting LFSR with seed from BRAM\r\n");
    Xil_Out32(LFSR_IP_BASEADDR + START_REG_OFFSET, 1);
    
    // Reset start signal
    Xil_Out32(LFSR_IP_BASEADDR + START_REG_OFFSET, 0);
    
    // Generate and print random numbers
    xil_printf("Generating random numbers:\r\n");
    for (int i = 0; i < 20; i++) {
        u8 random_value = (u8)Xil_In32(LFSR_IP_BASEADDR + RANDOM_OUT_REG_OFFSET);
        u8 valid = (u8)Xil_In32(LFSR_IP_BASEADDR + VALID_OUT_REG_OFFSET);
        
        if (valid) {
            xil_printf("Random[%d]: 0x%02x\r\n", i, random_value);
        } else {
            xil_printf("Error: Output not valid\r\n");
        }
        
        // Generate next value
        usleep(100000); // 100ms delay for demonstration
    }
    
    // Change the seed and test again
    seed = 0x81;
    xil_printf("\r\nChanging seed to: 0x%02x\r\n", seed);
    
    // Set new seed input value
    Xil_Out32(LFSR_IP_BASEADDR + SEED_IN_REG_OFFSET, seed);
    
    // Set write_seed to true
    Xil_Out32(LFSR_IP_BASEADDR + WRITE_SEED_REG_OFFSET, 1);
    
    // Reset write_seed to false
    Xil_Out32(LFSR_IP_BASEADDR + WRITE_SEED_REG_OFFSET, 0);
    
    // Start the LFSR with new seed
    Xil_Out32(LFSR_IP_BASEADDR + START_REG_OFFSET, 1);
    Xil_Out32(LFSR_IP_BASEADDR + START_REG_OFFSET, 0);
    
    // Generate and print more random numbers
    xil_printf("Generating random numbers with new seed:\r\n");
    for (int i = 0; i < 10; i++) {
        u8 random_value = (u8)Xil_In32(LFSR_IP_BASEADDR + RANDOM_OUT_REG_OFFSET);
        xil_printf("Random[%d]: 0x%02x\r\n", i, random_value);
        usleep(100000); // 100ms delay
    }
    
    xil_printf("LFSR Application Complete\r\n");
    cleanup_platform();
    return 0;
}