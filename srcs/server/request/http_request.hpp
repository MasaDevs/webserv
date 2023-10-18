#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP
#include <iostream>
#include <string>
#include <map>
enum    http_status
{
	METHOD,
	HEADER,
	BODY,
	FINISHED,
	ERROR,
};

enum    http_method
{
	GET,
	POST,
	DELETE,
};

enum http_version
{
	HTTP_1_0,
	HTTP_1_1,
	HTTP_2_0,
};

enum http_error
{
	BAD_REQUEST = 400,	
};

class   HttpRequest
{
	public:
		HttpRequest();
		int									parseHttpRequest(std::string const &line);
		std::string							getHeaderValue(std::string const &key) const;
	private:
		int									method_;
		int									status_;
		int									version_;
		int									error_status_;
		std::string							request_path_;
		std::map<std::string, std::string>	query_;
		std::map<std::string, std::string>	header_;
		std::string							body_;
		int									parseHttpMethod(std::string const &line);
		void								setHeaderValue(std::string const &key, std::string const &value);
		void								setStatus(int const &status);
		void								setErrorStatus(int const &error_status);
};
#endif
