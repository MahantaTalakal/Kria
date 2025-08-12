#include "ap_int.h"
#include "ap_axi_sdata.h"
#include <hls_stream.h>

// BRAM for storing the seed (single value)
static ap_uint<8> seed_bram[1];

void lfsr_HLS(
    bool start,             // Signal to start LFSR with seed from BRAM
    bool write_seed,        // Signal to write seed to BRAM
    ap_uint<8> seed_in,     // Input seed value when write_seed is true
    ap_uint<8> &random_out, // Output random number
    bool &valid_out)        // Output valid signal
{
    #pragma HLS INTERFACE s_axilite port=start bundle=CTRL
    #pragma HLS INTERFACE s_axilite port=write_seed bundle=CTRL
    #pragma HLS INTERFACE s_axilite port=seed_in bundle=CTRL
    #pragma HLS INTERFACE s_axilite port=random_out bundle=CTRL
    #pragma HLS INTERFACE s_axilite port=valid_out bundle=CTRL
    #pragma HLS INTERFACE ap_ctrl_none port=return
    
    // BRAM interface - HLS will infer the appropriate BRAM resource
    #pragma HLS RESOURCE variable=seed_bram core=RAM_1P

    // Static variable to hold current LFSR state across function calls
    static ap_uint<8> lfsr_state = 0x01;
    
    // Default outputs
    valid_out = false;
    
    // Handle seed write operation
    if (write_seed) {
        seed_bram[0] = seed_in;
    }
    
    // Handle LFSR start/reset with seed from BRAM
    if (start) {
        lfsr_state = seed_bram[0];
        if (lfsr_state == 0) lfsr_state = 0x01; // Avoid all-zero state
        valid_out = true;
        random_out = lfsr_state;
    }
    else {
        // Standard 8-bit maximal LFSR (Fibonacci implementation)
        // Polynomial: x^8 + x^6 + x^5 + x^4 + 1
        if (lfsr_state != 0) { // Ensure we're not in a stuck state
            ap_uint<1> feedback = ((lfsr_state >> 7) & 1) ^
                                 ((lfsr_state >> 5) & 1) ^
                                 ((lfsr_state >> 4) & 1) ^
                                 ((lfsr_state >> 3) & 1);
            
            lfsr_state = (lfsr_state << 1) | feedback;
            valid_out = true;
            random_out = lfsr_state;
        }
    }
}
