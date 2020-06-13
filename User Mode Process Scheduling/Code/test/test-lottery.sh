#!/bin/sh

NUM_TRIALS=5

rm output.txt

{ echo "$i 25 $(time ./busy $i 2>&1)" ; } >> output.txt &
{ echo "$i 50 $(time ./busy $i 2>&1)" ; } >> output.txt &
{ echo "$i 100 $(time ./busy $i 2>&1)" ; } >> output.txt &
{ echo "$i 200 $(time ./busy $i 2>&1)" ; } >> output.txt &
{ echo "$i 400 $(time ./busy $i 2>&1)" ; } >> output.txt &
