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
 * @file test_l1_vlan_hal.c
 * @page vlan_hal Level 1 Tests
 *
 * ## Module's Role
 * This module includes Level 1 functional tests (success and failure scenarios).
 * This is to ensure that the vlan_hal APIs meet the requirements across all vendors.
 *
 * **Pre-Conditions:**  None@n
 * **Dependencies:** None@n
 *
 * Ref to API Definition specification documentation : [VLANhalSpec.md.md](../../../docs/pages/VLANhalSpec.md)
 */
#include <ut.h>
#include <ut_log.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "cJSON.h"
#include "vlan_hal.h"

static int gTestGroup = 1;
static int gTestID = 1;

char **if_Name = NULL;
int num_ifName = 0;
char **br_Name = NULL;
int num_brName = 0;

/**function to read the json config file and return its content as a string
 *IN : json file name
 *OUT : content of json file as string
 **/
static char *read_file(const char *filename)
{
    FILE *file = NULL;
    long length = 0;
    char *content = NULL;
    size_t read_chars = 0;

    /* open in read mode */
    file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Please place vlan_config file, where your binary is placed\n");
        exit(1);
    }
    else
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
                    content = (char *)malloc((size_t)length + sizeof(""));
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
            else
            {
                printf("vlan_config file is empty. please add configuration\n");
                exit(1);
            }
        }
        fclose(file);
    }
    return content;
}

/**function to read the json config file and return its content as a json object
 *IN : json file name
 *OUT : content of json file as a json object
 **/
static cJSON *parse_file(const char *filename)
{
    cJSON *parsed = NULL;
    char *content = read_file(filename);
    parsed = cJSON_Parse(content);

    if (content != NULL)
    {
        free(content);
    }

    return parsed;
}

void free_brname(void)
{
    int i = 0;
    if (br_Name != NULL)
    {
        for (i = 0; i < num_brName; i++)
        {
            free(br_Name[i]);
        }
        free(br_Name);
    }
}

int get_brname(void)
{
    char configFile[] = "./vlan_config";
    cJSON *value = NULL;
    cJSON *json = NULL;
    cJSON *item = NULL;
    int i = 0;

    printf("Checking br_Name\n");
    json = parse_file(configFile);
    if (json == NULL)
    {
        printf("Failed to parse config\n");
        return -1;
    }
    value = cJSON_GetObjectItem(json, "br_Name");
    // null check and object is string, value->valuestring
    if ((value != NULL) && (cJSON_IsArray(value)))
    {
        num_brName = cJSON_GetArraySize(value);
        printf("Number of br_Name : %d \n", num_brName);

        // Allocate memory for factoryCmVariant
        br_Name = (char **)malloc(num_brName * sizeof(char *));
        if (br_Name == NULL)
        {
            printf("Memory allocation failed\n");
            cJSON_Delete(json);
            return -1;
        }
        cJSON_ArrayForEach(item, value)
        {
            if (i < num_brName && cJSON_IsString(item))
            {
                // Allocate memory for each string and copy the content
                br_Name[i] = (char *)malloc((strlen(item->valuestring) + 1) * sizeof(char));
                if (br_Name[i] == NULL)
                {
                    printf("Memory allocation failed\n");
                    free_brname();
                    cJSON_Delete(json);
                    return -1;
                }

                strcpy(br_Name[i], item->valuestring);
                i++;
            }
        }
    }
    // Free cJSON object as it is no longer needed
    cJSON_Delete(json);
    return 0;
}

void free_ifname(void)
{
    int i = 0;
    if (if_Name != NULL)
    {
        for (i = 0; i < num_ifName; i++)
        {
            free(if_Name[i]);
        }
        free(if_Name);
    }
}

int get_ifname(void)
{
    char configFile[] = "./vlan_config";
    cJSON *value = NULL;
    cJSON *json = NULL;
    cJSON *item = NULL;
    int i = 0;

    printf("Checking if_Name\n");
    json = parse_file(configFile);
    if (json == NULL)
    {
        printf("Failed to parse config\n");
        return -1;
    }
    value = cJSON_GetObjectItem(json, "if_Name");
    // null check and object is string, value->valuestring
    if ((value != NULL) && (cJSON_IsArray(value)))
    {
        num_ifName = cJSON_GetArraySize(value);
        printf("Number of if_Name : %d \n", num_ifName);

        // Allocate memory for factoryCmVariant
        if_Name = (char **)malloc(num_ifName * sizeof(char *));
        if (if_Name == NULL)
        {
            printf("Memory allocation failed\n");
            cJSON_Delete(json);
            return -1;
        }
        cJSON_ArrayForEach(item, value)
        {
            if (i < num_ifName && cJSON_IsString(item))
            {
                // Allocate memory for each string and copy the content
                if_Name[i] = (char *)malloc((strlen(item->valuestring) + 1) * sizeof(char));
                if (if_Name[i] == NULL)
                {
                    printf("Memory allocation failed\n");
                    free_ifname();
                    cJSON_Delete(json);
                    return -1;
                }

                strcpy(if_Name[i], item->valuestring);
                i++;
            }
        }
    }
    // Free cJSON object as it is no longer needed
    cJSON_Delete(json);
    return 0;
}

/**
 * @brief Test case to verify the functionality of vlan_hal_addGroup function.
 *
 * This test case checks the functionality of the vlan_hal_addGroup function by adding a VLAN group with a given group name and default VLAN ID.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoking vlan_hal_addGroup with valid groupName = Value from config file, default_vlanID = "1" | groupName = Value from config file, default_vlanID = "1" | RETURN_OK | Should be successful |
 */
void test_l1_vlan_hal_positive1_addGroup(void)
{
    gTestID = 1;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    char groupName[64] = {"\0"};
    const char default_vlanID[5] = "1";
    int result = 0;

    strcpy(groupName, br_Name[0]);
    UT_LOG_DEBUG("Invoking vlan_hal_addGroup with valid groupName: %s and default_vlanID: %s", groupName, default_vlanID);
    result = vlan_hal_addGroup(groupName, default_vlanID);

    UT_LOG_DEBUG("vlan_hal_addGroup API returns : %d", result);
    UT_ASSERT_EQUAL(result, RETURN_OK);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Validate the functionality of vlan_hal_addGroup API
 *
 * This test case validates the functionality of vlan_hal_addGroup API by adding a VLAN group with specified VLAN ID and group name.
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
 * | 01 | Invoking vlan_hal_addGroup with groupName = "brlan5", default_vlanID = "200" | groupName = "brlan5", default_vlanID = "200" | RETURN_OK | Should be successful |
 */
void test_l1_vlan_hal_positive2_addGroup(void)
{
    gTestID = 2;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    char groupName[64] = "brlan5";
    const char default_vlanID[15] = "200";
    int result = 0;

    UT_LOG_DEBUG("Invoking vlan_hal_addGroup with valid dgroupName: %s and default_vlanID: %s", groupName, default_vlanID);
    result = vlan_hal_addGroup(groupName, default_vlanID);

    UT_LOG_DEBUG("vlan_hal_addGroup api returns: %d", result);
    UT_ASSERT_EQUAL(result, RETURN_OK);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Test the function vlan_hal_addGroup() with positive inputs.
 *
 * This test case is used to verify the functionality of the vlan_hal_addGroup() function. It checks if the function correctly adds a VLAN group with the given group name and default VLAN ID.
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
 * | 01 | Invoking vlan_hal_addGroup with groupName = "brlan10", default_vlanID = "4094" | groupName = brlan10, default_vlanID = 4094 | RETURN_OK | Should be successful |
 */
void test_l1_vlan_hal_positive3_addGroup(void)
{
    gTestID = 3;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    char groupName[64] = "brlan10";
    const char default_vlanID[5] = "4094";
    int result = 0;

    UT_LOG_DEBUG("Invoking vlan_hal_addGroup with valid groupName: %s and default_vlanID: %s", groupName, default_vlanID);
    result = vlan_hal_addGroup(groupName, default_vlanID);

    UT_LOG_DEBUG("vlan_hal_addGroup API returns : %d", result);
    UT_ASSERT_EQUAL(result, RETURN_OK);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Test case to verify the functionality of vlan_hal_addGroup API.
 *
 * This test case is used to verify whether the vlan_hal_addGroup API is functioning correctly by adding a VLAN group with the given group name and default VLAN ID.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 004 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console. @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- | -------------- | ----- |
 * | 01 | Invoking vlan_hal_addGroup with valid groupName = Value from config file, default_vlanID = "1" | groupName = Value from config file, default_vlanID = "1" | RETURN_OK | Should be successful |
 */
void test_l1_vlan_hal_positive4_addGroup(void)
{
    gTestID = 4;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    char groupName[64] = {"\0"};
    const char default_vlanID[5] = "1";
    int result = 0;

    strcpy(groupName, br_Name[0]);
    UT_LOG_DEBUG("Invoking vlan_hal_addGroup with groupName: %s, default_vlanID: %s", groupName, default_vlanID);
    result = vlan_hal_addGroup(groupName, default_vlanID);
    UT_LOG_DEBUG("Return Value: %d", result);
    UT_ASSERT_EQUAL(result, RETURN_OK);

    UT_LOG_DEBUG("Invoking vlan_hal_addGroup with same valid groupName: %s and default_vlanID: %s", groupName, default_vlanID);
    result = vlan_hal_addGroup(groupName, default_vlanID);
    UT_LOG_DEBUG("Return Value: %d", result);
    UT_ASSERT_EQUAL(result, RETURN_OK);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Test to check the behavior of the vlan_hal_addGroup function when an empty group name is passed as a parameter.
 *
 * This test verifies that the vlan_hal_addGroup function returns the expected error code when an empty group name is provided.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 005 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chooses to run the test in interactive mode, then the test case has to be selected via console. @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoking vlan_hal_addGroup with groupName = "", default_vlanID = "1"  | groupName = "", default_vlanID = "1" | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative1_addGroup(void)
{
    gTestID = 5;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    char groupName[64] = "";
    const char default_vlanID[5] = "1";
    int result = 0;

    UT_LOG_DEBUG("Invoking vlan_hal_addGroup with invalid groupName: Empty string and default_vlanID: %s", default_vlanID);
    result = vlan_hal_addGroup(groupName, default_vlanID);

    UT_LOG_DEBUG("vlan_hal_addGroup API returns: %d", result);
    UT_ASSERT_EQUAL(result, RETURN_ERR);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}
/**
 * @brief Test case to verify the behavior of the vlan_hal_addGroup function when adding a group with invalid input parameters.
 *
 * This test case is used to verify that the vlan_hal_addGroup function returns the correct error code when adding a group with invalid input parameters.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 006 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- | -------------- | ----- |
 * | 01 | Invoking vlan_hal_addGroup valid groupName = Value from config file, default_vlanID = "" | groupName = Value from config file, default_vlanID = "" | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative2_addGroup(void)
{
    gTestID = 6;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int i = 0;
    char groupName[64] = {"\0"};
    const char default_vlanID[5] = "";
    int result = 0;

    for (i = 0; i < num_brName; i++)
    {
        result = 0;
        strcpy(groupName, br_Name[i]);

        UT_LOG_DEBUG("Invoking vlan_hal_addGroup with valid groupName: %s and invalid default_vlanID: Empty string ", groupName);
        result = vlan_hal_addGroup(groupName, default_vlanID);

        UT_LOG_DEBUG("vlan_hal_addGroup API returns: %d", result);
        UT_ASSERT_NOT_EQUAL_FATAL(result, RETURN_ERR);
    }
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief This test case checks the error scenario of the vlan_hal_addGroup() function.
 *
 * The objective of this test is to verify that the vlan_hal_addGroup() function returns the expected error code when called with invalid parameters.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 007 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoking vlan_hal_addGroup with groupName = "brlanXYZ", default_vlanID = "1" | groupName = "brlanXYZ", default_vlanID = "1" | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative3_addGroup(void)
{
    gTestID = 7;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    char groupName[64] = "brlanXYZ";
    const char default_vlanID[5] = "1";
    int result = 0;

    UT_LOG_DEBUG("Invoking vlan_hal_addGroup with invalid groupName: %s and valid default_vlanID: %s", groupName, default_vlanID);
    result = vlan_hal_addGroup(groupName, default_vlanID);

    UT_LOG_DEBUG("vlan_hal_addGroup API returns: %d", result);
    UT_ASSERT_EQUAL(result, RETURN_ERR);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Test case to validate the negative scenario of the vlan_hal_addGroup function.
 *
 * This test case verifies the behavior of the vlan_hal_addGroup function when the vlanID is invalid.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 008 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If the user chooses to run the test in interactive mode, then the test case has to be selected via console. @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description                                          | Test Data                                      | Expected Result     | Notes              |
 * | :--------------: | ---------------------------------------------------- | ---------------------------------------------- | ------------------- | ------------------ |
 * | 01   | Invoking vlan_hal_addGroup with valid groupName = Value from config file, default_vlanID = "0"  | groupName = Value from config file, default_vlanID = "0" | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative4_addGroup(void)
{
    gTestID = 8;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int i = 0;
    char groupName[64] = {"\0"};
    const char default_vlanID[5] = "0";
    int result = 0;

    for (i = 0; i < num_brName; i++)
    {
        result = 0;
        strcpy(groupName, br_Name[i]);

        UT_LOG_DEBUG("Invoking vlan_hal_addGroup with valid groupName: %s and invalid default_vlanID: %s", groupName, default_vlanID);
        result = vlan_hal_addGroup(groupName, default_vlanID);

        UT_LOG_DEBUG("vlan_hal_addGroup API returns: %d", result);
        UT_ASSERT_NOT_EQUAL_FATAL(result, RETURN_ERR);
    }
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief This function tests the negative scenario for adding a VLAN group in the L1 VLAN HAL.
 *
 * In this test, the function vlan_hal_addGroup is invoked with a group name and a default VLAN ID of -1.
 * The objective of this test is to verify the behavior of the function when a negative VLAN ID is provided.
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
 * | Variation / Step | Description                                            | Test Data                                      | Expected Result | Notes       |
 * | :--------------: | -------------------------------------------------------| ---------------------------------------------- | ----------------| ----------- |
 * |01| Invoking vlan_hal_addGroup with valid groupName = Value from config file, default_vlanID = "-1" | groupName = Value from config file, default_vlanID = "-1" |  RETURN_ERR     | Should Fail |
 */
void test_l1_vlan_hal_negative5_addGroup(void)
{
    gTestID = 9;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int i = 0;
    char groupName[64] = {"\0"};
    const char default_vlanID[5] = "-1";
    int result = 0;

    for (i = 0; i < num_brName; i++)
    {
        result = 0;
        strcpy(groupName, br_Name[i]);

        UT_LOG_DEBUG("Invoking vlan_hal_addGroup with valid groupName: %s and invalid default_vlanID: %s", groupName, default_vlanID);
        result = vlan_hal_addGroup(groupName, default_vlanID);

        UT_LOG_DEBUG("vlan_hal_addGroup API returns: %d", result);
        UT_ASSERT_NOT_EQUAL_FATAL(result, RETURN_ERR);
    }
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Test case to verify the behavior when adding a VLAN group with invalid vlanID.
 *
 * This test case validates the functionality of the API vlan_hal_addGroup when it is invoked with invalid vlanID.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- | -------------- | ----- |
 * | 01 | Invoking vlan_hal_addGroup with valid groupName = Value from config file, default_vlanID = "4095"  | groupName = Value from config file, default_vlanID = "4095" | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative6_addGroup(void)
{
    gTestID = 10;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int i = 0;
    char groupName[64] = {"\0"};
    const char default_vlanID[5] = "4095";
    int result = 0;

    for (i = 0; i < num_brName; i++)
    {
        result = 0;
        strcpy(groupName, br_Name[i]);

        UT_LOG_DEBUG("Invoking vlan_hal_addGroup with valid groupName: %s and invalid default_vlanID: %s", groupName, default_vlanID);
        result = vlan_hal_addGroup(groupName, default_vlanID);

        UT_LOG_DEBUG("vlan_hal_addGroup API returns: %d", result);
        UT_ASSERT_NOT_EQUAL_FATAL(result, RETURN_ERR);
    }
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief This test case verifies the behavior of the vlan_hal_addGroup function when the groupName parameter is NULL.
 *
 * The purpose of this test is to ensure that the function handles the scenario where the groupName parameter is NULL correctly.
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
 * | 01 | Invoking vlan_hal_addGroup with groupName = NULL, default_vlanID: "1" | groupName = NULL, default_vlanID = "1" | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative7_addGroup(void)
{
    gTestID = 11;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    const char *groupName = NULL;
    const char default_vlanID[5] = "1";
    int result = 0;

    UT_LOG_DEBUG("Invoking vlan_hal_addGroup with invalid groupName: NULL and valid default_vlanID: %s", default_vlanID);
    result = vlan_hal_addGroup(groupName, default_vlanID);

    UT_LOG_DEBUG("vlan_hal_addGroup API returns: %d", result);
    UT_ASSERT_EQUAL(result, RETURN_ERR);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Test to verify the behavior of the vlan_hal_addGroup function when the group name is NULL.
 *
 * This test is meant to verify the behavior of the vlan_hal_addGroup function when the group name is NULL. The function
 * should return an error value.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 012 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- | --------------- | ----- |
 * | 01 | Invoking vlan_hal_addGroup with valid groupName = Value from config file, default_vlanID = NULL | groupName = Value from config file, default_vlanID = NULL | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative8_addGroup(void)
{
    gTestID = 12;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int i = 0;
    char groupName[64] = {"\0"};
    const char *default_vlanID = NULL;
    int result = 0;

    for (i = 0; i < num_brName; i++)
    {
        result = 0;
        strcpy(groupName, br_Name[i]);

        UT_LOG_DEBUG("Invoking vlan_hal_addGroup with valid groupName: %s and invalid default_vlanID: NULL", groupName);
        result = vlan_hal_addGroup(groupName, default_vlanID);

        UT_LOG_DEBUG("vlan_hal_addGroup API returns: %d", result);
        UT_ASSERT_EQUAL(result, RETURN_ERR);
    }
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Test function to verify the behavior of the vlan_hal_delGroup function when deleting a VLAN group.
 *
 * This test is designed to verify the functionality of the vlan_hal_delGroup function to properly delete a VLAN group.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 013 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console. @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoking vlan_hal_addGroup with groupName = "brlan5" | groupName = "brlan5" | RETURN_OK | Should successful |
 */
void test_l1_vlan_hal_positive1_delGroup(void)
{
    gTestID = 13;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    char groupName[64] = "brlan5";
    int result = 0;

    UT_LOG_DEBUG("Invoking vlan_hal_delGroup with valid groupName: %s", groupName);
    result = vlan_hal_delGroup(groupName);

    UT_LOG_DEBUG("vlan_hal_delGroup API returns: %d", result);
    UT_ASSERT_EQUAL(result, RETURN_OK);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Test the negative scenario of deleting a VLAN group using vlan_hal_delGroup function.
 *
 * This test verifies that the vlan_hal_delGroup function returns an RETURN_OK when trying to delete a VLAN group.
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
 *
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoking vlan_hal_delGroup with groupName = "brlanXYZ" | groupName = "brlanXYZ" | RETURN_OK | Should be successful |
 */
void test_l1_vlan_hal_positive2_delGroup(void)
{
    gTestID = 14;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    char groupName[64] = "brlanXYZ";
    int result = 0;

    UT_LOG_DEBUG("Invoking vlan_hal_delGroup with invalid groupName: %s", groupName);
    result = vlan_hal_delGroup(groupName);

    UT_LOG_DEBUG("vlan_hal_delGroup API returns: %d", result);
    UT_ASSERT_EQUAL(result, RETURN_OK);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Test to verify the negative scenario of deleting a VLAN group with an empty group name.
 *
 * This test case is used to verify the behavior of the function vlan_hal_delGroup() when deleting a VLAN group with an empty group name as input.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoking vlan_hal_delGroup with groupName = ""  | groupName = "" | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative1_delGroup(void)
{
    gTestID = 15;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    char groupName[64] = "";
    int result = 0;

    UT_LOG_DEBUG("Invoking vlan_hal_delGroup with invalid groupName: Empty String");
    result = vlan_hal_delGroup(groupName);

    UT_LOG_DEBUG("vlan_hal_delGroup API returns: %d", result);
    UT_ASSERT_EQUAL(result, RETURN_ERR);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief This test case is used to verify the negative scenario of the vlan_hal_delGroup function.
 *
 * The objective of this test is to check if the vlan_hal_delGroup function correctly handles the case where the groupName argument is NULL.
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
 * | 01 | Invoking vlan_hal_delGroup with groupName = NULL | groupName = NULL | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative2_delGroup(void)
{
    gTestID = 16;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    const char *groupName = NULL;
    int result = 0;

    UT_LOG_DEBUG("Invoking vlan_hal_delGroup with invalid groupName: NULL");
    result = vlan_hal_delGroup(groupName);

    UT_LOG_DEBUG("vlan_hal_delGroup API: %d", result);
    UT_ASSERT_EQUAL(result, RETURN_ERR);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Test case to verify the functionality of vlan_hal_addInterface function in L1 VLAN HAL module.
 *
 * This test case is used to verify the functionality of vlan_hal_addInterface API. It checks if the API is able to add a VLAN interface to a given VLAN group.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 017 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console. @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- | -------------- | ----- |
 * | 01 | Invoking vlan_hal_addInterface with valid groupName = Value from config file file, valid ifName = Value from config file and vlanID = "1"  | groupName = Value from config file, ifName = Value from config file, vlanID = "1" | RETURN_OK | Should be successful |
 */
void test_l1_vlan_hal_positive1_addInterface(void)
{
    gTestID = 17;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int i = 0;
    char groupName[64] = {"\0"};
    char ifName[64] = {"\0"};
    const char vlanID[5] = "1";
    strcpy(ifName, if_Name[0]);
    int result = 0;

    for (i = 0; i < num_brName; i++)
    {
        result = 0;
        strcpy(groupName, br_Name[i]);

        UT_LOG_DEBUG("Invoking vlan_hal_addInterface with valid groupName: %s, ifName: %s and vlanID: %s", groupName, ifName, vlanID);
        result = vlan_hal_addInterface(groupName, ifName, vlanID);

        UT_LOG_DEBUG("vlan_hal_addInterface returns : %d", result);
        UT_ASSERT_NOT_EQUAL_FATAL(result, RETURN_OK);
    }
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief  Test case to verify the functionality of vlan_hal_addInterface function.
 *
 *  The objective of this test is to check if the vlan_hal_addInterface function correctly handles the case were it is invoked with invalid arguments
 *
 * **Test Group ID:** Basic: 01 @n
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
 * | 01 | Invoking vlan_hal_addInterface with groupName = "", valid ifName = Value from config file, vlanID = "1" | groupName = "", ifName = Value from config file, vlanID = "1" | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative1_addInterface(void)
{
    gTestID = 18;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int i = 0;
    char groupName[64] = "";
    char ifName[64] = {"\0"};
    const char vlanID[5] = "1";
    int result = 0;

    for (i = 0; i < num_ifName; i++)
    {
        result = 0;
        strcpy(ifName, if_Name[i]);

        UT_LOG_DEBUG("Invoking vlan_hal_addInterface with invalid groupName: Empty string, valid ifName: %s and vlanID: %s", groupName, ifName, vlanID);
        result = vlan_hal_addInterface(groupName, ifName, vlanID);

        UT_LOG_DEBUG("vlan_hal_addInterface API returns : %d", result);
        UT_ASSERT_NOT_EQUAL_FATAL(result, RETURN_ERR);
    }
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Test for negative case of the "vlan_hal_addInterface" function
 *
 * This test is to validate the behavior of the "vlan_hal_addInterface" function when the input interface name is empty.
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
 * | 01 | Invoking vlan_hal_addInterface with valid groupName = Value from config file, ifName = "", vlanID = "1" | groupName = Value from config file, ifName = "", vlanID = "1" | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative2_addInterface(void)
{
    gTestID = 19;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int i = 0;
    char groupName[64] = {"\0"};
    char ifName[64] = "";
    const char vlanID[5] = "1";
    int result = 0;

    for (i = 0; i < num_brName; i++)
    {
        result = 0;
        strcpy(groupName, br_Name[i]);

        UT_LOG_DEBUG("Invoking vlan_hal_addInterface with valid groupName: %s, invalid ifName: Empty String and valid vlanID: %s", groupName, vlanID);
        result = vlan_hal_addInterface(groupName, ifName, vlanID);

        UT_LOG_DEBUG("vlan_hal_addInterface API returns: %d", result);
        UT_ASSERT_NOT_EQUAL_FATAL(result, RETURN_ERR);
    }
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief This function tests the functionality of vlan_hal_addInterface when an empty vlanID is provided.
 *
 * The objective of this test is to verify that vlan_hal_addInterface returns RETURN_ERR when an empty vlanID is provided.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 020 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console. @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data |Expected Result |Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoking vlan_hal_addInterface with valid groupName = Value from config file, valid ifName = Value from config file, vlanID = "" | groupName = Value from config file, ifName = Value from config file, vlanID = "" | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative3_addInterface(void)
{
    gTestID = 20;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int i = 0;
    char groupName[64] = {"\0"};
    char ifName[64] = {"\0"};
    const char vlanID[5] = "";
    int result = 0;

    for (i = 0; i < num_ifName; i++)
    {
        result = 0;
        strcpy(ifName, if_Name[i]);
        strcpy(groupName, br_Name[i]);

        UT_LOG_DEBUG("Invoking vlan_hal_addInterface with valid groupName: %s, ifName: %s and invalid vlanID: Empty string", groupName, ifName);
        result = vlan_hal_addInterface(groupName, ifName, vlanID);

        UT_LOG_DEBUG("vlan_hal_addInterface returns : %d", result);
        UT_ASSERT_EQUAL(result, RETURN_ERR);
    }
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief This test case is used to verify the behavior of the vlan_hal_addInterface function when invalid arguments are provided.
 *
 * The test verifies that if the function is called with an invalid groupName and it returns an error code.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 021 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If the user chooses to run the test in interactive mode, the test case has to be selected via the console. @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoking vlan_hal_addInterface groupName = "brlanXYZ", valid ifName = Value from config file, vlanID = "1" | groupName = "brlanXYZ", ifName = Value from config file, vlanID = "1" | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative4_addInterface(void)
{
    gTestID = 21;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int i = 0;
    char groupName[64] = "brlanXYZ";
    char ifName[64] = {"\0"};
    const char vlanID[5] = "1";
    int result = 0;

    for (i = 0; i < num_ifName; i++)
    {
        result = 0;
        strcpy(ifName, if_Name[i]);

        UT_LOG_DEBUG("Invoking vlan_hal_addInterface with invalid groupName: %s, valid ifName: %s and vlanID: %s", groupName, ifName, vlanID);
        result = vlan_hal_addInterface(groupName, ifName, vlanID);

        UT_LOG_DEBUG("vlan_hal_addInterface returns : %d", result);
        UT_ASSERT_NOT_EQUAL_FATAL(result, RETURN_ERR);
    }
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Test case to verify the behavior of the vlan_hal_addInterface function when invalid vlanID is provided.
 *
 * This unit test is used to verify the behavior of the function vlan_hal_addInterface when negative inputs are provided. It checks whether the function returns RETURN_ERR as expected in case of negative inputs.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 022 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
 *
 * **Test Procedure:** @n
 *
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoking vlan_hal_addInterface with valid groupName = Value from config file, valid ifName = Value from config file, vlanID = "0" | groupName = Value from config file, ifName = Value from config file, vlanID = "0" | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative5_addInterface(void)
{
    gTestID = 22;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int i = 0;
    char groupName[64] = {"\0"};
    char ifName[64] = {"\0"};
    const char vlanID[5] = "0";
    int result = 0;

    for (i = 0; i < num_ifName; i++)
    {
        result = 0;
        strcpy(ifName, if_Name[i]);
        strcpy(groupName, br_Name[i]);

        UT_LOG_DEBUG("Invoking vlan_hal_addInterface with valid groupName: %s, ifName: %s and invalid vlanID: %s", groupName, ifName, vlanID);
        result = vlan_hal_addInterface(groupName, ifName, vlanID);

        UT_LOG_DEBUG("vlan_hal_addInterface returns : %d", result);
        UT_ASSERT_NOT_EQUAL_FATAL(result, RETURN_ERR);
    }

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief This test case is used to validate the negative scenario of the vlan_hal_addInterface function.
 *
 * The purpose of this test is to verify the behavior of the vlan_hal_addInterface function when a negative vlanID is passed as an argument. The expected result is that the function should return RETURN_ERR.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 023 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If the user chooses to run the test in interactive mode, the test case should be selected via the console. @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data                                                   | Expected Result | Notes            |
 * | :--------------: | ----------- | ----------------------------------------------------------  | --------------- | ----------------- |
 * | 01 | Invoking vlan_hal_addInterface with valid groupName = Value from config file, valid ifName = Value from config file, vlanID = "-1"| groupName = Value from config file, ifName = Value from config file, vlanID = "-1"| RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative6_addInterface(void)
{
    gTestID = 23;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int i = 0;
    char groupName[64] = {"\0"};
    char ifName[64] = {"\0"};
    const char vlanID[5] = "-1";
    int result = 0;

    for (i = 0; i < num_ifName; i++)
    {
        result = 0;
        strcpy(ifName, if_Name[i]);
        strcpy(groupName, br_Name[i]);

        UT_LOG_DEBUG("Invoking vlan_hal_addInterface with valid groupName: %s, ifName: %s and invalid vlanID: %s", groupName, ifName, vlanID);
        result = vlan_hal_addInterface(groupName, ifName, vlanID);

        UT_LOG_DEBUG("vlan_hal_addInterface API returns : %d", result);
        UT_ASSERT_NOT_EQUAL_FATAL(result, RETURN_ERR);
    }

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief This test verifies the behavior of the vlan_hal_addInterface function when a VLAN with the maximum ID is added.
 *
 * The purpose of this test is to ensure that the function returns an error when it is invked with invalid vlanID.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 024 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- | --------------- | ----- |
 * | 01 | Invoking vlan_hal_addInterface with valie groupName = Value from config file, valid ifName = Value from config file, vlanID = "4095" | groupName = Value from config file, ifName = Value from config file, vlanID = "4095" | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative7_addInterface(void)
{
    gTestID = 24;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int i = 0;
    char groupName[64] = {"\0"};
    char ifName[64] = {"\0"};
    const char vlanID[5] = "4095";
    int result = 0;

    for (i = 0; i < num_ifName; i++)
    {
        result = 0;
        strcpy(ifName, if_Name[i]);
        strcpy(groupName, br_Name[i]);

        UT_LOG_DEBUG("Invoking vlan_hal_addInterface with valid groupName: %s, ifName: %s and invalid vlanID: %s", groupName, ifName, vlanID);
        result = vlan_hal_addInterface(groupName, ifName, vlanID);

        UT_LOG_DEBUG("vlan_hal_addInterface returns : %d", result);
        UT_ASSERT_NOT_EQUAL_FATAL(result, RETURN_ERR);
    }

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Test to verify the behavior of the vlan_hal_addInterface function when the group name is NULL.
 *
 * This test is to verify the behavior of the vlan_hal_addInterface function when the group name is NULL. The function should return an error value.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 025 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data |Expected Result |Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoking vlan_hal_addInterface with groupName = NULL, valid ifName = Value from config file, vlanID = "1"  | groupName = NULL, ifName = Value from config file, vlanID = "1" | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative8_addInterface(void)
{
    gTestID = 25;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int i = 0;
    const char *groupName = NULL;
    char ifName[64] = {"\0"};
    const char vlanID[5] = "1";
    int result = 0;

    for (i = 0; i < num_ifName; i++)
    {
        result = 0;
        strcpy(ifName, if_Name[i]);

        UT_LOG_DEBUG("Invoking vlan_hal_addInterface with invalid groupName: NULL, valid ifName: %s and vlanID: %s", ifName, vlanID);
        result = vlan_hal_addInterface(groupName, ifName, vlanID);

        UT_LOG_DEBUG("vlan_hal_addInterface returns : %d", result);
        UT_ASSERT_NOT_EQUAL_FATAL(result, RETURN_ERR);
    }

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Test the function vlan_hal_addInterface with invalid input arguments
 *
 * This test is meant to verify the behavior of the vlan_hal_addInterface function when the ifName is NULL. The function should return an error value.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 026 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- | -------------- | ----- |
 * | 01 | Invoking vlan_hal_addInterface with valid groupName = Value from config file, ifName = NULL, vlanID = "1"  | groupName = Value from config file, ifName = NULL, vlanID = "1" | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative9_addInterface(void)
{
    gTestID = 26;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int i = 0;
    char groupName[64] = {"\0"};
    const char *ifName = NULL;
    const char vlanID[5] = "1";
    int result = 0;

    for (i = 0; i < num_brName; i++)
    {
        result = 0;
        strcpy(groupName, br_Name[i]);

        UT_LOG_DEBUG("Invoking vlan_hal_addInterface with valid groupName: %s, invalid ifName: NULL and vlanID: %s", groupName, vlanID);
        result = vlan_hal_addInterface(groupName, ifName, vlanID);

        UT_LOG_DEBUG("vlan_hal_addInterface returns : %d", result);
        UT_ASSERT_NOT_EQUAL_FATAL(result, RETURN_ERR);
    }

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Test case to verify the negative behavior of vlan_hal_addInterface function when vlanID is NULL.
 *
 * This test case checks if the function vlan_hal_addInterface handles the NULL vlanID input correctly.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 027 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console. @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoking vlan_hal_addInterface with valid groupName = Value from config file, valid ifName = Value from config file, vlanID = NULL | groupName = Value from config file, ifName = Value from config file, vlanID = NULL | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative10_addInterface(void)
{
    gTestID = 27;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int i = 0;
    char groupName[64] = {"\0"};
    char ifName[64] = {"\0"};
    const char *vlanID = NULL;
    int result = 0;

    for (i = 0; i < num_ifName; i++)
    {
        result = 0;
        strcpy(ifName, if_Name[i]);
        strcpy(groupName, br_Name[i]);

        UT_LOG_DEBUG("Invoking vlan_hal_addInterface with valid groupName: %s, ifName: %s and invalid vlanID: NULL", groupName, ifName);
        result = vlan_hal_addInterface(groupName, ifName, vlanID);

        UT_LOG_DEBUG("vlan_hal_addInterface API returns:%d", result);
        UT_ASSERT_NOT_EQUAL_FATAL(result, RETURN_ERR);
    }
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Test case to verify the functionality of vlan_hal_delInterface when deleting a VLAN interface.
 *
 * The objective of this test case is to ensure that the vlan_hal_delInterface function is working correctly when deleting a VLAN interface.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 028 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None. @n
 * **Dependencies:** None. @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console. @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- | -------------- | ----- |
 * | 01 | Invoking vlan_hal_delInterface with valid groupName = Value from config file, ifName = "ethXYZ", vlanID = "1" | groupName = Value from config file, ifName = "ethXYZ", vlanID = "1" | RETURN_OK | Should be successful |
 */
void test_l1_vlan_hal_positive1_delInterface(void)
{
    gTestID = 28;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int i = 0;
    char groupName[64] = {"\0"};
    char ifName[64] = "ethXYZ";
    const char vlanID[5] = "1";
    int result = 0;

    for (i = 0; i < num_brName; i++)
    {
        result = 0;
        strcpy(groupName, br_Name[i]);

        UT_LOG_DEBUG("Invoking vlan_hal_delInterface with valid groupName=%s, ifName=%s, vlanID=%s", groupName, ifName, vlanID);
        result = vlan_hal_delInterface(groupName, ifName, vlanID);

        UT_LOG_DEBUG("vlan_hal_delInterface API returns: %d", result);
        UT_ASSERT_NOT_EQUAL_FATAL(result, RETURN_OK);
    }
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief This test case is used to verify the functionality of the vlan_hal_delInterface API in a negative scenario.
 *
 * The objective of this test is to check if the vlan_hal_delInterface API returns the expected error code when called with a specific set of parameters.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 029 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoking vlan_hal_delInterface with groupName = "", valid ifName = Value from config file, vlanID = "1"  | groupName = "", ifName = Value from config file, vlanID = "1" | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative1_delInterface(void)
{
    gTestID = 29;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int i = 0;
    char groupName[64] = "";
    char ifName[64] = {"\0"};
    const char vlanID[64] = "1";
    int result = 0;

    for (i = 0; i < num_ifName; i++)
    {
        result = 0;
        strcpy(ifName, if_Name[i]);

        UT_LOG_DEBUG("Invoking vlan_hal_delInterface with invalid groupName = Empty string, ifName = %s, vlanID = %s", ifName, vlanID);
        result = vlan_hal_delInterface(groupName, ifName, vlanID);

        UT_LOG_DEBUG("vlan_hal_delInterface returns: %d", result);
        UT_ASSERT_NOT_EQUAL_FATAL(result, RETURN_ERR);
    }
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief This test case is for testing the negative scenario of the vlan_hal_delInterface function.
 *
 * This test case verifies the behavior of the vlan_hal_delInterface function when an empty interface name is provided.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 030 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
 *
 * **Test Procedure:** @n
 *
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- | -------------- | ----- |
 * | 01 | Invoking vlan_hal_delInterface with valid groupName = Value from config file, ifName = "", vlanID = "1" | groupName = Value from config file, ifName = "", vlanID = "1" | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative2_delInterface(void)
{
    gTestID = 30;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int i = 0;
    char groupName[64] = {"\0"};
    char ifName[64] = "";
    const char vlanID[5] = "1";
    int result = 0;

    for (i = 0; i < num_brName; i++)
    {
        result = 0;
        strcpy(groupName, br_Name[i]);

        UT_LOG_DEBUG("Invoking vlan_hal_delInterface with valid groupName=%s, invalid ifName=Empty string and valid vlanID=%s", groupName, vlanID);
        result = vlan_hal_delInterface(groupName, ifName, vlanID);

        UT_LOG_DEBUG("vlan_hal_delInterface returns: %d", result);
        UT_ASSERT_NOT_EQUAL_FATAL(result, RETURN_ERR);
    }
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief This test case verifies the failure scenario when deleting a VLAN interface using the "vlan_hal_delInterface" API.
 *
 * The test checks if the API returns the expected error code when deleting a VLAN interface with invalid VLAN ID.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 031 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
 *
 * **Test Procedure:** @n
 * The test procedure for this test case is as follows:
 *
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoking vlan_hal_delInterface with valid groupName = Value from config file, valid ifName = Value from config file, vlanID = "" | groupName = Value from config file, ifName = Value from config file, vlanID = "" |  RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative3_delInterface(void)
{
    gTestID = 31;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int i = 0;
    char groupName[64] = {"\0"};
    char ifName[64] = {"\0"};
    const char vlanID[64] = "";
    int result = 0;

    for (i = 0; i < num_ifName; i++)
    {
        result = 0;
        strcpy(ifName, if_Name[i]);
        strcpy(groupName, br_Name[i]);

        UT_LOG_DEBUG("Invoking vlan_hal_delInterface with valid groupName=%s, ifName=%s and invalid vlanID=Empty string", groupName, ifName);
        result = vlan_hal_delInterface(groupName, ifName, vlanID);

        UT_LOG_DEBUG("Return value: %d", result);
        UT_ASSERT_NOT_EQUAL_FATAL(result, RETURN_ERR);
    }
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Test case to validate the function vlan_hal_delInterface for negative scenario
 *
 * This test case is used to verify the function vlan_hal_delInterface returns an error code when it is invoked with invalid vlanID.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 032 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- | -------------- | ----- |
 * | 01 | Invoking vlan_hal_delInterface valid groupName = Value from config file, valid ifName = Value from config file, vlanID = "0"  | groupName = Value from config file, ifName = Value from config file, vlanID = "0" | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative4_delInterface(void)
{
    gTestID = 32;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int i = 0;
    char groupName[64] = {"\0"};
    char ifName[64] = {"\0"};
    const char vlanID[5] = "0";
    int result = 0;

    for (i = 0; i < num_ifName; i++)
    {
        result = 0;
        strcpy(ifName, if_Name[i]);
        strcpy(groupName, br_Name[i]);

        UT_LOG_DEBUG("Invoking vlan_hal_delInterface with valid groupName=%s, ifName=%s and invalid vlanID=%s", groupName, ifName, vlanID);
        result = vlan_hal_delInterface(groupName, ifName, vlanID);

        UT_LOG_DEBUG("vlan_hal_delInterface returns: %d", result);
        UT_ASSERT_NOT_EQUAL_FATAL(result, RETURN_ERR);
    }
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Test case to verify the functionality of vlan_hal_delInterface when a negative VLAN ID is provided
 *
 * This test case checks whether the function vlan_hal_delInterface works correctly when a negative VLAN ID is provided as input.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 033 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data                                                   | Expected Result          | Notes              |
 * | :--------------: | ----------- | ----------------------------------------------------------- | ------------------------ | ------------------ |
 * |       01         | Invoking vlan_hal_delInterface with valid groupName = Value from config file, valid ifName = Value from config file, vlanID = "-1" | groupName = Value from config file, ifName = Value from config file, vlanID = "-1" | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative5_delInterface(void)
{
    gTestID = 33;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int i = 0;
    char groupName[64] = {"\0"};
    char ifName[64] = {"\0"};
    const char vlanID[5] = "-1";
    int result = 0;

    for (i = 0; i < num_ifName; i++)
    {
        result = 0;
        strcpy(ifName, if_Name[i]);
        strcpy(groupName, br_Name[i]);

        UT_LOG_DEBUG("Invoking vlan_hal_delInterface with valid groupName=%s, ifName=%s and invalid vlanID=%s", groupName, ifName, vlanID);
        result = vlan_hal_delInterface(groupName, ifName, vlanID);

        UT_LOG_DEBUG("vlan_hal_delInterface API returns: %d", result);
        UT_ASSERT_NOT_EQUAL_FATAL(result, RETURN_ERR);
    }
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief This test case is to verify the negative scenario of the vlan_hal_delInterface function.
*
* The objective of this test is to verify the behavior of the vlan_hal_delInterface function when invalid vlanID is provided.
*
* **Test Group ID:** Basic: 01 @n
* **Test Case ID:** 034 @n
* **Priority:** High @n@n
*
* **Pre-Conditions:** None @n
* **Dependencies:** None @n
* **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
*
* **Test Procedure:** @n
* | Variation / Step | Description | Test Data |Expected Result |Notes |
| :----: | --------- | ---------- |-------------- | ----- |
| 01 | Invoking vlan_hal_delInterface with valid groupName = Value from config file, valid ifName = Value from config file, vlanID = "4095" | groupName = Value from config file, ifName = Value from config file, vlanID = "4095" | RETURN_ERR | Should Fail |
*/
void test_l1_vlan_hal_negative6_delInterface(void)
{
    gTestID = 34;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int i = 0;
    char groupName[64] = {"\0"};
    char ifName[64] = {"\0"};
    const char vlanID[5] = "4095";
    int result = 0;

    for (i = 0; i < num_ifName; i++)
    {
        result = 0;
        strcpy(ifName, if_Name[i]);
        strcpy(groupName, br_Name[i]);

        UT_LOG_DEBUG("Invoking vlan_hal_delInterface with valid groupName=%s, ifName=%s and invalid vlanID=%s", groupName, ifName, vlanID);
        result = vlan_hal_delInterface(groupName, ifName, vlanID);

        UT_LOG_DEBUG("vlan_hal_delInterface returns: %d", result);
        UT_ASSERT_NOT_EQUAL_FATAL(result, RETURN_ERR);
    }
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Test the negative case of vlan_hal_delInterface function.
 *
 * This test case is used to verify the behavior of vlan_hal_delInterface function when passing NULL as the
 * group name and valid interface name and VLAN ID. The expected result is that the function should return
 * an error code (RETURN_ERR).
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 035 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
 *
 * **Test Procedure:** @n
 * 1. Test the negative scenario when passing NULL as the group name and valid interface name and VLAN ID.
 *  | Variation / Step | Description | Test Data | Expected Result | Notes |
 *  | :----: | --------- | ---------- | -------------- | ----- |
 *  | 01 | Invoking vlan_hal_delInterface with groupName = NULL, valid ifName = Value from config file, vlanID = "1" | groupName = NULL, ifName = Value from config file, vlanID = "1" | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative7_delInterface(void)
{
    gTestID = 35;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int i = 0;
    const char *groupName = NULL;
    char ifName[64] = {"\0"};
    const char vlanID[5] = "1";
    int result = 0;

    for (i = 0; i < num_ifName; i++)
    {
        result = 0;
        strcpy(ifName, if_Name[i]);

        UT_LOG_DEBUG("Invoking vlan_hal_delInterface with invalid groupName=NULL and valid ifName=%s, vlanID=%s", ifName, vlanID);
        result = vlan_hal_delInterface(groupName, ifName, vlanID);

        UT_LOG_DEBUG("vlan_hal_delInterface returns: %d", result);
        UT_ASSERT_NOT_EQUAL_FATAL(result, RETURN_ERR);
    }
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Test the function vlan_hal_delInterface with NULL ifName.
 *
 * This test case verifies the behavior of vlan_hal_delInterface function when ifName is NULL. The function should return a specific error code.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 036 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data                                             | Expected Result                            | Notes              |
 * | :--------------: | ----------- | ---------------------------------------------------- | ------------------------------------------ | ------------------ |
 * |       01         | Invoking vlan_hal_delInterface with valid groupName = Value from config file, ifName = NULL, vlanID = "1"| groupName = Value from config file, ifName = NULL, vlanID = "1" | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative8_delInterface(void)
{
    gTestID = 36;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int i = 0;
    char groupName[64] = {"\0"};
    const char *ifName = NULL;
    const char vlanID[5] = "1";
    int result = 0;

    for (i = 0; i < num_brName; i++)
    {
        result = 0;
        strcpy(groupName, br_Name[i]);
        UT_LOG_DEBUG("Invoking vlan_hal_delInterface with valid groupName=%s, invalid ifName=NULL and valid vlanID=%s", groupName, ifName, vlanID);
        result = vlan_hal_delInterface(groupName, ifName, vlanID);

        UT_LOG_DEBUG("vlan_hal_delInterface returns: %d", result);
        UT_ASSERT_NOT_EQUAL_FATAL(result, RETURN_ERR);
    }
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Test the function vlan_hal_delInterface with invalid input arguments.
 *
 * This test case verifies the behavior of the vlan_hal_delInterface function when attempting to delete a VLAN interface. The function should return an error code in this scenario.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 037 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, @n
 * then the test case has to be selected via console.
 *
 * **Test Procedure:** @n
 *| Variation / Step | Description | Test Data | Expected Result | Notes |
 *|-----------------:|-------------|-----------|-----------------|-------|
 *|      01         |  Invoking vlan_hal_delInterface with valid groupName = Value from config file, valid ifName = Value from config file, vlanID = NULL | groupName = Value from config file, ifName = Value from config file, vlanID = NULL | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative9_delInterface(void)
{
    gTestID = 37;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int i = 0;
    char groupName[64] = {"\0"};
    char ifName[64] = {"\0"};
    const char *vlanID = NULL;
    int result = 0;

    for (i = 0; i < num_ifName; i++)
    {
        result = 0;
        strcpy(ifName, if_Name[i]);
        strcpy(groupName, br_Name[i]);

        UT_LOG_DEBUG("Invoking vlan_hal_delInterface with valid groupName=%s, ifName=%s and invalid vlanID=NULL", groupName, ifName);
        result = vlan_hal_delInterface(groupName, ifName, vlanID);

        UT_LOG_DEBUG("vlan_hal_delInterface returns: %d", result);
        UT_ASSERT_NOT_EQUAL_FATAL(result, RETURN_ERR);
    }
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Test the functionality of the VLAN_HAL_printGroup function for the 'brlan0' group.
 * The objective of this test is to verify that the VLAN_HAL_printGroup function correctly prints the VLAN group information for the 'brlan0' group.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 038 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If the user chooses to run the test in interactive mode, the test case has to be selected via the console. @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description                       | Test Data                            | Expected Result                  | Notes            |
 * | :--------------: | --------------------------------- | ------------------------------------ | -------------------------------- | ---------------- |
 * |       01         | Invoking vlan_hal_printGroup with valid groupName = Value from config file | groupName = Value from config file | RETURN_OK  | Should be successful. |
 */
void test_l1_vlan_hal_positive1_printGroup(void)
{
    gTestID = 38;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int i = 0;
    char groupName[64] = {"\0"};
    int result = 0;

    for (i = 0; i < num_brName; i++)
    {
        result = 0;
        strcpy(groupName, br_Name[i]);

        UT_LOG_DEBUG("Invoking vlan_hal_printGroup with valid groupName = %s", groupName);
        result = vlan_hal_printGroup(groupName);

        UT_LOG_DEBUG("vlan_hal_printGroup returns: %d", result);
        UT_ASSERT_NOT_EQUAL_FATAL(result, RETURN_OK);
    }
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Test case to verify the behavior of the "vlan_hal_printGroup" function when an empty string is passed as the groupName parameter.
 *
 * This test case checks whether the "vlan_hal_printGroup" function handles the scenario when an empty string is passed as the groupName parameter correctly.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 039 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoking vlan_hal_printGroup with groupName = "" | groupName = "" | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative1_printGroup(void)
{
    gTestID = 39;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    char groupName[64] = "";
    int result = 0;

    UT_LOG_DEBUG("Invoking vlan_hal_printGroup with invalid groupName = %s", groupName);
    result = vlan_hal_printGroup(groupName);

    UT_LOG_DEBUG("vlan_hal_printGroup API returns: %d", result);
    UT_ASSERT_EQUAL(result, RETURN_ERR);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief This test case is to verify the functionality of the function vlan_hal_printGroup when the groupName contains only numbers.
 *
 * The objective of this test is to ensure that the function correctly handles the scenario when the groupName consists of only numeric characters.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 040 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console. @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes   |
 * | :--------------: | ----------- | --------- | --------------- | ------- |
 * |       01         | Invoking vlan_hal_printGroup with groupName = "1234"| groupName = "1234" | RETURN_ERR | Should Fail|
 */
void test_l1_vlan_hal_negative2_printGroup(void)
{
    gTestID = 40;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    const char groupName[64] = "1234";
    int result = 0;

    UT_LOG_DEBUG("Invoking vlan_hal_printGroup with invalid groupName = %s", groupName);
    result = vlan_hal_printGroup(groupName);

    UT_LOG_DEBUG("vlan_hal_printGroup API returns: %d", result);
    UT_ASSERT_EQUAL(result, RETURN_ERR);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Test case to verify the behavior of the function `vlan_hal_printGroup` when the group name contains special characters.
 *
 * This test case is used to check if the vlan_hal_printGroup API returns an error code when it is invoked with invalid groupName.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 041 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If the user chooses to run the test in interactive mode, then the test case has to be selected via the console. @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description                                       | Test Data                                 | Expected Result                         | Notes                                   |
 * | :--------------: | ------------------------------------------------- | ----------------------------------------- | ----------------------------------------| --------------------------------------- |
 * |       01         | Invoking vlan_hal_printGroup with groupName = "brlan@10"| groupName = "brlan@10" | RETURN_ERR  | Should fail|
 */
void test_l1_vlan_hal_negative3_printGroup(void)
{
    gTestID = 41;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    char groupName[64] = "brlan@10";
    int result = 0;

    UT_LOG_DEBUG("Invoking vlan_hal_printGroup with invalid groupName = %s", groupName);
    result = vlan_hal_printGroup(groupName);

    UT_LOG_DEBUG("vlan_hal_printGroup API returns: %d", result);
    UT_ASSERT_EQUAL(result, RETURN_ERR);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Test case to verify the behavior of the vlan_hal_printGroup function when groupName is NULL.
 *
 * The objective of this test is to ensure that the vlan_hal_printGroup function returns RETURN_ERR when the groupName is NULL.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 042 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console. @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoking vlan_hal_printGroup with groupName = NULL | groupName = NULL | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative4_printGroup(void)
{
    gTestID = 42;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    const char *groupName = NULL;
    int result = 0;

    UT_LOG_DEBUG("Invoking vlan_hal_printGroup with invalid groupName = %s", groupName);
    result = vlan_hal_printGroup(groupName);

    UT_LOG_DEBUG("vlan_hal_printGroup API returns: %d", result);
    UT_ASSERT_EQUAL(result, RETURN_ERR);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Test case to check the behavior of vlan_hal_printGroup when the given group does not exist.
 *
 * This test case is used to verify the behavior of the vlan_hal_printGroup function when the specified group does not exist.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 043 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If the user chooses to run the test in interactive mode, then the test case has to be selected via the console. @n
 *
 * **Test Procedure:** @n
 * The test procedure for this test case is as follows:
 *
 * | Variation / Step | Description                                            | Test Data             | Expected Result | Notes      |
 * | :-------------: | --------------------------------------------------------| --------------------- | ----------------| -----------|
 * |       01        | Invoking vlan_hal_printGroup with groupName = "brlanXYZ"| groupName = "brlanXYZ"|  RETURN_ERR     | Should Fail|
 */
void test_l1_vlan_hal_negative5_printGroup(void)
{
    gTestID = 43;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    char groupName[64] = "brlanXYZ";
    int result = 0;

    UT_LOG_DEBUG("Invoking vlan_hal_printGroup with invalid groupName = %s", groupName);
    result = vlan_hal_printGroup(groupName);

    UT_LOG_DEBUG("vlan_hal_printGroup API returns: %d", result);
    UT_ASSERT_EQUAL(result, RETURN_ERR);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Unit test function to test the vlan_hal_printGroup API with mixed case group name.
 *
 * This test case verifies the functionality of the vlan_hal_printGroup API when a mixed case group name is provided as input. It checks whether the API is able to handle the mixed case group name correctly and returns the expected result.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 044 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If the user chooses to run the test in interactive mode, then the test case has to be selected via the console. @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description                                             | Test Data            |Expected Result  | Notes       |
 * | :-------------:  | ------------------------------------------------------- |----------------------| --------------- | ------------|
 * |      01          | Invoking vlan_hal_printGroup with  groupName = "bRLaN0" | groupName = "bRLaN0" | RETURN_ERR      | Should Fail |
 */
void test_l1_vlan_hal_negative6_printGroup(void)
{
    gTestID = 44;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    const char groupName[64] = "bRLaN0";
    int result = 0;

    UT_LOG_DEBUG("Invoking vlan_hal_printGroup with invalid groupName = %s", groupName);
    result = vlan_hal_printGroup(groupName);

    UT_LOG_DEBUG("vlan_hal_printGroup returns: %d", result);
    UT_ASSERT_EQUAL(result, RETURN_ERR);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Test for the function vlan_hal_printAllGroup in the L1 VLAN HAL module.
 *
 * This test verifies the functionality of the vlan_hal_printAllGroup function by invoking it and checking its return value.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 045 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- | --------------- | ----- |
 * | 01 | Invoke vlan_hal_printAllGroup API and check if call succeeds | None | RETURN_OK | Should be successful |
 */
void test_l1_vlan_hal_positive1_printAllGroup(void)
{
    gTestID = 45;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int result = 0;

    UT_LOG_DEBUG("Invoking vlan_hal_printAllGroup.");
    result = vlan_hal_printAllGroup();

    UT_LOG_DEBUG("vlan_hal_printAllGroup API returns : %d", result);
    UT_ASSERT_EQUAL(result, RETURN_OK);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Test case to verify the functionality of "vlan_hal_delete_all_Interfaces" function
 *
 * This test case verifies the functionality of the "vlan_hal_delete_all_Interfaces" function by deleting all the VLAN interfaces associated with a given VLAN group.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 046 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If the user chooses to run the test in interactive mode, the test case has to be selected via the console. @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----:           | ---------   | ----------|  -------------- | ----- |
 * | 01               | Invoking vlan_hal_delete_all_Interfaces with valid groupName = Value from config file| groupName = Value from config file| RETURN_OK | Should be successful |
 */
void test_l1_vlan_hal_positive1_delete_all_Interfaces(void)
{
    gTestID = 46;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);
    int i = 0;
    char groupName[64] = {"\0"};
    int result = 0;

    for (i = 0; i < num_brName; i++)
    {
        result = 0;
        strcpy(groupName, br_Name[i]);

        UT_LOG_DEBUG("Invoking vlan_hal_delete_all_Interfaces with valid groupName: %s", groupName);
        result = vlan_hal_delete_all_Interfaces(groupName);

        UT_LOG_DEBUG("vlan_hal_delete_all_Interfaces returns: %d", result);
        UT_ASSERT_NOT_EQUAL_FATAL(result, RETURN_OK);
    }
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief This test case verifies the negative scenario of the function vlan_hal_delete_all_Interfaces where it fails to delete all interfaces.
 *
 * This test is important to ensure the function handles the failure case correctly and returns an appropriate error code.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 047 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console. @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description                                                | Test Data | Expected Result  | Notes |
 * | :----:           | -----------------------------------------------------------| ----------|--------------    | ----- |
 * | 01               | Invoking vlan_hal_delete_all_Interfaces with groupName = ""| groupName = "" | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative1_delete_all_Interfaces(void)
{
    gTestID = 47;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);
    char groupName[64] = "";
    int result = 0;

    UT_LOG_DEBUG("Invoking vlan_hal_delete_all_Interfaces with invalid groupName: Empty string");
    result = vlan_hal_delete_all_Interfaces(groupName);

    UT_LOG_DEBUG("vlan_hal_delete_all_Interfaces returns: %d", result);
    UT_ASSERT_EQUAL(result, RETURN_ERR);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Unit test for the function vlan_hal_delete_all_Interfaces when called with an invalid group name
 *
 * This test case checks the behavior of the vlan_hal_delete_all_Interfaces function when called with an invalid group name. The objective is to verify that the function returns the expected error code.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 048 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If the user chooses to run the test in interactive mode, the test case has to be selected via the console. @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data                        | Expected Result          | Notes                |
 * | :---------------: | ----------- | -------------------------------- | ------------------------ | -------------------- |
 * |       01          | Invoking vlan_hal_delete_all_Interfaces with groupName = "1234"  | groupName = "1234" |RETURN_ERR| Should Fail |
 */
void test_l1_vlan_hal_negative2_delete_all_Interfaces(void)
{
    gTestID = 48;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    char groupName[64] = "1234";
    int result = 0;

    UT_LOG_DEBUG("Invoking vlan_hal_delete_all_Interfaces with invalid groupName: %s", groupName);
    result = vlan_hal_delete_all_Interfaces(groupName);

    UT_LOG_DEBUG("vlan_hal_delete_all_Interfaces API returns: %d", result);
    UT_ASSERT_EQUAL(result, RETURN_ERR);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Test to verify the negative scenario of deleting all interfaces in a VLAN group
 *
 * This test case checks the behavior of the vlan_hal_delete_all_Interfaces function when called with an invalid group name. The objective is to ensure that the API returns an error code.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 049 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console. @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data                        | Expected Result          | Notes                |
 * | :---------------: | ----------- | -------------------------------- | ------------------------ | -------------------- |
 * | 01 | Invoking vlan_hal_delete_all_Interfaces with groupName = "brlan@10" | groupName = "brlan@10" | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative3_delete_all_Interfaces(void)
{
    gTestID = 49;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    char groupName[64] = "brlan@10";
    int result = 0;

    UT_LOG_DEBUG("Invoking vlan_hal_delete_all_Interfaces with invalid groupName: %s", groupName);
    result = vlan_hal_delete_all_Interfaces(groupName);

    UT_LOG_DEBUG("vlan_hal_delete_all_Interfaces returns: %d", result);
    UT_ASSERT_EQUAL(result, RETURN_ERR);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief This test case is used to verify the error handling of the function 'vlan_hal_delete_all_Interfaces' when a NULL groupName is provided.
 *
 * The objective of this test case is to ensure that the function handles the NULL groupName parameter correctly and returns the expected result.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 050 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console. @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- | -------------- | ----- |
 * | 01 | Invoking vlan_hal_delete_all_Interfaces with groupName = NULL | groupName = NULL | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative4_delete_all_Interfaces(void)
{
    gTestID = 50;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    const char *groupName = NULL;
    int result = 0;

    UT_LOG_DEBUG("Invoking vlan_hal_delete_all_Interfaces with invalid groupName: NULL");
    result = vlan_hal_delete_all_Interfaces(groupName);

    UT_LOG_DEBUG("vlan_hal_delete_all_Interfaces returns: %d", result);
    UT_ASSERT_EQUAL(result, RETURN_ERR);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**

 * @brief Test case to verify the functionality of `vlan_hal_delete_all_Interfaces` when deleting all VLAN interfaces with non-existent group name
 *
 * The objective of this test is to ensure that the `vlan_hal_delete_all_Interfaces` function returns `RETURN_ERR` when attempting to delete all VLAN interfaces with a group name that does not exist.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 051 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoking vlan_hal_delete_all_Interfaces with groupName = "brlanXYZ" | groupName = "brlanXYZ" | RETURN_ERR | Should Fail|
 */
void test_l1_vlan_hal_negative5_delete_all_Interfaces(void)
{
    gTestID = 51;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    char groupName[64] = "brlanXYZ";
    int result = 0;

    UT_LOG_DEBUG("Invoking vlan_hal_delete_all_Interfaces with invalid groupName: %s", groupName);
    result = vlan_hal_delete_all_Interfaces(groupName);

    UT_LOG_DEBUG("vlan_hal_delete_all_Interfaces API returns: %d", result);
    UT_ASSERT_EQUAL(result, RETURN_ERR);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Test case to validate the negative scenario of the function 'vlan_hal_delete_all_Interfaces'
 *
 * This test case verifies the scenario where the function 'vlan_hal_delete_all_Interfaces' is invoked with an invalid group name.
 * The objective of this test is to check whether the function returns an error code when invoked with an invalid group name.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 052 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
 *
 * **Test Procedure:** @n
 *| Variation / Step | Description | Test Data | Expected Result | Notes |
 *| :----: | --------- | ---------- |-------------- | ----- |
 *| 01 | Invoking vlan_hal_delete_all_Interfaces with groupName = "bRLaN0"  | groupName = "bRLaN0" | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative6_delete_all_Interfaces(void)
{
    gTestID = 52;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);
    char groupName[64] = "bRLaN0";
    int result = 0;

    UT_LOG_DEBUG("Invoking vlan_hal_delete_all_Interfaces with invalid groupName: %s", groupName);
    result = vlan_hal_delete_all_Interfaces(groupName);

    UT_LOG_DEBUG("vlan_hal_delete_all_Interfaces API returns: %d", result);
    UT_ASSERT_EQUAL(result, RETURN_ERR);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Test to check if a specific group is available in the Linux bridge brlan0
 *
 * This test is used to determine if a specific group is available in the Linux bridge brlan0. It invokes the _is_this_group_available_in_linux_bridge API with the bridge name brlan0 and checks if the return value is equal to RETURN_OK.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 053 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console. @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoking _is_this_group_available_in_linux_bridge with valid br_name = Value from config file | br_name = Value from config file | RETURN_OK | Should be successful |
 */
void test_l1_vlan_hal_positive1_is_this_group_available_in_linux_bridge(void)
{
    gTestID = 53;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int i = 0;
    char br_name[64] = {"\0"};
    int result = 0;

    for (i = 0; i < num_brName; i++)
    {
        result = 0;
        strcpy(br_name, br_Name[i]);

        UT_LOG_DEBUG("Invoking _is_this_group_available_in_linux_bridge with valid br_name: %s", br_name);
        result = _is_this_group_available_in_linux_bridge(br_name);

        UT_LOG_DEBUG("_is_this_group_available_in_linux_bridge API returns: %d", result);
        UT_ASSERT_NOT_EQUAL_FATAL(result, RETURN_OK);
    }
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Unit test to verify the behavior of _is_this_group_available_in_linux_bridge function when the group name is an empty string.
 *
 * This test case checks if the _is_this_group_available_in_linux_bridge function returns the expected error code when provided with an empty string as the bridge name.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID: 054 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
 *
 * **Test Procedure:** @n
 *
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoking _is_this_group_available_in_linux_bridge with br_name = ""  | br_name = "" | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative1_is_this_group_available_in_linux_bridge(void)
{
    gTestID = 54;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    char br_name[64] = "";
    int result = 0;

    UT_LOG_DEBUG("Invoking _is_this_group_available_in_linux_bridge with invalid br_name:Empty String");
    result = _is_this_group_available_in_linux_bridge(br_name);

    UT_LOG_DEBUG("_is_this_group_available_in_linux_bridge API returns: %d", result);
    UT_ASSERT_EQUAL(result, RETURN_ERR);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Test case to check if a group is available in the Linux bridge (numbers only)
 *
 * This test case checks the availability of a group in the Linux bridge. It verifies if the function _is_this_group_available_in_linux_bridge correctly returns an error when the group name consists of only numeric characters.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 055 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoking _is_this_group_available_in_linux_bridge with br_name = "1234"| br_name = 1234 | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative2_is_this_group_available_in_linux_bridge(void)
{
    gTestID = 55;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    char br_name[64] = "1234";
    int result = 0;

    UT_LOG_DEBUG("Invoking _is_this_group_available_in_linux_bridge with invalid br_name: %s", br_name);
    result = _is_this_group_available_in_linux_bridge(br_name);

    UT_LOG_DEBUG("_is_this_group_available_in_linux_bridge API returns: %d", result);
    UT_ASSERT_EQUAL(result, RETURN_ERR);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief This test verifies if the function _is_this_group_available_in_linux_bridge correctly identifies the presence of a specific group in the Linux bridge when the group name contains special characters.
 *
 * The test ensures that the _is_this_group_available_in_linux_bridge function returns an error code when the group name contains special characters, as expected.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 056 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If the user chooses to run the test in interactive mode, then the test case has to be selected via console. @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data |Expected Result |Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoking _is_this_group_available_in_linux_bridge with br_name = "brlan@10"  | br_name = "brlan@10" | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative3_is_this_group_available_in_linux_bridge(void)
{
    gTestID = 56;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);
    char br_name[64] = "brlan@10";
    int result = 0;

    UT_LOG_DEBUG("Invoking _is_this_group_available_in_linux_bridge with invalid br_name: %s", br_name);
    result = _is_this_group_available_in_linux_bridge(br_name);

    UT_LOG_DEBUG("_is_this_group_available_in_linux_bridge API returns: %d", result);
    UT_ASSERT_EQUAL(result, RETURN_ERR);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Test case to check if a group is available in the Linux bridge when the bridge name is a null string.
 *
 * This test case is used to check if the function "_is_this_group_available_in_linux_bridge" correctly handles the situation when the bridge name is a null string.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 057 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If the user chooses to run the test in interactive mode, they have to select this test case via the console. @n
 *
 * **Test Procedure:** @n
 *  | Variation / Step | Description | Test Data | Expected Result | Notes |
 *  | :----: | --------- | ---------- |-------------- | ----- |
 *  | 01 | Invoking _is_this_group_available_in_linux_bridge with br_name = NULL  | br_name = NULL | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative4_is_this_group_available_in_linux_bridge(void)
{
    gTestID = 57;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);
    char *br_name = NULL;
    int result = 0;

    UT_LOG_DEBUG("Invoking _is_this_group_available_in_linux_bridge with invalid br_name: NULL");
    result = _is_this_group_available_in_linux_bridge(br_name);

    UT_LOG_DEBUG("_is_this_group_available_in_linux_bridge API returns: %d", result);
    UT_ASSERT_EQUAL(result, RETURN_ERR);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Tests the function _is_this_group_available_in_linux_bridge with an invalid bridge name
 *
 * This test case checks if the function _is_this_group_available_in_linux_bridge properly handles an invalid bridge name.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 058 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- | -------------- | ----- |
 * | 01 | Invoking _is_this_group_available_in_linux_bridge with br_name = "brlanXYZ"  | br_name = "brlanXYZ" | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative5_is_this_group_available_in_linux_bridge(void)
{
    gTestID = 58;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);
    char br_name[64] = "brlanXYZ";
    int result = 0;

    UT_LOG_DEBUG("Invoking _is_this_group_available_in_linux_bridge with invalid br_name: %s", br_name);
    result = _is_this_group_available_in_linux_bridge(br_name);

    UT_LOG_DEBUG("_is_this_group_available_in_linux_bridge API returns: %d", result);
    UT_ASSERT_EQUAL(result, RETURN_ERR);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief @brief Tests the function _is_this_group_available_in_linux_bridge with an invalid bridge name
 *
 *  This test case checks if the function _is_this_group_available_in_linux_bridge properly handles an invalid bridge name.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 059 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- | -------------- | ----- |
 * | 01 | Invoking _is_this_group_available_in_linux_bridge with br_name = "bRLaN0" | br_name = "bRLaN0" | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative6_is_this_group_available_in_linux_bridge(void)
{
    gTestID = 59;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);
    char br_name[64] = "bRLaN0";
    int result = 0;

    UT_LOG_DEBUG("Invoking _is_this_group_available_in_linux_bridge with invalid br_name: %s", br_name);
    result = _is_this_group_available_in_linux_bridge(br_name);

    UT_LOG_DEBUG("_is_this_group_available_in_linux_bridge API returns: %d", result);
    UT_ASSERT_EQUAL(result, RETURN_ERR);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief This test verifies if a specific interface with a valid VLAN ID is available in the Linux bridge
 *
 * The objective of this test is to check if the function "_is_this_interface_available_in_linux_bridge" correctly determines
 * if a given interface with a valid VLAN ID exists in the Linux bridge.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 060 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If the user chooses to run the test in interactive mode, then the test case has to be selected via console @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- | -------------- | ----- |
 * | 01 | Invoking _is_this_interface_available_in_linux_bridge with valid ifName = Value from config file, vlanID = "10" | ifName = Value from config file, vlanID = "10" | RETURN_OK | Should be successful |
 */
void test_l1_vlan_hal_positive1_is_this_interface_available_in_linux_bridge(void)
{
    gTestID = 60;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int i = 0;
    char ifName[64] = {"\0"};
    char vlanID[5] = "10";
    int result = 0;

    for (i = 0; i < num_ifName; i++)
    {
        result = 0;
        strcpy(ifName, if_Name[i]);
        UT_LOG_DEBUG("Invoking _is_this_interface_available_in_linux_bridge with valid parameters ifName: %s, vlanID: %s", ifName, vlanID);
        result = _is_this_interface_available_in_linux_bridge(ifName, vlanID);

        UT_LOG_DEBUG("_is_this_interface_available_in_linux_bridge API returns : %d", result);
        UT_ASSERT_NOT_EQUAL_FATAL(result, RETURN_OK);
    }
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Tests if a given interface and minimum valid VLAN ID is available in the Linux bridge
 *
 * This test case checks if a particular interface and minimum valid VLAN ID is available in the Linux bridge. It tests the function
 * "_is_this_interface_available_in_linux_bridge", which returns the result of the availability check.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 061 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoking _is_this_interface_available_in_linux_bridge with valid ifName = Value from config file, vlanID = "1" | ifName = Value from config file, vlanID = "1" | RETURN_OK | Should be successful |
 */
void test_l1_vlan_hal_positive2_is_this_interface_available_in_linux_bridge(void)
{
    gTestID = 61;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int i = 0;
    char ifName[64] = {"\0"};
    char vlanID[5] = "1";
    int result = 0;

    for (i = 0; i < num_ifName; i++)
    {
        result = 0;
        strcpy(ifName, if_Name[i]);

        UT_LOG_DEBUG("Invoking _is_this_interface_available_in_linux_bridge with valid parameters ifName: %s, vlanID: %s", ifName, vlanID);
        result = _is_this_interface_available_in_linux_bridge(ifName, vlanID);

        UT_LOG_DEBUG("_is_this_interface_available_in_linux_bridge API returns:%d", result);
        UT_ASSERT_NOT_EQUAL_FATAL(result, RETURN_OK);
    }
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief This test case is used to verify the functionality of the is_this_interface_available_in_linux_bridge API.
 *
 * This test is designed to verify if the _is_this_interface_available_in_linux_bridge function correctly identifies the availability of a valid interface and maximum valid VLAN ID in the Linux bridge.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 062 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoking _is_this_interface_available_in_linux_bridge with valid ifName = if_Name[2], vlanID = "4094" | ifName = if_Name[2], vlanID = "4094" | RETURN_OK | Should be successful |
 */
void test_l1_vlan_hal_positive3_is_this_interface_available_in_linux_bridge(void)
{
    gTestID = 62;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int i = 0;
    char ifName[64] = {"\0"};
    char vlanID[5] = "4094";
    int result = 0;

    for (i = 0; i < num_ifName; i++)
    {
        result = 0;
        strcpy(ifName, if_Name[i]);

        UT_LOG_DEBUG("Invoking _is_this_interface_available_in_linux_bridge with valid parameters ifName: %s, vlanID: %s", ifName, vlanID);
        result = _is_this_interface_available_in_linux_bridge(ifName, vlanID);

        UT_LOG_DEBUG("_is_this_interface_available_in_linux_bridge API returns:%d", result);
        UT_ASSERT_NOT_EQUAL_FATAL(result, RETURN_OK);
    }
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Verify if an interface is available in the Linux bridge with an empty interface name
 *
 * This test case verifies the functionality of the _is_this_interface_available_in_linux_bridge function by passing an empty string as the interface name. The objective of this test is to check if the function handles invalid input of an empty interface name correctly.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 063 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoking _is_this_interface_available_in_linux_bridge with ifName = "", vlanID = "10" | ifName = "", vlanID = "10" | RETURN_ERR | Should Fail|
 */
void test_l1_vlan_hal_negative1_is_this_interface_available_in_linux_bridge(void)
{
    gTestID = 63;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);
    char ifName[64] = "";
    char vlanID[5] = "10";
    int result = 0;

    UT_LOG_DEBUG("Invoking _is_this_interface_available_in_linux_bridge with invalid ifName: Empty string, vlanID: %s", vlanID);
    result = _is_this_interface_available_in_linux_bridge(ifName, vlanID);

    UT_LOG_DEBUG("_is_this_interface_available_in_linux_bridge API returns:%d", result);
    UT_ASSERT_EQUAL(result, RETURN_ERR);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief This test case is used to verify the functionality of the get firmware name API when provided with invalid arguments.
 *
 * The p_is_this_interface_available_in_linux_bridge function is tested in this test case to ensure that it returns an error status when an empty string is passed as an argument.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 064 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console. @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoking _is_this_interface_available_in_linux_bridge with valid ifName = Value from config file, vlanID = "" | ifName = Value from config file, vlanID = "" | RETURN_ERR | Should Fail  |
 */
void test_l1_vlan_hal_negative2_is_this_interface_available_in_linux_bridge(void)
{
    gTestID = 64;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int i = 0;
    char ifName[64] = {"\0"};
    char vlanID[5] = "";
    int result = 0;

    for (i = 0; i < num_ifName; i++)
    {
        result = 0;
        strcpy(ifName, if_Name[i]);

        UT_LOG_DEBUG("Invoking _is_this_interface_available_in_linux_bridge with valid ifName: %s and invalid vlanID: Empty string", ifName);
        result = _is_this_interface_available_in_linux_bridge(ifName, vlanID);

        UT_LOG_DEBUG("_is_this_interface_available_in_linux_bridge API returns : %d", result);
        UT_ASSERT_NOT_EQUAL_FATAL(result, RETURN_ERR);
    }
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Test to check if the interface is available in the Linux bridge when an invalid VLAN ID below the minimum range is provided.
 *
 * This test verifies whether the API `_is_this_interface_available_in_linux_bridge` correctly handles the scenario where an invalid vlanID is provided. The test checks if the API returns the expected result in this scenario.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 065 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If the user chooses to run the test in interactive mode, then they must select this test case via the console. @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoking _is_this_interface_available_in_linux_bridge with valid ifName = Value from config file, vlanID = "0" | ifName = Value from config file, vlanID = "0"| RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative4_is_this_interface_available_in_linux_bridge(void)
{
    gTestID = 65;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int i = 0;
    char ifName[64] = {"\0"};
    char vlanID[5] = "0";
    int result = 0;

    for (i = 0; i < num_ifName; i++)
    {
        result = 0;
        strcpy(ifName, if_Name[i]);

        UT_LOG_DEBUG("Invoking _is_this_interface_available_in_linux_bridge with valid ifName: %s and invalid vlanID: %s", ifName, vlanID);
        result = _is_this_interface_available_in_linux_bridge(ifName, vlanID);

        UT_LOG_DEBUG("_is_this_interface_available_in_linux_bridge API returns:%d", result);
        UT_ASSERT_NOT_EQUAL_FATAL(result, RETURN_ERR);
    }
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Test case to check if a given interface is available in the Linux bridge for an invalid VLAN ID above the maximum range.
 *
 * This test case checks if the function "_is_this_interface_available_in_linux_bridge" returns the correct result when an invalid vlanID  is provided.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 066 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If the user chooses to run the test in interactive mode, they must select this test case via the console. @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * |      01     | Invoking _is_this_interface_available_in_linux_bridge with valid ifName = Value from config file, vlanID = "4095" | ifName = Value from config file, vlanID = "4095" | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative5_is_this_interface_available_in_linux_bridge(void)
{
    gTestID = 66;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int i = 0;
    char ifName[64] = {"\0"};
    char vlanID[5] = "4095";
    int result = 0;

    for (i = 0; i < num_ifName; i++)
    {
        result = 0;
        strcpy(ifName, if_Name[i]);

        UT_LOG_DEBUG("Invoking _is_this_interface_available_in_linux_bridge with valid ifName: %s and invalid vlanID: %s", ifName, vlanID);
        result = _is_this_interface_available_in_linux_bridge(ifName, vlanID);

        UT_LOG_DEBUG("_is_this_interface_available_in_linux_bridge API returns:%d", result);
        UT_ASSERT_NOT_EQUAL_FATAL(result, RETURN_ERR);
    }
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Test to check the behaviour of the API _is_this_interface_available_in_linux_bridge when the interface name is invalid.
 *
 * This test is to verify '_is_this_interface_available_in_linux_bridge' returns error code when it is invoked with invalid interface name(NULL).
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 067 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console. @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description                                                                             | Test Data                              | Expected Result | Notes              |
 * | :--------------: | --------------------------------------------------------------------------------------- | -------------------------------------- | --------------- | ------------------ |
 * |       01         | Invoking _is_this_interface_available_in_linux_bridge with ifName = NULL, vlanID = "10"| ifName = NULL, vlanID = "10"| RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative6_is_this_interface_available_in_linux_bridge(void)
{
    gTestID = 67;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    char *ifName = NULL;
    char vlanID[5] = "10";
    int result = 0;

    UT_LOG_DEBUG("Invoking _is_this_interface_available_in_linux_bridge with invalid ifName: NULL and valid vlanID: %s", vlanID);
    result = _is_this_interface_available_in_linux_bridge(ifName, vlanID);

    UT_LOG_DEBUG("_is_this_interface_available_in_linux_bridge API returns : %d", result);
    UT_ASSERT_EQUAL(result, RETURN_ERR);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Test case to check if an interface is available in Linux bridge with an invalid VLAN ID (null string).
 *
 * This test case checks whether the _is_this_interface_available_in_linux_bridge function correctly handles the scenario
 * where the VLAN ID provided is null. It verifies that the function returns an error code in this case.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 068 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoking _is_this_interface_available_in_linux_bridge with valid ifName = Value from config file, vlanID = NULL| ifName = Value from config file, vlanID = NULL | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative7_is_this_interface_available_in_linux_bridge(void)
{
    gTestID = 68;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int i = 0;
    char ifName[64] = {"\0"};
    char *vlanID = NULL;
    int result = 0;

    for (i = 0; i < num_ifName; i++)
    {
        result = 0;
        strcpy(ifName, if_Name[i]);

        UT_LOG_DEBUG("Invoking _is_this_interface_available_in_linux_bridge with valid ifName: %s and invalid vlanID: NULL", ifName);
        result = _is_this_interface_available_in_linux_bridge(ifName, vlanID);

        UT_LOG_DEBUG("_is_this_interface_available_in_linux_bridge API returns : %d", result);
        UT_ASSERT_NOT_EQUAL_FATAL(result, RETURN_ERR);
    }
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Test case to check if the interface is available in Linux bridge with an invalid VLAN ID containing alphabets
 *
 * This test case checks the functionality of the internal function _is_this_interface_available_in_linux_bridge() when an interface name and an invalid VLAN ID containing alphabets are provided as inputs. The objective of this test is to verify if the function correctly detects the invalid VLAN ID and returns an error.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 069 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If the user chooses to run the test in interactive mode, the test case has to be selected via the console. @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoking _is_this_interface_available_in_linux_bridge with valid ifName = Value from config file, vlanID = "abc" | ifName = Value from config file, vlanID = "abc" | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative9_is_this_interface_available_in_linux_bridge(void)
{
    gTestID = 69;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int i = 0;
    char ifName[64] = {"\0"};
    char vlanID[5] = "abc";
    int result = 0;

    for (i = 0; i < num_ifName; i++)
    {
        result = 0;
        strcpy(ifName, if_Name[i]);

        UT_LOG_DEBUG("Invoking _is_this_interface_available_in_linux_bridge with valid ifName: %s and invalid vlanID: %s", ifName, vlanID);
        result = _is_this_interface_available_in_linux_bridge(ifName, vlanID);

        UT_LOG_DEBUG("_is_this_interface_available_in_linux_bridge API returns : %d", result);
        UT_ASSERT_NOT_EQUAL_FATAL(result, RETURN_ERR);
    }
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief This test case verifies whether the given interface is available in the specified Linux bridge and VLAN ID.
 *
 * This test case tests the functionality of the function _is_this_interface_available_in_given_linux_bridge() by checking if the specified interface is available in the specified Linux bridge and VLAN ID.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 070 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console. @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoking _is_this_interface_available_in_given_linux_bridge with valid ifName = Value from config file, valid br_name = Value from config file, vlanID = "10"  | ifName = Value from config file, br_name = Value from config file, vlanID = "10" |  RETURN_OK | Should be successful |
 */
void test_l1_vlan_hal_positive1_is_this_interface_available_in_given_linux_bridge(void)
{
    gTestID = 70;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int i = 0;
    char ifName[64] = {"\0"};
    char br_name[64] = {"\0"};
    char vlanID[5] = "10";
    int result = 0;

    for (i = 0; i < num_ifName; i++)
    {
        result = 0;
        strcpy(ifName, if_Name[i]);
        strcpy(br_name, br_Name[i]);

        UT_LOG_DEBUG("Invoking _is_this_interface_available_in_given_linux_bridge with valid ifName = %s, br_name = %s and vlanID = %s", ifName, br_name, vlanID);
        result = _is_this_interface_available_in_given_linux_bridge(ifName, br_name, vlanID);

        UT_LOG_DEBUG("_is_this_interface_available_in_given_linux_bridge API retuns: %d", result);
        UT_ASSERT_NOT_EQUAL_FATAL(result, RETURN_OK);
    }
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Test case to check if the specified interface is available in the given Linux bridge.
 *
 * This test case checks the functionality of the function "_is_this_interface_available_in_given_linux_bridge" by verifying if the specified interface is available in the given Linux bridge.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 071 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- | --------------- | ----- |
 * | 01 | Invoking _is_this_interface_available_in_given_linux_bridge with ifName = "", br_name = "brlan0", vlanID = "10"| ifName = "", br_name = "brlan0", vlanID = "10" | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative1_is_this_interface_available_in_given_linux_bridge(void)
{
    gTestID = 71;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int i = 0;
    char ifName[64] = "";
    char br_name[64] = {"\0"};
    char vlanID[5] = "10";
    int result = 0;

    for (i = 0; i < num_brName; i++)
    {
        result = 0;
        strcpy(br_name, br_Name[i]);

        UT_LOG_DEBUG("Invoking _is_this_interface_available_in_given_linux_bridge with invalid ifName = Empty string, valid br_name = %s and vlanID = %s", br_name, vlanID);
        result = _is_this_interface_available_in_given_linux_bridge(ifName, br_name, vlanID);

        UT_LOG_DEBUG("_is_this_interface_available_in_given_linux_bridge API returns: %d", result);
        UT_ASSERT_NOT_EQUAL_FATAL(result, RETURN_ERR);
    }
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief This test checks if the interface is available in the given Linux bridge for a specific VLAN ID.
 *
 * This test invokes the function _is_this_interface_available_in_given_linux_bridge with different input arguments to check if the interface is correctly identified or not in the given Linux bridge for a specific VLAN ID.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 072 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoking _is_this_interface_available_in_given_linux_bridge with valid ifName = Value from config file, br_name = "", vlanID = "10" | ifName = Value from config file, br_name = "", vlanID = "10" | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative2_is_this_interface_available_in_given_linux_bridge(void)
{
    gTestID = 72;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int i = 0;
    char ifName[64] = {"\0"};
    char br_name[64] = "";
    char vlanID[5] = "10";
    int result = 0;

    for (i = 0; i < num_ifName; i++)
    {
        result = 0;
        strcpy(ifName, if_Name[i]);

        UT_LOG_DEBUG("Invoking _is_this_interface_available_in_given_linux_bridge with valid ifName = %s, invalid br_name = Empty string and valid vlanID = %s", ifName, vlanID);
        result = _is_this_interface_available_in_given_linux_bridge(ifName, br_name, vlanID);

        UT_LOG_DEBUG("_is_this_interface_available_in_given_linux_bridge API returns: %d", result);
        UT_ASSERT_NOT_EQUAL_FATAL(result, RETURN_ERR);
    }
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Tests the functionality of _is_this_interface_available_in_given_linux_bridge API when the interface name and bridge name are valid but VLAN ID is empty.
 *
 * This test case is used to check if the _is_this_interface_available_in_given_linux_bridge API returns an error when the interface name and bridge name are valid but VLAN ID is empty.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 073 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If the user chooses to run the test in interactive mode, then the test case has to be selected via console. @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoking _is_this_interface_available_in_given_linux_bridge with valid ifName = Value from config file, valid br_name = Value from config file, vlanID = "" | ifName = Value from config file, br_name = Value from config file, vlanID = "" | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative3_is_this_interface_available_in_given_linux_bridge(void)
{
    gTestID = 73;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int i = 0;
    char ifName[64] = {"\0"};
    char br_name[64] = {"\0"};
    char vlanID[5] = "";
    int result = 0;

    for (i = 0; i < num_ifName; i++)
    {
        result = 0;
        strcpy(ifName, if_Name[i]);
        strcpy(br_name, br_Name[i]);

        UT_LOG_DEBUG("Invoking _is_this_interface_available_in_given_linux_bridge with valid ifName = %s, br_name = %s and invalid vlanID = Empty string", ifName, br_name);
        result = _is_this_interface_available_in_given_linux_bridge(ifName, br_name, vlanID);

        UT_LOG_DEBUG("_is_this_interface_available_in_given_linux_bridge API returns: %d", result);
        UT_ASSERT_NOT_EQUAL_FATAL(result, RETURN_ERR);
    }
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief This test case is for testing the function _is_this_interface_available_in_given_linux_bridge for a negative scenario where the vlanID is invalid.
 *
 * This test verifies the functionality of the "_is_this_interface_available_in_given_linux_bridge" function when it is invoked with invalid vlanID. The test expects the function to return an error value.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 074 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If the user chooses to run the test in interactive mode, then the test case has to be selected via the console. @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- | -------------- | ----- |
 * | 01 | Invoking _is_this_interface_available_in_given_linux_bridge with valid ifName = Value from config file, valid br_name = Value from config file, vlanID = "0" | ifName = Value from config file, br_name = Value from config file, vlanID = "0" | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative5_is_this_interface_available_in_given_linux_bridge(void)
{
    gTestID = 74;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int i = 0;
    char ifName[64] = {"\0"};
    char br_name[64] = {"\0"};
    char vlanID[5] = "0";
    int result = 0;

    for (i = 0; i < num_ifName; i++)
    {
        result = 0;
        strcpy(ifName, if_Name[i]);
        strcpy(br_name, br_Name[i]);

        UT_LOG_DEBUG("Invoking _is_this_interface_available_in_given_linux_bridge with valid ifName = %s, br_name = %s and invalid vlanID = %s", ifName, br_name, vlanID);
        result = _is_this_interface_available_in_given_linux_bridge(ifName, br_name, vlanID);

        UT_LOG_DEBUG("_is_this_interface_available_in_given_linux_bridge API returns: %d", result);
        UT_ASSERT_NOT_EQUAL_FATAL(result, RETURN_ERR);
    }
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief This test is used to check the behaviour of the function _is_this_group_available_in_linux_bridge when invoked with an invalid vlanID.
 *
 * This test case checks whether the _is_this_interface_available_in_given_linux_bridge function returns error code when it is invoked with an invalid vlanID.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 075 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- | -------------- | ----- |
 * | 01 | Invoking _is_this_interface_available_in_given_linux_bridge with valid ifName = Value from config file, valid br_name = Value from config file, vlanID = 4095 | ifName = Value from config file, br_name = Value from config file, vlanID = 4095 | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative6_is_this_interface_available_in_given_linux_bridge(void)
{
    gTestID = 75;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int i = 0;
    char ifName[64] = {"\0"};
    char br_name[64] = {"\0"};
    char vlanID[64] = "4095";
    int result = 0;

    for (i = 0; i < num_ifName; i++)
    {
        result = 0;
        strcpy(ifName, if_Name[i]);
        strcpy(br_name, br_Name[i]);

        UT_LOG_DEBUG("Invoking _is_this_interface_available_in_given_linux_bridge with valid ifName = %s, br_name = %s and invalid vlanID = %s", ifName, br_name, vlanID);
        result = _is_this_interface_available_in_given_linux_bridge(ifName, br_name, vlanID);

        UT_LOG_DEBUG("_is_this_interface_available_in_given_linux_bridge API returns: %d", result);
        UT_ASSERT_NOT_EQUAL_FATAL(result, RETURN_ERR);
    }
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief This test case verifies the functionality of _is_this_interface_available_in_given_linux_bridge when ifName parameter is NULL
 *
 * This test case checks if the function correctly handles the case when ifName is NULL. It verifies if the function returns an error value when ifName is NULL.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 076 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
 *
 * **Test Procedure:**  @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoking _is_this_interface_available_in_given_linux_bridge with ifName = NULL, br_name = "brlan0", vlanID = "10" | ifName = NULL, br_name = "brlan0", vlanID = "10" | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative7_is_this_interface_available_in_given_linux_bridge(void)
{
    gTestID = 76;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int i = 0;
    char *ifName = NULL;
    char br_name[64] = {"\0"};
    char vlanID[5] = "10";
    int result = 0;

    for (i = 0; i < num_brName; i++)
    {
        result = 0;
        strcpy(br_name, br_Name[i]);

        UT_LOG_DEBUG("Invoking _is_this_interface_available_in_given_linux_bridge with invalid ifName = NULL, valid br_name = %s and vlanID = %s", br_name, vlanID);
        result = _is_this_interface_available_in_given_linux_bridge(ifName, br_name, vlanID);

        UT_LOG_DEBUG("_is_this_interface_available_in_given_linux_bridge API returns: %d", result);
        UT_ASSERT_NOT_EQUAL_FATAL(result, RETURN_ERR);
    }
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief This test case aims to verify the functionality of the "_is_this_interface_available_in_given_linux_bridge" function by checking if the interface is available in the given Linux bridge with a negative scenario.
 *
 * * This test is to verify the behavior of the _is_this_interface_available_in_given_linux_bridge function when the br_name is NULL. The function should return an error value.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 077 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If the user chooses to run the test in interactive mode, then the test case has to be selected via console. @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoking _is_this_interface_available_in_given_linux_bridge with valid ifName = Value from config file, br_name = NULL, vlanID = "10" | ifName = Value from config file, br_name = NULL, vlanID = "10" | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative8_is_this_interface_available_in_given_linux_bridge(void)
{
    gTestID = 77;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int i = 0;
    char ifName[64] = {"\0"};
    char *br_name = NULL;
    char vlanID[5] = "10";
    int result = 0;

    for (i = 0; i < num_ifName; i++)
    {
        result = 0;
        strcpy(ifName, if_Name[i]);

        UT_LOG_DEBUG("Invoking _is_this_interface_available_in_given_linux_bridge with valid ifName = %s, invalid br_name = NULL and valid vlanID = %s", ifName, vlanID);
        result = _is_this_interface_available_in_given_linux_bridge(ifName, br_name, vlanID);

        UT_LOG_DEBUG("_is_this_interface_available_in_given_linux_bridge API returns: %d", result);
        UT_ASSERT_NOT_EQUAL_FATAL(result, RETURN_ERR);
    }
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief This test verifies whether a given interface is available in a specific Linux bridge or not.
 *
 * The test checks the functionality of the function "_is_this_interface_available_in_given_linux_bridge" by passing a specific interface name and bridge name. The function returns the result of the check.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 078 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If the user chooses to run the test in interactive mode, then the test case has to be selected via the console. @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description                                                                               | Test Data                                          | Expected Result          | Notes          |
 * | :--------------: | ------------------------------------------------------------------------------------------| -------------------------------------------------- | -------------------------| ---------------|
 * |       01         | Invoking _is_this_interface_available_in_given_linux_bridge with valid ifName = Value from config file, valid br_name = Value from config file, vlanID = NULL| ifName = Value from config file, br_name = Value from config file, vlanID = NULL | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative9_is_this_interface_available_in_given_linux_bridge(void)
{
    gTestID = 78;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int i = 0;
    char ifName[64] = {"\0"};
    char br_name[64] = {"\0"};
    char *vlanID = NULL;
    int result = 0;

    for (i = 0; i < num_ifName; i++)
    {
        result = 0;
        strcpy(ifName, if_Name[i]);
        strcpy(br_name, br_Name[i]);

        UT_LOG_DEBUG("Invoking _is_this_interface_available_in_given_linux_bridge with valid ifName = %s, br_name = %s and invalid vlanID = NULL", ifName, br_name);
        result = _is_this_interface_available_in_given_linux_bridge(ifName, br_name, vlanID);

        UT_LOG_DEBUG("_is_this_interface_available_in_given_linux_bridge API returns: %d", result);
        UT_ASSERT_NOT_EQUAL_FATAL(result, RETURN_ERR);
    }
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Test to check if the interface is available in a given Linux bridge.
 *
 * This test checks whether an interface is available in a specific Linux bridge by calling the function _is_this_interface_available_in_given_linux_bridge.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 079 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console. @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * |       01         | Invoking _is_this_interface_available_in_given_linux_bridge with valid ifName = Value from config file, br_name = "brl@n0", vlanID = "10" | ifName = Value from config file, br_name = "brl@n0", vlanID = "10" | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative11_is_this_interface_available_in_given_linux_bridge(void)
{
    gTestID = 79;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int i = 0;
    char ifName[64] = {"\0"};
    char br_name[64] = "brl@n0";
    char vlanID[5] = "10";
    int result = 0;

    for (i = 0; i < num_ifName; i++)
    {
        result = 0;
        strcpy(ifName, if_Name[i]);

        UT_LOG_DEBUG("Invoking _is_this_interface_available_in_given_linux_bridge with valid ifName = %s, invalid br_name = %s anf valid vlanID = %s", ifName, br_name, vlanID);
        result = _is_this_interface_available_in_given_linux_bridge(ifName, br_name, vlanID);

        UT_LOG_DEBUG("_is_this_interface_available_in_given_linux_bridge API returns: %d", result);
        UT_ASSERT_NOT_EQUAL_FATAL(result, RETURN_ERR);
    }
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief This test is used to verify if an interface is available in a given Linux bridge when an invalid VLAN ID is provided.
 *
 * This test case verifies the behavior of the _is_this_interface_available_in_given_linux_bridge() function when an invalid VLAN ID is provided.
 *
 * **Test Group ID:** Basic: 01 / Module: 02 @n
 * **Test Case ID:** 080 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- | -------------- | ----- |
 * | 01 | Invoking _is_this_interface_available_in_given_linux_bridge valid ifName = Value from config file, valid br_name = Value from config file, vlanID = "abc"  | ifName = Value from config file, br_name = Value from config file, vlanID = "abc" | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative12_is_this_interface_available_in_given_linux_bridge(void)
{
    gTestID = 80;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int i = 0;
    char ifName[64] = {"\0"};
    char br_name[64] = {"\0"};
    char vlanID[5] = "abc";
    int result = 0;

    for (i = 0; i < num_ifName; i++)
    {
        result = 0;
        strcpy(ifName, if_Name[i]);
        strcpy(br_name, br_Name[i]);

        UT_LOG_DEBUG("Invoking _is_this_interface_available_in_given_linux_bridge with valid f_name = %s, br_name = %s and invalid vlanID = %s", ifName, br_name, vlanID);
        result = _is_this_interface_available_in_given_linux_bridge(ifName, br_name, vlanID);

        UT_LOG_DEBUG("_is_this_interface_available_in_given_linux_bridge API retuns: %d", result);
        UT_ASSERT_NOT_EQUAL_FATAL(result, RETURN_ERR);
    }

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief This test is used to verify the functionality of the insert_VLAN_ConfigEntry API.
 *
 * The objective of this test is to ensure that the insert_VLAN_ConfigEntry function successfully inserts a VLAN configuration entry.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 081 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console. @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- | -------------- | ----- |
 * | 01 | Invoking insert_VLAN_ConfigEntry with valid groupName = Value from config file, vlanID = "100"| groupName = Value from config file, vlanID = "100" | RETURN_OK | Should be successful |
 */
void test_l1_vlan_hal_positive1_insert_VLAN_ConfigEntry(void)
{
    gTestID = 81;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int i = 0;
    char groupName[64] = {"\0"};
    char vlanID[5] = "100";
    int result = 0;

    for (i = 0; i < num_ifName; i++)
    {
        result = 0;
        strcpy(groupName, br_Name[i]);

        UT_LOG_DEBUG("Invoking insert_VLAN_ConfigEntry with valid groupName: %s, vlanID: %s", groupName, vlanID);
        result = insert_VLAN_ConfigEntry(groupName, vlanID);

        UT_LOG_DEBUG("insert_VLAN_ConfigEntry API returns:%d", result);
        UT_ASSERT_NOT_EQUAL_FATAL(result, RETURN_OK);
    }
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Test to validate the functionality of insert_VLAN_ConfigEntry().
 *
 * This test is used to verify the functionality of the insert_VLAN_ConfigEntry() function. The test case inserts a VLAN configuration entry with a specified VLAN ID into the specified group.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 082 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
 *
 * Test Procedure:
 * | Variation / Step | Description                                                  | Test Data                          | Expected Result                | Notes              |
 * | :--------------: | ------------------------------------------------------------ | ---------------------------------- | ------------------------------ | ------------------ |
 * |       01         | Invoking insert_VLAN_ConfigEntry with valid groupName = Value from config file, vlanID = "1"  | groupName = Value from config file, vlanID = "1" | result = RETURN_OK  | Should be successful |
 */
void test_l1_vlan_hal_positive2_insert_VLAN_ConfigEntry(void)
{
    gTestID = 82;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int i = 0;
    char groupName[64] = {"\0"};
    char vlanID[5] = "1";
    int result = 0;

    for (i = 0; i < num_ifName; i++)
    {
        result = 0;
        strcpy(groupName, br_Name[i]);

        UT_LOG_DEBUG("Invoking insert_VLAN_ConfigEntry with valid groupName: %s, vlanID: %s", groupName, vlanID);
        result = insert_VLAN_ConfigEntry(groupName, vlanID);

        UT_LOG_DEBUG("insert_VLAN_ConfigEntry API returns : %d", result);
        UT_ASSERT_NOT_EQUAL_FATAL(result, RETURN_OK);
    }
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Test the functionality of the insert_VLAN_ConfigEntry function.
 *
 * This test case verifies that the insert_VLAN_ConfigEntry function correctly inserts a VLAN configuration entry.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 083 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- | -------------- | ----- |
 * | 01 | Invoking insert_VLAN_ConfigEntry with groupName = "brlan403", vlanID = "4094" | groupName = "brlan403", vlanID = "4094" | RETURN_OK | Should be successful |
 */
void test_l1_vlan_hal_positive3_insert_VLAN_ConfigEntry(void)
{
    gTestID = 83;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);
    char groupName[64] = "brlan403";
    char vlanID[5] = "4094";
    int result = 0;

    UT_LOG_DEBUG("Invoking insert_VLAN_ConfigEntry with valid groupName: %s, vlanID: %s", groupName, vlanID);
    result = insert_VLAN_ConfigEntry(groupName, vlanID);

    UT_LOG_DEBUG("insert_VLAN_ConfigEntry API returns : %d", result);
    UT_ASSERT_EQUAL(result, RETURN_OK);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Test function for the insert_VLAN_ConfigEntry API
 *
 * This test function checks whether the insert_VLAN_ConfigEntry API correctly inserts a VLAN configuration entry into a specified group.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 084 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoking insert_VLAN_ConfigEntry with groupName = "brlan7", vlanID = "2000" | groupName = "brlan7", vlanID = "2000" | RETURN_OK | Should be successful |
 */
void test_l1_vlan_hal_positive4_insert_VLAN_ConfigEntry(void)
{
    gTestID = 84;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);
    char groupName[64] = "brlan7";
    char vlanID[5] = "2000";
    int result = 0;

    UT_LOG_DEBUG("Invoking insert_VLAN_ConfigEntry with valid groupName: %s, vlanID: %s", groupName, vlanID);
    result = insert_VLAN_ConfigEntry(groupName, vlanID);

    UT_LOG_DEBUG("insert_VLAN_ConfigEntry API returns:%d", result);
    UT_ASSERT_EQUAL(result, RETURN_OK);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief This test case is used to validate the behavior of the insert_VLAN_ConfigEntry function when there is a negative scenario.
 *
 * The purpose of this test is to verify the behavior of the insert_VLAN_ConfigEntry function when an incorrect groupName parameter is provided.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 085 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console. @n
 *
 * **Test Procedure:** @n
 *
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoking insert_VLAN_ConfigEntry with groupName = NULL, vlanID = "100"  | groupName = NULL, vlanID = "100" | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative1_insert_VLAN_ConfigEntry(void)
{
    gTestID = 85;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);
    char *groupName = NULL;
    char vlanID[5] = "100";
    int result = 0;

    UT_LOG_DEBUG("Invoking insert_VLAN_ConfigEntry with invalid groupName: NULL and valid vlanID: %s", vlanID);
    result = insert_VLAN_ConfigEntry(groupName, vlanID);

    UT_LOG_DEBUG("insert_VLAN_ConfigEntry API returns:%d", result);
    UT_ASSERT_EQUAL(result, RETURN_ERR);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Test function to verify the behavior of insert_VLAN_ConfigEntry when the VLAN ID is NULL.
 *
 * This test checks if insert_VLAN_ConfigEntry correctly handles the case when the VLAN ID is NULL.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 086 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoking insert_VLAN_ConfigEntry with valid groupName = Value from config file, vlanID = NULL | groupName = Value from config file, vlanID = NULL | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative2_insert_VLAN_ConfigEntry(void)
{
    gTestID = 86;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int i = 0;
    char groupName[64] = {"\0"};
    char *vlanID = NULL;
    int result = 0;

    for (i = 0; i < num_brName; i++)
    {
        result = 0;
        strcpy(groupName, br_Name[i]);

        UT_LOG_DEBUG("Invoking insert_VLAN_ConfigEntry with valid groupName: %s and invalid vlanID: NULL", groupName);
        result = insert_VLAN_ConfigEntry(groupName, vlanID);

        UT_LOG_DEBUG("insert_VLAN_ConfigEntry API returns:%d", result);
        UT_ASSERT_NOT_EQUAL_FATAL(result, RETURN_ERR);
    }
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Test the insert_VLAN_ConfigEntry function with invalid groupName and valid vlanID.
 *
 * This test case verifies the behavior of the insert_VLAN_ConfigEntry function when the groupName parameter is empty and the vlanID parameter is valid. The objective of this test is to ensure that the function handles this scenario correctly and returns the expected error code.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 087 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description                                                         | Test Data                      | Expected Result | Notes       |
 * | :--------------: | --------------------------------------------------------------------| ------------------------------ | --------------- | ------------|
 * |       01         | Invoking insert_VLAN_ConfigEntry with groupName = "", vlanID = "100"| groupName = "", vlanID = "100" | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative3_insert_VLAN_ConfigEntry(void)
{
    gTestID = 87;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    char groupName[64] = "";
    char vlanID[5] = "100";
    int result = 0;

    UT_LOG_DEBUG("Invoking insert_VLAN_ConfigEntry with invalid groupName: Empty string and valid vlanID: %s", vlanID);
    result = insert_VLAN_ConfigEntry(groupName, vlanID);

    UT_LOG_DEBUG("insert_VLAN_ConfigEntry API returns:%d", result);
    UT_ASSERT_EQUAL(result, RETURN_ERR);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief This test case is for testing the insert_VLAN_ConfigEntry function for a negative scenario where the vlanID argument is empty.
 *
 * The objective of this test is to verify that the insert_VLAN_ConfigEntry function handles the case when the vlanID argument is empty correctly.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 088 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console. @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoking insert_VLAN_ConfigEntry with valid groupName = Value from config file, vlanID = "" | groupName = Value from config file, vlanID = "" | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative4_insert_VLAN_ConfigEntry(void)
{
    gTestID = 88;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int i = 0;
    char groupName[64] = {"\0"};
    char vlanID[5] = "";
    int result = 0;

    for (i = 0; i < num_brName; i++)
    {
        result = 0;
        strcpy(groupName, br_Name[i]);

        UT_LOG_DEBUG("Invoking insert_VLAN_ConfigEntry with valid groupName: %s and invalid vlanID: Empty string", groupName);
        result = insert_VLAN_ConfigEntry(groupName, vlanID);

        UT_LOG_DEBUG("insert_VLAN_ConfigEntry API returns:%d", result);
        UT_ASSERT_NOT_EQUAL_FATAL(result, RETURN_ERR);
    }
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Test case to verify the behavior of the insert_VLAN_ConfigEntry function when invalid group name is provided.
 *
 * This test case checks whether the insert_VLAN_ConfigEntry function correctly handles the scenario when an invalid group name is provided. The function should return an error in this case.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 089 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | --------------------------------- | ---------------- | ------ |
 * | 01 | Invoking insert_VLAN_ConfigEntry with groupName = "brlan114", vlanID = "100"  | groupName = "brlan114", vlanID = "100" | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative5_insert_VLAN_ConfigEntry(void)
{
    gTestID = 89;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    char groupName[64] = "brlan114";
    char vlanID[5] = "100";
    int result = 0;

    UT_LOG_DEBUG("Invoking insert_VLAN_ConfigEntry with invalid groupName: %s and valid vlanID: %s", groupName, vlanID);
    result = insert_VLAN_ConfigEntry(groupName, vlanID);

    UT_LOG_DEBUG("insert_VLAN_ConfigEntry API returns:%d", result);
    UT_ASSERT_EQUAL(result, RETURN_ERR);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Test case to verify the behavior of insert_VLAN_ConfigEntry when invalid VLAN ID is provided.
 *
 * This test case is to ensure that the insert_VLAN_ConfigEntry function behaves correctly when an invalid VLAN ID is provided. It verifies that the function returns an error code when the VLAN ID is invalid.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 090 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
 *
 * **Test Procedure:** @n
 * | Variation / Step   | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoking insert_VLAN_ConfigEntry with valid groupName = Value from config file, vlanID = "0" | groupName = Value from config file, vlanID = "0" | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative6_insert_VLAN_ConfigEntry(void)
{
    gTestID = 90;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int i = 0;
    char groupName[64] = {"\0"};
    char vlanID[5] = "0";
    int result = 0;

    for (i = 0; i < num_brName; i++)
    {
        result = 0;
        strcpy(groupName, br_Name[i]);

        UT_LOG_DEBUG("Invoking insert_VLAN_ConfigEntry with valid groupName: %s and invalid vlanID: %s", groupName, vlanID);
        result = insert_VLAN_ConfigEntry(groupName, vlanID);

        UT_LOG_DEBUG("insert_VLAN_ConfigEntry API returns:%d", result);
        UT_ASSERT_NOT_EQUAL_FATAL(result, RETURN_ERR);
    }
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief This test case is used to verify the behavior of the insert_VLAN_ConfigEntry function when the vlanID parameter is negative.
 *
 * This test focuses on testing the error handling of the insert_VLAN_ConfigEntry function when the vlanID parameter is passed as a negative value. The test checks if the function correctly returns an error code when the vlanID is negative.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 091 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If the user chooses to run the test in interactive mode, then the test case has to be selected via console. @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoking insert_VLAN_ConfigEntry with valid groupName = Value from config file, vlanID = "-1" | groupName = Value from config file, vlanID = "-1" | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative7_insert_VLAN_ConfigEntry(void)
{
    gTestID = 91;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int i = 0;
    char groupName[64] = {"\0"};
    char vlanID[5] = "-1";
    int result = 0;

    for (i = 0; i < num_brName; i++)
    {
        result = 0;
        strcpy(groupName, br_Name[i]);

        UT_LOG_DEBUG("Invoking insert_VLAN_ConfigEntry with valid groupName: %s and invalid lanID: %s", groupName, vlanID);
        result = insert_VLAN_ConfigEntry(groupName, vlanID);

        UT_LOG_DEBUG("insert_VLAN_ConfigEntry API returns:%d", result);
        UT_ASSERT_NOT_EQUAL_FATAL(result, RETURN_ERR);
    }
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief This test case is used to verify the behavior of the insert_VLAN_ConfigEntry function when it is invoked with invalid argument.
 *
 * This test focuses on testing the error handling of the insert_VLAN_ConfigEntry function when the vlanID parameter is passed as a negative value. The test checks if the function correctly returns an error code when the vlanID is invalid.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 092 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
 *
 * **Test Procedure:** @n
 * | Variation / Step   | Description | Test Data | Expected Result | Notes |
 * | :----: | ----------------------------------------------------------------- | ---------- | --------------- | ----- |
 * | 01 | Invoking insert_VLAN_ConfigEntry with valid groupName = Value from config file, vlanID = "5000"| groupName = Value from config file, vlanID = "5000" | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative8_insert_VLAN_ConfigEntry(void)
{
    gTestID = 92;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int i = 0;
    char groupName[64] = {"\0"};
    char vlanID[5] = "5000";
    int result = 0;

    for (i = 0; i < num_brName; i++)
    {
        result = 0;
        strcpy(groupName, br_Name[i]);

        UT_LOG_DEBUG("Invoking insert_VLAN_ConfigEntry with valid groupName: %sa and invalid vlanID: %s", groupName, vlanID);
        result = insert_VLAN_ConfigEntry(groupName, vlanID);

        UT_LOG_DEBUG("insert_VLAN_ConfigEntry API returns:%d", result);
        UT_ASSERT_NOT_EQUAL_FATAL(result, RETURN_ERR);
    }
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Test case to verify the behavior of the insert_VLAN_ConfigEntry function when invalid VLAN ID is provided.
 *
 * This test case checks whether the insert_VLAN_ConfigEntry function returns an error when an invalid VLAN ID is provided as input.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 093 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console. @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- | --------------- | ----- |
 * | 01 | Invoking insert_VLAN_ConfigEntry with valid groupName = Value from config file, vlanID = "1a2b" | groupName = Value from config file, vlanID = "1a2b" | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative9_insert_VLAN_ConfigEntry(void)
{
    gTestID = 93;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int i = 0;
    char groupName[64] = {"\0"};
    char vlanID[5] = "1a2b";
    int result = 0;

    for (i = 0; i < num_brName; i++)
    {
        result = 0;
        strcpy(groupName, br_Name[i]);

        UT_LOG_DEBUG("Invoking insert_VLAN_ConfigEntry with valid groupName: %s and invalid vlanID: %s", groupName, vlanID);
        result = insert_VLAN_ConfigEntry(groupName, vlanID);

        UT_LOG_DEBUG("insert_VLAN_ConfigEntry API returns:%d", result);
        UT_ASSERT_NOT_EQUAL_FATAL(result, RETURN_ERR);
    }
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Test case to verify the functionality of deleting a VLAN config entry with a valid group name (brlan0).
 *
 * This test case verifies that the function delete_VLAN_ConfigEntry can successfully delete a VLAN config entry for a given group name. The function is expected to return RETURN_OK if the deletion is successful.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 094 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If the user chooses to run the test in interactive mode, then the test case has to be selected via the console. @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- | -------------- | ----- |
 * | 01 | Invoking delete_VLAN_ConfigEntry with valid groupName = Value from config file| groupName = Value from config file | RETURN_OK | Should be successful |
 */
void test_l1_vlan_hal_positive1_delete_VLAN_ConfigEntry(void)
{
    gTestID = 94;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int i = 0;
    char groupName[64] = {"\0"};
    int result = 0;

    for (i = 0; i < num_brName; i++)
    {
        result = 0;
        strcpy(groupName, br_Name[i]);

        UT_LOG_DEBUG("Invoking delete_VLAN_ConfigEntry with valid groupName:%s", groupName);
        result = delete_VLAN_ConfigEntry(groupName);

        UT_LOG_DEBUG("delete_VLAN_ConfigEntry API returns:%d", result);
        UT_ASSERT_NOT_EQUAL_FATAL(result, RETURN_OK);
    }
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Test case to verify the behavior when an empty string is passed as the group name parameter to the delete_VLAN_ConfigEntry function.
 *
 * This test case checks the behavior of the delete_VLAN_ConfigEntry function when an empty string is passed as the group name parameter. The objective of this test is to ensure that the function handles invalid input correctly.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 095 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console. @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- | -------------- | ----- |
 * | 01 | Invoking delete_VLAN_ConfigEntry with groupName = ""| groupName = "" | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative1_delete_VLAN_ConfigEntry(void)
{
    gTestID = 95;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    char groupName[64] = "";
    int result = 0;

    UT_LOG_DEBUG("Invoking delete_VLAN_ConfigEntry with invalid groupName: Empty string");
    result = delete_VLAN_ConfigEntry(groupName);

    UT_LOG_DEBUG("delete_VLAN_ConfigEntry API returns :%d", result);
    UT_ASSERT_EQUAL(result, RETURN_ERR);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Verify that delete_VLAN_ConfigEntry returns an error when the group name is NULL.
 *
 * This test checks if delete_VLAN_ConfigEntry successfully returns an error when the group name passed to it is NULL.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 096 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console. @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoking delete_VLAN_ConfigEntry with groupName = NULL | groupName = NULL | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative2_delete_VLAN_ConfigEntry(void)
{
    gTestID = 96;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    char *groupName = NULL;
    int result = 0;

    UT_LOG_DEBUG("Invoking delete_VLAN_ConfigEntry with invalid groupName: NULL");
    result = delete_VLAN_ConfigEntry(groupName);

    UT_LOG_DEBUG("delete_VLAN_ConfigEntry API returns :%d", result);
    UT_ASSERT_EQUAL(result, RETURN_ERR);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Test case to verify the behavior of delete_VLAN_ConfigEntry function when an invalid group name is provided.
 *
 * This test case checks whether the delete_VLAN_ConfigEntry function returns an expected error code (RETURN_ERR) when an invalid group name (random_string) is provided as input.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 097 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console. @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- | -------------- | ----- |
 * | 01 | Invoking delete_VLAN_ConfigEntry with groupName = "brlan115" | groupName = "brlan115" | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative3_delete_VLAN_ConfigEntry(void)
{
    gTestID = 97;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    char groupName[64] = "brlan115";
    int result = 0;

    UT_LOG_DEBUG("Invoking delete_VLAN_ConfigEntry with invalid groupName:%s", groupName);
    result = delete_VLAN_ConfigEntry(groupName);

    UT_LOG_DEBUG("delete_VLAN_ConfigEntry API returns :%d", result);
    UT_ASSERT_EQUAL(result, RETURN_ERR);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Test to validate the behavior of delete_VLAN_ConfigEntry when the input groupName is a substring of a valid groupName.
 *
 * This test is intended to verify the behavior of delete_VLAN_ConfigEntry when the input groupName is a substring of a valid groupName. The test checks if the function returns a negative error code.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 098 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console. @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- | -------------- | ----- |
 * | 01 | Invoking delete_VLAN_ConfigEntry with groupName = "brla" | groupName = "brla" | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative4_delete_VLAN_ConfigEntry(void)
{
    gTestID = 98;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    char groupName[64] = "brla";
    int result = 0;

    UT_LOG_DEBUG("Invoking delete_VLAN_ConfigEntry with invalid groupName:%s", groupName);
    result = delete_VLAN_ConfigEntry(groupName);

    UT_LOG_DEBUG("delete_VLAN_ConfigEntry API returns :%d", result);
    UT_ASSERT_EQUAL(result, RETURN_ERR);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief This test case is used to verify the functionality of the get_vlanId_for_GroupName API.
 *
 * The objective of this test is to verify whether the API can successfully retrieve the VLAN ID for a given group name.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 099 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console. @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoking get_vlanId_for_GroupName with valid groupName = Value from config file, vlanID = valid buffer | groupName = Value from config file, vlanID = valid buffer | RETURN_OK | Should be successful |
 */
void test_l1_vlan_hal_positive1_get_vlanId_for_GroupName(void)
{
    gTestID = 99;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int i = 0;
    char groupName[64] = {"\0"};
    char vlanID[5] = {"\0"};
    int Vlan_Id = 0;
    int result = 0;

    for (i = 0; i < num_brName; i++)
    {
        result = 0;
        strcpy(groupName, br_Name[i]);

        UT_LOG_DEBUG("Invoking get_vlanId_for_GroupName with valid groupName: %s and vlanID buffer: %s", groupName);
        result = get_vlanId_for_GroupName(groupName, vlanID);

        UT_LOG_DEBUG("get_vlanId_for_GroupName API returns: %d", result);
        Vlan_Id = atoi(vlanID);
        UT_LOG_DEBUG("vlanID = %d", Vlan_Id);
        UT_ASSERT_NOT_EQUAL_FATAL(result, RETURN_OK);

        if (Vlan_Id >= 1 && Vlan_Id <= 4094)
        {
            UT_LOG_DEBUG("vlan id  is %d", Vlan_Id);
            UT_PASS("vlan id validation Success");
        }
        else
        {
            UT_LOG_DEBUG("vlan id is %d", Vlan_Id);
            UT_FAIL("vlan id validation Failed");
        }
    }
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Tests the function get_vlanId_for_GroupName for negative scenario.
 *
 * This test case checks the behavior of get_vlanId_for_GroupName function when provided with a NULL groupName parameter.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 100 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None. @n
 * **Dependencies:** None. @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console. @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoking get_vlanId_for_GroupName with groupName = NULL, vlanID buffer = valid buffer | groupName = NULL, vlanID buffer = valid buffer | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative1_get_vlanId_for_GroupName(void)
{
    gTestID = 100;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    const char *groupName = NULL;
    char vlanID[5] = {"\0"};
    int result = 0;

    UT_LOG_DEBUG("Invoking get_vlanId_for_GroupName with invalid groupName: %s and valid vlanID buffer", groupName);
    result = get_vlanId_for_GroupName(groupName, vlanID);

    UT_LOG_DEBUG("get_vlanId_for_GroupName API returns: %d", result);
    UT_ASSERT_EQUAL(result, RETURN_ERR);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Unit test to verify the behavior of get_vlanId_for_GroupName when groupName is NULL.
 *
 * This test case checks the behavior of the function get_vlanId_for_GroupName when the groupName parameter is NULL.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 101 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If the user chooses to run the test in interactive mode, then the test case has to be selected via the console. @n
 *
 * **Test Procedure:** @n
 *
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- | --------------- | ----- |
 * | 01 | Invoking get_vlanId_for_GroupName with valid groupName = Value from config file, vlanID = NULL | groupName = Value from config file, vlanID = NULL | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative2_get_vlanId_for_GroupName(void)
{
    gTestID = 101;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int i = 0;
    char groupName[64] = {"\0"};
    char *vlanID = NULL;
    int result = 0;

    for (i = 0; i < num_brName; i++)
    {
        result = 0;
        strcpy(groupName, br_Name[i]);

        UT_LOG_DEBUG("Invoking get_vlanId_for_GroupName with valid groupName: %s and vlanID buffer: NULL", groupName);
        result = get_vlanId_for_GroupName(groupName, vlanID);

        UT_LOG_DEBUG("get_vlanId_for_GroupName API returns: %d", result);
        UT_ASSERT_EQUAL(result, RETURN_ERR);
    }
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * Tests the behavior of get_vlanId_for_GroupName when the group name is empty.
 *
 * The objective of this test is to verify the functionality of the get_vlanId_for_GroupName function when the group name provided is empty.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 102 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- | --------------- | ----- |
 * | 01 | Invoking get_vlanId_for_GroupName with groupName = "" and vlanID = valid buffer  | groupName = "", vlanID = valid buffer | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative3_get_vlanId_for_GroupName(void)
{
    gTestID = 102;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    char groupName[64] = "";
    char vlanID[5] = {"\0"};
    int result = 0;

    UT_LOG_DEBUG("Invoking get_vlanId_for_GroupName with input groupName: %s and vlanID buffer: %s", groupName, vlanID);
    result = get_vlanId_for_GroupName(groupName, vlanID);

    UT_LOG_DEBUG("get_vlanId_for_GroupName API returns:%d", result);
    UT_ASSERT_EQUAL(result, RETURN_ERR);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Tests the function to retrieve VLAN ID for a given group name when the group name is invalid.
 *
 * This test verifies the behavior of the get_vlanId_for_GroupName() function when an invalid group name is provided.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 103 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console. @n
 *
 * **Test Procedure:** @n
 *| Variation / Step | Description | Test Data | Expected Result | Notes |
 *| :--------------: | ----------- | --------- | --------------- | ----- |
 *|       01         | Invoking get_vlanId_for_GroupName with groupName = "brlan114", vlanID = valid buffer | groupName = "brlan114", vlanID = valid buffer  | RETURN_ERR | Should Fail |
 */
void test_l1_vlan_hal_negative4_get_vlanId_for_GroupName(void)
{
    gTestID = 103;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    char groupName[64] = "brlan114";
    char vlanID[5] = {"\0"};
    int result = 0;

    UT_LOG_DEBUG("Invoking get_vlanId_for_GroupName with invalid groupName: %s and valid vlanID buffer", groupName);
    result = get_vlanId_for_GroupName(groupName, vlanID);

    UT_LOG_DEBUG("get_vlanId_for_GroupName API returns: %d", result);
    UT_ASSERT_EQUAL(result, RETURN_ERR);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief This test case is used to verify the basic invocation of the print_all_vlanId_Configuration function.
 *
 * The objective of this test is to ensure that the print_all_vlanId_Configuration function is invoked correctly and returns RETURN_OK as expected.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 104 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console. @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- | --------------- | ----- |
 * | 01 |Invoke print_all_vlanId_Configuration and check if it succeeds | None | RETURN_OK | Should be successful |
 */
void test_l1_vlan_hal_positive1_print_all_vlanId_Configuration(void)
{
    gTestID = 104;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int result = 0;

    UT_LOG_DEBUG("Invoking print_all_vlanId_Configuration");
    result = print_all_vlanId_Configuration();

    UT_LOG_DEBUG("print_all_vlanId_Configuration API returns :%d", result);
    UT_ASSERT_EQUAL(result, RETURN_OK);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Test case to check the functionality of the _get_shell_outputbuffer function when valid input parameters are provided.
 *
 * This test case verifies the behavior of the _get_shell_outputbuffer function when valid input parameters are passed,
 * and checks if the output values and return status are as expected.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 105 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description                                                                                                      | Test Data                                                                  | Expected Result| Notes                |
 * | :----:           | -----------------------------------------------------------------------------------------------------------------| ---------------------------------------------------------------------------| -------------- | -------------------- |
 * | 01               | Invoking _get_shell_outputbuffer with cmd = valid linux shell command, out = valid buffer, len = valid value | cmd = valid linux shell command, out = valid buffer, len = valid value | None           | Should be successful |
 */
void test_l1_vlan_hal_positive1_get_shell_outputbuffer(void)
{
    gTestID = 105;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    char cmd[512] = "brctl show | grep -w brlan0";
    char out[512] = {"\0"};
    int len = 512;

    UT_LOG_DEBUG("Invoking _get_shell_outputbuffer with valid cmd : %s, out : valid buffer and len : valid value", cmd);
    _get_shell_outputbuffer(cmd, out, len);

    UT_LOG_DEBUG("Output String = %s\n and Length of the output string = %d", out, len);

    if (len <= 512)
    {
        UT_LOG_DEBUG("Length of output string is = %d which is a valid value", len);
        UT_PASS("Output string length validation success");
    }
    else
    {
        UT_LOG_DEBUG("Length of output string is = %d which is an invalid value", len);
        UT_PASS("Output string length validation failed");
    }
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Test case to check the behavior when passing a NULL command to _get_shell_outputbuffer function
 *
 * This test case verifies the behavior of the _get_shell_outputbuffer function when a NULL command is passed.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 106 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- | --------------- | ----- |
 * | 01 | Invoking _get_shell_outputbuffer cmd = NULL, out = valid buffer, len = valid value | cmd = NULL, out = valid buffer, len = valid value | None | Should Fail |
 */
void test_l1_vlan_hal_negative1_get_shell_outputbuffer(void)
{
    gTestID = 106;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    char *cmd = NULL;
    char out[512] = {"\0"};
    int len = 512;

    UT_LOG_DEBUG("Invoking _get_shell_outputbuffer with invalid cmd : NULL, out : valid buffer and len : valid value");
    _get_shell_outputbuffer(cmd, out, len);

    UT_PASS("_get_shell_outputbuffer validation success");
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Test case to check the behavior of _get_shell_outputbuffer function when the out buffer is NULL.
 *
 * This test case checks if the _get_shell_outputbuffer function handles the NULL out buffer properly and does not result in any undefined behavior.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 107 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description                                                                                       | Test Data                                                  | Expected Result | Notes        |
 * | :----:           | --------------------------------------------------------------------------------------------------| ---------- ------------------------------------------------|--------------   | ------------ |
 * | 01               | Invoking _get_shell_outputbuffer with  cmd = valid linux shell command, out = NULL, len = 512 | cmd = valid linux shell command, out = NULL, len = 512 | None            | Should Fail  |
 */
void test_l1_vlan_hal_negative2_get_shell_outputbuffer(void)
{
    gTestID = 107;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    char cmd[512] = "brctl show | grep -w brlan0";
    char *out = NULL;
    int len = 512;

    UT_LOG_DEBUG("Invoking _get_shell_outputbuffer with valid cmd : %s, invalid out : NULL and len : valid value");
    _get_shell_outputbuffer(cmd, out, len);

    UT_PASS("_get_shell_outputbuffer validation success");
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Test the function _get_shell_outputbuffer_res with positive input values
 *
 * This test case is used to verify the correctness of the _get_shell_outputbuffer_res function when provided with valid input values.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 108 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoking _get_shell_outputbuffer_res with fp = Valid pointer, out = valid buffer, len = valid value | fp = Valid pointer, out = valid buffer, len = valid value  | None | Should be successful |
 */
void test_l1_vlan_hal_positive1_get_shell_outputbuffer_res(void)
{
    gTestID = 108;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);
    char out[512] = {"\0"};
    int len = 512;
    FILE *fp = popen("ls", "r");
    if (fp != NULL)
    {
        UT_LOG_DEBUG("Invoking _get_shell_outputbuffer_res with valid fp, out : valid buffer and len : valid value");
        _get_shell_outputbuffer_res(fp, out, len);

        UT_LOG_DEBUG("Output String = %s\n and Length of the output string = %d", out, len);
        if (len <= 512)
        {
            UT_LOG_DEBUG("Length of output string is = %d which is a valid value", len);
            UT_PASS("Output string length validation success");
        }
        else
        {
            UT_LOG_DEBUG("Length of output string is = %d which is an invalid value", len);
            UT_PASS("Output string length validation failed");
        }
    }
    else
    {
        UT_LOG_DEBUG("popen operation failed");
        UT_FAIL("Fails to open the pipe");
    }
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Test case to validate the behavior of _get_shell_outputbuffer_res function when file pointer is NULL.
 *
 * This test case checks whether the _get_shell_outputbuffer_res function behaves as expected when the file pointer is NULL. It verifies the return status of the function.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 109 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- | -------------- | ----- |
 * | 01 | Invoking _get_shell_outputbuffer_res function with fp = NULL, out = valid buffer, len = valid value  | out = valid buffer, len = valid value | None | Should Fail |
 */
void test_l1_vlan_hal_negative1_get_shell_outputbuffer_res(void)
{
    gTestID = 109;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);
    FILE *fp = NULL;
    char out[512] = {"\0"};
    int len = 512;

    UT_LOG_DEBUG("Invoking _get_shell_outputbuffer_res with invalid fp : NULL, out : valid buffer and len : valid value");
    _get_shell_outputbuffer_res(fp, out, len);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief Test case to verify the behavior of the _get_shell_outputbuffer_res function when the output buffer is NULL.
 *
 * The _get_shell_outputbuffer_res function is called with a NULL output buffer and a valid file pointer. The objective of this test
 * is to verify that the function handles this scenario correctly.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 110 @n
 * **Priority:** High @n@n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console. @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- | -------------- | ----- |
 * | 01 | Invoking _get_shell_outputbuffer_res function with fp = Valid FILE pointer, out = NULL, len = Valid value | fp = Valid FILE pointer, out = NULL, len = Valid value | None | should be successful |
 */
void test_l1_vlan_hal_negative2_get_shell_outputbuffer_res(void)
{
    gTestID = 110;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    char *out = NULL;
    int len = 512;
    FILE *fp = popen("ls", "r");
    if (fp != NULL)
    {
        UT_LOG_DEBUG("Invoking _get_shell_outputbuffer_res with valid fp, invalid out : NULL and len : valid value");
        _get_shell_outputbuffer_res(fp, out, len);

        UT_LOG_DEBUG("Exiting test_l1_vlan_hal_negative2_get_shell_outputbuffer_res...");
    }
    else
    {
        UT_LOG_DEBUG("popen operation failed");
        UT_FAIL("Fails to open the pipe");
    }
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

static UT_test_suite_t *pSuite = NULL;

/**
 * @brief Register the main tests for this module
 *
 * @return int - 0 on success, otherwise failure
 */
int test_vlan_hal_l1_register(void)
{
    // Create the test suite
    pSuite = UT_add_suite("[L1 vlan_hal]", NULL, NULL);
    if (pSuite == NULL)
    {
        return -1;
    }

    // Add tests to the suite
    UT_add_test(pSuite, "l1_vlan_hal_positive1_addGroup", test_l1_vlan_hal_positive1_addGroup);
    UT_add_test(pSuite, "l1_vlan_hal_positive2_addGroup", test_l1_vlan_hal_positive2_addGroup);
    UT_add_test(pSuite, "l1_vlan_hal_positive3_addGroup", test_l1_vlan_hal_positive3_addGroup);
    UT_add_test(pSuite, "l1_vlan_hal_positive4_addGroup", test_l1_vlan_hal_positive4_addGroup);
    UT_add_test(pSuite, "l1_vlan_hal_negative1_addGroup", test_l1_vlan_hal_negative1_addGroup);
    UT_add_test(pSuite, "l1_vlan_hal_negative2_addGroup", test_l1_vlan_hal_negative2_addGroup);
    UT_add_test(pSuite, "l1_vlan_hal_negative3_addGroup", test_l1_vlan_hal_negative3_addGroup);
    UT_add_test(pSuite, "l1_vlan_hal_negative4_addGroup", test_l1_vlan_hal_negative4_addGroup);
    UT_add_test(pSuite, "l1_vlan_hal_negative5_addGroup", test_l1_vlan_hal_negative5_addGroup);
    UT_add_test(pSuite, "l1_vlan_hal_negative6_addGroup", test_l1_vlan_hal_negative6_addGroup);
    UT_add_test(pSuite, "l1_vlan_hal_negative7_addGroup", test_l1_vlan_hal_negative7_addGroup);
    UT_add_test(pSuite, "l1_vlan_hal_negative8_addGroup", test_l1_vlan_hal_negative8_addGroup);
    UT_add_test(pSuite, "l1_vlan_hal_positive1_delGroup", test_l1_vlan_hal_positive1_delGroup);
    UT_add_test(pSuite, "l1_vlan_hal_positive2_delGroup", test_l1_vlan_hal_positive2_delGroup);
    UT_add_test(pSuite, "l1_vlan_hal_negative1_delGroup", test_l1_vlan_hal_negative1_delGroup);
    UT_add_test(pSuite, "l1_vlan_hal_negative2_delGroup", test_l1_vlan_hal_negative2_delGroup);
    UT_add_test(pSuite, "l1_vlan_hal_positive1_addInterface", test_l1_vlan_hal_positive1_addInterface);
    UT_add_test(pSuite, "l1_vlan_hal_negative1_addInterface", test_l1_vlan_hal_negative1_addInterface);
    UT_add_test(pSuite, "l1_vlan_hal_negative2_addInterface", test_l1_vlan_hal_negative2_addInterface);
    UT_add_test(pSuite, "l1_vlan_hal_negative3_addInterface", test_l1_vlan_hal_negative3_addInterface);
    UT_add_test(pSuite, "l1_vlan_hal_negative4_addInterface", test_l1_vlan_hal_negative4_addInterface);
    UT_add_test(pSuite, "l1_vlan_hal_negative5_addInterface", test_l1_vlan_hal_negative5_addInterface);
    UT_add_test(pSuite, "l1_vlan_hal_negative6_addInterface", test_l1_vlan_hal_negative6_addInterface);
    UT_add_test(pSuite, "l1_vlan_hal_negative7_addInterface", test_l1_vlan_hal_negative7_addInterface);
    UT_add_test(pSuite, "l1_vlan_hal_negative8_addInterface", test_l1_vlan_hal_negative8_addInterface);
    UT_add_test(pSuite, "l1_vlan_hal_negative9_addInterface", test_l1_vlan_hal_negative9_addInterface);
    UT_add_test(pSuite, "l1_vlan_hal_negative10_addInterface", test_l1_vlan_hal_negative10_addInterface);
    UT_add_test(pSuite, "l1_vlan_hal_positive1_delInterface", test_l1_vlan_hal_positive1_delInterface);
    UT_add_test(pSuite, "l1_vlan_hal_negative1_delInterface", test_l1_vlan_hal_negative1_delInterface);
    UT_add_test(pSuite, "l1_vlan_hal_negative2_delInterface", test_l1_vlan_hal_negative2_delInterface);
    UT_add_test(pSuite, "l1_vlan_hal_negative3_delInterface", test_l1_vlan_hal_negative3_delInterface);
    UT_add_test(pSuite, "l1_vlan_hal_negative4_delInterface", test_l1_vlan_hal_negative4_delInterface);
    UT_add_test(pSuite, "l1_vlan_hal_negative5_delInterface", test_l1_vlan_hal_negative5_delInterface);
    UT_add_test(pSuite, "l1_vlan_hal_negative6_delInterface", test_l1_vlan_hal_negative6_delInterface);
    UT_add_test(pSuite, "l1_vlan_hal_negative7_delInterface", test_l1_vlan_hal_negative7_delInterface);
    UT_add_test(pSuite, "l1_vlan_hal_negative8_delInterface", test_l1_vlan_hal_negative8_delInterface);
    UT_add_test(pSuite, "l1_vlan_hal_negative9_delInterface", test_l1_vlan_hal_negative9_delInterface);
    UT_add_test(pSuite, "l1_vlan_hal_positive1_printGroup", test_l1_vlan_hal_positive1_printGroup);
    UT_add_test(pSuite, "l1_vlan_hal_negative1_printGroup", test_l1_vlan_hal_negative1_printGroup);
    UT_add_test(pSuite, "l1_vlan_hal_negative2_printGroup", test_l1_vlan_hal_negative2_printGroup);
    UT_add_test(pSuite, "l1_vlan_hal_negative3_printGroup", test_l1_vlan_hal_negative3_printGroup);
    UT_add_test(pSuite, "l1_vlan_hal_negative4_printGroup", test_l1_vlan_hal_negative4_printGroup);
    UT_add_test(pSuite, "l1_vlan_hal_negative5_printGroup", test_l1_vlan_hal_negative5_printGroup);
    UT_add_test(pSuite, "l1_vlan_hal_negative6_printGroup", test_l1_vlan_hal_negative6_printGroup);
    UT_add_test(pSuite, "l1_vlan_hal_positive1_printAllGroup", test_l1_vlan_hal_positive1_printAllGroup);
    UT_add_test(pSuite, "l1_vlan_hal_positive1_delete_all_Interfaces", test_l1_vlan_hal_positive1_delete_all_Interfaces);
    UT_add_test(pSuite, "l1_vlan_hal_negative1_delete_all_Interfaces", test_l1_vlan_hal_negative1_delete_all_Interfaces);
    UT_add_test(pSuite, "l1_vlan_hal_negative2_delete_all_Interfaces", test_l1_vlan_hal_negative2_delete_all_Interfaces);
    UT_add_test(pSuite, "l1_vlan_hal_negative3_delete_all_Interfaces", test_l1_vlan_hal_negative3_delete_all_Interfaces);
    UT_add_test(pSuite, "l1_vlan_hal_negative4_delete_all_Interfaces", test_l1_vlan_hal_negative4_delete_all_Interfaces);
    UT_add_test(pSuite, "l1_vlan_hal_negative5_delete_all_Interfaces", test_l1_vlan_hal_negative5_delete_all_Interfaces);
    UT_add_test(pSuite, "l1_vlan_hal_negative6_delete_all_Interfaces", test_l1_vlan_hal_negative6_delete_all_Interfaces);
    UT_add_test(pSuite, "l1_vlan_hal_positive1_is_this_group_available_in_linux_bridge", test_l1_vlan_hal_positive1_is_this_group_available_in_linux_bridge);
    UT_add_test(pSuite, "l1_vlan_hal_negative1_is_this_group_available_in_linux_bridge", test_l1_vlan_hal_negative1_is_this_group_available_in_linux_bridge);
    UT_add_test(pSuite, "l1_vlan_hal_negative2_is_this_group_available_in_linux_bridge", test_l1_vlan_hal_negative2_is_this_group_available_in_linux_bridge);
    UT_add_test(pSuite, "l1_vlan_hal_negative3_is_this_group_available_in_linux_bridge", test_l1_vlan_hal_negative3_is_this_group_available_in_linux_bridge);
    UT_add_test(pSuite, "l1_vlan_hal_negative4_is_this_group_available_in_linux_bridge", test_l1_vlan_hal_negative4_is_this_group_available_in_linux_bridge);
    UT_add_test(pSuite, "l1_vlan_hal_negative5_is_this_group_available_in_linux_bridge", test_l1_vlan_hal_negative5_is_this_group_available_in_linux_bridge);
    UT_add_test(pSuite, "l1_vlan_hal_negative6_is_this_group_available_in_linux_bridge", test_l1_vlan_hal_negative6_is_this_group_available_in_linux_bridge);
    UT_add_test(pSuite, "l1_vlan_hal_positive1_is_this_interface_available_in_linux_bridge", test_l1_vlan_hal_positive1_is_this_interface_available_in_linux_bridge);
    UT_add_test(pSuite, "l1_vlan_hal_positive2_is_this_interface_available_in_linux_bridge", test_l1_vlan_hal_positive2_is_this_interface_available_in_linux_bridge);
    UT_add_test(pSuite, "l1_vlan_hal_positive3_is_this_interface_available_in_linux_bridge", test_l1_vlan_hal_positive3_is_this_interface_available_in_linux_bridge);
    UT_add_test(pSuite, "l1_vlan_hal_negative1_is_this_interface_available_in_linux_bridge", test_l1_vlan_hal_negative1_is_this_interface_available_in_linux_bridge);
    UT_add_test(pSuite, "l1_vlan_hal_negative2_is_this_interface_available_in_linux_bridge", test_l1_vlan_hal_negative2_is_this_interface_available_in_linux_bridge);
    UT_add_test(pSuite, "l1_vlan_hal_negative4_is_this_interface_available_in_linux_bridge", test_l1_vlan_hal_negative4_is_this_interface_available_in_linux_bridge);
    UT_add_test(pSuite, "l1_vlan_hal_negative5_is_this_interface_available_in_linux_bridge", test_l1_vlan_hal_negative5_is_this_interface_available_in_linux_bridge);
    UT_add_test(pSuite, "l1_vlan_hal_negative6_is_this_interface_available_in_linux_bridge", test_l1_vlan_hal_negative6_is_this_interface_available_in_linux_bridge);
    UT_add_test(pSuite, "l1_vlan_hal_negative7_is_this_interface_available_in_linux_bridge", test_l1_vlan_hal_negative7_is_this_interface_available_in_linux_bridge);
    UT_add_test(pSuite, "l1_vlan_hal_negative9_is_this_interface_available_in_linux_bridge", test_l1_vlan_hal_negative9_is_this_interface_available_in_linux_bridge);
    UT_add_test(pSuite, "l1_vlan_hal_positive1_is_this_interface_available_in_given_linux_bridge", test_l1_vlan_hal_positive1_is_this_interface_available_in_given_linux_bridge);
    UT_add_test(pSuite, "l1_vlan_hal_negative1_is_this_interface_available_in_given_linux_bridge", test_l1_vlan_hal_negative1_is_this_interface_available_in_given_linux_bridge);
    UT_add_test(pSuite, "l1_vlan_hal_negative2_is_this_interface_available_in_given_linux_bridge", test_l1_vlan_hal_negative2_is_this_interface_available_in_given_linux_bridge);
    UT_add_test(pSuite, "l1_vlan_hal_negative3_is_this_interface_available_in_given_linux_bridge", test_l1_vlan_hal_negative3_is_this_interface_available_in_given_linux_bridge);
    UT_add_test(pSuite, "l1_vlan_hal_negative5_is_this_interface_available_in_given_linux_bridge", test_l1_vlan_hal_negative5_is_this_interface_available_in_given_linux_bridge);
    UT_add_test(pSuite, "l1_vlan_hal_negative6_is_this_interface_available_in_given_linux_bridge", test_l1_vlan_hal_negative6_is_this_interface_available_in_given_linux_bridge);
    UT_add_test(pSuite, "l1_vlan_hal_negative7_is_this_interface_available_in_given_linux_bridge", test_l1_vlan_hal_negative7_is_this_interface_available_in_given_linux_bridge);
    UT_add_test(pSuite, "l1_vlan_hal_negative8_is_this_interface_available_in_given_linux_bridge", test_l1_vlan_hal_negative8_is_this_interface_available_in_given_linux_bridge);
    UT_add_test(pSuite, "l1_vlan_hal_negative9_is_this_interface_available_in_given_linux_bridge", test_l1_vlan_hal_negative9_is_this_interface_available_in_given_linux_bridge);
    UT_add_test(pSuite, "l1_vlan_hal_negative11_is_this_interface_available_in_given_linux_bridge", test_l1_vlan_hal_negative11_is_this_interface_available_in_given_linux_bridge);
    UT_add_test(pSuite, "l1_vlan_hal_negative12_is_this_interface_available_in_given_linux_bridge", test_l1_vlan_hal_negative12_is_this_interface_available_in_given_linux_bridge);
    UT_add_test(pSuite, "l1_vlan_hal_positive1_insert_VLAN_ConfigEntry", test_l1_vlan_hal_positive1_insert_VLAN_ConfigEntry);
    UT_add_test(pSuite, "l1_vlan_hal_positive2_insert_VLAN_ConfigEntry", test_l1_vlan_hal_positive2_insert_VLAN_ConfigEntry);
    UT_add_test(pSuite, "l1_vlan_hal_positive3_insert_VLAN_ConfigEntry", test_l1_vlan_hal_positive3_insert_VLAN_ConfigEntry);
    UT_add_test(pSuite, "l1_vlan_hal_positive4_insert_VLAN_ConfigEntry", test_l1_vlan_hal_positive4_insert_VLAN_ConfigEntry);
    UT_add_test(pSuite, "l1_vlan_hal_negative1_insert_VLAN_ConfigEntry", test_l1_vlan_hal_negative1_insert_VLAN_ConfigEntry);
    UT_add_test(pSuite, "l1_vlan_hal_negative2_insert_VLAN_ConfigEntry", test_l1_vlan_hal_negative2_insert_VLAN_ConfigEntry);
    UT_add_test(pSuite, "l1_vlan_hal_negative3_insert_VLAN_ConfigEntry", test_l1_vlan_hal_negative3_insert_VLAN_ConfigEntry);
    UT_add_test(pSuite, "l1_vlan_hal_negative4_insert_VLAN_ConfigEntry", test_l1_vlan_hal_negative4_insert_VLAN_ConfigEntry);
    UT_add_test(pSuite, "l1_vlan_hal_negative5_insert_VLAN_ConfigEntry", test_l1_vlan_hal_negative5_insert_VLAN_ConfigEntry);
    UT_add_test(pSuite, "l1_vlan_hal_negative6_insert_VLAN_ConfigEntry", test_l1_vlan_hal_negative6_insert_VLAN_ConfigEntry);
    UT_add_test(pSuite, "l1_vlan_hal_negative7_insert_VLAN_ConfigEntry", test_l1_vlan_hal_negative7_insert_VLAN_ConfigEntry);
    UT_add_test(pSuite, "l1_vlan_hal_negative8_insert_VLAN_ConfigEntry", test_l1_vlan_hal_negative8_insert_VLAN_ConfigEntry);
    UT_add_test(pSuite, "l1_vlan_hal_negative9_insert_VLAN_ConfigEntry", test_l1_vlan_hal_negative9_insert_VLAN_ConfigEntry);
    UT_add_test(pSuite, "l1_vlan_hal_positive1_delete_VLAN_ConfigEntry", test_l1_vlan_hal_positive1_delete_VLAN_ConfigEntry);
    UT_add_test(pSuite, "l1_vlan_hal_negative1_delete_VLAN_ConfigEntry", test_l1_vlan_hal_negative1_delete_VLAN_ConfigEntry);
    UT_add_test(pSuite, "l1_vlan_hal_negative2_delete_VLAN_ConfigEntry", test_l1_vlan_hal_negative2_delete_VLAN_ConfigEntry);
    UT_add_test(pSuite, "l1_vlan_hal_negative3_delete_VLAN_ConfigEntry", test_l1_vlan_hal_negative3_delete_VLAN_ConfigEntry);
    UT_add_test(pSuite, "l1_vlan_hal_negative4_delete_VLAN_ConfigEntry", test_l1_vlan_hal_negative4_delete_VLAN_ConfigEntry);
    UT_add_test(pSuite, "l1_vlan_hal_positive1_get_vlanId_for_GroupName", test_l1_vlan_hal_positive1_get_vlanId_for_GroupName);
    UT_add_test(pSuite, "l1_vlan_hal_negative1_get_vlanId_for_GroupName", test_l1_vlan_hal_negative1_get_vlanId_for_GroupName);
    UT_add_test(pSuite, "l1_vlan_hal_negative2_get_vlanId_for_GroupName", test_l1_vlan_hal_negative2_get_vlanId_for_GroupName);
    UT_add_test(pSuite, "l1_vlan_hal_negative3_get_vlanId_for_GroupName", test_l1_vlan_hal_negative3_get_vlanId_for_GroupName);
    UT_add_test(pSuite, "l1_vlan_hal_negative4_get_vlanId_for_GroupName", test_l1_vlan_hal_negative4_get_vlanId_for_GroupName);
    UT_add_test(pSuite, "l1_vlan_hal_positive1_print_all_vlanId_Configuration", test_l1_vlan_hal_positive1_print_all_vlanId_Configuration);
    UT_add_test(pSuite, "l1_vlan_hal_positive1_get_shell_outputbuffer", test_l1_vlan_hal_positive1_get_shell_outputbuffer);
    UT_add_test(pSuite, "l1_vlan_hal_negative1_get_shell_outputbuffer", test_l1_vlan_hal_negative1_get_shell_outputbuffer);
    UT_add_test(pSuite, "l1_vlan_hal_negative2_get_shell_outputbuffer", test_l1_vlan_hal_negative2_get_shell_outputbuffer);
    UT_add_test(pSuite, "l1_vlan_hal_positive1_get_shell_outputbuffer_res", test_l1_vlan_hal_positive1_get_shell_outputbuffer_res);
    UT_add_test(pSuite, "l1_vlan_hal_negative1_get_shell_outputbuffer_res", test_l1_vlan_hal_negative1_get_shell_outputbuffer_res);
    UT_add_test(pSuite, "l1_vlan_hal_negative2_get_shell_outputbuffer_res", test_l1_vlan_hal_negative2_get_shell_outputbuffer_res);

    return 0;
}