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

extern int get_brname(void);
extern int get_ifname(void);
extern void free_brname(void);
extern void free_ifname(void);
extern int register_hal_l1_tests( void );
extern char** if_Name;
extern int  num_ifName; 
extern char** br_Name;
extern int  num_brName; 

int main(int argc, char** argv)
{
    int registerReturn = 0;
    int i = 0;

    if (get_brname() == 0)
    {
        printf("Got the br_Name values :\n");
        for (i= 0;i < num_brName; i++)
        {
            printf("%s \n", br_Name[i]);
        }
    }
    else
    {
        printf("Failed to get br_Name value\n");
    }
    
    if (get_ifname() == 0)
    {
        printf("Got the if_Name values :\n");
        for (i= 0;i < num_ifName; i++)
        {
            printf("%s \n", if_Name[i]);
        }
    }
    else
    {
        printf("Failed to get if_Name value\n");
    }

    /* Register tests as required, then call the UT-main to support switches and triggering */
    UT_init( argc, argv );
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
    
    free_brname();
    free_ifname();
    return 0;
}

