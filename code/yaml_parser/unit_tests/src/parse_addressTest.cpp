#include <vector>
#include "parse_addressTest.hpp"
#include "parse_address.hpp"

parse_addressTest::parse_addressTest() : a(ssc::random_data_generator::DataGenerator(666))
{
}

TEST_F(parse_addressTest, can_parse_websocket_url)
{
    ASSERT_TRUE(parseWebSocketURL("wss://www.google.com:443/webhp?gws_rd=ssl#q=cpp"));
    ASSERT_TRUE(parseWebSocketURL("wss://www.google.com:443"));
    ASSERT_TRUE(parseWebSocketURL("wss://130.66.124.6:210"));
    ASSERT_TRUE(parseWebSocketURL("ws://localhost:210"));
    ASSERT_FALSE(parseWebSocketURL("http://localhost:210"));
    ASSERT_FALSE(parseWebSocketURL("https://localhost:210"));
}

TEST_F(parse_addressTest, can_create_YamlOuput_from_websocket_url_1)
{
    YamlOutput out = build_YamlOutput_from_WS_URL("ws://localhost:210");
    ASSERT_EQ("ws",out.format);
    ASSERT_EQ("ws://localhost",out.address);
    ASSERT_EQ(210,out.port);
    ASSERT_EQ("",out.filename);
    ASSERT_EQ(std::vector<std::string>(),out.data);
}

TEST_F(parse_addressTest, can_create_YamlOuput_from_websocket_url_2)
{
    YamlOutput out = build_YamlOutput_from_WS_URL("ws://130.66.124.6:210");
    ASSERT_EQ("ws",out.format);
    ASSERT_EQ("ws://130.66.124.6",out.address);
    ASSERT_EQ(210,out.port);
    ASSERT_EQ("",out.filename);
    ASSERT_EQ(std::vector<std::string>(),out.data);
}

TEST_F(parse_addressTest, can_create_YamlOuput_from_websocket_url_3)
{
    YamlOutput out = build_YamlOutput_from_WS_URL("ws://www.google.com:443/webhp?gws_rd=ssl#q=cpp");
    ASSERT_EQ("ws",out.format);
    ASSERT_EQ("ws://www.google.com",out.address);
    ASSERT_EQ(443,out.port);
    ASSERT_EQ("",out.filename);
    ASSERT_EQ(std::vector<std::string>(),out.data);
}

TEST_F(parse_addressTest, can_create_YamlOuput_from_websocket_url_4)
{
    ASSERT_THROW(build_YamlOutput_from_WS_URL("http://www.google.com:443/webhp?gws_rd=ssl#q=cpp"), ParseAddressException);
}
