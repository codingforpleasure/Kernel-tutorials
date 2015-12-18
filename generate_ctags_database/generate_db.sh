
#!/bin/sh

LINUX_TREE=/usr/src/linux

COLLECTION_OF_SOURCE_FILES_NAMES=relevant_files_for_db.files

DATABASE_NAME=tags #The default according to man

colorful_echo() 
{
	  local code="\033["
		case "$1" in
					red    ) color="${code}1;31m";;
					green  ) color="${code}1;32m";;
					yellow ) color="${code}1;33m";;
					blue   ) color="${code}1;34m";;
					purple ) color="${code}1;35m";;
					cyan   ) color="${code}1;36m";;
					gray   ) color="${code}0;37m";;
					*) local text="$1"
		esac
		[ -z "$text" ] && local text="$color$2${code}0m"
		echo "$text"
}


# You should add more paths in case you are working on other parts of the kernel tree.
generate_file_names()
{

	colorful_echo green "Attention: Generating a collection of files to be parsed (filename: ${COLLECTION_OF_SOURCE_FILES_NAMES})"

	find "$LINUX_TREE/include/linux/" \
		-name "*.[chxsS]" -print > ./${COLLECTION_OF_SOURCE_FILES_NAMES}

	find "$LINUX_TREE/include/asm-generic" \
		-name "*.[chxsS]" -print >> ./${COLLECTION_OF_SOURCE_FILES_NAMES}

	find "$LINUX_TREE/include/uapi" \
		-name "*.[chxsS]" -print >> ./${COLLECTION_OF_SOURCE_FILES_NAMES}

	find "$LINUX_TREE/ipc/" \
		-name "*.[chxsS]" -print >> ./${COLLECTION_OF_SOURCE_FILES_NAMES}

	find "$LINUX_TREE/net/netlink" \
		-name "*.[chxsS]" -print >> ./${COLLECTION_OF_SOURCE_FILES_NAMES}

	find "$LINUX_TREE/net/netlink" \
		-name "*.[chxsS]" -print >> ./${COLLECTION_OF_SOURCE_FILES_NAMES}

	find "$LINUX_TREE/net/mac80211" \
		-name "*.[chxsS]" -print >> ./${COLLECTION_OF_SOURCE_FILES_NAMES}

	find "$LINUX_TREE/net/wireless" \
		-name "*.[chxsS]" -print >> ./${COLLECTION_OF_SOURCE_FILES_NAMES}

	find "$LINUX_TREE/net/core" \
		-name "*.[chxsS]" -print >> ./${COLLECTION_OF_SOURCE_FILES_NAMES}

	find "$LINUX_TREE/kernel" \
		-name "*.[chxsS]" -print >> ./${COLLECTION_OF_SOURCE_FILES_NAMES}

	find "$LINUX_TREE/kernel" \
		-name "*.[chxsS]" -print >> ./${COLLECTION_OF_SOURCE_FILES_NAMES}
}



# main starts here:
generate_file_names

colorful_echo green "Attention: Generating a database file \"${DATABASE_NAME}\" for lookup symbols..."

# ctag's arguments:
# L => Read from file a list of file names for which tags should be generated
# f => The output filename which would be generated

ctags -L ./${COLLECTION_OF_SOURCE_FILES_NAMES} -f ${DATABASE_NAME}

colorful_echo green "Attention: Now you should add a single line to the .vimrc which refers to this database:"
colorful_echo cyan "           set tags="`pwd`"/tags"
