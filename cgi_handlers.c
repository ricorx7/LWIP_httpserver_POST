/*
 * cgi_handlers.c
 *
 * Created: 2/14/2018 10:42:23 AM
 *  Author: rico
 */ 
#include "httpd.h"

/*
 * Create all CGI Handlers to handle CGI calls.
 */
/* Cgi call table, only one CGI used */
tCGI CGI_TAB[2];


/* CGI handler for Upload control */
const char * Upload_test_CGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);

/* Html request for "/cgi/upload.cgi" will start Upload_test_CGI_Handler */
const tCGI UPLOAD_TEST_CGI={"/cgi/upload.cgi", Upload_test_CGI_Handler};

/* CGI handler for Vars control */
const char * Vars_test_CGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);

/* Html request for "/cgi/vars.cgi" will start Vars_test_CGI_Handler */
const tCGI VARS_TEST_CGI={"/cgi/vars.cgi", Vars_test_CGI_Handler};


/**
  * @brief  CGI handler for Uploading control 
  * GET CGI Example.
  * https://github.com/withrobot/myCortex-STM32F4/blob/master/ex14.1_ETH_webserver/httpd_cgi_ssi.c
  */
const char * Upload_test_CGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
	LWIP_DEBUGF(HTTPD_DEBUG, ("Upload_test_CGI_Handler, iIndex=%s", iIndex));
	LWIP_DEBUGF(HTTPD_DEBUG, ("Upload_test_CGI_Handler, iIndex=%s", iNumParams));
	int i = 0;
	for (i=0; i<iNumParams; i++)
	{
		LWIP_DEBUGF(HTTPD_DEBUG, ("Upload_test_CGI_Handler, param=%s", pcParam[i]));
		LWIP_DEBUGF(HTTPD_DEBUG, ("Upload_test_CGI_Handler, value=%s", pcValue[i]));
	}

	return "/index.html";
}

const char * Vars_test_CGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
	// Do Nothing
	
	return "/index.html";
}


void cgi_handlers_init()
{
  // Initialize the CGI
  CGI_TAB[0] = UPLOAD_TEST_CGI;							// Add the CGI Handler to the array
  http_set_cgi_handlers(CGI_TAB, 2);					// Register the CGI handler
  
  CGI_TAB[1] = VARS_TEST_CGI;							// Add the CGI Handler to the array
  http_set_cgi_handlers(CGI_TAB, 2);					// Register the CGI handler
}