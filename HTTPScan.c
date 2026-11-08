#include <stdio.h> 
#include <stdlib.h>
#include <string.h> // Allows Program to handle strings
#include <curl/curl.h> // Allows Program to send Web Requests

size_t header_callback(char *buffer, size_t size, size_t nitems, void *userdata) {
    // Safely write the exact chunk of header data straight to the terminal
    fwrite(buffer, size, nitems, stdout);
    
    return size * nitems; // Tell libcurl we successfully handled all bytes
}

int main(){

    char url[256];

    // Asks for Input
    printf("Enter Website:");

    // Reads Input
    char *input = fgets(url, sizeof(url), stdin);

    // Checks if Input is successful
    if (input != NULL) {

        // Finds Newline 
        int i = 0;
        while (url[i] != '\0') {

            // Replaces NewLine with terminator
            if (url[i] == '\n') {
                url[i] = '\0';
                break;
            }

            i++;

        }
    }

    CURL *curl;
    CURLcode result;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if(curl) {
        // Tells curl the url
        curl_easy_setopt(curl, CURLOPT_URL, url);

        //Follow redirects
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        //GRAB HEADERS ONLY
        curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);

        //Register Callback Function
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_callback);

        //SSL certificate Checker
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);

        //Execute Request
        result = curl_easy_perform(curl);

        // Check for Network Errors
        if (result != CURLE_OK){
            printf("Error: %s\n", curl_easy_strerror(result));
        }

        //Cleanup Session Handle
        curl_easy_cleanup(curl);
    }

    //Clean up global Environment
    curl_global_cleanup();

    return 0;
}

