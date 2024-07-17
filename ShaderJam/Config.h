#pragma once
#include <string>

struct Config {
	int width;
	int height;
	std::string vertexShader;
	std::string effectShader;
	std::string postShader;
	std::string windowType;
};

enum CfgCode {
	width,
	height,
	vertexShader,
	effectShader,
	postShader,
	windowType,
	UKN
};


class Cfg
{
	
public:
	static Config GetCfg();
	static void ParseConfig();
private:
	static Config cfg;
};