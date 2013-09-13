# This script generates api_id decode list header from api list
#
# use:
# cat api_names.txt | awk -f ./gen_api_id_mapping_header_list.awk > sample.h
# where api_names.txt is function name list file

BEGIN {
    api_id = 1
    macro_prefix = "API_ID_"
    print "/*"
    print " * this file genereted by <swap-probe> progect with cmd <make headers>"
    print " */"

    print "const char *api_id_list[] = {"
} {
    if ( $0 == "" ) {
	print
    } else {
	orig = $0
	def = orig
	gsub(/[,:()*&~\[\] ]/, "_", def)
	def = macro_prefix def "\","
	printf "\"%-135s//%d %s\n", def, api_id, orig
	api_id = api_id + 1
    }
}

END {
    print "};"
}
