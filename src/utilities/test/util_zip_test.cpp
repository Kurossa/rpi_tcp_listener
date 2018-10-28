#include "gtest/gtest.h"
#include <utilities/zip.h>
#include <utilities/logger.h>

using namespace utils;

namespace
{

bool FileExists (const std::string& name) {
    return ( access( name.c_str(), F_OK ) != -1 );
}

void RemoveFileIfExists (const std::string& name) {
    if (FileExists(name)) {
        std::remove(name.c_str());
    }
}

bool ReadFileBlockToBuffer(std::vector<char>& out_vector, std::string& file_name)
{
    char out_buffer[FILE_BLOCK];
    bzero(out_buffer,FILE_BLOCK);
    size_t out_buffer_count = 0;
    FILE* out_file = fopen(file_name.c_str(), "rb");
    if (out_file != nullptr) {
        fseek(out_file, 0, SEEK_SET);
        out_buffer_count = fread(out_buffer, 1, FILE_BLOCK, out_file);
        fclose(out_file);
        out_vector = std::vector<char>(out_buffer, out_buffer + out_buffer_count);
        return true;
    }
    return false;
}

int RandNum (int min_value = 0, int max_value = 255) {
    EXPECT_TRUE (max_value > min_value);
    if (max_value < min_value) {
        max_value = min_value + 1;
    }
    return (min_value + (rand() % static_cast<int>(max_value - min_value + 1)));
}

}

class ZipTest : public ::testing::Test
{
public:
    enum ZipMode
    {
        ZIP_COMPRESS,
        ZIP_UNCOMPRESS
    };

    ZipTest() {}
    ~ZipTest()
    {
        RemoveFileIfExists(test_file_name_m);
        RemoveFileIfExists(output_file_name_m);
        RemoveFileIfExists(output2_file_name_m);
    }

protected:
    virtual void SetUp() {}
    virtual void TearDown() {}

    void TestCompressUncompress(  const std::vector<char>& test_vector
                                , const std::vector<char>& expected_result_vector
                                ,ZipMode mode = ZIP_COMPRESS);

    std::string test_file_name_m = "test_file.bin";
    std::string output_file_name_m = "output_file.bin";
    std::string output2_file_name_m = "output2_file.bin";
};

void ZipTest::TestCompressUncompress(const std::vector<char>& test_vector, const std::vector<char>& expected_result_vector, ZipMode mode)
{
    FILE* test_file = fopen(test_file_name_m.c_str(), "wb");
    fwrite(test_vector.data(), 1, test_vector.size(), test_file);
    fclose(test_file);

    ZipStatus status = ZipStatus::ZIP_NUM;
    switch (mode)
    {
    case ZIP_COMPRESS:
        status = utils::ZipCompress(test_file_name_m.c_str(), output_file_name_m.c_str());
        break;
    case ZIP_UNCOMPRESS:
        status = utils::ZipUncompress(test_file_name_m.c_str(), output_file_name_m.c_str());
        break;
    default:
        ASSERT_TRUE(false);
    }
    EXPECT_EQ (ZipStatus::OK, status);
    ASSERT_TRUE(FileExists(output_file_name_m));
    std::vector<char> out_vector;
    EXPECT_TRUE(ReadFileBlockToBuffer(out_vector, output_file_name_m));
    ASSERT_TRUE(test_vector.size() == out_vector.size());
    // Test if compressed content is different than input buffer
    EXPECT_FALSE(0 == memcmp( test_vector.data(), out_vector.data(), test_vector.size() ));
    // Test if compressed content is equal to expected result
    EXPECT_TRUE(0 == memcmp( expected_result_vector.data(), out_vector.data(), expected_result_vector.size() ));

    RemoveFileIfExists(test_file_name_m);
    RemoveFileIfExists(output_file_name_m);
}

TEST_F (ZipTest, ZipCompressEvenFileSize) {
    std::vector<char> test_vector = {'1','2','3','4'};
    std::vector<char> expected_result_vector = {'2', '1', '4', '3'};
    TestCompressUncompress(test_vector, expected_result_vector, ZIP_COMPRESS);
}

TEST_F (ZipTest, ZipCompressOddFileSize) {
    std::vector<char> test_vector = {'1', '2', '3', '4', '5'};
    std::vector<char> expected_result_vector = {'2', '1', '4', '3', '5'};
    TestCompressUncompress(test_vector, expected_result_vector, ZIP_COMPRESS);
}

TEST_F (ZipTest, ZipUncompressEvenFileSize) {
    std::vector<char> test_vector = {'2', '1', '4', '3'};
    std::vector<char> expected_result_vector = {'1','2','3','4'};
    TestCompressUncompress(test_vector, expected_result_vector, ZIP_UNCOMPRESS);
}

TEST_F (ZipTest, ZipUncompressOddFileSize) {
    std::vector<char> test_vector = {'2', '1', '4', '3', '5'};
    std::vector<char> expected_result_vector = {'1', '2', '3', '4', '5'};
    TestCompressUncompress(test_vector, expected_result_vector, ZIP_UNCOMPRESS);
}

TEST_F(ZipTest, ZipCompressUncompressRandomPayload) {
    int rand_size = RandNum(100, FILE_BLOCK);
    std::vector<char> random_test_block;
    for (int i = 0; i < rand_size; ++i) {
        random_test_block.push_back(RandNum(1,255));
    }

    FILE* test_file = fopen(test_file_name_m.c_str(), "wb");
    fwrite(random_test_block.data(), 1, random_test_block.size(), test_file);
    fclose(test_file);

    ZipStatus status = ZipCompress(test_file_name_m.c_str(), output_file_name_m.c_str());
    EXPECT_EQ (ZipStatus::OK, status);
    status = ZipUncompress(output_file_name_m.c_str(), output2_file_name_m.c_str());
    EXPECT_EQ (ZipStatus::OK, status);

    ASSERT_TRUE(FileExists(output_file_name_m));
    ASSERT_TRUE(FileExists(output2_file_name_m));

    std::vector<char> out_vector;
    EXPECT_TRUE(ReadFileBlockToBuffer(out_vector, output2_file_name_m));
    EXPECT_EQ(random_test_block.size(), out_vector.size());
    std::vector<char>::iterator it_rnd_test = random_test_block.begin();
    std::vector<char>::iterator it_out = out_vector.begin();
    for (; (it_rnd_test != random_test_block.end()) || (it_out != out_vector.end()); ++it_rnd_test, ++it_out) {
        EXPECT_EQ(*it_rnd_test, *it_out);
    }

    RemoveFileIfExists(test_file_name_m);
    RemoveFileIfExists(output_file_name_m);
    RemoveFileIfExists(output2_file_name_m);
}
 
TEST_F (ZipTest, ZipSameSourceAsDestination) {
    ZipStatus status = ZIP_NUM;
    char input_zip[25] = "same_file.txt";
    char output_zip[25] = "same_file.txt";
    status = ZipUncompress(input_zip, output_zip);
    EXPECT_EQ(status, ZipStatus::SAME_SRC_DST);
    status = ZipCompress(input_zip, output_zip);
    EXPECT_EQ(status, ZipStatus::SAME_SRC_DST);
}

TEST_F (ZipTest, ZipFileNotExists) {
        ZipStatus status = ZIP_NUM;
        char input_zip[25] = "not_existing_file.txt";
        status = ZipCompress(input_zip, output_file_name_m.c_str());
        EXPECT_EQ(status, ZipStatus::FILE_NOT_EXISTS);
        status = ZipUncompress(input_zip, output_file_name_m.c_str());
        EXPECT_EQ(status, ZipStatus::FILE_NOT_EXISTS);
}
