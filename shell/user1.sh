function check_file_idmap()
{
	if test -f $file
	then

	else
		for file in `ls $1`
		do
		    if [ -d $1"/"$file ]
		    then
		        check_file_idmap $1"/"$file
		    else        
				echo "${file}"
				uid=$(stat -c"%u" ${CONTAINER_PATH}/${CID}/diff/${filename})
				echo "${uid}"
				[[ $uid -ne 100000 ]] && msg_err "${FUNCNAME[0]}:${LINENO} - The uid of the image is not correctly mapped" && ((ret++))
				echo "${gid}"
				gid=$(stat -c"%g" ${CONTAINER_PATH}/${CID}/diff/${filename})
				[[ $gid -ne 100000 ]] && msg_err "${FUNCNAME[0]}:${LINENO} - The gid of the image is not correctly mapped" && ((ret++))
		    fi
		done
	fi
}
