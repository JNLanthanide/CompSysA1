#!/usr/bin/env bash

# Exit immediately if any command below fails.
set -e

make


echo "Generating a test_files directory.."
mkdir -p test_files
rm -f test_files/*


echo "Generating test files.."
printf "Hello, World!\n" > test_files/ascii.input
printf "Hello, World!" > test_files/ascii2.input
printf "Hello,\x00World!\n" > test_files/data.input
printf "" > test_files/empty.input#!/usr/bin/env bash

# Exit immediately if any command below fails.
set -e

make


echo "Generating a test_files directory.."
mkdir -p test_files
rm -f test_files/*
echo "Generating test files.."
printf "Hello, World!\n" > test_files/ascii.input
printf "Hello, World!" > test_files/ascii2.input
printf "jlaldiawdakowdokawkoda,d,as.d,a,.sd.awdkaowd" > test_files/ascii3.input
printf "\0x27" > test_files/ascii4.input
printf "       hej" > test_files/ascii5.input
printf "\"l\"" > test_files/ascii6.input
printf ";;;;;;," > test_files/ascii7.input
printf "\0x08" > test_files/ascii8.input
printf "\0x07" > test_files/ascii9.input
printf "{n_2} 901 jdiiw" > test_files/ascii10.input
printf "\n test" > test_files/ascii11.input
printf "\\ test" > test_files/ascii12.input

printf "Hello,\x00World!\n" > test_files/data.input
printf "Test \0x06" > test_files/data2.input
printf "\0x06" > test_files/data3.input
printf "\0x0E" > test_files/data4.input
printf "\0x05" > test_files/data5.input
printf "\0x04" > test_files/data6.input
printf "\0x03" > test_files/data7.input
printf "\0x02" > test_files/data8.input
printf "\0x01" > test_files/data9.input
printf "\0x19" > test_files/data10.input
printf "\0x18" > test_files/data11.input
printf "\0x17" > test_files/data12.input

printf "" > test_files/empty.input


echo "Running the tests.."
exitcode=0
for f in test_files/*.input
do
  echo ">>> Testing ${f}.."
  file    "${f}" | sed 's/ASCII text.*/ASCII text/' > "${f}.expected"
  ./file  "${f}" > "${f}.actual"

  if ! diff -u "${f}.expected" "${f}.actual"
  then
    echo ">>> Failed :-("
    exitcode=1
  else
    echo ">>> Success :-)"
  fi
done
exit $exitcode

### TODO: Generate more test files ###


echo "Running the tests.."
exitcode=0
f=test_files/*.input
echo ">>> Testing ${f}.."
file    ${f} | sed 's/ASCII text.*/ASCII text/' > test_files/expected
./file  ${f} > test_files/actual

if ! diff -u test_files/expected test_files/actual
then
  echo ">>> Failed :-("
  exitcode=1
else
  echo ">>> Success :-)"
fi

exit $exitcode
