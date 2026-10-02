#!/bin/bash

echo pwd=$(pwd)

update_date(){
    file="aliddns_cpp/version.h"
    if [ ! -f "$file" ]; then
        echo "file not found: $file"
        exit 1
    fi

    cur_time=$(date '+%F' | sed 's/://g' | sed 's/-//g')
    echo cur_time=$cur_time

    sed -i "s/APP_BUILD_DATE = .*/APP_BUILD_DATE = \"$cur_time\";/" $file

    echo "updated $file"
    cat $file

}

update_date

