#include <yaml-cpp/yaml.h>

class YAMLHandler {
public:
    // 写入YAML文件
    bool writeDataToYAML(const std::string& filename, const YAML::Node& data) {
        try {
            YAML::Emitter out;
            out << data;
            std::ofstream file(filename);
            if (file.is_open()) {
                file << out.c_str();
                file.close();
                return true;
            } else {
                std::cerr << "Unable to open file: " << filename << std::endl;
                return false;
            }
        } catch (const YAML::EmitterException& e) {
            std::cerr << "YAML Emitter error: " << e.what() << std::endl;
            return false;
        }
    }

    // 读取YAML文件
    YAML::Node readDataFromYAML(const std::string& filename) {
        try {
            YAML::Node data = YAML::LoadFile(filename);
            return data;
        } catch (const YAML::BadFile& e) {
            std::cerr << "Error reading YAML file: " << e.what() << std::endl;
            return YAML::Node();
        } catch (const YAML::ParserException& e) {
            std::cerr << "YAML Parser error: " << e.what() << std::endl;
            return YAML::Node();
        }
    }
};