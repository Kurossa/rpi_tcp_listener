#include "gtest/gtest.h"
 
TEST (ZipTest, PositiveNos) { 
    ASSERT_EQ(1, 1);
    //EXPECT_EQ (18.0, square-root (324.0));
    //EXPECT_EQ (25.4, square-root (645.16));
    //EXPECT_EQ (50.3321, square-root (2533.310224));
}
 
TEST (ZipTest, ZeroAndNegativeNos) { 
	ASSERT_EQ(1, 1);
    //ASSERT_EQ (0.0, square-root (0.0));
    //ASSERT_EQ (-1, square-root (-22.0));
}
