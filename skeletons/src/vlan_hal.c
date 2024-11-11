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
#include <string.h>
#include <stdlib.h>
#include <setjmp.h>
#include "vlan_hal.h"

int vlan_hal_addGroup(const char *groupName, const char *default_vlanID)
{
  /*TODO: Implement Me!*/
  (void)groupName;
  (void)default_vlanID;
  return (int)0;
}

int vlan_hal_delGroup(const char *groupName)
{
  /*TODO: Implement Me!*/
  (void)groupName;
  return (int)0;
}

int vlan_hal_addInterface(const char *groupName, const char *ifName, const char *vlanID)
{
  /*TODO: Implement Me!*/
  (void)groupName;
  (void)ifName;
  (void)vlanID;
  return (int)0;
}

int vlan_hal_delInterface(const char *groupName, const char *ifName, const char *vlanID)
{
  /*TODO: Implement Me!*/
  (void)groupName;
  (void)ifName;
  (void)vlanID;
  return (int)0;
}

int vlan_hal_printGroup(const char *groupName)
{
  /*TODO: Implement Me!*/
  (void)groupName;
  return (int)0;
}

int vlan_hal_printAllGroup(void)
{
  /*TODO: Implement Me!*/
  return (int)0;
}

int vlan_hal_delete_all_Interfaces(const char *groupName)
{
  /*TODO: Implement Me!*/
  (void)groupName;
  return (int)0;
}

int _is_this_group_available_in_linux_bridge(char *br_name)
{
  /*TODO: Implement Me!*/
  (void)br_name;
  return (int)0;
}

int _is_this_interface_available_in_linux_bridge(char *if_name, char *vlanID)
{
  /*TODO: Implement Me!*/
  (void)if_name;
  (void)vlanID;
  return (int)0;
}

int _is_this_interface_available_in_given_linux_bridge(char *if_name, char *br_name, char *vlanID)
{
  /*TODO: Implement Me!*/
  (void)if_name;
  (void)br_name;
  (void)vlanID;
  return (int)0;
}

void _get_shell_outputbuffer(char *cmd, char *out, int len)
{
  /*TODO: Implement Me!*/
  (void)cmd;
  (void)out;
  (void)len;
}

void _get_shell_outputbuffer_res(FILE *fp, char *out, int len)
{
  /*TODO: Implement Me!*/
  (void)fp;
  (void)out;
  (void)len;
}

int insert_VLAN_ConfigEntry(char *groupName, char *vlanID)
{
  /*TODO: Implement Me!*/
  (void)groupName;
  (void)vlanID;
  return (int)0;
}

int delete_VLAN_ConfigEntry(char *groupName)
{
  /*TODO: Implement Me!*/
  (void)groupName;
  return (int)0;
}

int get_vlanId_for_GroupName(const char *groupName, char *vlanID)
{
  /*TODO: Implement Me!*/
  (void)groupName;
  (void)vlanID;
  return (int)0;
}

int print_all_vlanId_Configuration(void)
{
  /*TODO: Implement Me!*/
  return (int)0;
}

