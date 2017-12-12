/******************************************************************************
 * File:    rwc.cpp
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.RWC
 *
 * Version:  1.0
 *
 * Date:    2017/12/01
 *
 * Author:   J C Gonzalez
 *
 * Copyright (C) 2015-2018 Euclid SOC Team @ ESAC
 *_____________________________________________________________________________
 *
 * Topic: General Information
 *
 * Purpose:
 *   Provides object implementation for class RWC
 *
 * Created by:
 *   J C Gonzalez
 *
 * Status:
 *   Prototype
 *
 * Dependencies:
 *   none
 *
 * Files read / modified:
 *   none
 *
 * History:
 *   See <Changelog>
 *
 * About: License Conditions
 *   See <License>
 *
 ******************************************************************************/

#include "rwc.h"

#include "filetools.h"
#include "scopeexit.h"

#include <fstream>
#include <string.h>
#include <unistd.h>

using namespace FileTools;

//------------------------------------------------------------
// Constructor
// Generate a temporary file with unique file name in /tmp
//------------------------------------------------------------
RWC::RWC()
{
    readFunc =
        [] (void *ptr, size_t size,
            size_t nmemb, void *userp) -> size_t {
        PostData *postData = (PostData*)userp;
        size_t byte_len = size * nmemb;
        if (postData->size < byte_len) {
            byte_len = postData->size;
        }
        memcpy(ptr, postData->ptr, byte_len);
        postData->ptr  += byte_len;
        postData->size -= byte_len;
        return byte_len;
    };
    
    fwriteFunc = 
        [] (void *ptr, size_t size,
            size_t nmemb, void *userp) -> size_t {
        return fwrite(ptr, size, nmemb, (FILE*)userp);
    };

    mwriteFunc = 
        [] (void *ptr, size_t size,
            size_t nmemb, void *userp) -> size_t {
        size_t realsize = size * nmemb;
        struct MemoryStruct *mem = (struct MemoryStruct *)userp;
        mem->memory = (char *)(realloc(mem->memory, mem->size + realsize + 1));
        if (mem->memory == NULL) {
            /* out of memory! */
            printf("not enough memory (realloc returned NULL)\n");
            return 0;
        }
        memcpy(&(mem->memory[mem->size]), ptr, realsize);
        mem->size += realsize;
        mem->memory[mem->size] = 0;        
        return realsize;
    };
    
    isAuthSet = false;
}
    
//------------------------------------------------------------
// Method: setAuth
// Set authorisation parameters
//------------------------------------------------------------
void RWC::setAuth(std::string & u, std::string & p, long aut)
{
    authType = aut;
    user = u;
    pwd = p;
    isAuthSet = true;
}

//------------------------------------------------------------
// Method: getContent
// Function to perform a GET request and obtain the result in
// a string
//------------------------------------------------------------
void RWC::getContent(std::string url, std::string & content,
                     std::string & result)
{
    std::string tmpFile = generateTmpFile();
    get(url, tmpFile, result);
    storeFileIntoString(tmpFile, content);
    unlink(tmpFile.c_str());
}
    
//------------------------------------------------------------
// Method: get
// Function to perform a GET request and obtain the result in
// the form of an output file
//------------------------------------------------------------
void RWC::get(std::string url, std::string localFile,
              std::string & result)
{
    // Get a curl object
    CURL *curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "curl_easy_init() failed\n");
        return;
    }
        
    ScopeExit curl_close([&curl] { curl_easy_cleanup(curl); });
        
    // Set url
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    // Set authorisation
    if (isAuthSet) {
        curl_easy_setopt(curl, CURLOPT_HTTPAUTH, authType);
        curl_easy_setopt(curl, CURLOPT_USERNAME, user.c_str());
        curl_easy_setopt(curl, CURLOPT_PASSWORD, pwd.c_str());
    }

    curl_easy_setopt(curl, CURLOPT_USERAGENT, "rwc-0.1");

    //curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    //curl_easy_setopt(curl, CURLOPT_HEADER, 1L);

    curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 50L);
    curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);

    // Set callback on receiving data
    FILE * fLocalFileHdl = fopen(localFile.c_str(), "wb");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, fwriteFunc);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)fLocalFileHdl);
        
    MemoryStruct responseHdr {(char*)malloc(1), 0};
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, mwriteFunc);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, (void*)&responseHdr);
        
    // Execute
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
        return;
    }
    fclose(fLocalFileHdl);

    result = std::string(responseHdr.memory);
}
    
//------------------------------------------------------------
// Method: post
// Function to POST the content of a file
//------------------------------------------------------------
void RWC::post(std::string url, std::string localFile,
               std::string & result, std::string contentType)
{
    // Set data to send
    struct curl_httppost *post1   = NULL;
    struct curl_httppost *postend = NULL;
    curl_formadd(&post1, &postend,
                 CURLFORM_COPYNAME,     "data",
                 CURLFORM_FILE,         localFile.c_str(),
                 CURLFORM_CONTENTTYPE,  contentType.c_str(),
                 CURLFORM_END);

    // Get a curl object
    CURL *curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "curl_easy_init() failed\n");
        return;
    }
    ScopeExit curl_close([&curl] { curl_easy_cleanup(curl); });
    ScopeExit form_clear([&post1] { curl_formfree(post1); });
    
    // Set url
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    // Set authorisation
    if (isAuthSet) {
        curl_easy_setopt(curl, CURLOPT_HTTPAUTH, authType);
        curl_easy_setopt(curl, CURLOPT_USERNAME, user.c_str());
        curl_easy_setopt(curl, CURLOPT_PASSWORD, pwd.c_str());
    }

    curl_easy_setopt(curl, CURLOPT_USERAGENT, "rwc-0.1");

    //curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curl, CURLOPT_HEADER, 1L);
    
    // Set HTTP method to POST
    curl_easy_setopt(curl, CURLOPT_HTTPPOST, post1);
    //curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");

    // Set callback on receiving data
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, mwriteFunc);
    MemoryStruct response {(char*)malloc(1), 0};
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&response);
    
    curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 50L);
    curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);

    // The output from the example URL is easier to read as plain text.
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Expect:");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    // Make the example URL work even if your CA bundle is missing
    //curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

    ScopeExit hdr_clear([&headers] { curl_slist_free_all(headers); });

    // Execute
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
    }

    result = std::string(response.memory);
}
    
//------------------------------------------------------------
// Method: postContent
// Function to POST a string
//------------------------------------------------------------
void RWC::postContent(std::string url, std::string & content,
                      std::string & result)
{
    std::string localFile = generateTmpFile();
    std::ofstream oFhdl(localFile);
    oFhdl << content;
    oFhdl.close();
    
    // Post file with content
    post(url, localFile, result);

    unlink(localFile.c_str());
}

