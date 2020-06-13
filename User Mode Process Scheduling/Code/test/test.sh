#!/bin/sh

NUM_TRIALS=5

rm output.txt

{ echo "1 $i $(time ./busy $i 2>&1)" ; } >> output.txt &
{ echo "2 $i $(time ./busy $i 2>&1)" ; } >> output.txt &
{ echo "3 $i $(time ./busy $i 2>&1)" ; } >> output.txt &
{ echo "4 $i $(time ./busy $i 2>&1)" ; } >> output.txt &
{ echo "5 $i $(time ./busy $i 2>&1)" ; } >> output.txt &
