#!/bin/bash
func_list_file=$1
da_tizen_file=$2
probe_tmp="/tmp/probe_list.tmp"
lib_tmp="/tmp/lib_list.tmp"

py_output=/tmp/res.py

###############################################
#              Print functions                #
###############################################

# python genarating
function print_section_begin_py()
{
    echo -e "lib_inst_list = [\n\t{\n">>$py_output
}

function print_section_end_py()
{
    echo -e "\t\t}\n]\n">>$py_output
}

function print_lib_begin_py()
{
    echo -e "\t\t\"$1\" :[">>$py_output
}

function print_lib_end_py()
{
    echo -e "\t\t],\n">>$py_output
}

function print_lib_probe_py()
{
    addr=$1
    handl_addr=$2
    func=$3
    echo -e "\t\t\t(0x$addr, 2, (0x$handl_addr, \"xxxx\", \"p\", \"$func\")),">>$py_output
}

#####################
# global generation #
#####################
function print_section_begin()
{
    print_section_begin_py
}

function print_section_end()
{
    print_section_end_py
}

function print_lib_begin()
{
    print_lib_begin_py $1
}

function print_lib_end()
{
    print_lib_end_py
}

function print_lib_probe()
{
    print_lib_probe_py $1 $2 $3
}
###############################################
#              Generate functions             #
###############################################
function get_real_path()
{
    lib=$1
    if [ -f "/usr/lib/$lib" ];then
        lib="/usr/lib/$lib"
    elif [ -f "/lib/$lib" ];then
        lib="/lib/$lib"
    else
        echo "file not found <$lib>" >&2
        exit 2
    fi
    echo $lib
}

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

                lib=`get_real_path $lib`
                echo $lib
            fi
        fi
    done < $1
}

function print_probes()
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
    while read filename; do
        short=${filename##*/}
        readelf $filename -sW > /tmp/$short.list
    done < $1

    readelf $da_tizen_file -sW > /tmp/$da_tizen_file.list
}


function generate_addres_list()
{
    lib=""
    lib_last=""

    print_section_begin

    while read line; do
        if [ "$line" != "" ];then
            lib="${line%%\ *}"
            func="${line#*\ }"

            if [ "$lib_last" != "$lib" ];then
                if [ "$lib_last" != "" ];then
                    print_lib_end
                fi
                lib_last=$lib
                print_lib_begin `get_real_path $lib`
            fi

            addr=`cat /tmp/$lib.list | grep " $func\$" | awk '{print $2}'| sort | uniq`
            handl_addr=`cat /tmp/$da_tizen_file.list | grep " $func\$" | awk '{print $2}'| sort | uniq`

            if [[ "$handl_addr" == ""  || 0x$handl_addr -eq 0x0 ]];then
                echo "WARNING: func <$func> not found in $da_tizen_file">&2
                continue
            fi

            if [[ "$addr" == "" || 0x$addr -eq 0x0 ]];then
                echo "WARNING: func <$func> not found in $lib">&2
                continue
            fi

            print_lib_probe "$addr" "$handl_addr" "$func"

        fi
    done < $1

    print_lib_end
    print_section_end

}

rm $lib_tmp
rm $probe_tmp
if [ -f $py_output ];then
rm $py_output
fi

print_libs $func_list_file | sort -t " " --key=1,1 | uniq  > $lib_tmp
print_probes $func_list_file | sort -t " " --key=1,1  > $probe_tmp
generate_libs_probe_list $lib_tmp

generate_addres_list $probe_tmp
