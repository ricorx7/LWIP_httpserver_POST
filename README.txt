LWIP HTTP Server

This version include handling POST with multipart handling.  This can handle file uploads and form data using POST.


# Multipart Handling
Within in the httpd_post.c file choose what to do when a POST multipart is received with these functions:
* read_header_name()
* read_header_value()
* read_part_data()
* read_on_part_data_begin()
* read_on_headers_complete()
* read_on_part_data_end()
* read_on_body_end()

The functions are called based off the parsing of the POST.  Here is the order of the parsing.
``` 
read_on_part_data_begin: (nul
 
read_header_name: Content-Disposition: 
read_header_value: form-data; name="key_name"

read_on_headers_complete: (null)

read_part_data: form_value			// May be called multiple times if a file

read_on_part_data_end: (null)

read_on_body_end: (null)
```



= Compile New Website Files =

- All files for the webserver will be located in fs folder
- Run the command
./makefsdata 
	- This will take all the files located in the fs folder and compile it to a file fsdata.c

= Turn On Debugging HTTPD =

- In config\lwipopts.h turn on debugging by uncommenting the line
#define LWIP_DEBUG

- Turn on HTTPD debugging
#define HTTPD_DEBUG						LWIP_DBG_ON


---------------------------------------------------------------------------------------

I can give you a generic guideline to process "simply" GET & POST on your 
embedded web server. What you have to do:

1/ On the accepted socket, read datas in a buffer until "\r\n\r\n" (included). 
This part is the "http_headers". It can be useful to add a 0x00 at the end of 
the block to use "string" functions from C runtime, or simply do a "printf".

2/ Extract from the first line (of the "http_headers" got in 1/) the method 
(should be "POST" or "GET"), and the document (could be "/" for the root). The 
method is the first word, the document the next.

3/ If the method (got in 2/) is "GET", copy (or use a pointer) all the datas 
from the first line which are after a possible "?". Note it's possible to not 
have any "?" in a request. At this point, note you can have between 0 to n 
bytes for these datas. I will call this memory block "http_params_datas" in 
next points. 
   
4/ If the method (got in 2/) is "POST", you have first to find in 
"http_headers" a line started by "Content-Length: ". The line have a form like 
"Content-Length: 32\r\n". When you found this line, convert the value of the 
line (32 is this sample) in a integer. This is the "post_data_size". Once you 
got that, read on the socket the next "post_data_size" bytes, and copy them (or 
use a pointer) in the memory block named "http_params_datas".

5/ At this point, you should have a memory block named "http_params_datas" 
(with its size). It can contain datas from GET parameters, or from POST datas. 
Note than GET have a maximum size of 1024 bytes. That's why, most of time, POST 
is used to get "big" datas. POST is also useful if you don't want to "show" on 
the address bar your parameters. The aim is to have a memory block to process 
on next point. It can be useful to add a 0x00 at the end of the block to use 
"string" functions from C runtime, or simply do a "printf".

6/ You can now read param by param your "http_params_datas", you will got each 
parameters from your "form". They are in the same form in GET or POST 
(something like "param1=value1&param2=value2").

That's all. This "framework" will work, but need some checkings. First is to 
not read more bytes than your buffer can contains. Other tips: if you try to do 
a UPLOAD, the processing is a little bit different from 6/, since you can to 
process a "multipart" document, with "boundary" tags to limit the datas (which 
can be binary datas). Look for "RFC1341 - 7.2.1 Multipart: The common syntax" 
for this case.

I can propose you two simple functions to "parse" (a big word in this case) 
http datas. They are basic tools to help to start, bug mainly used the C 
runtime (note they were used on win32, I just change them with lwIP types, so, 
it SHOULD work).

err_t http_getheadervalue( char* lpszHeaderName, char* lpszHeaders, char* 
lpszBuffer, u32_t dwBufferSize)
{ err_t bResult    = ERR_ARG;
  char* lpszHeader = NULL;
  
  memset( lpszBuffer, 0, dwBufferSize);
  lpszHeader = strstr( lpszHeaders, lpszHeaderName);
  if (lpszHeader!=NULL)
   { if (sscanf( lpszHeader+strlen(lpszHeaderName)+1/*:*/+1/*SPACE*/, 
"%[^\r\n]", lpszBuffer)>=0)
      { bResult = ERR_OK;
      }
   }
  return bResult;
}

err_t http_getlinevalue( char* lpszValueName, char* lpszLine, char* lpszBuffer, 
u32_t dwBufferSize)
{ err_t  bResult      = ERR_ARG;
  char* lpszValue     = NULL;
  char  szValue[256]  = "";
  
  memset( lpszBuffer, 0, dwBufferSize);
  memset( szValue,    0, sizeof(szValue));
     
  lpszValue = strstr( lpszLine, lpszValueName);
  if (lpszValue!=NULL)
   { if (sscanf( lpszValue+strlen(lpszValueName)+1/*=*/, "%[^ ;&\r\n]", 
szValue)>=0)
      { /* Note you have to process special escape sequence, or special chars: 
by example, any "+" have to be replace by a " " */
        /* Can you find in your datas some sequence like "%20". You have to 
replace them by the a matching ASCII value. For "%20", its 0x20, so, a "space"
        /* etc...*/
        bResult = ERR_OK
      }
   }
  return bResult;
}
