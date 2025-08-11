#include <iostream>
#include <ap_int.h>

// Declaration of the HLS function
void adder_HLS(
    ap_uint<32> a,
    ap_uint<32> b,
    ap_uint<32> &sum);

int main() {
    // Test vectors
    const int NUM_TESTS = 5;
    ap_uint<32> a_vals[NUM_TESTS] = {0, 1, 0xFFFFFFFF, 12345678, 0x80000000};
    ap_uint<32> b_vals[NUM_TESTS] = {0, 0xFFFFFFFF, 1, 87654321, 0x80000000};

    bool all_passed = true;
    
    for (int i = 0; i < NUM_TESTS; ++i) {
        ap_uint<32> a = a_vals[i];
        ap_uint<32> b = b_vals[i];
        ap_uint<32> sum;

        // Call the HLS function
        adder_HLS(a, b, sum);

        // Expected result
        ap_uint<32> expected = a + b;

        // Print results
        std::cout << "Test " << i << ": a=0x" << std::hex << a \
                  << ", b=0x" << b \
                  << ", sum=0x" << sum \
                  << ", expected=0x" << expected;

        if (sum == expected) {
            std::cout << " -> PASSED" << std::endl;
        } else {
            std::cout << " -> FAILED" << std::endl;
            all_passed = false;
        }
    }

    if (all_passed) {
        std::cout << "\nAll tests passed!" << std::endl;
        return 0;
    } else {
        std::cout << "\nSome tests failed." << std::endl;
        return 1;
    }
}
