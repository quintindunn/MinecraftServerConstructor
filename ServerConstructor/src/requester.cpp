#include <iostream>
#include <curl/curl.h>
#include <string>
#include <vector>
#include <stdio.h>

#include "requester.h"

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

size_t WriteCallbackFile(void* ptr, size_t size, size_t nmemb, FILE* stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

std::string http_get(std::string url, int attempts) {
    for (int i = 0; i < attempts; i++) {
        // Makes a GET request using CURL.
        CURL* curl;
        CURLcode res;
        std::string readBuffer;
        bool retry = false;

        curl_global_init(CURL_GLOBAL_DEFAULT);
        curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
#ifdef VERBOSE_CURL
            curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
#endif
            curl_easy_setopt(curl, CURLOPT_USERAGENT, UA);

            res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
                retry = true;
            }

            curl_easy_cleanup(curl);
        }
        curl_global_cleanup();
        if (retry) {
            continue;
        }
        return readBuffer;
    }
}

std::string http_get(std::string url) {
    return http_get(url, 1);
}

void http_get_file(std::string url, std::string output_file, int attempts) {
    for (int i = 0; i < attempts; i++) {
        // Makes a GET request using CURL.
        CURL* curl;
        CURLcode res;
        FILE* fp;
        bool retry = false;

        curl_global_init(CURL_GLOBAL_DEFAULT);
        curl = curl_easy_init();
        if (curl) {
            errno_t err = fopen_s(&fp, output_file.c_str(), "wb");

            if (err != 0 || fp == nullptr) {
                fprintf(stderr, "Error opening file: %s\n", output_file.c_str());
                curl_easy_cleanup(curl);
                curl_global_cleanup();
                continue;
            }

            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallbackFile);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
#ifdef VERBOSE_CURL
            curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
#endif
            curl_easy_setopt(curl, CURLOPT_USERAGENT, UA);

            res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
                retry = true;
            }

            curl_easy_cleanup(curl);
            fclose(fp);
        }
        curl_global_cleanup();
        if (retry) {
            continue;
        }
        return;
    }
    
}

void http_get_file(std::string url, std::string output_file) {
    return http_get_file(url, output_file, 1);
}