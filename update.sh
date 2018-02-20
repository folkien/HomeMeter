#!/bin/bash
revision=$(git rev-parse --short HEAD)
datetime=$(git show -s --format=%ci)
echo "Revision ${revision}, date-time ${datetime}."


cp -rfv version.h.tmpl version.h
# insert changes
sed -i "s/HASHTOREPLACE/${revision}/g" version.h
sed -i "s/DATETOREPLACE/${datetime}/g" version.h
