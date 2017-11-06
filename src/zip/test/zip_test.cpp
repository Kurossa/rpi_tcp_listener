#include "gtest/gtest.h"
#include <zip/zip.h>
#include <utilities/logger.h>

class ZipTest : public ::testing::Test
{
protected:
     virtual void SetUp()
     {
        logSetVerboseMode(true);
        logInit();

     }

     virtual void TearDown()
    {
        logClose();
    }
};



TEST_F (ZipTest, PositiveNos) {
    eZipStaus_t status = ZIP_NUM;
    char input_zip[5] = {'1','2','3','4','5'};
    char output_zip[5] = {0,0,0,0,0};
    status = zip_uncompress(input_zip, output_zip);
    EXPECT_EQ (status , ZIP_OK);
    //eZipStaus_t zip_uncompress(const char* source_name, const char* destination_name);
    //eZipStaus_t zip_compress(const char* source_name, const char* destination_name);
    ASSERT_EQ(1, 1);
    //EXPECT_EQ (18.0, square-root (324.0));
    //EXPECT_EQ (25.4, square-root (645.16));
    //EXPECT_EQ (50.3321, square-root (2533.310224));
}
 
TEST_F (ZipTest, ZipSameSourceAsDestination) {
    eZipStaus_t status = ZIP_NUM;
    char input_zip[25] = "in_file.txt";
    char output_zip[25] = "in_file.txt";
    status = zip_uncompress(input_zip, output_zip);
    EXPECT_EQ(status, ZIP_SAME_SRC_DST);
}
