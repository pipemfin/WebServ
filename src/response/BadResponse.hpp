//                                   //
// Created by:                       //
//              Watcher Jarr         //
//         on:                       //
//              8/2/21				 //
//                                   //

#ifndef BADRESPONSE_HPP
#define BADRESPONSE_HPP

#include "AResponse.hpp"

class BadResponse : public AResponse {
public:
	BadResponse();
	BadResponse(long long maxContent, const std::string & root, const std::string & fileName);
	~BadResponse();

	std::string	generateResponse(int res);
	std::string	generateHeader(int status);
	std::string	generateBody();
};

#endif //BADRESPONSE_HPP