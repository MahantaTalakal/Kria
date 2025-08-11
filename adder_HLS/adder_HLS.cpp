#include <ap_int.h>

void adder_HLS(
    ap_uint<32> a,
    ap_uint<32> b,
    ap_uint<32> &sum)
{
  #pragma HLS INTERFACE s_axilite port=a      bundle=CTRL
  #pragma HLS INTERFACE s_axilite port=b      bundle=CTRL
  #pragma HLS INTERFACE s_axilite port=sum    bundle=CTRL
  #pragma HLS INTERFACE ap_ctrl_none port=return

  sum = a + b;
}
