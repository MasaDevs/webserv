#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include "struct.hpp"
#include "http_request.hpp"
#include <cstring>
#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

namespace server {

class HttpResponse {
private:
	std::string response_;
	int	status_code_;
	std::map<std::string, std::string> header_;
	std::string stream_;
	std::fstream ofs_;
	static std::map<int, std::string> const status_line_;

public:
	HttpResponse();
	~HttpResponse();
	int initialize();
	int setFstream(std::string file_name);
	void setStatusCode(int status_code);
	void setHeaderValue(std::string, std::string);
	void pourStream(HttpRequest request);
	RequestProcessStatus setSendBuffer2(char* buffer, size_t max_buffer_size);
	RequestProcessStatus setSendBuffer(char* buffer, size_t max_buffer_size);
	std::string substr_response(size_t size);
};

HttpResponse	exec_request(HttpRequest const &request);
HttpResponse	exec_error(HttpRequest const &request);

} // namespace server

#endif