#!/bin/bash
 ret=0
 test="check_the_management_directory_for_userns_remap => (${FUNCNAME[@]})"
	ISULAD_ROOT_PATH="/var/lib/isulad/100000.100000"
	msg_info "${test} starting..."

	uid=$(stat -c"%u" ${ISULAD_ROOT_PATH}/engines)
	[[ $uid -ne 100000 ]] && msg_err "${FUNCNAME[0]}:${LINENO} - The uid of the engines directory is not correctly mapped" && ((ret++))
	gid=$(stat -c"%g" ${ISULAD_ROOT_PATH}/engines)
	[[ $gid -ne 100000 ]] && msg_err "${FUNCNAME[0]}:${LINENO} - The gid of the engines directory is not correctly mapped" && ((ret++))

	uid=$(stat -c"%u" ${ISULAD_ROOT_PATH}/storage)
	[[ $uid -ne 100000 ]] && msg_err "${FUNCNAME[0]}:${LINENO} - The uid of the storage directory is not correctly mapped" && ((ret++))
	gid=$(stat -c"%g" ${ISULAD_ROOT_PATH}/storage)
	[[ $gid -ne 100000 ]] && msg_err "${FUNCNAME[0]}:${LINENO} - The gid of the storage directory is not correctly mapped" && ((ret++))
	
	uid=$(stat -c"%u" ${ISULAD_ROOT_PATH}/volumes)
	[[ $uid -ne 100000 ]] && msg_err "${FUNCNAME[0]}:${LINENO} - The uid of the volumes directory is not correctly mapped" && ((ret++))
	gid=$(stat -c"%g" ${ISULAD_ROOT_PATH}/volumes)
	[[ $gid -ne 100000 ]] && msg_err "${FUNCNAME[0]}:${LINENO} - The gid of the volumes directory is not correctly mapped" && ((ret++))

	mod=$(stat -c"%a" ${ISULAD_ROOT_PATH}/mnt)
	[[ $uid -ne 751 ]] && msg_err "${FUNCNAME[0]}:${LINENO} - The permissions of the mnt directory are not set correctly" && ((ret++))

	mod=$(stat -c"%a" ${ISULAD_ROOT_PATH}/mnt/rootfs)
	[[ $uid -ne 751 ]] && msg_err "${FUNCNAME[0]}:${LINENO} - The permissions of the rootfs directory are not set correctly" && ((ret++))

	msg_info "${test} finished with return ${ret}..."
