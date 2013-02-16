#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libimobiledevice/libimobiledevice.h>
#include <libimobiledevice/lockdown.h>
#include <libimobiledevice/installation_proxy.h>

#include "functions.h"


int get_device_id(char *args, char *result)
{
    int i;
    char **dev_list;
	if (idevice_get_device_list(&dev_list, &i) < 0) {
        fprintf(stderr, "ERROR: Unable to retrieve device list!\n");
        return -1;
    }
    strcpy(result, dev_list[0]);
    idevice_device_list_free(dev_list);
    return i;
}

int get_device_info(char *args, char *result)
{

    idevice_t phone = NULL;
    instproxy_client_t instproxy_client = NULL;
    lockdownd_client_t 	lockdownd_client = NULL;
    int ret = 0;
    const char * label="ibrowser";
    plist_t node = NULL;

    ret = idevice_new(&phone, NULL);
    if (ret != IDEVICE_E_SUCCESS) {
        printf("No device found, is it plugged in?\n");
        return -1;
    }

    if (LOCKDOWN_E_SUCCESS != (lockdownd_client_new_with_handshake(phone, &lockdownd_client, label))) {
        idevice_free(phone);
        return -1;
    }

    if(LOCKDOWN_E_SUCCESS != lockdownd_get_value(lockdownd_client, NULL, NULL, &node) ) {
        idevice_free(phone);
        return -1;
    }

    char *xml_doc=NULL;
    uint32_t xml_length;
    plist_to_xml(node, &xml_doc, &xml_length);

    strncpy(result, xml_doc, xml_length);

    plist_free(node);


    return xml_length;
}

int get_app_list(char *args, char *result)
{

    idevice_t phone = NULL;
    instproxy_client_t instproxy_client = NULL;
    lockdownd_client_t 	lockdownd_client = NULL;
    int ret = 0;
    const char * label="ibrowser";
    uint16_t port = 0;
    
    ret = idevice_new(&phone, NULL);
    if (ret != IDEVICE_E_SUCCESS) {
        printf("No device found, is it plugged in?\n");
        return -1;
    }
    
    if (LOCKDOWN_E_SUCCESS != (lockdownd_client_new_with_handshake(phone, &lockdownd_client, label))) {
		idevice_free(phone);
		return -1;
	}
    
    if(LOCKDOWN_E_SUCCESS != (lockdownd_start_service(lockdownd_client,"com.apple.mobile.installation_proxy",&port)))
    {
        printf("lockdownd_start_service error");
        return -1;
    }
    
    if((ret=instproxy_client_new(phone,port,&instproxy_client)) != INSTPROXY_E_SUCCESS)
    {
        printf("instproxy_client_new error");
        return -1;
    }

    plist_t client_opts = instproxy_client_options_new();
    instproxy_client_options_add(client_opts, "ApplicationType", "User", NULL);

    plist_t apps;
    
    if((ret=instproxy_browse(instproxy_client,client_opts,&apps)) != INSTPROXY_E_SUCCESS)
    {
        printf("instproxy_browse error");
        return -1;
    }

    char *xml_doc=NULL;
	uint32_t xml_length;
    plist_to_xml(apps, &xml_doc, &xml_length);

    strncpy(result, xml_doc, xml_length);

    plist_free(apps);


    return xml_length;
}
