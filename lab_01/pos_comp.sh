#!/bin/bash
cd ./func_tests
../app.exe < "$1" > out.txt
grep -e "[0-9]*\.\?[0-9]\+[eE][+-][0-9]*" -e "[+-]inf" out.txt > results.txt
diff results.txt "$2"
if [[ -z "$(diff results.txt $2)" ]]; then
    exit 0
fi
exit 1

