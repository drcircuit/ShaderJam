#include "Config.h"
#include "ErrorLogger.h"
#include <fstream>

CfgCode mapCfgCode(std::string const& str) {
	if (str == "width") return width;
	if (str == "height") return height;
	if (str == "vertex_shader") return vertexShader;
	if (str == "effect_shader") return effectShader;
	if (str == "post_shader") return postShader;
	if (str == "window_type") return windowType;
	return UKN;
}

Config Cfg::cfg;

void Cfg::ParseConfig() {
	std::string filename = "settings.cfg";
	std::ifstream file(filename);
	std::string line;
	CfgCode code;
	Cfg::cfg = { 1920, 1080, "vertex.hlsl", "effect.hlsl", "post.hlsl", "fullscreen"};
	while (getline(file, line)) {
		size_t delimiterPos = line.find('=');
		size_t semicolonPos = line.find(';', delimiterPos);
		if (delimiterPos != std::string::npos && semicolonPos != std::string::npos) {
			std::string key = line.substr(0, delimiterPos);
			std::string value = line.substr(delimiterPos + 1, semicolonPos - delimiterPos - 1);

			// Trim leading and trailing whitespace from key and value
			key.erase(0, key.find_first_not_of(" \t"));
			key.erase(key.find_last_not_of(" \t") + 1);
			value.erase(0, value.find_first_not_of(" \t"));
			value.erase(value.find_last_not_of(" \t") + 1);
			code = mapCfgCode(key);
			int num = 0;
			switch (code) {
				num = 0;
				case width:
					num = std::stoi(value);
					if (num > 0) Cfg::cfg.width = num;
					break;

				case height:
					num = std::stoi(value);
					if (num > 0) Cfg::cfg.height = num;
					break;
				case vertexShader:
					Cfg::cfg.vertexShader = value;
					break;
				case effectShader:
					Cfg::cfg.effectShader = value;
					break;
				case postShader:
					Cfg::cfg.postShader = value;
					break;
				case windowType:
					Cfg::cfg.windowType = value;
			}
		}
		else {
			// Log warning if the format is incorrect
			ErrorLogger::Log("Error parsing settings.cfg");
		}
	}
	file.close();
}

Config Cfg::GetCfg() {
	return Cfg::cfg;
}