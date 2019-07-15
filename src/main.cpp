#include <iostream>

#include <fileReader.h>
#include <Json.h>

int main()
{
    const std::string filename = "example.json";
    std::string fileContent = FileUtils::FileReader::read_file(filename);

    Json::Json json = Json::Json::parse_from_string(fileContent);

	std::cout << "PORT: " << json["server"]["port"].get_value_as_int() << std::endl;
	std::cout << "===========================================" << std::endl;

	auto length = json["peripherals"].get_size();

	for(size_t i = 0; i < length; i++)
	{
		auto name = json["peripherals"][i]["name"].get_value_as_string();
		auto type = json["peripherals"][i]["type"].get_value_as_string();
		auto pin = json["peripherals"][i]["pin"].get_value_as_int();

		std::cout << "name: " << name << std::endl;
		std::cout << "pin: " << pin << std::endl;
		std::cout << "type: " << type << std::endl;

		std::cout << "===========================================" << std::endl;
	}

    return 0;
}   
