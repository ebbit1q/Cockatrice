#include "../common/rng_sfmt.h"

#include "gtest/gtest.h"

RNG_Abstract *rng = new RNG_SFMT;

namespace {

TEST(RngTest, Generate)
{
    int max = (unsigned int) -1 >> 1;
    int one = rng->rand(0, max);
    int two = rng->rand(0, max);
    ASSERT_NE(one, two) << "it's extremely unlikely these two numbers are the same";
}

TEST(RngTest, Average)
{
    int max = (unsigned int) -1 >> 1;
    int size = 1000000;
    long long total{};
    for(int i = max - size; i < max; ++i){
        int one = rng->rand(0, i);
        int two = rng->rand(0, i);
        total += one - two;
    }
    total /= size;
    ASSERT_LT(total, size) << "it's extremely unlikely this is more now";
    ASSERT_GT(total, -size) << "it's extremely unlikely this is less now";
}

} // namespace

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
