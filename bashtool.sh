#!/bin/bash

echo pwd=$(pwd)

create_version(){
    # 获取版本信息
    appname=$(cat aliddns_cpp/version.h | grep APP_NAME | awk -F= '{print $2}' | sed 's/["; ]//g'   )
    echo appname=$appname

    version=$(cat aliddns_cpp/version.h | grep APP_VERSION | awk -F = '{print $2}' | sed 's/["; ]//g'   )
    echo version=$version
    # 20261002_120011
    #cur_time=$(date '+%F_%T' | sed 's/://g' | sed 's/-//g')
    cur_time=$(date '+%F' | sed 's/://g' | sed 's/-//g')
    echo cur_time=$cur_time

    tag_name="$version.$cur_time"
    echo tag_name=$tag_name

    name="$appname $tag_name"
}
set_gitenv(){
    if [ -z "$GITHUB_ENV" ];then
        echo "GITHUB_ENV is empty"
        return
    fi

    echo "set GITHUB_ENV"
    echo GITHUB_ENV=$GITHUB_ENV

    echo name=$name | tee -a $GITHUB_ENV
    echo tag_name=$tag_name | tee -a $GITHUB_ENV
    # 这里不包含.zip后缀
    echo zip_name=$appname-$tag_name | tee -a $GITHUB_ENV

    cat $GITHUB_ENV

}

create_version
set_gitenv

