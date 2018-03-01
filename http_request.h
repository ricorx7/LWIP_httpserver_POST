/*
 * http_request.h
 *
 * Created: 2/9/2018 1:05:04 PM
 *  Author: rico
 */ 


#ifndef HTTP_REQUEST_H_
#define HTTP_REQUEST_H_

typedef struct http_request {
	char *uri;
	char *post_data;
	uint8_t is_post;
	char *params;
} HTTPRequest;


#endif /* HTTP_REQUEST_H_ */