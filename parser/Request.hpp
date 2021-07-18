//                                          ┏┻━━┻┓        //
// Created by:                        ╭┓┏╮  ┃▕▏▕▏┃  ╭┓┏╮  //
//        Jolene Radioactive          ┃┗┛┃  ┃┏┳┳┓┃  ┃┗┛┃  //
//         on:                        ╰┳┳╯  ┃┗┻┻┛┃  ╰┳┳╯  //
//              07/21				    ┃┃┏━┻━━━━┻━┓┃┃    //
//                                      ┃╰┫╭━━━━━━╮┣╯┃    //
//                                      ╰━┫┃╱╲╱╲╱╲┃┣━╯    //

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <map>
#include <fstream>
#include <sstream>

class Request{
private:
	std::string _method;
	std::string _path;
	std::string _protocol;
	std::string _host;
	std::string _content_type;
	std::string _content_length;
	int			_err;

public:
	Request();
	~Request();
	
	void setErr(int err);
	int getErr() const;

	std::string getMethod() const;
	std::string getPath() const;
	std::string getProtocol() const;
	std::string getHost() const;
	std::string getCType() const;
	std::string getCLength() const;


	std::vector<std::string> getarray(std::string req);
	void strrequest(std::vector<std::string> request);
	void getheaders(std::vector<std::string> request);
	void getrequest(std::vector<std::string> request);
	void postrequest(std::vector<std::string> request);
	void deleterequest(std::vector<std::string> request);

};

#endif