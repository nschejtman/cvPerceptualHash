#include <limits>
#include <cmath>
#include <bitset>

/**
 * Find the closest power of 2 for the specified number
 * @param number
 * @return
 */
int nearest2nValue(int number) {
    double n = 0, distance = std::numeric_limits<double>::max(), dNumber = (double) number;
    while (abs(dNumber - pow((double) 2, n + 1)) < distance) {
        n += 1;
        distance = abs(number - pow((double) 2, n));
    }
    return (int) pow((double) 2, n);
}

/**
 * Checks if the specified number is a power of 2
 * @param number
 * @return
 */
bool isPowerOfTwo(int number) {
    return (number != 0) && ((number & (number - 1)) == 0);

}

/**
 * Calculates the Hamming distance between two floats x and y. The Hamming distance is the number of different bits at
 * each position
 * @param x
 * @param y
 * @return
 */
float hammingDistance(float x, float y) {
    std::bitset<32> xBits(x), yBits(y);
    xBits ^= yBits;
    return (float) xBits.count();
}