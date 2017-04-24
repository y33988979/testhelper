#!/bin/sh                                                                       
isExist=`pgrep monitor`
if [ $isExist != ""] ; then
	echo "the monitor.sh is running..."
	exit 0
fi
path="/root/HI3716/logs"
log_size_max=`expr 30 \* 1024`
#log_size_max=$[ 30 * 1024 ]
echo ${log_size_max}
while(true)
do
	cmd=`du -sk $path`
	size=`echo $cmd|awk '{print $1}'`
	echo ${size}
	if [[ $size -gt $log_size_max ]]; then
		cd $path
		echo "rm path!!!!"
		rm $(ls -rt $path | head -10)
		cd -
	fi
	sleep 5
done

