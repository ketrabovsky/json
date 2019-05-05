#include <gtest/gtest.h>

#include "../Json.h"
#include "../fileReader.h"

TEST(JsonTest, JsonFromBoolean)
{
    Json::Json json(true);
    ASSERT_EQ("true", json.to_string());
}

TEST(JsonTest, FromString)
{
    const std::string str = "Hello world!";
    const std::string expect = "\"Hello world!\"";
    Json::Json json(str);
    
    ASSERT_EQ(expect, json.to_string());
}

TEST(JsonTest, FromNull)
{
    Json::Json json(nullptr);
    
    ASSERT_EQ("null", json.to_string());
}

TEST(JsonTest, FromInt)
{
    Json::Json json(5);
    
    ASSERT_EQ("5", json.to_string());
}

TEST(JsonTest, ParseIntFromString)
{
    const std::string str = ": 123,";

    Json::Json json = Json::Json::parse_from_string(str);
    EXPECT_EQ(123, (int)json);
}

TEST(JsonTest, ParseStringFromString)
{
    const std::string str = ": \"Hello world!\",";

    Json::Json json = Json::Json::parse_from_string(str);
    EXPECT_EQ("Hello world!", (std::string)json);
}

TEST(JsonTest, ParseArrayFromString)
{
    const std::string str = "[1, 2, \"name\"]";

    Json::Json json = Json::Json::parse_from_string(str);

    EXPECT_EQ(1, json[0].get_value_as_int());
    EXPECT_EQ(2, json[1].get_value_as_int());
    EXPECT_EQ("name", json[2].get_value_as_string());
    ASSERT_EQ(3, json.get_size());
}

/*
 * Nested arrays aren't supported yet
 */ 
TEST(JsonTest, NestedAarray)
{
    const std::string str = "[1, [2, 3, 4], \"name\"]";

    Json::Json json = Json::Json::parse_from_string(str);


    EXPECT_EQ(1, json[0].get_value_as_int());
    EXPECT_EQ(2, json[1][0].get_value_as_int());
    EXPECT_EQ(3, json[1][1].get_value_as_int());
    EXPECT_EQ(4, json[1][2].get_value_as_int());
	EXPECT_EQ("name", json[2].get_value_as_string());
    ASSERT_EQ(3, json.get_size());
}

TEST(JsonTest, ParseObjectFromString)
{
    const std::string str = "{\"name\" : \"relay_1\", \"pin\" : 15 }";

    Json::Json json = Json::Json::parse_from_string(str);

    EXPECT_EQ("relay_1", (std::string)json["name"]);
    EXPECT_EQ(15, (int)json["pin"]);
}

TEST(JsonTest, ParseNestedObjects)
{
    const std::string str = "{\"name\" : {\"relay_1\": \"relay\", \"pin\":15},\"costam\": 90 }";

    Json::Json json = Json::Json::parse_from_string(str);

    EXPECT_EQ("relay", json["name"]["relay_1"].get_value_as_string());
    EXPECT_EQ(15, json["name"]["pin"].get_value_as_int());
    EXPECT_EQ(90, json["costam"].get_value_as_int());
}

TEST(JsonTest, ParseObjectWithArrayFromString)
{
    const std::string str = "{\"name\" : [16, 2, 3], \"pin\" : 15 }";

    Json::Json json = Json::Json::parse_from_string(str);

    auto c = json["name"];
    EXPECT_EQ(16, c[0]);
    EXPECT_EQ(2, c[1]);
    EXPECT_EQ(3, c[2]);
    EXPECT_EQ(15, json["pin"].get_value_as_int());
}

TEST(JsonTest, ParseArrayOfObjectsFromString)
{
    const std::string str = "[{\"name\" : [16, 2, 3], \"pin\" : 15 }, {\"name\" : \"relay_2\", \"pin\" : 30 } ]";

    Json::Json json = Json::Json::parse_from_string(str);

    EXPECT_EQ("ARRAY", json[0]["name"].get_type());
    EXPECT_EQ("relay_2", json[1]["name"].get_value_as_string());
    EXPECT_EQ(15, json[0]["pin"].get_value_as_int());
    EXPECT_EQ(30, json[1]["pin"].get_value_as_int());
}

TEST(JsonTest, ParseArrayInObject)
{
	const std::string str = " { \"server\": { \"port\": 5050, \"ip\": \"192.168.0.16\" }, \"peripherals\" : [ { \"name\": \"relay_1\", \"pin\": 15, \"type\": \"relay\" }, { \"name\": \"relay_2\", \"pin\": 16, \"type\": \"relay\" }, { \"name\": \"therm_1\", \"pin\": 4, \"type\": \"therm\"} ] } ";

    Json::Json json = Json::Json::parse_from_string(str);

    EXPECT_EQ(5050, json["server"]["port"].get_value_as_int());
    EXPECT_EQ("relay_1", json["peripherals"][0]["name"].get_value_as_string());
    EXPECT_EQ("relay_2", json["peripherals"][1]["name"].get_value_as_string());
    EXPECT_EQ("therm_1", json["peripherals"][2]["name"].get_value_as_string());
    EXPECT_EQ("relay", json["peripherals"][0]["type"].get_value_as_string());
    EXPECT_EQ("relay", json["peripherals"][1]["type"].get_value_as_string());
    EXPECT_EQ("therm", json["peripherals"][2]["type"].get_value_as_string());
    EXPECT_EQ(15, json["peripherals"][0]["pin"].get_value_as_int());
    EXPECT_EQ(16, json["peripherals"][1]["pin"].get_value_as_int());
    EXPECT_EQ(4, json["peripherals"][2]["pin"].get_value_as_int());
}

TEST(JsonTest, CreateObject)
{
    Json::Json pin(15);
    Json::Json name(std::string("relay_1"));

    Json::Json json(std::string("pin"), pin);
    json.add_pair(std::string("name"), name);

    const std::string expected = "{\"name\":\"relay_1\",\"pin\":15}";

    std::string parsed = json.to_string();
    EXPECT_EQ(expected, parsed);
}

TEST(JsonTest, CreateArray)
{
    Json::Json name(std::string("relay_1"));
    Json::Json costam(16);
    Json::Json costam2(std::string("relay_2"));

    std::vector<Json::Json> a = {name, costam, costam2};

    Json::Json array(a);

    const std::string expected = "[\"relay_1\",16,\"relay_2\"]";

    EXPECT_EQ(expected, array.to_string());
}

TEST(JsonTest, ArrayInObject)
{
    Json::Json val1(24);
    Json::Json val2(16);
    Json::Json val3(89);

    std::vector<Json::Json> array_values = {val1, val2, val3};
    Json::Json array(array_values);

    Json::Json name(std::string("relay_1"));
    Json::Json type(std::string("type"));
    Json::Json type_value(std::string("relay"));
    
    Json::Json final(std::string("name"), name);
    final.add_pair("values", array);
    final.add_pair("type", type_value);
    
    const std::string expected = "{\"name\":\"relay_1\",\"type\":\"relay\",\"values\":[24,16,89]}";

    EXPECT_EQ(expected, final.to_string());
}

TEST(JsonTest, ArrayOfObjects)
{
    Json::Json name1(std::string("relay_1"));
    Json::Json pin1(16);
    Json::Json name2(std::string("relay_2"));
    Json::Json pin2(15);

    Json::Json obj1(std::string("name"), name1);
    obj1.add_pair(std::string("pin"), pin1);

    Json::Json obj2(std::string("name"), name2);
    obj2.add_pair(std::string("pin"), pin2);

    std::vector<Json::Json> values = {obj1, obj2};
    Json::Json array(values);

    const std::string expected = "[{\"name\":\"relay_1\",\"pin\":16},{\"name\":\"relay_2\",\"pin\":15}]";
    EXPECT_EQ(expected, array.to_string());
}

TEST(JsonTest, ArrayTest)
{
    Json::Json name1(std::string("relay_1"));
    Json::Json pin1(16);
    Json::Json name2(std::string("relay_2"));
    Json::Json pin2(15);

    Json::Json obj1(std::string("name"), name1);
    obj1.add_pair(std::string("pin"), pin1);

    Json::Json obj2(std::string("name"), name2);
    obj2.add_pair(std::string("pin"), pin2);

    std::vector<Json::Json> values = {obj1, obj2};
    Json::Json array(values);

    const std::string expected1 = "{\"name\":\"relay_1\",\"pin\":16}";
    const std::string expected2 = "{\"name\":\"relay_2\",\"pin\":15}";
    EXPECT_EQ(expected1, array[0].to_string());
    EXPECT_EQ(expected2, array[1].to_string());
}

TEST(JsonTest, fileTest)
{
    const std::string file_content = "[\r\n    {\r\n        \"name\": \"zawor_1\",\r\n        \"pin\": 10,\r\n        \"type\": \"relay\"\r\n    },\r\n    {\r\n        \"name\": \"zawor_2\",\r\n        \"pin\": 16,\r\n        \"type\": \"relay\"\r\n    }\r\n]";

    Json::Json json = Json::Json::parse_from_string(file_content);

    EXPECT_EQ("zawor_1", json[0]["name"].get_value_as_string());
    EXPECT_EQ("zawor_2", json[1]["name"].get_value_as_string());
    EXPECT_EQ(10, json[0]["pin"].get_value_as_int());
    EXPECT_EQ(16, json[1]["pin"].get_value_as_int());
}
