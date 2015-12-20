#!/bin/sh

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

colorful_echo  cyan "\n1) Cleaning the cyclic buffer of dmesg.."
sudo dmesg -c > /dev/null

colorful_echo cyan "\n2) Cleaning leftovers objects..."
make clean > /dev/null

colorful_echo cyan "\n3) Building again..."
make

colorful_echo cyan "\n4) Inserting the module to the kernel"
sudo insmod ./*.ko

colorful_echo cyan "\n5) Unloading the module:"
sudo rmmod ./*.ko

colorful_echo green "\n6) dmesg content:"
dmesg
echo "\n"
