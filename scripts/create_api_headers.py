#!/usr/bin/python2

import re
import argparse
import os
import sys

def gen_api_id_mapping_header(lines, prefix='API_ID_'):

    H_HEADER = """\
/*
 * this file genereted by <swap-probe> project with cmd <make headers>
 */

#ifndef __API_ID_MAP__
#define __API_ID_MAP__

#ifdef __cplusplus
extern "C"{
#endif

"""

    F_HEADER = """\
#ifdef __cplusplus
}
#endif

#endif /* __API_ID_MAP__ */
"""

    added_functions = []
    header = ''
    index = 1
    for line in lines:
        if len(line) == 0:
            header += '\n'
        elif line[0] == '#':
            header += '/* %s */\n' % line.replace('*', '\*')
        else:
            parts = map(lambda x: x.strip(), line.split(';'))

            if len(parts) != 3:
                header += '//#warning %-135s  //bad string \n' % line

            name = parts[0]
            func_name = re.sub('(\[|\]|\(|\)|:|\*|&|\,)', '_', parts[1].replace(' ', '_'))

            if func_name in added_functions:
                continue

            header += '#define %-135s %d // %s\n' % (prefix + func_name, index, name)

            added_functions.append(func_name);

            index += 1

    return H_HEADER + header + F_HEADER
    
    
def gen_api_id_list_header(lines):

    H_HEADER = """\
/*
 * this file genereted by <swap-probe> project with cmd <make headers>
 */

#ifdef __cplusplus
extern "C"{
#endif

#define X_API_MAP_LIST \\
"""
    
    F_HEADER = """
#ifdef __cplusplus
}
#endif
"""

    header = ''
    i = 0

    data_keys = [
        '#filename',
        '#lib',
        '#feature',
        '#feature_always'
    ]

    added_functions = []
    data = {}
    index = 1
    while i < len(lines):
        line = lines[i]
        if len(line) == 0:
            pass
        elif line[0] == '#':
            parts = filter(lambda x: len(x) != 0, line.split(' '))
            directive = parts[0]
            if len(parts) != 1:
                value = parts[1]

            if directive in data_keys:
                data['#feature'] = '0'
                data['#feature_always'] = '0'

                for k, key in enumerate(data_keys):
                    data[directive] = value
                    i += 1
                    line = lines[i]

                    if len(line) == 0 or line[0] != '#':
                        break

                    directive, value = filter(lambda x: len(x) != 0, line.split(' '))

                if data['#feature'] == '---':
                    data['#feature'] = '0'

                if len(line) == 0 or line[0] != '#':
                    continue

                continue
        else:
            parts = map(lambda x: x.strip(), line.split(';'))
            func_name = parts[1].replace(' ', '')

            if func_name in added_functions:
                i += 1
                continue

            header += 'X(%d, "%s", %s, %s, %s, %s) \\\n' % (index, func_name, data['#filename'], data['#lib'], data['#feature'], data['#feature_always'])
            added_functions.append(func_name)
            index += 1

        i += 1

    return H_HEADER + header + F_HEADER


parser = argparse.ArgumentParser(description='Generate header files.')
parser.add_argument('filename', help='input filename')
parser.add_argument('--out', help='Path to directory where generated headers will be stored. If no path specified, generated headers will be stored in current directory')

args = parser.parse_args()

api_filename = args.filename
out_dir = '.' if args.out is None else args.out

if not os.path.exists(out_dir):
    sys.stderr.write('Directory %s does not exist\n' % os.path.abspath(out_dir))
    exit(1)

with open(api_filename, 'r') as f:
    data = f.read()

api_id_mapping_header_name = 'api_id_mapping.h'
api_id_list_header_name = 'x_define_api_id_list.h'

lines = data.split('\n')

with open(out_dir + '/' + api_id_mapping_header_name, 'w') as out:
    out.write(gen_api_id_mapping_header(lines))

with open(out_dir + '/' + api_id_list_header_name, 'w') as out:
    out.write(gen_api_id_list_header(lines))
