#!/bin/bash

SCRIPT_DIR="$(cd $(dirname "$0") && pwd)"
cd "$SCRIPT_DIR"
cd ci

function info {
    GREEN='\e[32m\e[1m'
    NC='\e[0m'
    printf "$GREEN$1$NC"
}

function error {
    RED='\e[31m\e[1m'
    NC='\e[0m'
    printf "$RED$1$NC"
}

ret=0

for file in *.sh; do
    info "Running test $file   "
    output="$(./$file 2>&1)"
    if [[ $? -eq 0 ]]; then
        info "[ Passed ]\n"
    else
        error "[ Failed ]\n"
        echo -e "$output"
        error "Test $file failed\n"
        ret=1
    fi
done

exit $ret
