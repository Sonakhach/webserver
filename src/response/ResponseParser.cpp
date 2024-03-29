#include "ResponseParser.hpp"
#include "server/Errors.hpp"
#include "cgi/Cgi.hpp"
#include "utils.hpp"
#include "types/MimeTypes.hpp"
#include "server/Errors.hpp"
#include "request/utils.hpp"

ResponseParser::ResponseParser(RequestParser req, std::map<int, Config *> configs_map)
{
    this->request = req;
    this->is_finish = 0;
    this->configs_map = configs_map;
    this->setCorrespondingLocation();
	this->launchResponse();
	// this->setErrorMap();
}

void ResponseParser::setResponse(std::string r){
	this->_response = r;
}

std::string ResponseParser::checkErrorFromConf(int status)
{
	std::vector<std::pair<std::string, std::string> > error_from_conf = this->corresponding_location->getError_page();

	if(error_from_conf.empty())
	{
		return NULL;
	}
	else{
		for(std::vector<std::pair<std::string, std::string> >::iterator it = error_from_conf.begin(); it != error_from_conf.end(); ++it)
		{
			if(atoi(it->first.c_str()) == status)
			{
				return it->second;
			}
		}
	}
	return NULL;
}

int ResponseParser::checkIsAloowedMethod(){
	std::string req_method = this->request.getMethod();
	std::vector<std::string> http_methods;
	http_methods.push_back("GET");
	http_methods.push_back("POST");
	http_methods.push_back("DELETE");
	std::vector<std::string>::iterator found = std::find(http_methods.begin(), http_methods.end(), req_method);
	if(found == http_methods.end())
		throw(405);
	std::vector<std::string> allowed_methods = this->corresponding_location->getMethods();
	found = std::find(allowed_methods.begin(), allowed_methods.end(), req_method);
	if(found == allowed_methods.end())
		throw(405);
	return 0;
}	

int ResponseParser::generateGetResponse()
{
	// Directives *loc = corresponding_location;
	// MimeTypes mime_types;
	// std::cout << "serve_root - " << serve_root << std::endl;
	// if (serve_root.find("favicon.ico") != std::string::npos)
	// {
	// 	if (!(is_file_exists(serve_root)))
	// 	{
	// 		return 0;
	// 	}
	// }

	// if (!loc->_return.empty())
	// {
	// 	Errors err;
	// 	this->_response = err.getStatusLine(302);
	// 	this->_response += "Location: " + loc->getReturn() + "\n";
	// 	this->_response += "Content-Length: 0\n\n";
	// 	return 0;
	// }

	// if (is_file_exists(serve_root))
	// {
	// 	if(is_regular_file(serve_root))
	// 	{
	// 		this->_response = generateResponseStringForPath(200, serve_root);
	// 	}
	// 	else
	// 	{
	// 		if(loc->_index.empty())
	// 		{
	// 			if (loc->_autoindex == "on")
	// 			{
	// 				this->_response = generateResponseStringForString(200, getDirContentHTML(serve_root));
	// 			}
	// 			else
	// 			{
	// 				throw(404);
	// 			}
	// 		}
	// 		else
	// 		{
	// 			bool is_404 = true;
	// 			for (unsigned long i = 0; i < loc->_index.size(); ++i)
	// 			{
	// 				std::string concat_paths = concatStrings(serve_root, loc->_index[i]);
	// 				if(is_file_exists(concat_paths) && is_regular_file(concat_paths))
	// 				{
	// 					this->_response = generateResponseStringForPath(200, concat_paths);
	// 					is_404 = false;
	// 					break;
	// 				}
	// 			}
	// 			if (is_404)
	// 			{
	// 				throw(404);
	// 			}
	// 		}
	// 	}
	// }
	// else
	// {
	// 	throw(404);
	// }
    return 0;
}

int ResponseParser::checkMaxBodySize(){
	std::string max_body_size = this->corresponding_location->getClient_max_body_size();
	std::string suffix = "kKmM";
	if(max_body_size.size())
	{
		size_t pos = suffix.find(max_body_size[max_body_size.length() - 1]);
		if(pos != std::string::npos)
			max_body_size = max_body_size.substr(0, max_body_size.size()-1);
		if (is_number(max_body_size))
			this->max_body_size_bytes = atoi(max_body_size.c_str());
		else
			return 0;
		if((pos == 0) || (pos == 1))
			max_body_size_bytes *= 1000; 
		if ((pos == 2) || (pos == 3))
			max_body_size_bytes *= 1000000;
		std::cout << "Max body size - " << max_body_size_bytes << std::endl;
		if(max_body_size_bytes < this->request.getPostReqBody().size())
		{
			throw(413);
		}
	}
	return 0;
}

int ResponseParser::generatePostResponse()
{
	// std::cout << this->request.getContentType() << std::endl;
	checkMaxBodySize();
	Cgi::execute(*this);
    std::ofstream out("serve_files/" + this->request.getPostReqFilename());
	out << this->request.getPostReqBody();
	out.close();
    return 0;
}

int ResponseParser::generateDeleteResponse()
{
	if (std::remove(this->getServerRoot().c_str()) == -1) 
        throw(404);
    return 0;
}

int ResponseParser::launchResponse()
{
	std::string req_method = this->request.getMethod();
	try{
		this->checkIsAloowedMethod();
		if(req_method == "GET")
		{
			this->generateGetResponse();
		}
		else if(req_method == "POST")
		{
			this->generatePostResponse();
		}
		else if(req_method == "DELETE")
		{
			this->generateDeleteResponse();
		}
	}
	catch(int status)
	{
		
		Errors *e = new Errors(status, *this);
		this->_response = e->getErrorResponse();
		delete e;
		return 0;
		
		// std::cout << "The response is returned staus code ---- " << status << std::endl;
	}
	std::string arr="HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length: 16\n\n<h1>testing</h1>";
	std::string my_response = "HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length: ";
	struct stat filestatus;
	stat("www/index.html", &filestatus );
	std::cout << "HOST -" << this->request.getHost() << std::endl;
	// std::cout << "URL --- " <<  this->request.getRoute() << std::endl;
	my_response += std::to_string(filestatus.st_size) + "\n\n";
	std::ifstream ifs("www/index.html");
	std::string content( (std::istreambuf_iterator<char>(ifs) ),
				(std::istreambuf_iterator<char>()    ) );
	my_response	+= content;
	this->_response = my_response;
	ifs.close();
    return 0;
};

std::string ResponseParser::getContentLengthLine(const std::string &path)
{
	std::string res("Content-Length: ");
	struct stat filestatus;
	stat(path.c_str(), &filestatus );
	res+= std::to_string(filestatus.st_size) + "\n";
	return res;
}

std::string ResponseParser::generateResponseStringForPath(const int status_code, const std::string &path)
{
	Errors err;
	std::string resp = err.getStatusLine(status_code) + MimeTypes::getContentTypeLine(path) + getContentLengthLine(path) + "\n";
	std::ifstream ifs(path);
	std::string content( (std::istreambuf_iterator<char>(ifs) ),
						(std::istreambuf_iterator<char>()) );
	resp += content;
	ifs.close();
	return resp;
}

std::string ResponseParser::generateResponseStringForString(const int status_code, const std::string &content)
{
	Errors err;
	std::string resp = err.getStatusLine(status_code) + "Content-Type:text/html\nContent-Length: " + std::to_string(content.length()) + "\n\n";
	resp += content;
	return resp;
}
