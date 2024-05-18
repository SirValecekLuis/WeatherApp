//
// Created by Tobias on 18.05.2024.
//

#ifndef WEATHERAPP_API_CALL_H
#define WEATHERAPP_API_CALL_H

#include <curl/curl.h>
#include <optional>
#include <string>


size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp);

std::optional<std::string> send_request(CURL *curl, const std::string &url);

#endif //WEATHERAPP_API_CALL_H
