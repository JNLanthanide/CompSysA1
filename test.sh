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
printf "" > test_files/empty.input

### TODO: Generate more test files ###
### A0 TESTS-------------------------------------------------------------------
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
## A0 Tests--------------------------------------------------------------------
printf "\0x18" > test_files/data13.input
printf "\0x19" > test_files/data14.input
printf "\0x1A" > test_files/data15.input
printf "\0x1B" > test_files/data16.input
printf "\0x1C" > test_files/data17.input
printf "\0x1D" > test_files/data18.input
printf "\0x1E" > test_files/data19.input
printf "\0x1F" > test_files/data20.input
printf "\0x20" > test_files/data21.input
printf "\0x21" > test_files/data22.input

printf "\xFF\n" > test_files/iso1.input
printf "\xFE\n" > test_files/iso2.input
printf "\xFD\n" > test_files/iso3.input
printf "\xFC\n" > test_files/iso4.input
printf "\xFB\n" > test_files/iso5.input
printf "\xFA\n" > test_files/iso6.input
printf "\xF9\n" > test_files/iso7.input
printf "\xF8\n" > test_files/iso8.input
printf "\xF7\n" > test_files/iso9.input
printf "\xA0\n" > test_files/is10.input
printf "\xF6\n" > test_files/is11.input
printf "\xA1\n" > test_files/is12.input

printf "Ψ\n" > test_files/utf8.input
printf "Ω\n" > test_files/utf82.input
printf "Φ\n" > test_files/utf83.input
printf "Π\n" > test_files/utf84.input
printf "Ο\n" > test_files/utf85.input
printf "Ξ\n" > test_files/utf86.input
printf "Ν\n" > test_files/utf87.input
printf "έ\n" > test_files/utf88.input
printf "ά\n" > test_files/utf89.input
printf "Ϋ\n" > test_files/utf810.input
printf "Ϊ\n" > test_files/utf811.input
printf "ΰ\n" > test_files/utf812.input

printf "\xFF\xFE wkdkadw" > test_files/utf16LE.input
printf "\xFF\xFE " > test_files/utf16LE2.input
printf "\xFF\xFE 32" > test_files/utf16LE3.input
printf "\xFF\xFE {}" > test_files/utf16LE4.input
printf "\xFF\xFE \n" > test_files/utf16LE5.input
printf "\xFF\xFE \x00\x0A" > test_files/utf16LE6.input
printf "\xFF\xFE \xFF" > test_files/utf16LE7.input
printf "\xFF\xFE 00" > test_files/utf16LE8.input
printf "\xFF\xFE \[" > test_files/utf16LE9.input
printf "\xFF\xFE """ > test_files/utf16LE10.input
printf "\xFF\xFE teee          st" > test_files/utf16LE11.input
printf "\xFF\xFE teee   st" > test_files/utf16LE12.input
printf "\xFE\xFF \ddd" > test_files/utf16BE.input
printf "" > test_files/empty.input

printf "\xFE\xFF wkdkadw" > test_files/utf16BE.input
printf "\xFE\xFF " > test_files/utf16BE2.input
printf "\xFE\xFF 32" > test_files/utf16BE3.input
printf "\xFE\xFF {}" > test_files/utf16BE4.input
printf "\xFE\xFF \n" > test_files/utf16BE5.input
printf "\xFE\xFF \x00\x0A" > test_files/utf16BE6.input
printf "\xFE\xFF \xFF" > test_files/utf16BE7.input
printf "\xFE\xFF 00" > test_files/utf16BE8.input
printf "\xFE\xFF \[" > test_files/utf16BE9.input
printf "\xFE\xFF """ > test_files/utf16BE10.input
printf "\xFE\xFF teee          st" > test_files/utf16BE11.input
printf "\xFE\xFF teee   st" > test_files/utf16BE12.input
printf "" > test_files/empty.input

echo "Running the tests.."
exitcode=0
f=test_files/*.input
echo ">>> Testing ${f}.."
file    ${f} | sed 's/ASCII text.*/ASCII text/' > test_files/expected
./file  ${f} > test_files/actual

file "NoPath" > test_files/expected
./file  "NoPath" > test_files/actual

if ! diff -u test_files/expected test_files/actual
then
  echo ">>> Failed :-("
  exitcode=1
else
  echo ">>> Success :-)"
fi

exit $exitcode
