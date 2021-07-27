//                                   //
// Created by:                       //
//              Watcher Jarr         //
//         on:                       //
//              7/8/21				 //
//                                   //

#ifndef OUTPUT_HPP
#define OUTPUT_HPP

#include <map>
#include <errno.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include <stdio.h>
#include <sstream>
#include "../src/Request.hpp"
#include "../src/Server.hpp"
#include <sys/types.h>
#include <sys/wait.h>

#define COLOR_DEFAULT "\e[0m"
#define COLOR_RED "\e[31m"
#define COLOR_GREEN "\e[32m"
#define COLOR_GREY "\e[37m"

typedef struct  s_client 
{
    int         socket;
    int         status;
    std::string buffer;
    Request     *request;
}               t_client;

ssize_t response(t_client *client);
std::vector<Server> *parsingConfiguration(char *config_name);
// ssize_t response(s_client client);
std::string trim(std::string old_string);
std::string trim_end(std::string old_string);
std::vector<std::string> splitvector(std::vector<std::string> old_vector, std::string str);

#endif //OUTPUT_HPP
