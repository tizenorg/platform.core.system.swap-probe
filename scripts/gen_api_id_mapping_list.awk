# This script generates api_id_mapping text file from api list

BEGIN {
	api_id = 1
	tags[1] = "#filename "
	tags[2] = "#lib "
	tags[3] = "#feature "
	tags[4] = "#feature_always "
	cur[1] = ""
	cur[2] = ""
	cur[3] = ""
	cur[4] = ""

	print "/*"
	print " * this file genereted by <swap-probe> project with cmd <make headers>"
	print " */"
	print ""
	print "#ifdef __cplusplus"
	print "extern \"C\"{"
	print "#endif"
	print ""
	print "#define X_API_MAP_LIST \\"

} {
	if ( $0 != ""  ) {
		if ( substr($0,1,1) == "#" ) {
			#-> #filename
			#-> #lib
			#-> #feature
			for (i = 1; i <= 4; i = i + 1) {
				tag = tags[i]
				if (substr($0,1,length(tag)) == tag) {
					split($0, splited, tag)
					#printf "%s=%s\n", tag, splited[2]
					cur[i] = splited[2]
					if (i == 1) {
						cur[2] = "\"???\""
						cur[3] = "0"
						cur[4] = "0"
					} else if ((i == 3 || i == 4) && cur[i] == "---") {
						cur[i] = "0"
					}
				}
			}
			#printf "%s\n", $0
		} else {
			api_name = $0
			split(api_name, splited, "###")
			if (splited[2] != ""){
				api_name = splited[2]
			}
			printf "X(%d, \"%s\", %s, %s, %s, %s) \\\n", api_id, api_name, cur[1], cur[2], cur[3], cur[4]
			api_id = api_id + 1
		}
	}
}

END {
	print ""
	print "#ifdef __cplusplus"
	print "}"
	print "#endif"
}
