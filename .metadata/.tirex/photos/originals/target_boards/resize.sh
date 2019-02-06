#!/usr/bin/env bash
#
# Resisze all pictures in folder

set -o errexit

for i in *.[Jj][Pp][Gg]; do
    filename="${i%%.*}"
    filename="$(echo "${filename}" | tr '[:upper:]' '[:lower:]')"
    convert ${i} -resize x300 ${filename}_thumb.png
done
