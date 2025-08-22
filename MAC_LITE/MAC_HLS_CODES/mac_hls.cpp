#include <ap_int.h>

void mac_HLS(
    ap_uint<8> a,
    ap_uint<8> b,
    ap_uint<1> reset,
    ap_uint<1> enable,
    ap_uint<16>& accumulator)
{
    // AXI-Lite control interface
#pragma HLS INTERFACE s_axilite port=a           bundle=CTRL
#pragma HLS INTERFACE s_axilite port=b           bundle=CTRL
#pragma HLS INTERFACE s_axilite port=reset       bundle=CTRL
#pragma HLS INTERFACE s_axilite port=enable      bundle=CTRL
#pragma HLS INTERFACE s_axilite port=accumulator bundle=CTRL
#pragma HLS INTERFACE s_axilite port=return      bundle=CTRL

// Persistent accumulator register
    static ap_uint<16> internal_accumulator = 0;

    // Reset condition
    if (reset) {
        internal_accumulator = 0;
    }
    // MAC operation
    else if (enable) {
        internal_accumulator += (a * b);
    }

    // Output current accumulator value
    accumulator = internal_accumulator;
}
