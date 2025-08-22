#include <stdio.h>
#include <stdint.h>
#include "ap_int.h"

// Function prototype for the MAC HLS module
void mac_HLS(
    ap_uint<8> a,
    ap_uint<8> b,
    ap_uint<1> reset,
    ap_uint<1> enable,
    ap_uint<16>& accumulator);

// Helper function to check if result matches expected value
void check_result(uint16_t actual, uint16_t expected, const char* test_name) {
    if (actual == expected) {
        printf("✓ %s: PASS (Value: %u)\n", test_name, actual);
    }
    else {
        printf("✗ %s: FAIL (Expected: %u, Got: %u)\n", test_name, expected, actual);
    }
}

int main() {
    ap_uint<16> accumulator = 0;
    uint16_t expected = 0;

    printf("\n=== MAC HLS Module Testbench ===\n\n");

    // Test 1: Reset functionality
    mac_HLS(0, 0, 1, 0, accumulator);
    check_result(accumulator, 0, "Reset");

    // Test 2: Simple multiply and accumulate
    mac_HLS(5, 7, 0, 1, accumulator);
    expected = 5 * 7;  // 35
    check_result(accumulator, expected, "Simple MAC");

    // Test 3: Accumulate another value
    mac_HLS(10, 3, 0, 1, accumulator);
    expected += 10 * 3;  // 35 + 30 = 65
    check_result(accumulator, expected, "Accumulation");

    // Test 4: Disabled operation (should not change accumulator)
    mac_HLS(100, 100, 0, 0, accumulator);
    check_result(accumulator, expected, "Disabled operation");

    // Test 5: Multiple accumulations
    for (int i = 1; i <= 5; i++) {
        mac_HLS(i, i, 0, 1, accumulator);
        expected += i * i;
    }
    check_result(accumulator, expected, "Multiple accumulations");

    // Test 6: Reset in the middle of operations
    mac_HLS(0, 0, 1, 0, accumulator);
    expected = 0;
    check_result(accumulator, expected, "Mid-operation reset");

    // Test 7: Edge case - max values
    mac_HLS(255, 255, 0, 1, accumulator);
    expected = 255 * 255;  // 65025
    check_result(accumulator, expected, "Max input values");

    printf("\n=== Testbench Complete ===\n");
    return 0;
}
