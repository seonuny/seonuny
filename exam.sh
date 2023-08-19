#!/bin/bash

RESULT_STR="abc-!234<de[ab](ac)<ad>"
echo ${RESULT_STR}
RESULT_STR=$(echo ${RESULT_STR} | sed -e 's/\([(){}<>!-]\)/\\\1/g')
echo ${RESULT_STR}
