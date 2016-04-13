#!/bin/sh
header_file=$1
target_path=$2
debug_postfix=$3
target_debug_path=$target_path$debug_postfix
debug_info_path="/usr/lib/debug/"
target_function="_dl_fixup"

function print_header_begin()
{
    echo -e "/***************************************************" >> $header_file
    echo -e " *   Autogenerated header by gen_linker_header.sh  *" >> $header_file
    echo -e " ***************************************************/" >> $header_file
    echo -e "#ifndef __LINKER_INFO_H__" >> $header_file
    echo -e "#define __LINKER_INFO_H__\n" >> $header_file
}

function print_header_end()
{
    echo -e "#endif /* __LINKER_INFO_H__ */" >> $header_file
}

function print_linker_path()
{
    echo -e "const char linker_path[] = \""$target_path"\";" >> $header_file
}

function print_dl_fixup_addr()
{
    echo -e "unsigned long dl_fixup_off = 0x$(readelf -sW $debug_info_path$target_debug_path | grep $target_function | awk '{print $2}') ;" >> $header_file
}

rm $header_file

print_header_begin
print_linker_path
print_dl_fixup_addr
print_header_end
