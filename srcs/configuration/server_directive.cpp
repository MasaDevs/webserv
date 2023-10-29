#include "server_directive.hpp"

ServerDirective::ServerDirective() {}

ServerDirective::~ServerDirective() {}

ServerDirective::ServerDirective(const ServerDirective& other) {
	port_ = other.port_;
	ip_address_ = other.ip_address_;
	server_name_ = other.server_name_;
	locations_ = other.locations_;
}

ServerDirective& ServerDirective::operator=(const ServerDirective& other) {
	if (this != &other) {
		port_ = other.port_;
		ip_address_ = other.ip_address_;
		server_name_ = other.server_name_;
		locations_ = other.locations_;
	}
	return *this;
}

int ServerDirective::parseServerDirective(std::vector<std::string>& tokens) {
	std::vector<std::string> location_tokens;
	std::vector<std::string> args;

	while (!tokens.empty()) {
		LocationDirective location_directive;
		std::string location_path;

		if (tokens.front() == "listen") {
			args = ParserUtils::extractTokensUntilSemicolon(tokens);
			std::cout << "________ listen ___________" << std::endl;
			ParserUtils::printTokens(args);
			parseListenDirective(args);
		} else if (tokens.front() == "server_name") {
			args = ParserUtils::extractTokensUntilSemicolon(tokens);
			std::cout << "________ server_name ___________" << std::endl;
			ParserUtils::printTokens(args);
			parseServerNameDirective(args);
		} else if (tokens.front() == "location") {
			tokens.erase(tokens.begin());
			if (tokens.size()) {
				location_path = tokens.front();
				tokens.erase(tokens.begin());
			} else {
				std::cerr << "Parse Error: Invalid Location" << std::endl;
				return -1;
			}
			location_tokens = ParserUtils::extractTokensFromBlock(tokens);
			location_directive.parseLocationDirective(location_path, location_tokens);
		} else {
			std::cout << "Parse Error: serverDirective" << std::endl;
			std::cout << tokens.front() << std::endl;
			return -1;
		}
		args.clear();
	}
	return 0;
}

int ServerDirective::parseListenDirective(std::vector<std::string>& tokens) {
	if (tokens.size() != 3) {
		return -1;
	}
	if (tokens[1] != ":") {
		return -1;
	}
	for (size_t i = 0; i < tokens[2].size(); ++i) {
		if (!std::isdigit(tokens[2][i])) {
			return -1;
		}
	}
	ip_address_ = tokens.front();
	port_ = tokens.back();
	return 0;
}

int ServerDirective::parseServerNameDirective(std::vector<std::string>& tokens) {
	if (tokens.size() != 1) {
		return -1;
	}
	server_name_ = tokens.front();
	return 0;
}

std::string ServerDirective::getPort() const {
	return port_;
}

std::string ServerDirective::getIpAddress() const {
	return ip_address_;
}

std::string ServerDirective::getServerName() const {
	return server_name_;
}

std::map<std::string, LocationDirective> ServerDirective::getLocations() const {
	return locations_;
}