#!/bin/bash

filename=prog

while read input output; do
    result=`./$filename $input`

    if [ x"$output" != x"$result" ]; then
        echo "TEST '$input' FAILED: expected '$output', got '$result'"
    fi
done <<END
    (((5+3)/2+1)*3)*2-5 25
    1+((5-2)*(5+2))*((3-2)*(3+2)) 106
    (3)+25 28
    2*11 22
    8/2 4
END
