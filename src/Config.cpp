//                                          ┏┻━━┻┓        //
// Created by:                        ╭┓┏╮  ┃▕▏▕▏┃  ╭┓┏╮  //
//        Jolene Radioactive          ┃┗┛┃  ┃┏┳┳┓┃  ┃┗┛┃  //
//         on:                        ╰┳┳╯  ┃┗┻┻┛┃  ╰┳┳╯  //
//              07/21				   ┃┃ ┏━┻━━━━┻━┓ ┃┃   //
//                                     ┃╰━┫╭━━━━━━╮┣━╯┃   //
//                                     ╰━━┫┃╱╲╱╲╱╲┃┣━━╯   //

#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <cstring>
#include <list>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <vector>
#include "Request.hpp"
#include "../inc/output.hpp"
#include  "Server.hpp"
#include <dirent.h>
#include <sys/types.h>

extern Logger logs;

std::vector<std::string> *readFile(char *config_name)
{
	std::ifstream               config_file(config_name);
	std::string                 temp;
	std::vector<std::string>    *configuration = new std::vector<std::string>;

	std::cout << "Start configuration reading..." << std::endl;
	if (!config_file)
	{
		std::cerr << "Configuration file not found!" << std::endl;
		delete configuration;
		return (NULL);
	}
	while (config_file)
	{
		std::getline(config_file, temp);
		if (config_file.eof())
		{
			configuration->push_back(temp);
			break;
		}
		else
			configuration->push_back(temp);
	}
	config_file.close();
	return (configuration);
}

Server  *print_error(Server *temp, int i, int flag)
{
	if (flag == 1)
		std::cerr << "Configuration file:" << i + 1 << ". Error not found" << std::endl;
	else if (flag == 2)
		std::cerr << "Configuration file:" << i + 1 << ". Incorrect path" << std::endl;
	else if (flag == 3)
		std::cerr << "Configuration file:" << i + 1 << ". Error reading file" << std::endl;
	else if (flag == 4)
		std::cerr << "Configuration file:" << i + 1 << ". Incorrect size" << std::endl;
	else if (flag == 5)
		std::cerr << "Configuration file:" << i + 1 << ". Unknown directive in location" << std::endl;
	temp = NULL;
	return (temp);
}

Server  *error_pages(Server *temp, std::string str, int i)
{
	std::map <int, std::string>::iterator it;
	int error;
	try
	{
		error = std::stoi(str.substr(15, 4));
	}
	catch(const std::exception& e)
	{
		std::cerr << "Configuration file:" << i + 1 << " Invalid error number" << std::endl;
		return (NULL);
	}
	it = temp->error_pages.find(error);
	if (it == temp->error_pages.end())
		temp = print_error(temp, i, 1);
	else
		it->second = trim(str.substr(19, str.length() - 19));
	if (temp != NULL && it != temp->error_pages.end() && it->second.empty())
		temp = print_error(temp, i, 2);
	else if (temp != NULL)
	{
		std::fstream fs(it->second, std::fstream::in);
		if (!fs.is_open())
			temp = print_error(temp, i, 3);
		else
			fs.close();
	}
	return(temp);
}

Server  *listen(Server *temp, std::string str, int i)
{
	t_socket *ts = new t_socket;
	if (str.find(":") == std::string::npos)
	{
		delete ts;
		return (NULL);
	}
	try
	{
		std::string ip =  trim(str.substr(11, str.find(":") - 11));
		ts->address = strdup(ip.c_str());
		ts->port = std::stoi(str.substr(str.find(":") + 1, str.length() - str.find(":")));
	}
	catch(const std::exception& e)
	{
		std::cerr << "Configuration file:" << i + 1 << " Incorrect ip or port" << std::endl;
		delete ts->address;
		delete ts;
		return (NULL);
	}
	temp->sockets.push_back(ts);
	return(temp);
}

void init(t_location *lctn)
{
	lctn->index = "";
	lctn->root = "";
	lctn->methods = 0;
	lctn->autoindex = 0;
	lctn->max_body_size = 0;
	lctn->redirect = "";
}

int getAllowsMethods(std::string str, int i)
{
	std::vector<std::string> massive;
	massive = std_split(str);
	int put = 0;
	int p = 0;
	int d = 0;
	int g = 0;
	int num = 4;
	for (size_t j = 0; j < massive.size(); j++)
	{
		if (massive[j].compare("GET") == 0 && g == 0)
		{
			g = 1;
		}
		else if (massive[j].compare("POST") == 0 && p == 0)
		{
			num += 2;
			p = 1;
		}
		else if (massive[j].compare("DELETE") == 0 && d == 0)
		{
			num += 1;
			d = 1;
		}
		else if (massive[j].compare("PUT") == 0 && put == 0)
		{
			num += 8;
			put = 1;
		}
		else 
		{
			std::cerr << "Configuration file:" << i + 1 << " Unknown method" << std::endl;
			return (-1);
		}
	}
	return(num);
}

long long int max_body_size(Server *temp, std::string str, int i)
{
	size_t n = std::count(str.begin(), str.end(), ' ');
	std::string size = str.substr(str.length() - 1, 1);
	long long int size_num = 0;
	int b = 1;
	if (n > 1 || !(size == "M" || size == "K" || size == "G" || size == "B"))
	{
		temp = print_error(temp, i, 4);
		return (-1);
	}
	if (size == "K")
		b = 1024;
	else if (size == "M")
		b = 1024 * 1024;
	else if (size == "G")
		b = 1024 * 1024 * 1024;
	try
	{
		if (temp != NULL && std::stoll(str.substr(0, str.length() - 2)) > 0)
			size_num = std::stoll(str.substr(0, str.length() - 2)) * b;
	}
	catch(const std::exception& e)
	{
		temp = print_error(temp, i, 4);
		return (-1);
	}
	return (size_num);
}

Server  *location(Server *temp, std::vector<std::string> *configuration, int i)
{
	t_location *lctn = new t_location;
	init(lctn);
	std::string loc = trim_end((*configuration)[i]);
	int k = 0;
	for (size_t j = i + 1; k == 0 && j < (*configuration).size(); ++j)
	{
		if (j + 1 != (*configuration).size())
			k = (*configuration)[j + 1].compare(0, 8, "        ");
		if ((*configuration)[j].compare(0, 13, "        root ") == 0)
			lctn->root = rslash_from_end(trim((*configuration)[j].substr(13, (*configuration)[j].length() - 13)));
		else if ((*configuration)[j].compare(0, 14, "        index ") == 0)
			lctn->index = trim((*configuration)[j].substr(14, (*configuration)[j].length() - 14));
		else if ((*configuration)[j].compare(0, 21, "        auto_index on") == 0)
			lctn->autoindex = 1;
		else if ((*configuration)[j].compare(0, 22, "        auto_index off") == 0)
			lctn->autoindex = 0;
		else if ((*configuration)[j].compare(0, 22, "        allow_methods ") == 0)
			lctn->methods = getAllowsMethods(trim((*configuration)[j].substr(22, (*configuration)[j].length() - 22)), j);
		else if ((*configuration)[j].compare(0, 22, "        max_body_size ") == 0)
			lctn->max_body_size = max_body_size(temp, trim((*configuration)[j].substr(22, (*configuration)[j].length() - 22)), j);
		else if ((*configuration)[j].compare(0, 15, "        return ") == 0)
			lctn->redirect = trim((*configuration)[j].substr(15, (*configuration)[j].length() - 15));
		else
			temp = print_error(temp, i, 5);
	}
	lctn->location = "/" + (loc.substr(14, loc.length() - 15));
	if (lctn->methods == - 1 || lctn->max_body_size == -1)
	{
		delete lctn;		
		return (NULL);
	}
	if (temp != NULL)
	{
		std::string ll = "/" + (loc.substr(14, loc.length() - 15));
		temp->locations[ll] = lctn;
	}
	return (temp);
}

Server  *upload_file_to(Server *temp, std::string str, int i)
{
	DIR *dir = NULL;
	temp->upload_file_to = trim(str.substr(19, str.length() - 19));
	if ((dir = opendir(temp->upload_file_to.c_str())) == NULL)
    {
        std::cerr << "Configuration file:" << i + 1 << ". " << strerror(errno) << std::endl;
        return (NULL);
    }
	closedir(dir);
	return (temp);
}

std::vector<std::string> server_names(std::string str)
{
	std::vector<std::string> names;
	std::string buffer = "";
	str += " ";
	for (int i = 0; i < (int)str.length(); i++)
	{
		if( str[i] != ' ')
			buffer += str[i];     
		else
		{
			names.push_back(buffer);
			buffer = "";
		}
	}
	return (names);
}


std::vector<Server*>  *pars(std::vector<Server*> *servers, std::vector<std::string> *configuration, int begin, int end)
{
	Server                      *temp = new Server;

	for (int i = begin; i < end && temp != NULL; ++i)
	{
		std::string str = trim_end((*configuration)[i]);
		if (str.compare(0, 11, "    listen ") == 0)
			temp = listen(temp, str, i);
		else if (str.compare(0, 16, "    server_name ") == 0)
			temp->server_name = server_names(trim(str.substr(16, str.length() - 16)));
		else if (str.compare(0, 14, "    location /") == 0 && str[str.length() - 1] == '/' && (*configuration)[i+1].compare(0, 13, "        root ") == 0)
			temp = location(temp, configuration, i);
		else if (str.compare(0, 15, "    error_page ") == 0)
			temp = error_pages(temp, str, i);
		else if (str.compare(0, 19, "    upload_file_to ") == 0)
			temp = upload_file_to(temp, str, i);
		else if (str == "server" || str.compare(0, 8, "        ") == 0)
			continue;
		else
		{
			std::cerr << "Configuration file:" << i + 1 << ". Unknown directive or directive without values" << std::endl;
			temp = NULL;
		}
	}
	if (temp == NULL)
		return (NULL);
	servers->push_back(temp);
	return (servers);
}

std::vector<Server*> *parsingConfiguration(char *config_name)
{
	std::vector<Server*>         *servers = new std::vector<Server*>;
	std::vector<std::string>    *configuration = readFile(config_name);
	int cnt = 0, begin = 0, end = 0;

	if (configuration == NULL)
	{
		delete servers;
		return (NULL);
	}
	for (size_t i = 0; i < configuration->size(); ++i)
	{
		if ((*configuration)[i] == "server")
			cnt += 1;
		if (trim((*configuration)[i]).empty())
		{
			std::cerr << "Configuration file: "<< i + 1 << " Empty string" << std::endl;
			return (NULL);
		}
	}
	if (configuration->size() == 0 || cnt == 0)
	{
		std::cerr << "Configuration file: no mandatory directives" << std::endl;
		return (NULL);
	}
	if (cnt == 1)
	{
		if ((servers = pars(servers, configuration, 0, configuration->size())) == NULL)
			return (NULL);
	}
	else if (cnt > 1)	
		for (size_t i = 0; i < configuration->size(); ++i)
			if ((*configuration)[i] == "server")
			{
				begin = i + 1;
				for (size_t j = i + 1; j < configuration->size(); ++j)
					if ((*configuration)[j] == "server" || j + 1 == configuration->size())
					{
						end = j + 1;
						if ((servers = pars(servers, configuration, begin, end)) == NULL)
							return (NULL);
						break;
					}
			}
	delete configuration;
	return (servers);
}
