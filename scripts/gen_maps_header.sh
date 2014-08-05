#!/bin/bash
func_list_file=$1
da_tizen_file=$2
probe_tmp="/tmp/probe_list.tmp"
lib_tmp="/tmp/lib_list.tmp"

py_output=/tmp/res.py
c_output=/tmp/ld_preload_probes.h
#c_output=/tmp/res.probes.tmp
c_output_feature_list_tmp=/tmp/res_feature_list.tmp
c_output_libs_arr_tmp=/tmp/res_libs_arr.tmp

cur_feature=""
cur_lib=""
cur_func=""
cur_filename=""

last_lib=""
last_feature=""
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

function print_feature_begin_py()
{
    echo -e "\t\t#FEATURE: $1">>$py_output
}

function print_feature_end_py()
{
    echo -e "\t\t#---------------">>$py_output
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

# C genarating
feature_count=0

function print_section_begin_c()
{
#    echo -e "struct some_type arr[] = {\n">>$c_output

    echo -e "/*" >> $c_output
    echo -e "* Autogenerated header" >> $c_output
    echo -e "*/" >> $c_output

    echo -e "#ifndef __LD_PRELOAD_PROBES__" >> $c_output
    echo -e "#define __LD_PRELOAD_PROBES__" >> $c_output

    echo -e "struct ld_probe_el_t{" >> $c_output
    echo -e "\tuint64_t orig_addr;" >> $c_output
    echo -e "\tuint64_t handler_addr;" >> $c_output
    echo -e "};\n" >> $c_output

    echo -e "struct ld_lib_list_el_t{" >> $c_output
    echo -e "\tchar *lib_name;" >> $c_output
    echo -e "\tuint32_t probe_count;" >> $c_output
    echo -e "\tstruct ld_probe_el_t *probes;" >> $c_output
    echo -e "};\n" >> $c_output

    echo -e "struct ld_feature_list_el_t{" >> $c_output
    echo -e "\tuint32_t lib_count;" >> $c_output
    echo -e "\tstruct ld_lib_list_el_t *libs;" >> $c_output
    echo -e "};\n" >> $c_output

    echo -e "struct feature_list_t{" >> $c_output
    echo -e "\tuint64_t feature_value;" >> $c_output
    echo -e "\tstruct ld_feature_list_el_t **feature_ld;" >> $c_output
    echo -e "};\n" >> $c_output

    echo end>&2
}

function print_section_end_c()
{
#    echo -e "}\n">>$c_output

    echo -e "int feature_to_data_count = $feature_count;" >> $c_output
    echo -e "struct feature_list_t feature_to_data[] = {" >> $c_output
    cat $c_output_feature_list_tmp >> $c_output
    echo -e "};" >> $c_output
#    cat $c_output >> $c_output
#    mv $c_output $c_output
    echo -e "#endif __LD_PRELOAD_PROBES__" >> $c_output
    echo end>&2
}

lib_count=0

function print_feature_begin_c()
{

    echo -e "/*************************** $1 *****************************/" >> $c_output

    let lib_count=0
    let feature_count=${feature_count}+1
    feature=$1
    if [ "$cur_feature" == "---" ]; then
        feature="0x000000"
    fi

#    echo -e "\t\t{ $feature, /* FEATURE */\n">>$c_output
}


function print_feature_end_c()
{
    feature=$last_feature
    if [ "$feature" == "---" ]; then
        feature="NOFEATURE"
    fi

    echo -e "struct ld_lib_list_el_t ${feature}_lib_list[] = {" >> $c_output
    cat $c_output_libs_arr_tmp >> $c_output
    echo -e "}; /* ${feature}_lib_list */\n" >> $c_output

    echo -e "struct ld_feature_list_el_t ${feature}_list = {" >> $c_output
    echo -e "\t $lib_count, ${feature}_lib_list" >> $c_output
    echo -e "}; /* ${feature}_list */\n" >> $c_output

    echo -e "\t{$feature, (struct ld_feature_list_el_t **)&${feature}_list}," >> $c_output_feature_list_tmp

    rm $c_output_libs_arr_tmp

#    echo -e "\t\t{\n">>$c_output
#    cat $c_output_libs_arr_tmp >> $c_output
#    echo -e "\t\t},\n">>$c_output

#    echo -e "\t\t},\n">>$c_output
}

probe_count=0

function print_lib_begin_c()
{

    let probe_count=0
    let lib_count=$lib_count+1

    feature=$cur_feature
    if [ "$feature" == "---" ]; then
        feature="NOFEATURE"
    fi
    var=${cur_lib##*/}
    var=${var%%.*}
    var=${feature}_${var}

    echo -e "/* \"$1\" */" >> $c_output
    echo -e "struct ld_probe_el_t ${var}_probe_list[] = {">>$c_output

#    echo -e "\t\t\t{ $var, /* lib name */\n">>$c_output
}

function print_lib_end_c()
{
    feature=$last_feature
    if [ "$feature" == "---" ]; then
        feature="NOFEATURE"
    fi
#    var=${last_lib##*/}
#    var=${var%%.*}
#    var=${feature}_${var}

    echo -e "};\n">>$c_output

    echo -e "\t{\"${last_lib}\", $probe_count, ${var}_probe_list}," >> $c_output_libs_arr_tmp
#    echo -e "\t\t\t\t$probe_count, /* probe count */" >> $c_output
#    cat $c_output_pr_list_tmp >> $c_output
#    rm $c_output_pr_list_tmp


#    echo -e "\t\t\t\t${var}_probe_list, /* probe list */" >> $c_output

#    echo -e "\t\t\t},\n">>$c_output

    echo "/* ================================================================ */" >> $c_output



#    if [ -f "$c_output" ]; then
#        rm $c_output
#    fi
}

function print_lib_probe_c()
{
    addr=$1
    handl_addr=$2
    func=$3
    let probe_count=${probe_count}+1
    echo -e "\t\t\t\t{0x$addr, 0x$handl_addr /* \"$func\" */},">>$c_output
}
#####################
# global generation #
#####################
function print_section_begin()
{
    print_section_begin_py
    print_section_begin_c
}

function print_section_end()
{
    print_section_end_py
    print_section_end_c
}

function print_feature_begin()
{
    print_feature_begin_py $1
    print_feature_begin_c $1
}

function print_feature_end()
{
    print_feature_end_py $1
    print_feature_end_c $1
}

function print_lib_begin()
{
    print_lib_begin_py $1
    print_lib_begin_c $1
}

function print_lib_end()
{
    print_lib_end_py
    print_lib_end_c
}

function print_lib_probe()
{
    print_lib_probe_py $1 $2 $3
    print_lib_probe_c $1 $2 $3
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
    filename="hz"
    feature="hz"
    while read line; do
        if [ "$line" != "" ];then
            if [ "${line:0:1}" != "#" ];then
                #echo "$lib"
                if [ "$lib" == "???" ];then
                    echo "WARNING skeep <$line> filename<$filename> lib is <$lib>">&2
                    continue
                elif [ "$lib" == "---" ];then
                    echo "WARNING skeep <$line> filename<$filename> lib is <$lib>">&2
                    continue
                fi

                echo "$feature $lib $line $filename"
            elif [ "${line:0:5}" == "#lib " ];then
                lib="${line#\#lib*\ }"
                lib="${lib#\"*}"
                lib="${lib%*\"}"
            elif [ "${line:0:9}" == "#filename" ];then
                filename="${line#\#filename*\ }"
                filename="${filename#\"*}"
                filename="${filename%*\"}"
            elif [ "${line:0:9}" == "#feature " ];then
                feature="${line#\#feature*\ }"
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

function get_elem()
{
    count=0;
    n=$1
    st=$2
    for i in $st; do
        let count=$count+1
        if [ n -eq count ];then
            return 
        fi
    done;
}

function generate_addres_list()
{

    print_section_begin

    while read line; do
        if [ "$line" != "" ];then
            cur_feature=`echo $line | awk '{print $1}'`
            cur_lib=`echo $line | awk '{print $2}'`
            cur_func=`echo $line | awk '{print $3}'`
            cur_filename=`echo $line | awk '{print $4}'`

            if [ "$last_feature" != "$cur_feature" ];then
                if [ "$last_feature" != "" ];then
                    print_lib_end
                    print_feature_end
                fi
                last_feature=$cur_feature
                last_lib=""
                print_feature_begin $cur_feature
            fi

            if [ "$last_lib" != "$cur_lib" ];then
                if [ "$last_lib" != "" ];then
                    print_lib_end
                fi
                last_lib=$cur_lib
                print_lib_begin `get_real_path $cur_lib`
            fi

            addr=`cat /tmp/$cur_lib.list | grep -e " $cur_func\$" | awk '{print $2}'| sort | uniq`

            if [[ "$addr" == "" || 0x$addr -eq 0x0 ]];then
                addr=`cat /tmp/$cur_lib.list | grep -e " $cur_func@@" | awk '{print $2}'| sort | uniq`
            fi

            handl_addr=`cat /tmp/$da_tizen_file.list | grep " $cur_func\$" | awk '{print $2}'| sort | uniq`

            if [[ "$handl_addr" == ""  || 0x$handl_addr -eq 0x0 ]];then
                echo "WARNING: func <$cur_func> not found in $da_tizen_file file<$cur_filename>">&2
                continue
            fi

            if [[ "$addr" == "" || 0x$addr -eq 0x0 ]];then
                echo "WARNING: func <$cur_func> not found in $cur_lib. addr=$addr file<$cur_filename>">&2
                continue
            fi

            print_lib_probe "$addr" "$handl_addr" "$cur_func"

        fi
    done < $1

    print_lib_end
    print_feature_end
    print_section_end

}

rm $lib_tmp
rm $probe_tmp

rm $py_output
rm $c_output

rm $c_output_libs_arr_tmp
#rm $c_output_pr_list_tmp
rm $c_output_feature_list_tmp

print_libs $func_list_file | sort -t " " --key=1,1 | uniq  > $lib_tmp
print_probes $func_list_file | sort -t " " --key=1,2  > $probe_tmp
generate_libs_probe_list $lib_tmp

generate_addres_list $probe_tmp
