#!/bin/sh
header_file=$1
debug_info_path="/usr/lib/debug/"
target_debug_path="/lib/ld-linux.so.2.debug"
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

function print_dl_fixup_addr()
{
    echo -e "DL_FIXUP_VALUE_TYPE ARCH_FIXUP_ATTRIBUTE (*dl_fixup_p)(" >> $header_file
    echo -e "# ifdef ELF_MACHINE_RUNTIME_FIXUP_ARGS" >> $header_file
    echo -e "       ELF_MACHINE_RUNTIME_FIXUP_ARGS," >> $header_file
    echo -e "# endif" >> $header_file
    echo -e "       struct link_map *, ElfW(Word)) = (DL_FIXUP_VALUE_TYPE ARCH_FIXUP_ATTRIBUTE (*)(" >> $header_file
    echo -e "# ifdef ELF_MACHINE_RUNTIME_FIXUP_ARGS" >> $header_file
    echo -e "       ELF_MACHINE_RUNTIME_FIXUP_ARGS," >> $header_file
    echo -e "# endif" >> $header_file
    echo -e "       struct link_map *, ElfW(Word))) 0x$(readelf -sW $debug_info_path$target_debug_path | grep $target_function | awk '{print $2}') ;" >> $header_file
}

rm $header_file

print_header_begin
print_dl_fixup_addr
print_header_end
