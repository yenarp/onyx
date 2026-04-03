#!/bin/sh
sed -E \
    -e '/^\s*\.(file|text|globl|p2align|type|size|cfi_|ident|addrsig|section|note|debug|loc|Lfunc|Ltmp|Lcu_|Lstr_|Lline_|Linfo_|Laddr_|Ldebug_|intel_syntax)/d' \
    -e '/^\s*#/d' \
    -e 's/\.L\.([a-zA-Z0-9_]+)/L__\1/g' \
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
    -e 's/(byte|word|dword|qword)\s+ptr/\1/gi'
