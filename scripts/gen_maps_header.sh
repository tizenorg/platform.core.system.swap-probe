#!/bin/bash
func_list_file=$1
da_tizen_file=$2
probe_tmp="/tmp/probe_list.tmp"
lib_tmp="/tmp/lib_list.tmp"

echo "generate LD maps header $func_list_file"

function print_libs()
{
    lib="HZ"
    while read line; do
        if [ "$line" != "" ];then
            if [ "${line:0:4}" == "#lib" ];then
                lib="${line#\#lib*\ }"
                lib="${lib#\"*}"
                lib="${lib%*\"}"

                if [ "$lib" == "???" ];then
                    continue
                elif [ "$lib" == "---" ];then
                    continue
                fi
                echo $lib
            fi
        fi
    done < $1
}


function print_all()
{
    lib="HZ"
    while read line; do
        if [ "$line" != "" ];then
            if [ "${line:0:1}" != "#" ];then
                #echo "$lib"
                if [ "$lib" == "???" ];then
                    continue
                elif [ "$lib" == "---" ];then
                    continue
                fi

                echo "$lib $line"
            elif [ "${line:0:4}" == "#lib" ];then
                lib="${line#\#lib*\ }"
                lib="${lib#\"*}"
                lib="${lib%*\"}"
            fi
        fi
    done < $1
}

function generate_libs_probe_list()
{
    while read line; do
        filename=""
        if [ -f "/usr/lib/$line" ];then
            filename="/usr/lib/$line"
        elif [ -f "/lib/$line" ];then
            filename="/lib/$line"
        else
            echo "file not found $line" >&2
            exit 2
        fi

        readelf $filename -sW > /tmp/$line.list
    done < $1

    readelf $da_tizen_file -sW > /tmp/$da_tizen_file.list
}

pr_to=/tmp/res.py
function print_lib_begin()
{
    echo "\"$1\" :[">>$pr_to
}

function print_lib_end()
{
    echo -e "],\n">>$pr_to
}

function print_lib_probe()
{
    addr=$1
    handl_addr=$2
    func=$3
    echo -e "\t{0x$addr, 2, {0x$handl_addr, \"xxxx\", \"p\", \"$func\"}},">>$pr_to
}

function generate_addres_list()
{
    lib=""
    lib_last=""
    while read line; do
        if [ "$line" != "" ];then
            lib="${line%%\ *}"
            func="${line#*\ }"
            #echo "lib=$lib">&2
            #echo "func=$func">&2
            #echo "<$lib> <$lib_last>" >&2

            if [ "$lib_last" != "$lib" ];then
                if [ "$lib_last" != "" ];then
                    print_lib_end
                fi
                lib_last=$lib
                print_lib_begin $lib
            fi

            #addr=`cat /tmp/$lib.list | grep $func | awk '{print $2}' | sort | uniq`
            addr=`cat /tmp/$lib.list | grep " $func\$" | awk '{print $2}'| sort | uniq`
            handl_addr=`cat /tmp/$da_tizen_file.list | grep " $func\$" | awk '{print $2}'| sort | uniq`
            #echo "$func $addr">&2

            if [[ "$handl_addr" == ""  || 0x$handl_addr -eq 0x0 ]];then
                echo "WARNING: func <$func> not found in $da_tizen_file">&2
                continue
            fi

            if [[ "$addr" == "" || 0x$addr -eq 0x0 ]];then
                echo "WARNING: func <$func> not found in $lib">&2
                continue
            fi

            print_lib_probe "$addr" "$handl_addr" "$func"

#            exit 3


#            if [ "${line:0:1}" != "#" ];then
#                #echo "$lib"
#                if [ "$lib" == "???" ];then
#                    continue
#                elif [ "$lib" == "---" ];then
#                    continue
#                fi
#
#                addr=`cat /tmp/$lib.list | grep $line | awk '{print $2}' | sort | uniq`
#                echo "$line $addr">&2
#                exit 3
##                echo "$lib $line"
#
#            elif [ "${line:0:4}" == "#lib" ];then
#                lib="${line#\#lib*\ }"
#                lib="${lib#\"*}"
#                lib="${lib%*\"}"
#            fi
        fi
    done < $1

    print_lib_end

}

rm $lib_tmp
rm $probe_tmp

print_libs $func_list_file | sort -t " " --key=1,1 | uniq  > $lib_tmp
print_all $func_list_file | sort -t " " --key=1,1  > $probe_tmp

generate_libs_probe_list $lib_tmp

#generate_libs_probe_list $tmp
if [ -f $pr_to ];then
rm $pr_to
fi

generate_addres_list $probe_tmp

