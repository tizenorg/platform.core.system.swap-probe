#!/usr/bin/python2

import string
import sys
import subprocess
import re
import os
from ctypes import *

defines={}

### use libparseelf.so ###
tIFUNC = 10
tFUNC = 2
class syms(Structure):
    pass

syms._fields_ = [("next", POINTER(syms)),
                ("prev", POINTER(syms)),
                ("name", c_char_p),
                ("addr", c_uint),
                ("type", c_uint)]
parselib = 0

def init_parse_lib():
    global parselib
    parselib = cdll.LoadLibrary("./libparserelf.so")

def check_elf(filename):
    return parselib.fast_check_elf(filename)

def get_all_symbols(filename):
    s = pointer(syms())
    ret = parselib.get_all_symbols(filename, byref(s))
    if ret != 0:
        print "Error in parselib, ret = " + str(ret)
        return None

    res = {}
    entry = s[0]
    while True:
        if entry.addr != 0:
            tmpaddr = hex(entry.addr).rstrip("L").lstrip("0x")
            if entry.type == tFUNC or entry.type == tIFUNC:
                if entry.name not in res:
                    res[entry.name] = ()
                if entry.type == tIFUNC:
                    tmpaddr = "FFFFFFFF"
                res[entry.name] += (tmpaddr.zfill(8), )

        if bool(entry.next):
            entry = entry.next[0]
        else:
            break
    return res
###

def __debug(str):
    print str
    sys.stdout.flush()

def __add_item(data_dict, dict_keys, data):
    """ dict_keys is supposed to be a list with length 2, where the first element is the
        key for data_dict and the second one is the key for data_dict[first_element]
    """

    last_el = 1

    if type(dict_keys[0]) is list:
        for key in dict_keys[0]:
            if data_dict is None:
                data_dict = {key : None}

            if type(dict_keys[len(dict_keys) - 1]) is not list:
                data_dict[key] = data
            else:
                if key not in data_dict:
                    data_dict[key] = {}
                data_dict[key] = __add_item(data_dict[key], dict_keys[last_el], data)
    else:
        if data_dict is None:
            data_dict = {dict_keys[0] : None}

        if type(dict_keys[len(dict_keys) - 1]) is not list:
            data_dict[dict_keys[0]] = data
        else:
            if dict_keys[0] not in data_dict:
                data_dict[dict_keys[0]] = {}
            data_dict[dict_keys[0]] = __add_item(data_dict[dict_keys[0]], dict_keys[last_el], data)

    return data_dict


#################### Parsing api_names_all.txt #####################
libs_in_dir = {}
def __get_libs_in_dir_by_mask(path_mask):
    if path_mask in libs_in_dir:
        return libs_in_dir[path_mask]

    libs_in_dir_tmp = []
    libs_paths = ""

    __debug("---> generate list on mask %s" % path_mask)
    libs_paths = libs_paths + path_mask + " "

    for root, dirnames, filenames in os.walk(os.path.dirname(path_mask)):
        for f in filenames:
            file_full_name = os.path.join(root, f)
            if check_elf(file_full_name) == 0:
                libs_in_dir_tmp.append(os.path.realpath(file_full_name))

    libs_in_dir[path_mask] = libs_in_dir_tmp
    __debug("<--- generate list on mask %s" % path_mask)

    return libs_in_dir_tmp

def __parse_file(libs_data, file):
    __debug("---> __parse_file")
    defines_tag = "#define"
    filename_tag = "#filename"
    lib_tag = "#lib"
    feature_tag = "#feature"

    current_filename = ""
    current_libs = []
    current_feature = ""
    current_line = 0

    for line in file:
        current_line = current_line + 1

        if line == "\n":
            continue
        elif line[:9] == filename_tag:
            current_filename = re.sub("\"", "", line.split()[1])
        elif line[:4] == lib_tag:
            lib_tmp = re.sub("\"", "", line.split()[1])
            current_libs = []
            if os.path.basename(lib_tmp) == "*":
                current_libs = __get_libs_in_dir_by_mask(lib_tmp)
            else:
                current_libs = [lib_tmp]
        elif line[:8] == feature_tag:
            current_feature = re.sub("\"", "", line.split()[1])
        elif line[:7] == defines_tag:
            list = line.split()
            if len(list) < 2 or len(list) > 3:
                __debug("[WARN]: Wrong string in api_names.txt +" + str(current_line) + ": <" + line + ">")
            if len(list) != 3:
                continue

            global defines
            __debug("defines[" + list[1] + "] = " + list[2])
            defines[list[1]] = list[2]
        elif line[:1] == "#":
                #__debug("[WARN]: commented code : <" + line + ">")
                continue
        else:
            splitted = line.split(';')

            if len(splitted) < 3:
                __debug("[WARN]: Wrong string in api_names.txt +" + str(current_line) + ": <" + line + ">")
                continue
            for i in range(0, len(splitted)):
                splitted[i] = re.sub(" ", "", splitted[i])
                splitted[i] = re.sub("\n", "", splitted[i])

            func = splitted[0]
            handler = defines["PROBE_NAME_PREFIX"] + splitted[1]
            probe_type = splitted[2]

            if len(current_libs) == 1 and (current_libs[0] == "???" or current_libs[0] == "---"):
                __debug("[WARN]: Skip func <" + func + "> file <" + current_filename + "> lib <" + current_libs[0] + ">")
                continue

            libs_data = __add_item(libs_data, [current_libs, [func, [current_feature]]], (handler, probe_type))

    __debug("<--- __parse_file")
    return libs_data


def parse_apis(func_list_file):
    __debug("---> parse_apis")
    libs_data = {}

    with open(func_list_file) as f:
        libs_data = __parse_file(libs_data, f)

    __debug("---< parse_apis")
    return libs_data


def parse_probe_lib(da_lib):
    __debug("parse_probe_lib")
    return get_all_symbols(da_lib)

####################################################################

################## Getting function's addresses ####################
def __get_addr_by_funcname_handler(lib_data, funcname):
    if funcname in lib_data:
        return lib_data[funcname]
    return None

def __get_addr_by_funcname_lib(lib_data, funcname):
    result = {}
    if funcname in lib_data:
        result[funcname] = lib_data[funcname]
    else:
        funcname_re = funcname.replace("@*", "$") + "|" + funcname.replace("*", ".*")
        for i in lib_data:
            if re.match(funcname_re, i) != None:
                result[i] = lib_data[i]

    if result == {}:
        return None
    else:
        return result

ERR_NO = 0
ERR_HANDLER_NOT_FOUND_IN_LD_LIB = 1
ERR_HANDLER_NO_FEATURE = 2
ERR_HANDLER_FOUND_BUT_CANNOT_BE_LINKED = 3
function_errors={}

def get_function_search_error(function_name):
    err = function_errors[function_name]
    err_str = "ERROR Unknown error"

    if err == ERR_HANDLER_FOUND_BUT_CANNOT_BE_LINKED:
        err_str = "ERROR function not found in libs"
    elif err == ERR_HANDLER_NO_FEATURE:
        err_str = "ERROR function cannot be set because no feature"
    elif err == ERR_HANDLER_NOT_FOUND_IN_LD_LIB:
        err_str = "ERROR function not found in preload lib"

    err_str = err_str + " func<%s> err#%d"
    return err_str % (function_name, err)

def iterate_over_libs(data, probe_lib):
    __debug("iterate_over_libs")
    feature_dict = {}
    for libname in data:
        lib_data = get_all_symbols(libname)
        if lib_data is None:
            continue
        for funcname in data[libname]:
            addr = __get_addr_by_funcname_lib(lib_data, funcname)
            if funcname not in function_errors:
                function_errors[funcname] = ERR_HANDLER_NO_FEATURE;

            for feature in data[libname][funcname]:
                if function_errors[funcname] == ERR_HANDLER_NO_FEATURE:
                    function_errors[funcname] = ERR_HANDLER_FOUND_BUT_CANNOT_BE_LINKED;

                handler_addr = __get_addr_by_funcname_handler(probe_lib, data[libname][funcname][feature][0])

                if handler_addr is None:
                    function_errors[funcname] = ERR_HANDLER_NOT_FOUND_IN_LD_LIB
                    continue

                if addr is not None:
                    feature_dict = __add_item(feature_dict, [feature, [libname, [funcname]]], (addr, handler_addr, data[libname][funcname][feature][1]))
                    function_errors[funcname] = ERR_NO;
    return feature_dict

####################################################################

####################### Generating headers #########################

def __print_license(file):
    license = "/********************************************************\n"\
              " * !!!!!!!!!!      Autogenerated header       !!!!!!!!!!\n"\
              " * !!!!!!!!!!      do not edit this file      !!!!!!!!!!\n"\
              " *\n"\
              " *  DA probe\n"\
              " *\n"\
              " * Copyright (c) 2000 - `date +%Y` Samsung Electronics Co., Ltd. All rights reserved.\n"\
              " *\n"\
              " * Contact:\n"\
              " *\n"\
              " * Alexander Aksenov <a.aksenov@samsung.com>\n"\
              " *\n"\
              " * This library is free software; you can redistribute it and/or modify it under\n"\
              " * the terms of the GNU Lesser General Public License as published by the\n"\
              " * Free Software Foundation; either version 2.1 of the License, or (at your option)\n"\
              " * any later version.\n"\
              " *\n"\
              " * This library is distributed in the hope that it will be useful, but WITHOUT ANY\n"\
              " * WARRANTY; without even the implied warranty of MERCHANTABILITY or\n"\
              " * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public\n"\
              " * License for more details.\n"\
              " *\n"\
              " * You should have received a copy of the GNU Lesser General Public License\n"\
              " * along with this library; if not, write to the Free Software Foundation, Inc., 51\n"\
              " * Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA\n"\
              " *\n"\
              " * Contributors:\n"\
              " * - Samsung RnD Institute Russia\n"\
              " *\n"\
              " */\n"
    file.write(license)

def __print_include_guard_top(file, defname):
    file.write("#ifndef " + defname + "\n")
    file.write("#define " + defname + "\n")
    file.write("\n")

def __print_include_guard_bottom(file, defname):
    file.write("\n")
    file.write("#endif /* " + defname + " */\n")

def __print_includes(file, includes):
    for include in includes:
        file.write("#include \"" + include + "\"\n")

def __print_feature_begin(file, feature):
    file.write("/******************** " + feature + " ********************/\n")

def __print_feature_end(file, libs_in_feature, feature_lib_list, feature_list):
    libs_cnt = 0

    file.write("struct ld_lib_list_el_t " + feature_lib_list + "[] = {\n")
    for lib in libs_in_feature:
        libs_cnt += 1
        file.write("\t{\"" + lib + "\", " + str(libs_in_feature[lib][0]) + ", " + libs_in_feature[lib][1] + "},\n")

    file.write("}; /* " + feature_lib_list + " */\n")
    file.write("\n")
    file.write("struct ld_feature_list_el_t " + feature_list + " = {\n")
    file.write("\t" + str(libs_cnt) + ", " + feature_lib_list + "\n")
    file.write("}; /* " + feature_list + " */\n")
    file.write("\n")

def __print_lib_begin(file, lib, lib_feature_name):
    file.write("/* " + lib + " */\n")
    file.write("struct ld_probe_el_t " + lib_feature_name + "[] = {\n")

def __print_lib_end(file):
    file.write("};\n")
    file.write("/* ======================================== */\n")
    file.write("\n")

def __print_probes(file, func, addr_list, handler, type):
    probes_cnt = 0
    for lib_func in addr_list:
        for func_addr in addr_list[lib_func]:
            probes_cnt += 1
            file.write("\t\t{0x" + str(func_addr) + ", 0x" + str(handler[0]) + ", " + str(type) +", \"" + str(lib_func) + "\"},\n")

    return probes_cnt

def __print_feature_list(file, features_cnt, features_list_dict):
    file.write("int feature_to_data_count = " + str(features_cnt) + ";\n")
    file.write("struct feature_list_t feature_to_data[] = {\n")
    for feature in features_list_dict:
        file.write("\t{.feature_value_0 = " + feature + ", .feature_value_1 = 0, .feature_ld = &" + features_list_dict[feature] + "},\n")
    file.write("};\n")

def __print_features(file, data):
    features_cnt = 0
    features_list_dict = {}

    for feature in data:
        write_feature = feature
        if write_feature == "---":
            write_feature = "NOFEATURE"
        feature_lib_list = write_feature + "_lib_list"
        feature_list = write_feature + "_list"
        if features_list_dict is None:
            features_list_dict = {write_feature : feature_list}
        else:
            features_list_dict[write_feature] = feature_list

        __print_feature_begin(file, write_feature)
        features_cnt += 1
        libs_in_feature = {}

        for lib in data[feature]:
            # skip /emul/*
            if lib.startswith('/emul/'):
                continue
            lib_feature_name = re.sub("\.", "_", re.sub("-", "__", os.path.basename(lib)))
            lib_feature_name = write_feature + "_" + lib_feature_name
            __print_lib_begin(file, lib, lib_feature_name)
            probes_in_feature = 0

            for func in data[feature][lib]:
                probes_in_feature += __print_probes(file, func, data[feature][lib][func][0], data[feature][lib][func][1], data[feature][lib][func][2])

            if libs_in_feature is None:
                libs_in_feature = {lib : (probes_in_feature, lib_feature_name)}
            else:
                libs_in_feature[lib] = (probes_in_feature, lib_feature_name)
            __print_lib_end(file)

        __print_feature_end(file, libs_in_feature, feature_lib_list, feature_list)

    __print_feature_list(file, features_cnt, features_list_dict)

def __print_probe_el_t(file):
    file.write("struct ld_probe_el_t {\n")
    file.write("\tuint64_t orig_addr;\n")
    file.write("\tuint64_t handler_addr;\n")
    file.write("\tuint8_t probe_type;\n")
    file.write("\tconst char *name;\n")
    file.write("};")

def __print_lib_list_el_t(file):
    file.write("struct ld_lib_list_el_t {\n")
    file.write("\tchar *lib_name;\n")
    file.write("\tuint32_t probe_count;\n")
    file.write("\tstruct ld_probe_el_t *probes;\n")
    file.write("};\n")

def __print_feature_list_el_t(file):
    file.write("struct ld_feature_list_el_t {\n")
    file.write("\tuint32_t lib_count;\n")
    file.write("\tstruct ld_lib_list_el_t *libs\n")
    file.write("};\n")

def __print_feature_list_t(file):
    file.write("struct feature_list_t {\n")
    file.write("\tenum feature_code_0 feature_value_0;\n")
    file.write("\tenum feature_code_1 feature_value_1;\n")
    file.write("\tstruct ld_feature_list_el_t *feature_ld;\n")
    file.write("};\n")

def __print_types(file):
    file.write("#define NOFEATURE 0xFFFFFFFF\n")
    file.write("#define IFUNC_UNDEF_ADDR 0xFFFFFFFF\n")
    file.write("\n")
    __print_probe_el_t(file)
    file.write("\n")
    __print_lib_list_el_t(file)
    file.write("\n")
    __print_feature_list_el_t(file)
    file.write("\n")
    __print_feature_list_t(file)
    file.write("\n")

def __print_probe_lib(file, da_inst_dir, da_lib, probe_lib):
    get_caller_addr = __get_addr_by_funcname_handler(probe_lib, "get_caller_addr")
    if get_caller_addr is None:
        __debug("[WARN]: <get_caller> address is not found!")
        return

    get_call_type_addr = __get_addr_by_funcname_handler(probe_lib, "get_call_type")
    if get_call_type_addr is None:
        __debug("[WARN]: <get_call_type> address is not found!")
        return

    write_msg_addr = __get_addr_by_funcname_handler(probe_lib, "write_msg")
    if write_msg_addr is None:
        __debug("[WARN]: <write_msg> address is not found!")
        return

    file.write("static const char *probe_lib = \"" + da_inst_dir + "/" + da_lib + "\";\n")
    file.write("static unsigned long get_caller_addr = 0x" + get_caller_addr[0] + ";\n")
    file.write("static unsigned long get_call_type_addr = 0x" + get_call_type_addr[0] + ";\n")
    file.write("static unsigned long write_msg_addr = 0x" + write_msg_addr[0] + ";\n")


def generate_headers(dict, da_inst_dir, da_lib, probe_lib):
    c_output="include/ld_preload_probes.h"
    c_output_types="include/ld_preload_types.h"
    c_output_probe_lib="include/ld_preload_probe_lib.h"

    if os.path.isfile(c_output):
        os.remove(c_output)
    if os.path.isfile(c_output_types):
        os.remove(c_output_types)
    if os.path.isfile(c_output_probe_lib):
        os.remove(c_output_probe_lib)

    __debug("----- generate <%s>" % c_output)
    with open(c_output, 'w') as file:
        __print_license(file)
        __print_include_guard_top(file, "__LD_PRELOAD_PROBES__")
        __print_includes(file, (os.path.basename(c_output_types),))
        __print_features(file, dict)
        __print_include_guard_bottom(file, "__LD_PRELOAD_PROBES__")

    __debug("----- generate <%s>" % c_output_types)
    with open(c_output_types, 'w') as file:
        __print_license(file)
        __print_include_guard_top(file, "__LD_PRELOAD_TYPES__")
        __print_types(file)
        __print_include_guard_bottom(file, "__LD_PRELOAD_TYPES__")

    __debug("----- generate <%s>" % c_output_probe_lib)
    with open(c_output_probe_lib, 'w') as file:
        __print_license(file)
        __print_include_guard_top(file, "__LD_PRELOAD_PROBE_LIB__")
        __print_probe_lib(file, da_inst_dir, da_lib, probe_lib)
        __print_include_guard_bottom(file, "__LD_PRELOAD_PROBE_LIB__")


##################################


func_list_file = sys.argv[1]
da_lib = sys.argv[2]
da_inst_dir = sys.argv[3]

init_parse_lib()
data = parse_apis(func_list_file)
probe_lib = parse_probe_lib(da_lib)
feature_dict = iterate_over_libs(data, probe_lib)
generate_headers(feature_dict, da_inst_dir, da_lib, probe_lib)

for i in function_errors:
    if function_errors[i] != ERR_NO:
        __debug("%s" % get_function_search_error(i))
