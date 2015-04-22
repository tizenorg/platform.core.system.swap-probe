# This script generates api_id_mapping text file from api list

function find_el_in_array(a, el)
{
	for (i in a)
		if (a[i] == el)
			return 1;

	return 0
}

BEGIN {
	api_id = 1
	handled_ids_cnt = 1
	handled_ids[1] = ""
} {
	if (( $0 != ""  ) && ( substr($0,1,1) != "#" )) {
		api_name = $0
		split(api_name, splited, "###")
		if (splited[2] != ""){
			api_name = splited[2]
		}
		split(api_name, splited, ",")
		api_name = splited[1]
		if (find_el_in_array(handled_ids, api_name) == false) {
			printf "%d %s\n", api_id, api_name
			api_id++
			handled_ids[handled_ids_cnt] = api_name
			handled_ids_cnt++
		}
	}
}
