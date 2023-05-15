#!/bin/bash

set -e

if [ -z "$1" ]; then
  echo "Please provide the path to the nginx source as an argument."
  exit 1
fi

module_path=$(pwd)
nginx_path="$1"

# Get the arguments from `nginx -V` minus any --add-dynamic-module args
ngx_args=($(nginx -V 2>&1 | awk -F 'configure arguments: ' '{print $2}' | sed 's/ --add-dynamic-module=[^ ]*//g'))
# join the array elements with spaces, preserving the quotes around arguments
ngx_args_string=$(printf "%s " "${ngx_args[@]}")

cd "$nginx_path"
eval "./configure $ngx_args_string --add-dynamic-module=$module_path"

make modules
sudo cp objs/ngx_http_host_stat_module.so /usr/lib/nginx/modules/

# check nginx configuration
nginx -t