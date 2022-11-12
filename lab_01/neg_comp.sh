#!/bin/bash
cd ./func_tests
../app.exe < "$1" > /dev/null 2>&1
rc="$?"
echo "$rc" > out.txt
diff out.txt "$2"
if [[ -z "$(diff out.txt $2)" && "$rc" -ne 0 ]]; then
    exit 0
fi
exit 1

