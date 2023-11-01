#include "http_response.hpp"

namespace server {

HttpResponse::HttpResponse()
	: response_(
		  "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\nContent-Length: "
		  "1098\r\n\r\n<!DOCTYPE html>\r\n<html>\r\n    <head>\r\n\r\n    <title>Extended Simple "
		  "Response</title>\r\n</head>\r\n<body>\r\n\r\n    <h1>Hello, this is an extended simple "
		  "HTTP response!</h1>\r\n\r\n<p>Lorem ipsum dolor sit amet, consectetur adipiscing elit. "
		  "Vivamus lacinia odio vitae vestibulum. Donec in efficitur leo. In nisl ligula, "
		  "vulputate "
		  "id diam in, condimentum tincidunt sapien. Maecenas gravida velit vitae dolor finibus, "
		  "in "
		  "feugiat urna interdum. Vestibulum euismod euismod velit, nec feugiat lacus feugiat sit "
		  "amet. Vestibulum consectetur sit amet lacus a pellentesque. Duis tincidunt, est sed "
		  "sodales tincidunt, ipsum erat elementum massa, at lacinia arcu ex a est. Fusce ut "
		  "congue "
		  "metus, sed tincidunt urna.</p>\r\n\r\n<p>Quisque egestas eget lacus non condimentum. "
		  "Sed "
		  "at imperdiet dui, vel facilisis velit. Proin ac neque nec arcu commodo aliquet. Morbi "
		  "tincidunt turpis et tincidunt. Fusce id dui id libero aliquet sagittis a at libero. "
		  "Nulla "
		  "at libero pharetra, bibendum metus sed, malesuada metus. Cras sollicitudin, quam "
		  "pellentesque lobortis auctor, ante eros bibendum nunc, sed bibendum quam ex non metus. "
		  "Curabitur in pharetra odio, in efficitur leo.</p>\r\n</body>\r\n</html>") {}

HttpResponse::~HttpResponse() {}

int HttpResponse::initialize() {
	return 0;
}

RequestProcessStatus HttpResponse::setSendBuffer(char* buffer, size_t max_buffer_size) {
	std::memset(buffer, 0, max_buffer_size);
	std::string buf = substr_response(max_buffer_size - 1);

	std::memcpy(buffer, buf.c_str(), buf.size());
	if (buf.size() == max_buffer_size - 1)
		return SENDING;

	return FINISH;
}

std::string HttpResponse::substr_response(size_t size) {
	std::string ans;
	if (response_.size() < size) {
		ans = response_.substr(0, response_.size());
		response_.clear();
	} else {
		ans = response_.substr(0, size);
		response_ = response_.substr(size + 1);
	}
	return ans;
}
//RequestProcessStatus HttpResponse::setSendBuffer2(char* buffer, size_t max_buffer_size) {


int	HttpResponse::setFstream(std::string file_name)
{
	ofs_.open(file_name.c_str());
	if (ofs_.fail())
		return (-1);
	return (0);
}

void	HttpResponse::setHeaderValue(std::string key, std::string value)
{
	header_.insert(std::make_pair(key, value));
}

void	HttpResponse::pourStream(HttpRequest request)
{
	stream_ = request.getServerProtocol() + " " + std::to_string(status_code_) + " " + "OK" + "\r\n";
	for (std::map<std::string, std::string>::iterator iter = header_.begin(); iter != header_.end(); iter++)
		stream_ += (iter->first + ": " + iter->second + "\r\n");
	stream_ += "\r\n";
}



HttpResponse	exec_get(HttpRequest const &request)
{
	HttpResponse	response;
	struct stat		file_info;	
	int				fd;

	// 本来ここでrequestのrequest_path_に対応するconfのバスを取得。今回は代わりにダミーファイルを利用。
	char path[256];
	memset(path, '\0', 256);
	getcwd(path, 256);
	std::string	file_name(path);
	std::cout << file_name << std::endl;
	file_name += "/templates/index.html";
	// ここまでダミー
	if (response.setFstream(file_name) < 0)
		return (exec_error(request));
	if (stat(file_name.c_str(), &file_info) != 0)
		return (exec_error(request));
	// permisson checkが必要。
	response.setHeaderValue("Content-Length", std::to_string(file_info.st_size));
	response.pourStream(request);	
	return (response);
}

HttpResponse	exec_error(HttpRequest const &request)
{
	HttpResponse	response;
	struct stat		file_info;	

	// 本来ここでrequestのrequest_path_に対応するconfのバスを取得。今回は代わりにダミーファイルを利用。
	char path[256];
	memset(path, '\0', 256);
	getcwd(path, 256);
	std::string	file_name(path);
	file_name += "/templates/error.html";
	// ここまでダミー
	if (response.setFstream(file_name) < 0)
		return (exec_error(request));
	if (stat(file_name.c_str(), &file_info) != 0)
		return (exec_error(request));
	// permisson checkが必要。
	response.setHeaderValue("Content-Length", std::to_string(file_info.st_size));
	response.pourStream(request);	
	return (response);
}
// responseに基本的な情報を設定して返す。
HttpResponse	exec_request(HttpRequest const &request)
{
	HttpResponse	response;

	//この時点のエラーはクライアントエラーなのでresponseにセットして返す。
	if (request.getHttpRequestStatus() == http_request_status::ERROR)
		return (exec_error(request));
	switch(request.getHttpMethod()) {
		case http_method::GET:
			response = exec_get(request);
			break;
	//	case http_method::POST:
	//		response = exec_post(request);
	//		break;
	//	case http_method::DELETE:
	//		response = exec_delete(request);
	//		break;
		default:
			response = exec_error(request);
	}
	return (response);
}

} // namespace server