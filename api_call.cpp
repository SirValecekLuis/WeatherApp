//
// Created by Tobias on 18.05.2024.
//
#include "api_call.h"


size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    // https://gist.github.com/alghanmi/c5d7b761b2c9ab199157
    ((std::string *) userp)->append((char *) contents, size * nmemb);
    return size * nmemb;
}

std::optional<std::string> send_request(CURL *curl, const std::string &url) {
    std::string read_buffer;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &read_buffer);
    curl_easy_perform(curl);    // Řádek, který spouští celý dotaz
    int code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code); // Kontrola HTTP odpovědi

    // Tady musí vracet "OK" což je 200 a string nesmí být prázdný, jinak to znamená, že HTTP dotaz nevrátil nic
    if (code != 200 || read_buffer.empty() || read_buffer == "[]") {
        return std::nullopt;
    }

    return {read_buffer};
}
