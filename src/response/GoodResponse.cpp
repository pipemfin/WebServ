//                                   //
// Created by:                       //
//              Watcher Jarr         //
//         on:                       //
//              7/9/21				 //
//                                   //

#include "GoodResponse.hpp"

GoodResponse::GoodResponse() : AResponse() {
}

GoodResponse::GoodResponse(const std::string &root, const std::string &fileName) : AResponse(root,fileName){
}

GoodResponse::GoodResponse(const GoodResponse &rhi){
	*this = rhi;
}

GoodResponse & GoodResponse::operator=(const GoodResponse &rhi){
	if (this == &rhi)
		return *this;
	AResponse::operator=(rhi);
	return *this;
}

GoodResponse::~GoodResponse(){
}

std::string GoodResponse::generateResponse() {
	if (getHead())
		return generateHeader();
	return generateHeader() + generateBody();
}

std::string GoodResponse::generateHeader() {
	std::stringstream str;
	str << _protocol << " "
		<< _status << " "
		<< _code[_status] << "\r\n"
		<< "Server: Equal-Rights/0.1.23" << "\r\n";
//	if (getFileName().find(".jpg") != std::string::npos || getFileName().find(".png") != std::string::npos)
//		str	<< "Transfer-Encoding: chunked" << "\r\n";
	str	<< "Content-Type: " << _indicateFileType() << "\r\n"
		<< "Content-Length: " << _calculateFileSize() << "\r\n"
		<< "Connection: keep-alive" << "\r\n"
		<< "\r\n";
	return str.str();
}

std::string GoodResponse::generateBody() {
	std::string			buffer;
	std::ifstream		file;
	std::stringstream	str;

	file.open((_root + _fileName).c_str(), std::ifstream::in);

	while (file.good()) {
		std::getline(file, buffer);
		str << buffer;
		if (file.good())
			str << "\n";
	}
	file.close();
	return str.str();
}
