#include "octree.hpp"

namespace LocationalCode
{
    int compute_locational_code(const aabb& bv, const unsigned root_size, const unsigned levels)
    {
        // rounded values
        glm::i64vec3 minP = glm::floor(bv.min);
        glm::i64vec3 maxP = glm::ceil(bv.max);

        // each location
        unsigned minLoc = compute_locational_code(minP, root_size, levels);
        unsigned maxLoc = compute_locational_code(maxP, root_size, levels);

        return common_locational_code(minLoc, maxLoc);
    }

    aabb compute_bv(unsigned loc, float size)
    {
        size /= 2.0f;
        aabb bv(vec3(-size), vec3(size));
        if (loc == 0b1)
            return { bv };

        const unsigned x = 1;
        const unsigned y = 1 << 1;
        const unsigned z = 1 << 2;

        unsigned sentinelBit = 0;
        unsigned startingBit = maxBits - 1;
        startingBit -= startingBit % 3;

        // get big sentinel bit number
        for (long int i = startingBit; i >= 0; i -= 3)
            if (loc & (1 << i))
            {
                sentinelBit = i;
                break;
            }

        loc -= (1 << sentinelBit);
        unsigned leftShift = maxBits - sentinelBit;
        unsigned rightShift = maxBits - 3;
        while (leftShift < maxBits)
        {
            unsigned code = loc << leftShift;
            code >>= rightShift;
            (code & x) ? bv.min.x += size : bv.max.x -= size;
            (code & y) ? bv.min.y += size : bv.max.y -= size;
            (code & z) ? bv.min.z += size : bv.max.z -= size;
            size /= 2.0f;
            leftShift += 3;
        }
        return aabb(bv.min, bv.max);
    }
}