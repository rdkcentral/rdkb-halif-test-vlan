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

#include <ut.h>
#include <ut_log.h>
#include <stdlib.h>
#include "cJSON.h"

extern int register_hal_l1_tests(void);
extern char **br_Name;
extern int num_brName;
extern char **if_Name;
extern int num_ifName;
extern char **invalid_brName;
extern int num_invalid_brName;
extern char **valid_vlanid;
extern int num_vlanid;

int main(int argc, char **argv)
{
    int registerReturn = 0;
    /* Register tests as required, then call the UT-main to support switches and triggering */
    UT_init(argc, argv);
    /* Check if tests are registered successfully */
    registerReturn = register_hal_l1_tests();
    if (registerReturn == 0)
    {
        printf("register_hal_l1_tests() returned success");
    }
    else
    {
        printf("register_hal_l1_tests() returned failure");
        return 1;
    }

    /* Begin test executions */
    UT_run_tests();

    for (int i = 0; i < num_brName; i++)
    {
        free(br_Name[i]);
    }
    free(br_Name);

    for (int i = 0; i < num_ifName; i++)
    {
        free(if_Name[i]);
    }
    free(if_Name);
    for (int i = 0; i < num_vlanid; i++)
    {
        free(valid_vlanid[i]);
    }
    free(valid_vlanid);
    for (int i = 0; i < num_invalid_brName; i++)
    {
        free(invalid_brName[i]);
    }
    free(invalid_brName);
    return 0;
}
