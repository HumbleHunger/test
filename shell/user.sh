#!/bin/bash
#
# attributes: isulad user namespaces remap
# concurrent: YES
# spend time: 4

#######################################################################
##- @Copyright (C) Huawei Technologies., Ltd. 2020. All rights reserved.
# - iSulad licensed under the Mulan PSL v2.
# - You can use this software according to the terms and conditions of the Mulan PSL v2.
# - You may obtain a copy of Mulan PSL v2 at:
# -     http://license.coscl.org.cn/MulanPSL2
# - THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
# - IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
# - PURPOSE.
# - See the Mulan PSL v2 for more details.
##- @Description:CI
##- @Author: liuyuji
##- @Create: 2021-08-20
#######################################################################

source ../helpers.sh

LCR_ROOT_PATH="/var/lib/isulad/100000.100000/engines/lcr"
CONTAINER_PATH="var/lib/isulad/100000.100000/storage/overlay"

function start()
{
	msg_info "test userns_remap starting..."

	check_valgrind_log
	[[ $? -ne 0 ]] && msg_err "${FUNCNAME[0]}:${LINENO} - memory leak before current testcase, please check...." && return ${FAILURE}

	start_isulad_without_valgrind --userns-remap="100000:100000:65535"
}

function test_userns_remap_with_pull_image()
{
	local ret=0
	local image="busybox"
	local test="test_userns_remap_with_pull_image => (${FUNCNAME[@]})"

	msg_info "${test} starting..."

	isula pull ${image}
	[[ $? -ne 0 ]] && msg_err "${FUNCNAME[0]}:${LINENO} - failed to pull image: ${image}" && return ${FAILURE}

	isula images | grep busybox
	[[ $? -ne 0 ]] && msg_err "${FUNCNAME[0]}:${LINENO} - missing list image: ${image}" && ((ret++))

	layer_id=$(isula inspect -f '{{.image.top_layer}}' ${image})

	echo "${layer_id}"

	for file in ${CONTAINER_PATH}/${layer_id}/diff/*
	do
		uid=$(stat -c"%u" ${CONTAINER_PATH}/${CID}/diff/${filename})
		[[ $uid -ne 100000 ]] && msg_err "${FUNCNAME[0]}:${LINENO} - The uid of the image is not correctly mapped" && ((ret++))

		gid=$(stat -c"%g" ${CONTAINER_PATH}/${CID}/diff/${filename})
		[[ $gid -ne 100000 ]] && msg_err "${FUNCNAME[0]}:${LINENO} - The gid of the image is not correctly mapped" && ((ret++))
	done
	
	msg_info "${test} finished with return ${ret}..."
	return ${ret}
}
```
function test_userns_remap_with_create_container()
{

}

function check_root_path()
{
	msg_info "check check_root_path starting..."


}

function check_file_owner()
{

}
```
function test_userns_remap_with_create_file()
{
	local ret=0
	local image="busybox"
	local test="test_userns_remap_with_create_file  => (${FUNCNAME[@]})"
	local filename="test"

	msg_info "${test} starting..."
	
	CID=$(isula run -itd ${image})
	isula exec -it ${CID} touch ${filename}
	[[ $? -ne 0 ]] && msg_err "${FUNCNAME[0]}:${LINENO} - Failed to create file in container" && ((ret++))

	uid=$(stat -c"%u" ${CONTAINER_PATH}/${CID}/diff/${filename})
	[[ $uid -ne 100000 ]] && msg_err "${FUNCNAME[0]}:${LINENO} - The uid is not correctly mapped" && ((ret++))

	gid=$(stat -c"%g" ${CONTAINER_PATH}/${CID}/diff/${filename})
	[[ $gid -ne 100000 ]] && msg_err "${FUNCNAME[0]}:${LINENO} - The gid is not correctly mapped" && ((ret++))

	isula rm -f $CID
	[[ $? -ne 0 ]] && msg_err "${FUNCNAME[0]}:${LINENO} - failed to rm container" && ((ret++))	

	msg_info "${test} finished with return ${ret}..."
	return ${ret}	
}

function check_lcr_config()
{
    msg_info "test userns_remap starting..."

	check_valgrind_log
	[[ $? -ne 0 ]] && msg_err "${FUNCNAME[0]}:${LINENO} - memory leak before current testcase, please check...." && return ${FAILURE}

	start_isulad_without_valgrind --userns-remap="100000:100000:65535"

    containername=test_create
    containerid=`isula run -itd --name $containername busybox`
    fn_check_eq "$?" "0" "create failed"
    testcontainer $containername running
    
    cat "$LCR_ROOT_PATH/$containerid/config"  | grep "lxc.idmap = u 0 100000 65535"
    fn_check_eq "$?" "0" "create failed"

    cat "$LCR_ROOT_PATH/$containerid/config"  | grep "lxc.idmap = g 0 100000 65535"
    fn_check_eq "$?" "0" "create failed"
    
    isula rm -f $containername
    fn_check_eq "$?" "0" "rm failed"

    isula inspect $containername
    fn_check_ne "$?" "0" "inspect should failed"
    
    containerid=`isula run -itd --name $containername --userns="host" busybox`
    fn_check_eq "$?" "0" "create failed"
    testcontainer $containername running
    
    cat "$LCR_ROOT_PATH/$containerid/config"  | grep "lxc.idmap = u 0 100000 65535"
    fn_check_ne "$?" "0" "uidmap should not exist"

    cat "$LCR_ROOT_PATH/$containerid/config"  | grep "lxc.idmap = g 0 100000 65535"
    fn_check_ne "$?" "0" "gidmap should not exist"
    
    isula rm -f $containername
    fn_check_eq "$?" "0" "rm failed"

    isula inspect $containername
    fn_check_ne "$?" "0" "inspect should failed"
    
    return $TC_RET_T
}


declare -i ans=0

start || ((ans++))
test_userns_remap_with_pull_image || ((ans++))
test_userns_remap_with_create_file || ((ans++))
check_lcr_config || ((ans++))




show_result ${ans} "user namespaces remap"
