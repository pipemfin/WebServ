//                             //
// Created by                  //
//            Zaibatsu         //
// on                          //
//            14.07.2021       //
//                             //

#include "../../inc/output.hpp"

ssize_t sendall(s_client* client){
	ssize_t result = 0;

	if (client->responseNotSend){
		result = send(client->socket, client->responseBuffer.c_str(), client->responseBuffer.length(), MSG_NOSIGNAL);

		if (static_cast<int>(result) == -1 || static_cast<int>(result) == 0){
			perror("");
			return result;
		}
		std::cout
				<< "---- Pack: " << "\n"
				<< "Data Left:\t" << client->responseBuffer.length() << "\n"
				<< "Send Result:\t" << result
				<< std::endl;

		//TODO: delete sleep for delay after send
		usleep(1000);

		if (result == static_cast<ssize_t>(client->responseBuffer.length())){
			std::cout << "All data was send. Clear buffer." << std::endl;
			client->responseBuffer.clear();
			client->responseNotSend = false;
			client->status = 0;
			return result;
		}
		try{
			client->responseBuffer = client->responseBuffer.substr(result);
		}
		catch (std::exception & e){
			std::cout << e.what() << std::endl;
		}
	}
	return result;
}

ssize_t response(s_client *client){
	std::cout << "--------------------> Response part <------------ " << std::endl;
	AResponse* response;
	ssize_t result;

	if (client->responseNotSend){
		return sendall(client);
	}

	std::cout << "client check crushed ⛔️ <--" << std::endl;
	client->request->getMethod();
	client->request->getErr();
	std::cout << "kek" << std::endl;
	t_location *location = get_location(client->request->getPath(), &client->server->locations);
	if (location == NULL){
		std::cout << " ---> Broken location <---" << std::endl;
		client->status = 0;
		return -1;
	}
	std::cout << "client check restored ✅ -->" << std::endl;

	if (client->request->getErr() != 0){
		std::cout << "Request error" << std::endl;
		client->responseBuffer = "la la la";
		std::string a = location->root;
		std::cout << "Request error 2" << std::endl;
		response = new BadResponse(400, location->root);
		std::cout << "Request error 3" << std::endl;
	}
	else{
		if (client->request->getMethod() == "GET"){
			response = methodGet(client);
		}

		if (client->request->getMethod() == "POST")
			response = methodPost(client);

		if (client->request->getMethod() == "HEAD"){
//			response->setHead(1);
			response = new BadResponse(405, location->root);
		}

		if (client->request->getMethod() == "DELETE")
			response = methodDelete(client);

		if (client->request->getMethod() == "PUT"){
			response = methodPut(client);
//			response = new BadResponse(201, location->root);
		}
	}
	std::cout << " -->" << std::endl;
	std::cout << response->generateHeader();
	std::cout << " -->" << std::endl;

	client->responseBuffer = response->generateResponse();
	client->responseNotSend = true;

	client->buffer.clear();
	result = sendall(client);

	delete response;
//	delete client->request;
	std::cout << "--------------------> Response END <------------ " << std::endl;
	return result;
}

//ssize_t sendall(s_client* client){
//	ssize_t result = 0;
//	std::string currentBuf;
//	std::string tmp;
//	size_t bufferSize = 262144;
//
//	if (client->responseNotSend){
//		currentBuf = client->responseBuffer;
//		result = send(client->socket, currentBuf.c_str(), currentBuf.length(), 0);
//		std::cout << "---- Pack:" << std::endl;
//		std::cout << "Data Left:\t\t" << std::setw(10) << client->responseBuffer.length() << std::endl;
//		std::cout << "Send Result:\t" << std::setw(10) << result << std::endl;
//
//		if (result == static_cast<ssize_t>(client->responseHead.length())) {
//			std::cout << "All data send. Clear header." << std::endl;
//			client->responseBuffer.clear();
//			client->headerWasNotSend = false;
//		}
//		else if (result > 0 && result < static_cast<ssize_t>(client->responseHead.length())){
//			std::cout << "Not all header send. Save buffer." << std::endl;
//			client->responseBuffer = client->responseBuffer.substr(result);
//			client->status = 1;
//		}
//		else if (result == 0){
//			std::cout << "I have no header to send. Clear buffer." << std::endl;
//			client->responseBuffer.clear();
//			client->headerWasNotSend = false;
//		}
//		else if (result < 0){
//			std::perror("Send: ");
////			client->headerWasSend = 1;
////			client->status = 0;
//		}
//	}
//}
//
//ssize_t sendall2(s_client *client){
//	ssize_t result = 0;
//	std::string currentBuf;
//	std::string tmp;
//	size_t bufferSize = 262144;
//
//	if (client->headerWasNotSend){
//		currentBuf = client->responseHead;
//		result = send(client->socket, currentBuf.c_str(), currentBuf.length(), 0);
//		std::cout << "---- Header Pack:" << std::endl;
//		std::cout << "Data Left:\t" << std::setw(10) << client->responseHead.length() << std::endl;
//		std::cout << "Send Result:\t" << std::setw(10) << result << std::endl;
//
//		if (result == static_cast<ssize_t>(client->responseHead.length())) {
//			std::cout << "All header send. Clear header." << std::endl;
//			client->responseHead.clear();
//			if (client->responseBody.empty())
//				client->status = 0;
//			client->headerWasNotSend = false;
//		}
//		else if (result > 0 && result < static_cast<ssize_t>(client->responseHead.length())){
//			std::cout << "Not all header send. Save buffer." << std::endl;
//			client->responseHead = client->responseHead.substr(result);
//			client->status = 1;
//		}
//		else if (result == 0){
//			std::cout << "I have no header to send. Clear buffer." << std::endl;
//			client->responseHead.clear();
//			if (client->responseBody.empty())
//				client->status = 0;
//			client->headerWasNotSend = false;
//		}
//		else if (result < 0){
//			std::perror("Send: ");
////			client->headerWasSend = 1;
////			client->status = 0;
//		}
//	}
//	else if (client->bodyWasNotSend){
//		if (client->responseBody.length() > bufferSize){
////			tmp = client->responseBody.substr(0, bufferSize);
////			currentBuf = SSTR(tmp.length());
//			currentBuf += client->responseBody.substr(0, bufferSize);
//		}
//		else
//			currentBuf = client->responseBody;
//
//		result = send(client->socket, currentBuf.c_str(), currentBuf.length(), MSG_NOSIGNAL);
//
//		std::cout << "---- Pack:" << std::endl;
//		std::cout << "Data Left:\t" << std::setw(10) << client->responseHead.length() + client->responseBody.length() << std::endl;
//		std::cout << "Send Result:\t" << std::setw(10) << result << std::endl;
//
//		if (result == static_cast<ssize_t>(client->responseBody.length())) {
//			std::cout << "All Body send. Clear Body." << std::endl;
//			client->responseBody.clear();
//			client->status = 0;
//			client->bodyWasNotSend = false;
//		}
//		else if (result > 0 && result < static_cast<ssize_t>(client->responseBody.length())){
//			std::cout << "Not all Body send. Save buffer." << std::endl;
//			client->responseBody = client->responseBody.substr(result);
//			client->status = 1;
//		}
//		else if (result == 0){
//			std::cout << "I have no Body to send. Clear buffer." << std::endl;
//			client->responseBody.clear();
//			client->status = 0;
//			client->bodyWasNotSend = false;
//		}
//		else if (result < 0){
//			std::perror("Send: ");
////			client->status = 0;
////			client->headerWasSend = 1;
//		}
//	}
//	return result;
//}
