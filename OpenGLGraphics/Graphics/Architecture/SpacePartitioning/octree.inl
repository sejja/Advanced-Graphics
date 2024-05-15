#ifndef _OCTREE__INL_
#define _OCTREE__INL_

#include "math.hpp"

namespace LocationalCode
{
    template<unsigned dimension>
    int compute_locational_code(glm::vec<dimension, long long> pos, const unsigned root_size, const unsigned levels)
    {
        pos += root_size / 2; // offset

        // check boundaries
        for (unsigned i = 0; i < dimension; ++i)
            if (pos[i] < 0 || pos[i] >= root_size)
                return 0b1;

        // compute bits
        unsigned bits = 0;
        unsigned maxPower = glm::log2(root_size);
        for (unsigned i = 0; i < maxPower; ++i)
        {
            unsigned pow2 = 1 << i;
            for (unsigned j = 0; j < dimension; ++j)
                if (pos[j] & pow2)
                    bits += 1 << (i * dimension + j);
        }

        // shifting
        unsigned shift = maxPower - levels;
        bits >>= (shift * dimension);

        // sentinel bit
        bits += 1 << (levels * dimension);

        return bits;
    }

    template<unsigned dimension>
    int common_locational_code(unsigned loc1, unsigned loc2)
    {
        if (loc1 == loc2) // Same
            return loc1;

        // loc1 == smallest
        unsigned smallest = loc1;
        // loc2 == biggest
        unsigned biggest = loc2;

        if (smallest > biggest)
            std::swap(smallest, biggest);

        unsigned startingBit = maxBits - 1;
        startingBit -= startingBit % dimension;
        unsigned bigSentinel = 0;
        unsigned smallSentinel = 0;

        // get biggest sentinel bit number
        for (long int i = startingBit; i >= 0; i -= dimension)
            if (biggest & (1 << i))
            {
                bigSentinel = i;
                break;
            }

        // get smallest sentinel bit number
        for (long int i = bigSentinel; i >= 0; i -= dimension)
            if (smallest & (1 << i))
            {
                smallSentinel = i;
                break;
            }

        // shift to discard last bits of biggest (if needed)
        biggest >>= (bigSentinel - smallSentinel);

        unsigned commonBits = 1;
        for (long int i = smallSentinel - dimension; i >= 0; i -= dimension)
        {
            unsigned smallShift = smallest >> i;
            unsigned bigShift = biggest >> i;
            if (smallShift == bigShift)
                commonBits = smallShift;
            else
                break;
        }
        return commonBits;
    }
}

#endif