/*
* If not stated otherwise in this file or this component's LICENSE file the
* following copyright and licenses apply:*
* Copyright 2023 RDK Management
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

/**
* @file test_vlan_eth_hal.c
* @page vlan_eth_hal Level 1 Tests
*
* ## Module's Role
* This module includes Level 1 functional tests (success and failure scenarios).
* This is to ensure that the vlan_eth_hal APIs meet the requirements across all vendors.
*
* **Pre-Conditions:**  None@n
* **Dependencies:** None@n
*
* Ref to API Definition specification documentation : [halSpec.md](../../../docs/halSpec.md)
*/

#include <ut.h>
#include <ut_log.h>
#include "vlan_eth_hal.h"
#include "cJSON.h"

char L2Name[64] = {0};
char L3Name[64] = {0};
char alias[32] = {0};
char vlanName[64] = {0};


/**function to read the json config file and return its content as a string
*IN : json file name
*OUT : content of json file as string
**/
static char* read_file(const char *filename)
{
    FILE *file = NULL;
    long length = 0;
    char *content = NULL;
    size_t read_chars = 0;
    UT_LOG("Entered the read file");
    /* open in read binary mode */
    file = fopen(filename, "rb");
    if (file != NULL)
    {
        /* get the length */
        if (fseek(file, 0, SEEK_END) == 0)
        {
            length = ftell(file);
            if (length > 0)
            {
                if (fseek(file, 0, SEEK_SET) == 0)
                {
                    /* allocate content buffer */
                    content = (char*)malloc((size_t)length + sizeof(""));
                    if (content != NULL)
                    {
                        /* read the file into memory */
                        read_chars = fread(content, sizeof(char), (size_t)length, file);
                        if ((long)read_chars != length)
                        {
                            free(content);
                            content = NULL;
                        }
                        else
                            content[read_chars] = '\0';
                    }
                }
            }
        }
    }
    UT_LOG("Entered the read file");
    fclose(file);
    return content;
}

/**function to read the json config file and return its content as a json object
*IN : json file name
*OUT : content of json file as a json object
**/
static cJSON *parse_file(const char *filename)
{
    cJSON *parsed = NULL;
    UT_LOG("Entered  parse_file");
    char *content = read_file(filename);
    parsed = cJSON_Parse(content);
    UT_LOG("After cJSON_Parse called");
    if(content != NULL)
    {
        free(content);
    }
    UT_LOG("Existing  parse_file");
    return parsed;
}

/* get the L2Name from configuration file */
int get_L2Name(void)
{
    char configFile[] =  "./vlan_eth_config";
    cJSON *value = NULL;
    cJSON *json = NULL;
    UT_LOG("Checking L2Name");
    json = parse_file(configFile);
    if(json == NULL)
    {
        printf("Failed to parse config\n");
        return -1;
    }
    value = cJSON_GetObjectItem(json, "L2Name");
    // null check and object is string, value->valuestring
    if((value != NULL) && (cJSON_IsString(value)))
    {
       strcpy(L2Name, value->valuestring);
    }
    UT_LOG("L2Name from config file is : %s",L2Name);
    return 0;
}

/* get the L3Name from configuration file */
int get_L3Name(void)
{
    char configFile[] =  "./vlan_eth_config";
    cJSON *value = NULL;
    cJSON *json = NULL;
    UT_LOG("Checking L3Name");
    json = parse_file(configFile);
    if(json == NULL)
    {
        printf("Failed to parse config\n");
        return -1;
    }
    value = cJSON_GetObjectItem(json, "L3Name");
    // null check and object is string, value->valuestring
    if((value != NULL) && (cJSON_IsString(value)))
    {
       strcpy(L3Name, value->valuestring);
    }
    UT_LOG("L3Name from config file is : %s",L3Name);
    return 0;
}

/* get the alias from configuration file */
int get_alias(void)
{
    char configFile[] =  "./vlan_eth_config";
    cJSON *value = NULL;
    cJSON *json = NULL;
    UT_LOG("Checking alias");
    json = parse_file(configFile);
    if(json == NULL)
    {
        printf("Failed to parse config\n");
        return -1;
    }
    value = cJSON_GetObjectItem(json, "alias");
    // null check and object is string, value->valuestring
    if((value != NULL) && (cJSON_IsString(value)))
    {
       strcpy(alias, value->valuestring);
    }
    UT_LOG("alias from config file is : %s",alias);
    return 0;
}

/* get the vlanName from configuration file */
int get_vlanName(void)
{
    char configFile[] =  "./vlan_eth_config";
    cJSON *value = NULL;
    cJSON *json = NULL;
    UT_LOG("Checking vlanName");
    json = parse_file(configFile);
    if(json == NULL)
    {
        printf("Failed to parse config\n");
        return -1;
    }
    value = cJSON_GetObjectItem(json, "vlanName");
    // null check and object is string, value->valuestring
    if((value != NULL) && (cJSON_IsString(value)))
    {
       strcpy(vlanName, value->valuestring);
    }
    UT_LOG("vlanName from config file is : %s",vlanName);
    return 0;
}

/**
* @brief Test case for initializing the VLAN Ethernet HAL module.
*
* This test case verifies the functionality of the vlan_eth_hal_init() API by initializing the VLAN Ethernet HAL module and checking the return value.
*
* **Test Group ID:** Basic: 01 @n
* **Test Case ID:** 001 @n
* **Priority:** High @n@n
*
* **Pre-Conditions:** None @n
* **Dependencies:** None @n
* **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
*
* **Test Procedure:** @n
* | Variation / Step | Description | Test Data | Expected Result  | Notes |
* | :----------: | ---------- | --------- | ----------- | --------- |
* | 01 | Invoke vlan_eth_hal_init which initializes VLAN Ethernet HAL module |None | RETURN_OK | Should be successful |
*/
void test_l1_vlan_eth_hal_positive1_vlan_eth_hal_init(void)
{
    UT_LOG("Entering test_l1_vlan_eth_hal_positive1_vlan_eth_hal_init...");
    int result = 0;

    UT_LOG("Invoking vlan_eth_hal_init");
    result = vlan_eth_hal_init();
    UT_LOG("vlan_eth_hal_init returns : %d", result);
    UT_ASSERT_EQUAL(result, RETURN_OK);

    UT_LOG("Exiting test_l1_vlan_eth_hal_positive1_vlan_eth_hal_init...");
}

/**
* @brief This test case is used to verify the functionality when API is called twice.
*
* The objective of this test is to ensure that the function `vlan_eth_hal_init()` called twice returns success.
*
* **Test Group ID:** Basic: 01 @n
* **Test Case ID:** 002 @n
* **Priority:** High @n@n
*
* **Pre-Conditions:** None @n
* **Dependencies:** None @n
* **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
*
* **Test Procedure:** @n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :----: | --------- | ---------- |-------------- | ----- |
* | 01 | Invoking vlan_eth_hal_init API twice | None | RETURN_OK | Should be successful |
*/
void test_l1_vlan_eth_hal_positive2_vlan_eth_hal_init(void)
{
    UT_LOG("Entering test_l1_vlan_eth_hal_positive2_vlan_eth_hal_init...");
    int result = 0;

    UT_LOG("Invoking vlan_eth_hal_init API");
    result = vlan_eth_hal_init();
    UT_LOG(" vlan_eth_hal_init returns: %d",result);
    UT_ASSERT_EQUAL(result, RETURN_OK);

    UT_LOG("Invoking vlan_eth_hal_init API again");
    result = vlan_eth_hal_init();
    UT_LOG(" vlan_eth_hal_init returns: %d",result);
    UT_ASSERT_EQUAL(result, RETURN_OK);

    UT_LOG("Exiting test_l1_vlan_eth_hal_positive2_vlan_eth_hal_init...");
}

/**
* @brief Test the function vlan_eth_hal_configureInterface for positive scenarios
*
* This test case is used to verify the functionality of the vlan_eth_hal_configureInterface function when called valid configuration values with doReconfigure as FALSE.
*
* **Test Group ID:** Basic: 01 @n
* **Test Case ID:** 003 @n
* **Priority:** High @n@n
*
* **Pre-Conditions:** None @n
* **Dependencies:** None @n
* **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
*
* **Test Procedure:** @n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :----: | --------- | ---------- |-------------- | ----- |
* | 01 |Invoke vlan_eth_hal_configureInterface with doReconfigure as FALSE | doReconfigure = FALSE | RETURN_OK | Should be successful |
*/
void test_l1_vlan_eth_hal_positive1_vlan_eth_hal_configureInterface(void)
{
    UT_LOG("Entering test_l1_vlan_eth_hal_positive1_vlan_eth_hal_configureInterface...");
    int result = 0;
    vlan_configuration_t *config = (vlan_configuration_t *)malloc(sizeof(vlan_configuration_t));
    if (config != NULL)
    {
        memset(config, 0, sizeof(vlan_configuration_t));
        //L2Name should be configured in vlan_eth_config file
        strcpy(config->L2Interface, L2Name);
        //L3Name should be configured in vlan_eth_config file
        strcpy(config->L3Interface, L3Name);
        config->VLANId = 1;
        config->TPId = 0;
        config->Status = VLAN_IF_UP;
        config->skbMarkingNumOfEntries = 1;
        config->skb_config = (vlan_skb_config_t *)malloc(sizeof(vlan_skb_config_t));
        if (config->skb_config != NULL)
        {
            memset(config->skb_config, 0, sizeof(vlan_skb_config_t));
            //alias should be configured in vlan_eth_config file
            strcpy(config->skb_config->alias, alias);
            config->skb_config->skbMark = 0;
            config->skb_config->skbPort = 0;
            config->skb_config->skbEthPriorityMark = 0;

            config->doReconfigure = FALSE;

            UT_LOG("Invoking vlan_eth_hal_configureInterface with valid configuration of doReconfigure = FALSE");
            result = vlan_eth_hal_configureInterface(config);
            UT_LOG(" vlan_eth_hal_configureInterface returns: %d",result);
            UT_ASSERT_EQUAL(result, RETURN_OK);

            free(config->skb_config);
            config->skb_config = NULL;
            free(config);
            config = NULL;
        }
        else
        {
            UT_LOG("Malloc operation failed for skb_config");
            UT_FAIL("Memory allocation with malloc failed for skb_config");
        }
        free(config);
        config = NULL;

    }
    else
    {
        UT_LOG("Malloc operation failed");
        UT_FAIL("Memory allocation with malloc failed");
    }
    UT_LOG("Exiting test_l1_vlan_eth_hal_positive1_vlan_eth_hal_configureInterface...");
}

/**
* @brief This function tests the positive case of the vlan_eth_hal_configureInterface function.
*
* This function tests whether the vlan_eth_hal_configureInterface function works correctly in the positive case. It sets up the vlan_configuration_t structure with doReconfigure as TRUE and then invokes the function with the configuration. The function asserts that the return value of the function is RETURN_OK.
*
* **Test Group ID:** Basic: 01 @n
* **Test Case ID:** 004 @n
* **Priority:** High @n@n
*
* **Pre-Conditions:** None @n
* **Dependencies:** None @n
* **User Interaction:** If the user chose to run the test in interactive mode, then the test case has to be selected via console. @n
*
* **Test Procedure:** @n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :----: | --------- | ---------- |-------------- | ----- |
* | 01 | Invoke vlan_eth_hal_configureInterface with doReconfigure as TRUE | doReconfigure = TRUE | RETURN_OK | Should be successful |
*/
void test_l1_vlan_eth_hal_positive2_vlan_eth_hal_configureInterface(void)
{
    UT_LOG("Entering test_l1_vlan_eth_hal_positive2_vlan_eth_hal_configureInterface...");
    int result = 0;
    vlan_configuration_t *config = (vlan_configuration_t *)malloc(sizeof(vlan_configuration_t));
    if (config != NULL)
    {
        memset(config, 0, sizeof(vlan_configuration_t));
        //L2Name should be configured in vlan_eth_config file
        strcpy(config->L2Interface, L2Name);
        //L3Name should be configured in vlan_eth_config file
        strcpy(config->L3Interface, L3Name);
        config->VLANId = 1;
        config->TPId = 0;
        config->Status = VLAN_IF_UP;
        config->skbMarkingNumOfEntries = 1;
        config->skb_config = (vlan_skb_config_t *)malloc(sizeof(vlan_skb_config_t));
        if (config->skb_config != NULL)
        {
            memset(config->skb_config, 0, sizeof(vlan_skb_config_t));
            //alias should be configured in vlan_eth_config file
            strcpy(config->skb_config->alias, alias);
            config->skb_config->skbMark = 0;
            config->skb_config->skbPort = 0;
            config->skb_config->skbEthPriorityMark = 0;

            config->doReconfigure = TRUE;
            UT_LOG("Invoking vlan_eth_hal_configureInterface with valid configuration of doReconfigure = TRUE");
            result = vlan_eth_hal_configureInterface(config);
            UT_LOG(" vlan_eth_hal_configureInterface returns: %d",result);
            UT_ASSERT_EQUAL(result, RETURN_OK);

            free(config->skb_config);
            config->skb_config = NULL;
            free(config);
            config = NULL;
        }
        else
        {
            UT_LOG("Malloc operation failed for skb_config");
            UT_FAIL("Memory allocation with malloc failed for skb_config");
        }
        free(config);
        config = NULL;
    }
    else
    {
        UT_LOG("Malloc operation failed");
        UT_FAIL("Memory allocation with malloc failed");
    }

    UT_LOG("Exiting test_l1_vlan_eth_hal_positive2_vlan_eth_hal_configureInterface...");
}

/**
* @brief Test case to verify the functionality of vlan_eth_hal_configureInterface function
*
* This test case is used to verify the functionality of vlan_eth_hal_configureInterface function by invoking it with with existing configuration.
*
* **Test Group ID:** Basic: 01 @n
* **Test Case ID:** 005 @n
* **Priority:** High @n@n
*
* **Pre-Conditions:** None @n
* **Dependencies:** None @n
* **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
*
* **Test Procedure:** @n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :----: | --------- | ---------- |-------------- | ----- |
* | 01 | Invoke vlan_eth_hal_configureInterface API twice| config = valid configuration pointer | RETURN_OK | Should be successful |
*/
void test_l1_vlan_eth_hal_positive3_vlan_eth_hal_configureInterface(void)
{
    UT_LOG("Entering test_l1_vlan_eth_hal_positive3_vlan_eth_hal_configureInterface...");
    int result = 0;
    vlan_configuration_t *config = (vlan_configuration_t *)malloc(sizeof(vlan_configuration_t));
    if (config != NULL)
    {
        memset(config, 0, sizeof(vlan_configuration_t));
        //L2Name should be configured in vlan_eth_config file
        strcpy(config->L2Interface, L2Name);
        //L3Name should be configured in vlan_eth_config file
        strcpy(config->L3Interface, L3Name);
        config->VLANId = 1;
        config->TPId = 0;
        config->Status = VLAN_IF_UP;
        config->skb_config = (vlan_skb_config_t *)malloc(sizeof(vlan_skb_config_t));
        if (config->skb_config != NULL)
        {
            memset(config->skb_config, 0, sizeof(vlan_skb_config_t));
            //alias should be configured in vlan_eth_config file
            strcpy(config->skb_config->alias, alias);
            config->skb_config->skbMark = 0;
            config->skb_config->skbPort = 0;
            config->skb_config->skbEthPriorityMark = 0;
            config->doReconfigure = TRUE;

            UT_LOG("Invoking vlan_eth_hal_configureInterface with valid configuration ");
            result = vlan_eth_hal_configureInterface(config);
            UT_LOG(" vlan_eth_hal_configureInterface returns: %d",result);
            UT_ASSERT_EQUAL(result, RETURN_OK);

            UT_LOG("Invoking vlan_eth_hal_configureInterface with existing configuration ");
            result = vlan_eth_hal_configureInterface(config);
            UT_LOG(" vlan_eth_hal_configureInterface returns: %d",result);
            UT_ASSERT_EQUAL(result, RETURN_OK);

            free(config->skb_config);
            config->skb_config = NULL;
            free(config);
            config = NULL;
        }
        else
        {
            UT_LOG("Malloc operation failed for skb_config");
            UT_FAIL("Memory allocation with malloc failed for skb_config");
        }
        free(config);
        config = NULL;
    }
    else
    {
        UT_LOG("Malloc operation failed");
        UT_FAIL("Memory allocation with malloc failed");
    }

    UT_LOG("Exiting test_l1_vlan_eth_hal_positive3_vlan_eth_hal_configureInterface...");
}

/**
* @brief This test case verifies the functionality of the vlan_eth_hal_configureInterface function.
*
* The objective of this test is to ensure that the vlan_eth_hal_configureInterface function works correctly by configuring the VLAN interface with minimum values of parameters.
*
* **Test Group ID:** Basic: 01 @n
* **Test Case ID:** 006 @n
* **Priority:** High @n@n
*
* **Pre-Conditions:** None @n
* **Dependencies:** None @n
* **User Interaction:** If the user chooses to run the test in interactive mode, then the test case has to be selected via the console. @n
*
* **Test Procedure:** @n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :----: | --------- | ---------- |-------------- | ----- |
* | 01 | Invoke vlan_eth_hal_configureInterface with valid minimum value of parameters| config = valid configuration pointer | RETURN_OK |Should be successful|
*/
void test_l1_vlan_eth_hal_positive4_vlan_eth_hal_configureInterface(void)
{
    UT_LOG("Entering test_l1_vlan_eth_hal_positive4_vlan_eth_hal_configureInterface...");
    int result = 0;
    vlan_configuration_t *config = (vlan_configuration_t *)malloc(sizeof(vlan_configuration_t));
    if (config != NULL)
    {
        memset(config, 0, sizeof(vlan_configuration_t));
        //L2Name should be configured in vlan_eth_config file
        strcpy(config->L2Interface, L2Name);
        //L3Name should be configured in vlan_eth_config file
        strcpy(config->L3Interface, L3Name);
        config->VLANId = 1;
        config->TPId = 0;
        config->Status = VLAN_IF_UP;
        config->skbMarkingNumOfEntries = 1;
        config->doReconfigure = TRUE;
        config->skb_config = (vlan_skb_config_t *)malloc(sizeof(vlan_skb_config_t));
        if (config->skb_config != NULL)
        {
            memset(config->skb_config, 0, sizeof(vlan_skb_config_t));
            //alias should be configured in vlan_eth_config file
            strcpy(config->skb_config->alias, alias);
            config->skb_config->skbMark = 0;
            config->skb_config->skbPort = 0;
            config->skb_config->skbEthPriorityMark = 0;

            UT_LOG("Invoking vlan_eth_hal_configureInterface with valid minimum value of parameters");
            result = vlan_eth_hal_configureInterface(config);
            UT_LOG(" vlan_eth_hal_configureInterface returns: %d",result);
            UT_ASSERT_EQUAL(result, RETURN_OK);

            free(config->skb_config);
            config->skb_config = NULL;
            free(config);
            config = NULL;
        }
        else
        {
            UT_LOG("Malloc operation failed for skb_config (min value)");
            UT_FAIL("Memory allocation with malloc failed for skb_config (min value)");
        }
        free(config);
        config = NULL;
    }
    else
    {
        UT_LOG("Malloc operation failed");
        UT_FAIL("Memory allocation with malloc failed");
    }

    UT_LOG("Exiting test_l1_vlan_eth_hal_positive4_vlan_eth_hal_configureInterface...");
}

/**
* @brief This test case verifies the functionality of the vlan_eth_hal_configureInterface function.
*
* The objective of this test is to ensure that the vlan_eth_hal_configureInterface function works correctly by configuring the VLAN interface with maximum values of parameters.
*
* **Test Group ID:** Basic: 01 @n
* **Test Case ID:** 007 @n
* **Priority:** High @n@n
*
* **Pre-Conditions:** None @n
* **Dependencies:** None @n
* **User Interaction:** If the user chooses to run the test in interactive mode, then the test case has to be selected via the console. @n
*
* **Test Procedure:** @n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :----: | --------- | ---------- |-------------- | ----- |
* | 01 | Invoke vlan_eth_hal_configureInterface with valid maximum value of parameters| config = valid configuration pointer | RETURN_OK |Should be successful |
*/
void test_l1_vlan_eth_hal_positive5_vlan_eth_hal_configureInterface(void)
{
    UT_LOG("Entering test_l1_vlan_eth_hal_positive5_vlan_eth_hal_configureInterface...");
    int result = 0;
    vlan_configuration_t *config = (vlan_configuration_t *)malloc(sizeof(vlan_configuration_t));
    if (config != NULL)
    {
        memset(config, 0, sizeof(vlan_configuration_t));
        //L2Name should be configured in vlan_eth_config file
        strcpy(config->L2Interface, L2Name);
        //L3Name should be configured in vlan_eth_config file
        strcpy(config->L3Interface, L3Name);
        config->VLANId = 4094;
        config->TPId = 4294967295;
        config->Status = VLAN_IF_UP;
        config->skbMarkingNumOfEntries = 1;
        config->skb_config = (vlan_skb_config_t *)malloc(sizeof(vlan_skb_config_t));
        if (config->skb_config != NULL)
        {
            memset(config->skb_config, 0, sizeof(vlan_skb_config_t));
            //alias should be configured in vlan_eth_config file
            strcpy(config->skb_config->alias, alias);
            config->skb_config->skbMark = 4294967295;
            config->skb_config->skbPort = 4294967295;
            config->skb_config->skbEthPriorityMark = 4294967295;

            config->doReconfigure = TRUE;
            UT_LOG("Invoking vlan_eth_hal_configureInterface with valid maximum value of parameters");
            result = vlan_eth_hal_configureInterface(config);
            UT_LOG(" vlan_eth_hal_configureInterface returns: %d",result);
            UT_ASSERT_EQUAL(result, RETURN_OK);

            free(config->skb_config);
            config->skb_config = NULL;
            free(config);
            config = NULL;
        }
        else
        {
            UT_LOG("Malloc operation failed for skb_config (max value)");
            UT_FAIL("Memory allocation with malloc failed for skb_config (max value)");
        }
        free(config);
        config = NULL;
    }
    else
    {
        UT_LOG("Malloc operation failed");
        UT_FAIL("Memory allocation with malloc failed");
    }

    UT_LOG("Exiting test_l1_vlan_eth_hal_positive5_vlan_eth_hal_configureInterface...");
}


/**
* @brief Test case to verify the positive scenario of vlan_eth_hal_configureInterface function.
*
* This test case validates the functionality of the vlan_eth_hal_configureInterface function when provided with different configuration status values.
*
* **Test Group ID:** Basic: 01 @n
* **Test Case ID:** 008 @n
* **Priority:** High @n@n
*
* **Pre-Conditions:** None @n
* **Dependencies:** None @n
* **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
*
* **Test Procedure:** @n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :----: | --------- | ---------- |-------------- | ----- |
* | 01 |Invoke vlan_eth_hal_configureInterface with different status values. | config = valid configuration pointer with config->Status ranging from 1 to 7| RETURN_OK | Should be successful |
*/
void test_l1_vlan_eth_hal_positive6_vlan_eth_hal_configureInterface(void)
{
    UT_LOG("Entering test_l1_vlan_eth_hal_positive6_vlan_eth_hal_configureInterface...");
    int i = 0 ;
    int result = 0;
    vlan_configuration_t *config = (vlan_configuration_t *)malloc(sizeof(vlan_configuration_t));
    if (config != NULL)
    {
        memset(config, 0, sizeof(vlan_configuration_t));
        //L2Name should be configured in vlan_eth_config file
        strcpy(config->L2Interface, L2Name);
        //L3Name should be configured in vlan_eth_config file
        strcpy(config->L3Interface, L3Name);
        config->VLANId = 1;
        config->TPId = 0;
        config->skbMarkingNumOfEntries = 1;
        config->skb_config = (vlan_skb_config_t *)malloc(sizeof(vlan_skb_config_t));
        if (config->skb_config != NULL)
        {
            memset(config->skb_config, 0, sizeof(vlan_skb_config_t));
            //alias should be configured in vlan_eth_config file
            strcpy(config->skb_config->alias, alias);
            config->skb_config->skbMark = 0;
            config->skb_config->skbPort = 0;
            config->skb_config->skbEthPriorityMark = 0;
            config->doReconfigure = TRUE;
            for(i = 1 ; i <= 7 ; i++)
            {
                config->Status = 1;
                UT_LOG("Invoking vlan_eth_hal_configureInterface with valid configuration ");
                result = vlan_eth_hal_configureInterface(config);
                UT_LOG(" vlan_eth_hal_configureInterface returns: %d",result);
                UT_ASSERT_EQUAL(result, RETURN_OK);
            }
            free(config->skb_config);
            config->skb_config = NULL;
            free(config);
            config = NULL;
        }
        else
        {
            UT_LOG("Malloc operation failed for skb_config");
            UT_FAIL("Memory allocation with malloc failed for skb_config");
        }
        free(config);
        config = NULL;
    }
    else
    {
        UT_LOG("Malloc operation failed");
        UT_FAIL("Memory allocation with malloc failed");
    }
    UT_LOG("Exiting test_l1_vlan_eth_hal_positive6_vlan_eth_hal_configureInterface...");
}

/**
* @brief This test case tests the function vlan_eth_hal_configureInterface in the L1 VLAN Ethernet HAL module.
*
* This test case verifies the functionality of the vlan_eth_hal_configureInterface function by invoking it with valid configuration values. It checks if the function returns RETURN_OK as expected.
*
* **Test Group ID:** Basic: 01 @n
* **Test Case ID:** 009 @n
* **Priority:** High @n@n
*
* **Pre-Conditions:** None @n
* **Dependencies:** None @n
* **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console. @n
*
* **Test Procedure:** @n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :----: | --------- | ---------- |-------------- | ----- |
* | 01 | Invoke vlan_eth_hal_configureInterface to test SKB Configuration | config = valid configuration values with multiple valid SKB Configurations| RETURN_OK | Should be successful |
*/
void test_l1_vlan_eth_hal_positive7_vlan_eth_hal_configureInterface(void)
{
    UT_LOG("Entering test_l1_vlan_eth_hal_positive7_vlan_eth_hal_configureInterface...");
    int result = 0;
    vlan_configuration_t *config = (vlan_configuration_t *)malloc(sizeof(vlan_configuration_t));
    if (config != NULL)
    {
        memset(config, 0, sizeof(vlan_configuration_t));
        //L2Name should be configured in vlan_eth_config file
        strcpy(config->L2Interface, L2Name);
        //L3Name should be configured in vlan_eth_config file
        strcpy(config->L3Interface, L3Name);
        config->VLANId = 1;
        config->TPId = 0;
        config->Status = VLAN_IF_UP;
        config->skbMarkingNumOfEntries = 2;
        config->skb_config = (vlan_skb_config_t *)malloc(sizeof(vlan_skb_config_t)* config->skbMarkingNumOfEntries);
        if (config->skb_config != NULL)
        {
            memset(config->skb_config, 0, sizeof(vlan_skb_config_t));
            //alias should be configured in vlan_eth_config file
            strcpy(config->skb_config[0].alias, alias);
            config->skb_config[0].skbMark = 0;
            config->skb_config[0].skbPort = 0;
            config->skb_config[0].skbEthPriorityMark = 0;

            //alias should be configured in vlan_eth_config file
            strcpy(config->skb_config[1].alias, alias);
            config->skb_config[1].skbMark = 1;
            config->skb_config[1].skbPort = 1;
            config->skb_config[1].skbEthPriorityMark = 1;
            config->doReconfigure = TRUE;

            UT_LOG("Invoking vlan_eth_hal_configureInterface to test SKB Configuration ");
            result = vlan_eth_hal_configureInterface(config);
            UT_LOG(" vlan_eth_hal_configureInterface returns: %d",result);
            UT_ASSERT_EQUAL(result, RETURN_OK);

            free(config->skb_config);
            config->skb_config = NULL;
            free(config);
            config = NULL;
        }
        else
        {
            UT_LOG("Malloc operation failed for skb_config");
            UT_FAIL("Memory allocation with malloc failed for skb_config");
        }
        free(config);
        config = NULL;
    }
    else
    {
        UT_LOG("Malloc operation failed");
        UT_FAIL("Memory allocation with malloc failed");
    }

    UT_LOG("Exiting test_l1_vlan_eth_hal_positive7_vlan_eth_hal_configureInterface...");
}

/**
* @brief This test case is used to verify the functionality of the VLAN_ETH_HAL_configureInterface function when invoked with a NULL pointer for config.
*
* The objective of this test is to ensure that the function returns the expected error code when invoked with a NULL pointer.
*
* **Test Group ID:** Basic: 01 @n
* **Test Case ID:** 010 @n
* **Priority:** High @n@n
*
* **Pre-Conditions:** None @n
* **Dependencies:** None @n
* **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
*
* **Test Procedure:** @n
*  | Variation / Step | Description | Test Data | Expected Result | Notes |
*  | :----: | --------- | ---------- | -------------- | ----- |
*  | 01 | Invoke vlan_eth_hal_configureInterface with a NULL config pointer | config = NULL | RETURN_ERR | Should be unsuccessful |
*/
void test_l1_vlan_eth_hal_negative1_vlan_eth_hal_configureInterface(void)
{
    UT_LOG("Entering test_l1_vlan_eth_hal_negative1_vlan_eth_hal_configureInterface...");
    int result = 0;
    vlan_configuration_t *config = NULL;

    UT_LOG("Invoking vlan_eth_hal_configureInterface a NULL pointer for config ");
    result = vlan_eth_hal_configureInterface(config);
    UT_LOG("vlan_eth_hal_configureInterface returns : %d", result);
    UT_ASSERT_EQUAL(result, RETURN_ERR);

    UT_LOG("Exiting test_l1_vlan_eth_hal_negative1_vlan_eth_hal_configureInterface...");
}

/**
* @brief This test case verifies the behavior of the vlan_eth_hal_configureInterface function when configured with an invalid VLAN ID.
*
* The purpose of this test is to verify that the function correctly handles an invalid VLAN ID and returns an error code.
*
* **Test Group ID:** Basic: 01 @n
* **Test Case ID:** 011 @n
* **Priority:** High @n@n
*
* **Pre-Conditions:** None @n
* **Dependencies:** None @n
* **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
*
* **Test Procedure:** @n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :----: | --------- | ---------- |-------------- | ----- |
* | 01 | Invoke vlan_eth_hal_configureInterface with invalid VLAN ID | config->vlanID = invalid value| RETURN_ERR | Should return an error |
*/
void test_l1_vlan_eth_hal_negative2_vlan_eth_hal_configureInterface(void)
{
    UT_LOG("Entering test_l1_vlan_eth_hal_negative2_vlan_eth_hal_configureInterface...");
    int result = 0;
    vlan_configuration_t *config = (vlan_configuration_t *)malloc(sizeof(vlan_configuration_t));
    if (config != NULL)
    {
        memset(config, 0, sizeof(vlan_configuration_t));
        //L2Name should be configured in vlan_eth_config file
        strcpy(config->L2Interface, L2Name);
        //L3Name should be configured in vlan_eth_config file
        strcpy(config->L3Interface, L3Name);
        config->VLANId = 4095;
        config->TPId = 0;
        config->Status = VLAN_IF_UP;
        config->skbMarkingNumOfEntries = 1;
        config->skb_config = (vlan_skb_config_t *)malloc(sizeof(vlan_skb_config_t));
        if (config->skb_config != NULL)
        {
            memset(config->skb_config, 0, sizeof(vlan_skb_config_t));
            //alias should be configured in vlan_eth_config file
            strcpy(config->skb_config->alias, alias);
            config->skb_config->skbMark = 0;
            config->skb_config->skbPort = 0;
            config->skb_config->skbEthPriorityMark = 0;

            config->doReconfigure = TRUE;
            UT_LOG("Invoking vlan_eth_hal_configureInterface with invalid VLAN ID ");
            result = vlan_eth_hal_configureInterface(config);
            UT_LOG(" vlan_eth_hal_configureInterface returns: %d",result);
            UT_ASSERT_EQUAL(result, RETURN_ERR);

            free(config->skb_config);
            config->skb_config = NULL;
            free(config);
            config = NULL;
        }
        else
        {
            UT_LOG("Malloc operation failed for skb_config");
            UT_FAIL("Memory allocation with malloc failed for skb_config");
        }
        free(config);
        config = NULL;
    }
    else
    {
        UT_LOG("Malloc operation failed");
        UT_FAIL("Memory allocation with malloc failed");
    }

    UT_LOG("Exiting test_l1_vlan_eth_hal_negative2_vlan_eth_hal_configureInterface...");
}

/**
* @brief This function tests the negative scenario of the vlan_eth_hal_configureInterface API.
*
* This test case verifies that the API returns an error when configuring the interface with an invalid status value.
*
* **Test Group ID:** Basic: 01 @n
* **Test Case ID:** 012 @n
* **Priority:** High @n@n
*
* **Pre-Conditions:** None @n
* **Dependencies:** None @n
* **User Interaction:** If the user chooses to run the test in interactive mode, then the test case has to be selected via console. @n
*
* **Test Procedure:** @n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :----: | --------- | ---------- | -------------- | ----- |
* | 01 | Invoke vlan_eth_hal_configureInterface with invalid status | config->Status = invalid value | RETURN_ERR | Should return an error |
*/
void test_l1_vlan_eth_hal_negative3_vlan_eth_hal_configureInterface(void)
{
    UT_LOG("Entering test_l1_vlan_eth_hal_negative3_vlan_eth_hal_configureInterface...");
    int result = 0;
    vlan_configuration_t *config = (vlan_configuration_t *)malloc(sizeof(vlan_configuration_t));
    if (config != NULL)
    {
        memset(config, 0, sizeof(vlan_configuration_t));
        //L2Name should be configured in vlan_eth_config file
        strcpy(config->L2Interface, L2Name);
        //L3Name should be configured in vlan_eth_config file
        strcpy(config->L3Interface, L3Name);
        config->VLANId = 1;
        config->TPId = 0;
        config->Status = 8;
        config->skbMarkingNumOfEntries = 1;
        config->skb_config = (vlan_skb_config_t *)malloc(sizeof(vlan_skb_config_t));
        if (config->skb_config != NULL)
        {
            memset(config->skb_config, 0, sizeof(vlan_skb_config_t));
            //alias should be configured in vlan_eth_config file
            strcpy(config->skb_config->alias, alias);
            config->skb_config->skbMark = 0;
            config->skb_config->skbPort = 0;
            config->skb_config->skbEthPriorityMark = 0;

            config->doReconfigure = TRUE;
            UT_LOG("Invoking vlan_eth_hal_configureInterface with invalid status");
            result = vlan_eth_hal_configureInterface(config);
            UT_LOG(" vlan_eth_hal_configureInterface returns: %d",result);
            UT_ASSERT_EQUAL(result, RETURN_ERR);

            free(config->skb_config);
            config->skb_config = NULL;
            free(config);
            config = NULL;
        }
        else
        {
            UT_LOG("Malloc operation failed for skb_config");
            UT_FAIL("Memory allocation with malloc failed for skb_config");
        }
        free(config);
        config = NULL;
    }
    else
    {
        UT_LOG("Malloc operation failed");
        UT_FAIL("Memory allocation with malloc failed");
    }

    UT_LOG("Exiting test_l1_vlan_eth_hal_negative3_vlan_eth_hal_configureInterface...");
}

/**
* @brief This test case tests the negative scenario of the function vlan_eth_hal_configureInterface.
*
* This test case checks if the function returns an error when attempting to configure the VLAN interface with empty string values for L2Interface and L3Interface.
*
* **Test Group ID:** Basic: 01 @n
* **Test Case ID:** 013 @n
* **Priority:** High @n@n
*
* **Pre-Conditions:** None @n
* **Dependencies:** None @n
* **User Interaction:** If the user chooses to run the test in interactive mode, they have to select this test case via the console @n
*
* **Test Procedure:** @n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :----: | --------- | ---------- | --------------- | ----- |
* | 01 |Invoke vlan_eth_hal_configureInterface with empty L2Interface and L3Interface |config->L2Interface = "" , config->L3Interface = "" | RETURN_ERR | Should return an error |
*/
void test_l1_vlan_eth_hal_negative4_vlan_eth_hal_configureInterface(void)
{
    UT_LOG("Entering test_l1_vlan_eth_hal_negative4_vlan_eth_hal_configureInterface...");
    int result = 0;
    vlan_configuration_t *config = (vlan_configuration_t *)malloc(sizeof(vlan_configuration_t));
    if (config != NULL)
    {
        memset(config, 0, sizeof(vlan_configuration_t));
        strcpy(config->L2Interface, "");
        strcpy(config->L3Interface,"");
        config->VLANId = 1;
        config->TPId = 0;
        config->Status = VLAN_IF_UP;
        config->skbMarkingNumOfEntries = 1;
        config->skb_config = (vlan_skb_config_t *)malloc(sizeof(vlan_skb_config_t));
        if (config->skb_config != NULL)
        {
            memset(config->skb_config, 0, sizeof(vlan_skb_config_t));
            //alias should be configured in vlan_eth_config file
            strcpy(config->skb_config->alias, alias);
            config->skb_config->skbMark = 0;
            config->skb_config->skbPort = 0;
            config->skb_config->skbEthPriorityMark = 0;

            config->doReconfigure = TRUE;
            UT_LOG("Invoking vlan_eth_hal_configureInterface with empty L2Interface and L3Interface ");
            result = vlan_eth_hal_configureInterface(config);
            UT_LOG(" vlan_eth_hal_configureInterface returns: %d",result);
            UT_ASSERT_EQUAL(result, RETURN_ERR);

            free(config->skb_config);
            config->skb_config = NULL;
            free(config);
            config = NULL;
        }
        else
        {
            UT_LOG("Malloc operation failed for skb_config");
            UT_FAIL("Memory allocation with malloc failed for skb_config");
        }
        free(config);
        config = NULL;
    }
    else
    {
        UT_LOG("Malloc operation failed");
        UT_FAIL("Memory allocation with malloc failed");
    }

    UT_LOG("Exiting test_l1_vlan_eth_hal_negative4_vlan_eth_hal_configureInterface...");
}

/**
* @brief This test case checks the functionality of the vlan_eth_hal_deleteInterface function by providing a valid interface name for deassociation.
*
* The objective of this test is to ensure that the vlan_eth_hal_deleteInterface function correctly deassociates a VLAN interface.
*
* **Test Group ID:** Basic: 01 @n
* **Test Case ID:** 014 @n
* **Priority:** High @n@n
*
* **Pre-Conditions:** None @n
* **Dependencies:** None @n
* **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
*
* **Test Procedure:** @n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :----: | --------- | ---------- | -------------- | ----- |
* | 01 | Invoke vlan_eth_hal_deleteInterface with valid interface name | vlan_ifname = valid value | RETURN_OK | Should be successful |
*/
void test_l1_vlan_eth_hal_positive1_vlan_eth_hal_deleteInterface(void)
{
    UT_LOG("Entering test_l1_vlan_eth_hal_positive1_vlan_eth_hal_deleteInterface...");
    int result = 0;
    char vlan_ifname[64] = {"\0"};
    //vlanName should be configured in vlan_eth_config file
    strcpy(vlan_ifname, vlanName);

    UT_LOG("Invoking vlan_eth_hal_deleteInterface with vlan_ifname: %s", vlan_ifname);
    result = vlan_eth_hal_deleteInterface(vlan_ifname);
    UT_LOG("vlan_eth_hal_deleteInterface returns : %d",result);
    UT_ASSERT_EQUAL(result, RETURN_OK);

    UT_LOG("Exiting test_l1_vlan_eth_hal_positive1_vlan_eth_hal_deleteInterface...");
}

/**
* @brief Unit test to validate the behavior of the function when the interface does not exist.
*
* This test case checks the behavior of the 'vlan_eth_hal_deleteInterface' function when the specified interface does not exist.
*
* **Test Group ID:** Basic: 01 @n
* **Test Case ID:** 015 @n
* **Priority:** High @n@n
*
* **Pre-Conditions:** None @n
* **Dependencies:** None @n
* **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
*
* **Test Procedure:** @n
* | Variation / Step | Description  | Test Data  | Expected Result | Notes  |
* | :---------: | ------------ | -----------| --------- | --------|
* | 01| Invoke vlan_eth_hal_deleteInterface with the specified interface does not exist.|vlan_ifname = non-existing interface value| RETURN_OK | Should be successful |
*/
void test_l1_vlan_eth_hal_positive2_vlan_eth_hal_deleteInterface(void)
{
    UT_LOG("Entering test_l1_vlan_eth_hal_positive2_vlan_eth_hal_deleteInterface...");
    int result = 0;
    char vlan_ifname[64] = "gretap0";

    UT_LOG("Invoking vlan_eth_hal_deleteInterface with interface : %s not exist", vlan_ifname);
    result = vlan_eth_hal_deleteInterface(vlan_ifname);
    UT_LOG("vlan_eth_hal_deleteInterface returns : %d",result);
    UT_ASSERT_EQUAL(result, RETURN_OK);

    UT_LOG("Exiting test_l1_vlan_eth_hal_positive2_vlan_eth_hal_deleteInterface...");
}

/**
* @brief This test case is used to verify the behavior of the function when providing a null interface name.
*
* The objective of this test is to ensure that the function correctly handles the case when a null interface name is provided.
*
* **Test Group ID:** Basic: 01 @n
* **Test Case ID:** 016 @n
* **Priority:** High @n@n
*
* **Pre-Conditions:** None @n
* **Dependencies:** None @n
* **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
*
* **Test Procedure:** @n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :----: | --------- | ---------- | -------------- | ----- |
* | 01 | Invoke vlan_eth_hal_deleteInterface with NULL interface name |vlan_ifname = NULL| RETURN_ERR | Should return error |
*/
void test_l1_vlan_eth_hal_negative1_vlan_eth_hal_deleteInterface(void)
{
    UT_LOG("Entering test_l1_vlan_eth_hal_negative1_vlan_eth_hal_deleteInterface...");
    int result = 0;
    const char *vlan_ifname = NULL;

    UT_LOG("Invoking vlan_eth_hal_deleteInterface with NULL interface name");
    result = vlan_eth_hal_deleteInterface(vlan_ifname);
    UT_LOG("vlan_eth_hal_deleteInterface returns : %d", result);
    UT_ASSERT_EQUAL(result, RETURN_ERR);

  UT_LOG("Exiting test_l1_vlan_eth_hal_negative1_vlan_eth_hal_deleteInterface...");
}

/**
* @brief This test case verifies the behavior of the vlan_eth_hal_deleteInterface function when an empty string is passed as the interface name.
*
* The purpose of this test is to ensure that the function handles the empty string input correctly and returns RETURN_ERR as expected.
*
* **Test Group ID:** Basic: 01 @n
* **Test Case ID:** 017 @n
* **Priority:** High @n@n
*
* **Pre-Conditions:** None @n
* **Dependencies:** None @n
* **User Interaction:** If the user chooses interactive mode, they need to select this test case via the console. @n
*
* **Test Procedure:** @n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :----: | --------- | ---------- | -------------- | ----- |
* | 01 | Invoke vlan_eth_hal_deleteInterface with empty interface name |vlan_ifname = ""| RETURN_ERR | Should return an error |
*/
void test_l1_vlan_eth_hal_negative2_vlan_eth_hal_deleteInterface(void)
{
    UT_LOG("Entering test_l1_vlan_eth_hal_negative2_vlan_eth_hal_deleteInterface...");
    int result = 0;
    char vlan_ifname[64] = {"\0"};
    strcpy(vlan_ifname, "");

    UT_LOG("Invoking vlan_eth_hal_deleteInterface with empty interface name.");
    result = vlan_eth_hal_deleteInterface(vlan_ifname);
    UT_LOG("vlan_eth_hal_deleteInterface returns : %d", result);
    UT_ASSERT_EQUAL(result, RETURN_ERR);

    UT_LOG("Exiting test_l1_vlan_eth_hal_negative2_vlan_eth_hal_deleteInterface...");
}

/**
* @brief  This test case verifies the behavior of the vlan_eth_hal_getInterfaceStatus function with valid interface name.
*
* The purpose of this test is to ensure that the vlan_eth_hal_getInterfaceStatus function with valid interface name ,returns RETURN_OK and a valid status.
*
* **Test Group ID:** Basic: 01  @n
* **Test Case ID:** 018 @n
* **Priority:** High @n@n
*
* **Pre-Conditions:** None @n
* **Dependencies:** None @n
* **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
*
* **Test Procedure:** @n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :----: | --------- | ---------- | -------------- | ----- |
* | 01 | Invoke vlan_eth_hal_getInterfaceStatus with valid input parameters | vlan_ifname = valid value, status = valid buffer | RETURN_OK | Should be successful |
*/
void test_l1_vlan_eth_hal_positive1_vlan_eth_hal_getInterfaceStatus(void)
{
    UT_LOG("Entering test_l1_vlan_eth_hal_positive1_vlan_eth_hal_getInterfaceStatus...");
    int result = 0;
    char vlan_ifname[64] = {"\0"};
    //vlanName should be configured in vlan_eth_config file
    strcpy(vlan_ifname, vlanName);
    vlan_interface_status_e status;

    UT_LOG("Invoking vlan_eth_hal_getInterfaceStatus with valid input parameters");
    result = vlan_eth_hal_getInterfaceStatus(vlan_ifname, &status);
    UT_LOG("Interface  status: %d", status);
    UT_LOG(" vlan_eth_hal_getInterfaceStatus returns : %d", result);
    UT_ASSERT_EQUAL(result, RETURN_OK);
    if(status >=1 && status <=7 )
    {
        UT_LOG("Interface status is %d which is a valid value", status);
        UT_PASS("Get Interface status success");
    }
    else
    {
        UT_LOG(" Interface status is %d which is an Invalid value", status);
        UT_FAIL("Get Interface status failed");
    }
    UT_LOG("Exiting test_l1_vlan_eth_hal_positive1_vlan_eth_hal_getInterfaceStatus...");
}

/**
* @brief Unit test to verify the behavior of the function `vlan_eth_hal_getInterfaceStatus` when provided with a null `vlan_ifname` argument.
*
* This test checks if the function correctly handles the case where the `vlan_ifname` argument is NULL.
*
* **Test Group ID:** Basic: 01 @n
* **Test Case ID:** 019 @n
* **Priority:** High @n@n
*
* **Pre-Conditions:** None @n
* **Dependencies:** None @n
* **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
*
* **Test Procedure:** @n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :----: | --------- | ---------- |-------------- | ----- |
* | 01 | Invoke the vlan_eth_hal_getInterfaceStatus with null interface name | vlan_ifname = NULL, status = valid buffer | RETURN_ERR | Should be unsuccessful |
*/
void test_l1_vlan_eth_hal_negative1_vlan_eth_hal_getInterfaceStatus(void)
{
    UT_LOG("Entering test_l1_vlan_eth_hal_negative1_vlan_eth_hal_getInterfaceStatus...");
    int result = 0;
    const char* vlan_ifname = NULL;
    vlan_interface_status_e status;

    UT_LOG("Invoking vlan_eth_hal_getInterfaceStatus with null interface name.");
    result = vlan_eth_hal_getInterfaceStatus(vlan_ifname, &status);
    UT_LOG("vlan_eth_hal_getInterfaceStatus returns : %d", result);
    UT_LOG(" Interface Status: %d", status);
    UT_ASSERT_EQUAL(result, RETURN_ERR);

    UT_LOG("Exiting test_l1_vlan_eth_hal_negative1_vlan_eth_hal_getInterfaceStatus...");
}

/**
* @brief Test case to verify the behavior of the function vlan_eth_hal_getInterfaceStatus() when a NULL status pointer is provided.
*
* The objective of this test case is to verify that the function properly handles the scenario where a NULL status pointer is provided as an argument.
*
* **Test Group ID:** Basic: 01 @n
* **Test Case ID:** 020 @n
* **Priority:** High @n@n
*
* **Pre-Conditions:** None @n
* **Dependencies:** None @n
* **User Interaction:** If the user chooses to run the test in interactive mode, then the test case has to be selected via the console. @n
*
* **Test Procedure:** @n
* | Variation / Step | Description  | Test Data | Expected Result  | Notes |
* | :----------: | -------- | --------- | ----------- | -------- |
* | 01 | Invoke vlan_eth_hal_getInterfaceStatus with null status and valid interface| vlan_ifname = valid value, status = NULL| RETURN_ERR | Should return error |
*/
void test_l1_vlan_eth_hal_negative2_vlan_eth_hal_getInterfaceStatus(void)
{
    UT_LOG("Entering test_l1_vlan_eth_hal_negative2_vlan_eth_hal_getInterfaceStatus...");
    int result = 0;
    char vlan_ifname[64] = {"\0"};
    vlan_interface_status_e* status = NULL;
    //vlanName should be configured in vlan_eth_config file
    strcpy(vlan_ifname, vlanName);

    UT_LOG("Invoking vlan_eth_hal_getInterfaceStatus with null status");
    result = vlan_eth_hal_getInterfaceStatus(vlan_ifname, status);
    UT_LOG("vlan_eth_hal_getInterfaceStatus returns: %d", result);
    UT_ASSERT_EQUAL(result, RETURN_ERR);

    UT_LOG("Exiting test_l1_vlan_eth_hal_negative2_vlan_eth_hal_getInterfaceStatus...");
}

/**
* @brief This test case is used to verify the behavior of the vlan_eth_hal_getInterfaceStatus API when a non-existent VLAN interface is provided.
*
* The objective of this test is to check the return status of the API when a non-existent VLAN interface name is provided.
*
* **Test Group ID:** Basic: 01 @n
* **Test Case ID:** 021 @n
* **Priority:** High @n@n
*
* **Pre-Conditions:** None @n
* **Dependencies:** None @n
* **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
*
* **Test Procedure:** @n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :----: | --------- | ---------- |-------------- | ----- |
* | 01 | Invoke vlan_eth_hal_getInterfaceStatus with non-existent VLAN interface |vlan_ifname = "wl0.100", status = valid buffer | RETURN_ERR | Should be unsuccessful |
*/
void test_l1_vlan_eth_hal_negative3_vlan_eth_hal_getInterfaceStatus(void)
{
    UT_LOG("Entering test_l1_vlan_eth_hal_negative3_vlan_eth_hal_getInterfaceStatus...");
    int result = 0;
    char vlan_ifname[64] = {"\0"};
    vlan_interface_status_e status;
    strcpy(vlan_ifname, "wl0.100");

    UT_LOG("Invoking vlan_eth_hal_getInterfaceStatus with non-existent VLAN interface");
    result = vlan_eth_hal_getInterfaceStatus(vlan_ifname, &status);
    UT_LOG(" Interface  status: %d", status);
    UT_LOG(" vlan_eth_hal_getInterfaceStatus returns: %d", result);
    UT_ASSERT_EQUAL(result, RETURN_ERR);

    UT_LOG("Exiting test_l1_vlan_eth_hal_negative3_vlan_eth_hal_getInterfaceStatus...");
}

static UT_test_suite_t * pSuite = NULL;

/**
 * @brief Register the main tests for this module
 *
 * @return int - 0 on success, otherwise failure
 */
int register_hal_tests(void)
{
    // Create the test suite
    pSuite = UT_add_suite("[L1 vlan_eth_hal]", NULL, NULL);
    if (pSuite == NULL)
    {
        return -1;
    }
    // List of test function names and strings
   const char* list1[] = {"l1_vlan_eth_hal_positive1_vlan_eth_hal_init", "l1_vlan_eth_hal_positive2_vlan_eth_hal_init", "l1_vlan_eth_hal_positive1_vlan_eth_hal_configureInterface", "l1_vlan_eth_hal_positive2_vlan_eth_hal_configureInterface", "l1_vlan_eth_hal_positive3_vlan_eth_hal_configureInterface", "l1_vlan_eth_hal_positive4_vlan_eth_hal_configureInterface", "l1_vlan_eth_hal_positive5_vlan_eth_hal_configureInterface", "l1_vlan_eth_hal_positive6_vlan_eth_hal_configureInterface", "l1_vlan_eth_hal_positive7_vlan_eth_hal_configureInterface", "l1_vlan_eth_hal_negative1_vlan_eth_hal_configureInterface", "l1_vlan_eth_hal_negative2_vlan_eth_hal_configureInterface", "l1_vlan_eth_hal_negative3_vlan_eth_hal_configureInterface", "l1_vlan_eth_hal_negative4_vlan_eth_hal_configureInterface", "l1_vlan_eth_hal_positive1_vlan_eth_hal_deleteInterface", "l1_vlan_eth_hal_positive2_vlan_eth_hal_deleteInterface", "l1_vlan_eth_hal_negative1_vlan_eth_hal_deleteInterface", "l1_vlan_eth_hal_negative2_vlan_eth_hal_deleteInterface", "l1_vlan_eth_hal_positive1_vlan_eth_hal_getInterfaceStatus", "l1_vlan_eth_hal_negative1_vlan_eth_hal_getInterfaceStatus", "l1_vlan_eth_hal_negative2_vlan_eth_hal_getInterfaceStatus", "l1_vlan_eth_hal_negative3_vlan_eth_hal_getInterfaceStatus" };
   void (*list2[])() = {test_l1_vlan_eth_hal_positive1_vlan_eth_hal_init, test_l1_vlan_eth_hal_positive2_vlan_eth_hal_init, test_l1_vlan_eth_hal_positive1_vlan_eth_hal_configureInterface, test_l1_vlan_eth_hal_positive2_vlan_eth_hal_configureInterface, test_l1_vlan_eth_hal_positive3_vlan_eth_hal_configureInterface, test_l1_vlan_eth_hal_positive4_vlan_eth_hal_configureInterface, test_l1_vlan_eth_hal_positive5_vlan_eth_hal_configureInterface, test_l1_vlan_eth_hal_positive6_vlan_eth_hal_configureInterface, test_l1_vlan_eth_hal_positive7_vlan_eth_hal_configureInterface, test_l1_vlan_eth_hal_negative1_vlan_eth_hal_configureInterface, test_l1_vlan_eth_hal_negative2_vlan_eth_hal_configureInterface, test_l1_vlan_eth_hal_negative3_vlan_eth_hal_configureInterface, test_l1_vlan_eth_hal_negative4_vlan_eth_hal_configureInterface, test_l1_vlan_eth_hal_positive1_vlan_eth_hal_deleteInterface, test_l1_vlan_eth_hal_positive2_vlan_eth_hal_deleteInterface, test_l1_vlan_eth_hal_negative1_vlan_eth_hal_deleteInterface, test_l1_vlan_eth_hal_negative2_vlan_eth_hal_deleteInterface, test_l1_vlan_eth_hal_positive1_vlan_eth_hal_getInterfaceStatus, test_l1_vlan_eth_hal_negative1_vlan_eth_hal_getInterfaceStatus, test_l1_vlan_eth_hal_negative2_vlan_eth_hal_getInterfaceStatus, test_l1_vlan_eth_hal_negative3_vlan_eth_hal_getInterfaceStatus };
    // Add tests to the suite
    for (int i = 0; i < sizeof(list1) / sizeof(list1[0]); i++)
    {
        UT_add_test(pSuite, list1[i], list2[i]);
    }
    return 0;
}
int main(int argc, char** argv)
{
    int registerReturn = 0;
    if (get_L2Name() == 0)
    {
        UT_LOG("Got the L2Interface value : %s", L2Name);
    }
    else
    {
        printf("Failed to get L2Interface value\n");
    }

        if (get_L3Name() == 0)
    {
        UT_LOG("Got the L3Interface value : %s", L3Name);
    }
    else
    {
        printf("Failed to get L3Interface value\n");
    }

    if (get_alias() == 0)
    {
        UT_LOG("Got the alias value : %s", alias);
    }
    else
    {
        printf("Failed to get alias value\n");
    }

    if (get_vlanName() == 0)
    {
        UT_LOG("Got the vlan Interface value : %s", vlanName);
    }
    else
    {
        printf("Failed to get vlan Interface value\n");
    }

    /* Register tests as required, then call the UT-main to support switches and triggering */
    UT_init( argc, argv );
    /* Check if tests are registered successfully */
    registerReturn = register_hal_tests();
    if (registerReturn == 0)
    {
        printf("register_hal_tests() returned success");
    }
    else
    {
        printf("register_hal_tests() returned failure");
        return 1;
    }
    /* Begin test executions */
    UT_run_tests();
    return 0;
}
