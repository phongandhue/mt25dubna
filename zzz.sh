#!/bin/bash
sed '/# */d' $1 >"$1".mod
sed '/# */d' $2 >"$2".mod
root -b -q "zzz.C(\""$1".mod\",\""$2".mod\")"