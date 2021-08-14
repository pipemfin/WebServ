//                                   //
// Created by:                       //
//              Watcher Jarr         //
//         on:                       //
//              8/2/21				 //
//                                   //

#include "BadResponse.hpp"

BadResponse::BadResponse() : AResponse() {
}

BadResponse::BadResponse(int status, const std::string &root) : AResponse(root,""){
	_status = status;
}

BadResponse::BadResponse(const BadResponse &rhi){
	*this = rhi;
}

BadResponse & BadResponse::operator=(const BadResponse &rhi){
	if (this == &rhi)
		return *this;
	AResponse::operator=(rhi);
	return *this;
}

BadResponse::~BadResponse(){
}

std::string BadResponse::generateResponse() {
	if (getHead())
		return generateHeader();
	return generateHeader() + generateBody();
}

std::string BadResponse::generateHeader() {
	std::stringstream str;
	str << _protocol << " "
		<< _status << " "
		<< _code[_status] << "\r\n"
		<< "Server: Equal-Rights/0.1.23" << "\r\n"
		<< "Content-Type: " << _indicateFileType() << "\r\n"
		<< "Content-Length: " << _calculateFileSize() << "\r\n"
//		TODO: need alive or close?
//		<< "Connection: keep-alive" << "\r\n"
		<< "Connection: close" << "\r\n"
		<< "\r\n";
	return str.str();
}

std::string BadResponse::generateBody() {
	std::string		buf;
	std::ifstream	file;

	file.open(_errorPage[_status], std::ifstream::in);

	buf.reserve(_calculateFileSize());
	buf.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
	file.close();

	return buf;
}
