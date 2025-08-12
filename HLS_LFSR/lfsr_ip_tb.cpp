#include <iostream>
#include <vector>
#include "ap_int.h"

// Function declaration
void lfsr_HLS(
    bool start,
    bool write_seed,
    ap_uint<8> seed_in,
    ap_uint<8> &random_out,
    bool &valid_out);

int main() {
    bool start, write_seed, valid_out;
    ap_uint<8> seed_in, random_out;
    std::vector<ap_uint<8>> random_values;
    int failures = 0;
    
    std::cout << "LFSR Testbench Started" << std::endl;
    
    // Test 1: Write seed and start LFSR
    std::cout << "Test 1: Writing seed 0x42 to BRAM" << std::endl;
    write_seed = true;
    start = false;
    seed_in = 0x42;
    lfsr_HLS(start, write_seed, seed_in, random_out, valid_out);
    
    // Start LFSR with seed from BRAM
    std::cout << "Starting LFSR with seed from BRAM" << std::endl;
    write_seed = false;
    start = true;
    lfsr_HLS(start, write_seed, 0, random_out, valid_out);
    
    if (!valid_out || random_out != 0x42) {
        std::cout << "FAILED: First output should be seed value" << std::endl;
        failures++;
    } else {
        std::cout << "Initial value (should be 0x42): 0x" << std::hex << random_out << std::endl;
    }
    
    // Generate sequence of random values
    std::cout << "Generating random sequence:" << std::endl;
    start = false;
    random_values.push_back(random_out);
    
    // Generate 20 random values
    for (int i = 0; i < 20; i++) {
        lfsr_HLS(start, write_seed, 0, random_out, valid_out);
        if (valid_out) {
            std::cout << "Random[" << i << "]: 0x" << std::hex << random_out << std::endl;
            random_values.push_back(random_out);
        }
    }
    
    // Test 2: Check for repeated values too early (should have period of 255)
    std::cout << "\nTest 2: Checking for early repeats in sequence" << std::endl;
    bool has_early_repeat = false;
    for (size_t i = 0; i < random_values.size(); i++) {
        for (size_t j = i + 1; j < random_values.size(); j++) {
            if (random_values[i] == random_values[j]) {
                std::cout << "FAILED: Found repeat value 0x" << std::hex << random_values[i]
                          << " at positions " << std::dec << i << " and " << j << std::endl;
                has_early_repeat = true;
                failures++;
                break;
            }
        }
        if (has_early_repeat) break;
    }
    
    if (!has_early_repeat) {
        std::cout << "PASSED: No early repeats in sequence" << std::endl;
    }
    
    // Test 3: Write a new seed and check reset
    std::cout << "\nTest 3: Testing seed change to 0x81" << std::endl;
    write_seed = true;
    seed_in = 0x81;
    lfsr_HLS(start, write_seed, seed_in, random_out, valid_out);
    
    write_seed = false;
    start = true;
    lfsr_HLS(start, write_seed, 0, random_out, valid_out);
    
    if (!valid_out || random_out != 0x81) {
        std::cout << "FAILED: LFSR did not reset with new seed" << std::endl;
        failures++;
    } else {
        std::cout << "PASSED: LFSR reset with new seed: 0x" << std::hex << random_out << std::endl;
    }
    
    // Summary
    if (failures == 0) {
        std::cout << "\nAll tests PASSED!" << std::endl;
        return 0;
    } else {
        std::cout << "\nSome tests FAILED! Total failures: " << failures << std::endl;
        return 1;
    }
}