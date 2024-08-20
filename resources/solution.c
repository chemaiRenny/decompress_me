/**
 * To build: gcc solution.c -lz -o solution
 * To run: ./solution
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <zlib.h>

#define HOST "51.11.50.70"
#define PORT 5050

#define Info(x, ...) printf("[INFO] " x "\r\n", ##__VA_ARGS__)
#define Error(x, ...) printf("[ERROR] " x "\r\n", ##__VA_ARGS__)

int main() {
    Info("starting application");

    int ret = 0;

    Info("Opening socket...");
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        Error("%s", strerror(errno));
        return -1;
    }

    //length retrieved by making a request using nc
    size_t response_size = 349;
    char server_response[response_size + 1];
    char hex_string[256];
    char byte_array[256];
    char decompressed_string[256];

    struct sockaddr_in server;

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr(HOST);

    Info("Connecting to host %s on port %d", HOST, PORT);
    if (connect(sockfd, (struct sockaddr*)&server, sizeof(server)) != 0) {
        ret = -1;
        goto exit;
    }

    Info("Receiving Data...");

    char* response = server_response;
    int count = 0;
    int total = 0;

    while (1) {
        if ((count = recv(sockfd, response + total, response_size - total, 0)) < 0) {
            Error("recv error!");
            ret = -1;
            goto exit;
        }

        Info("Received %d bytes from server", count);

        total += count;
        if (total >=  response_size) {
            Info("Done receiving data from server");
            break;
        }
    }

    server_response[total] = '\0';

    printf("\r\n++++++++++ DATA +++++++++++\r\n");
    printf("%s\r\n", server_response);
    printf("----------- DATA -----------\r\n\r\n");

    Info("Parsing Server Response...");
    if ((sscanf((const char*)server_response, "%*[^\n] %*[^\n] %*[^\n] %*[^\n] %[^\n]", hex_string)) < 0) {
        Error("Error getting hex string");
        ret = -1;
        goto exit;
    }

    Info("Hex String: %s", hex_string);
    size_t hex_len = strlen(hex_string);

    Info("Decoding Hex to Byte Array");

    char* hex_pos = hex_string;

    for (size_t count = 0; count < hex_len / 2; count++) {
        sscanf(hex_pos, "%2hhx", &byte_array[count]);
        hex_pos += 2;
    }

    Info("Decompressing byte array ...");
    z_stream strm;
    strm.next_in = (Bytef*)byte_array;
    strm.avail_in = (uInt)(hex_len / 2);
    strm.total_out = 0;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;

    inflateInit2(&strm, (16 + MAX_WBITS));
    while (1) {
        strm.next_out = (Bytef*)(decompressed_string + strm.total_out);
        strm.avail_out = sizeof(decompressed_string) - strm.total_out;

        int err = inflate(&strm, Z_SYNC_FLUSH);
        if (err == Z_STREAM_END) {
            Info("Gzip done!");
            break;
        } else if (err != Z_OK)  {
            ret = -1;
            Error("gzip decompress error %d", err);
            goto exit;
        }
    }
    inflateEnd(&strm);

    decompressed_string[strm.total_out] = '\0';

    Info("Decompressed String: %s", decompressed_string);

    Info("Sending decompressed string to server..");

    //add new line to mark end of input
    decompressed_string[strm.total_out] = '\n';
    decompressed_string[strm.total_out + 1] = '\0';

    if (send(sockfd, decompressed_string, strlen(decompressed_string), 0) > 0) {
        count = 0;
        total = 0;
        response = server_response;
        Info("Receiving Response ...");
        while ((count = recv(sockfd, response + total, response_size - total, 0)) > 0) {
            total += count;
            Info("Received %d bytes from server", count);
        }

        if (count < 0) {
            Error("recv error!");
            ret = -1;
            goto exit;
        }
        server_response[total] = '\0';

        printf("\r\n++++++++++ DATA +++++++++++\r\n");
        printf("%s\r\n", server_response);
        printf("----------- DATA -----------\r\n\r\n");
    } else {

        Error("send error!");
        ret = -1;
        goto exit;
    }

exit:
    if (errno != 0) {
        Error("%s", strerror(errno));
    }
    Info("Closing socket...");
    close(sockfd);

    return ret;
}