#!/bin/sh

PREFIX=$(echo "$1" | tr '/' '_')
if [ -z "$PREFIX" ]; then
    PREFIX="global"
fi

sed -E \
    -e '/^\s*\.(file|text|globl|p2align|type|size|cfi_|ident|addrsig|section|note|debug|loc|Lfunc|Ltmp|Lcu_|Lstr_|Lline_|Linfo_|Laddr_|Ldebug_|intel_syntax)/d' \
    -e '/^\s*#/d' \
    -e 's/\.LBB/.@@BB/g' \
    -e 's/\.Ltmp/.@@tmp/g' \
    -e "s/\\.L([a-zA-Z0-9_.]+)/L_${PREFIX}_\\1/g" \
    -e 's/\.@@BB/.LBB/g' \
    -e 's/\.@@tmp/.Ltmp/g' \
    -e 's/\\n/\x27, 10, \x27/g' \
    -e 's/\\r/\x27, 13, \x27/g' \
    -e 's/\\t/\x27, 9, \x27/g' \
    -e 's/\\0/\x27, 0, \x27/g' \
    -e 's/^\s*\.asciz\s+"(.*)"/db \x27\1\x27, 0/' \
    -e 's/^\s*\.byte\s+/db /g' \
    -e 's/^\s*\.short\s+/dw /g' \
    -e 's/^\s*\.long\s+/dd /g' \
    -e 's/^\s*\.quad\s+/dq /g' \
    -e '/^\s*$/d' \
    -e 's/#/;/g' \
    -e 's/^\s*\.l?comm\s+([a-zA-Z0-9_]+)\s*,\s*([0-9]+).*/\1: rb \2/g' \
    -e 's/^\s*\.zero\s+([0-9]+)/times \1 db 0/g' \
    -e 's/(byte|word|dword|qword)\s+ptr/\1/gi'
