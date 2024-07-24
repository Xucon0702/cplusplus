int main() {
    YAMLHandler handler;

    // 构造YAML数据
    YAML::Node config;
    config["name"] = "Example Config";
    config["version"] = 1.0;
    config["settings"]["debug"] = true;
    config["settings"]["log_level"] = "info";

    // 写入YAML文件
    if (handler.writeDataToYAML("config.yaml", config)) {
        std::cout << "Successfully wrote to config.yaml" << std::endl;
    } else {
        std::cout << "Failed to write to config.yaml" << std::endl;
    }

    // 读取YAML文件
    YAML::Node readConfig = handler.readDataFromYAML("config.yaml");
    if (!readConfig.IsNull()) {
        std::cout << "Name: " << readConfig["name"].as<std::string>() << std::endl;
        std::cout << "Version: " << readConfig["version"].as<double>() << std::endl;
        std::cout << "Debug: " << (readConfig["settings"]["debug"].as<bool>() ? "true" : "false") << std::endl;
        std::cout << "Log Level: " << readConfig["settings"]["log_level"].as<std::string>() << std::endl;
    } else {
        std::cout << "No data read from config.yaml" << std::endl;
    }

    return 0;
}