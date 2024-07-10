#!/bin/bash

HELIOS="../HeliosCLI/helios"

FILE=$1
VALIDATE=$2
TESTCOUNT=$3
NUMFILES=$4
QUIET=$5

if [ "$FILE" == "" ]; then
  echo "$0 <test file> <validate> <total tests>"
  exit 1
fi

if [ "$VALIDATE" == "" ]; then
  VALIDATE=0
fi

if [ "$NUMFILES" == "" ]; then
  NUMFILES=1
fi

# Clear the Helios.storage file before running the test
if [[ "$ARGS" == *"-s"* ]]; then
  # Assuming the storage file is in the same directory as the Helios executable
  STORAGE_FILE=$(dirname "$HELIOS")/Helios.storage
  if [ -f "$STORAGE_FILE" ]; then
    rm -f "$STORAGE_FILE"

    # Check if the file was successfully deleted
    if [ -f "$STORAGE_FILE" ]; then
      echo -e "\e[31mERROR: Failed to delete Helios.storage file at $STORAGE_FILE\e[0m"
      exit 1
    fi

    if [ "$QUIET" -eq 0 ]; then
      echo -e "\e[33mCleared Helios.storage before running test with -s argument\e[0m"
    fi
  else
    if [ "$QUIET" -eq 0 ]; then
      echo -e "\e[33mNo Helios.storage file found to clear\e[0m"
    fi
  fi
fi




INPUT="$(grep "Input=" $FILE | cut -d= -f2)"
BRIEF="$(grep "Brief=" $FILE | cut -d= -f2)"
ARGS="$(grep "Args=" $FILE | cut -d= -f2)"
TESTNUM="$(echo $FILE | cut -d/ -f2 | cut -d_ -f1 | cut -d/ -f2)"
TESTNUM=$((10#$TESTNUM))

if [ "$QUIET" -eq 0 ]; then
  echo -e -n "\e[31mRecording test ($TESTCOUNT/$NUMFILES) \e[33m[\e[97m$BRIEF\e[33m] \e[33m[\e[97m$ARGS\e[33m]...\e[0m"
fi
TEMP_FILE="tmp/${FILE}.out"
# Append the output of the $HELIOS command to the temp file
# NOTE: When recording the tests we don't use valgrind because
#       the valgrind output should be clean anyway. But when running
#       the test valgrind is used with --leak-check=full --show-leak-kinds=all
echo "Input=${INPUT}" > "$TEMP_FILE"
echo "Brief=${BRIEF}" >> "$TEMP_FILE"
echo "Args=${ARGS}" >> "$TEMP_FILE"
echo "--------------------------------------------------------------------------------" >> "$TEMP_FILE"

# strip any \r in case this was run on windows
$HELIOS $ARGS --no-timestep --hex <<< $INPUT >> $TEMP_FILE

sed -i 's/\r//g' $TEMP_FILE
# Replace the original file with the modified temp file
mv $TEMP_FILE $FILE
if [ "$QUIET" -eq 0 ]; then
  echo -e "\e[96mOK\e[0m"
else
  echo -n "."
fi
if [ "$VALIDATE" -eq 1 ]; then
  $HELIOS $ARGS --no-timestep --color <<< $INPUT
  echo -e "\e[31mRecorded \e[33m[\e[97m$BRIEF\e[33m] \e[33m[\e[97m$ARGS\e[33m]\e[0m"
  echo -en "${YELLOW}Is this correct? (Y/n):${WHITE} "
  read -e CONFIRM
  if [[ $CONFIRM == [nN] || $CONFIRM == [nN][oO] ]]; then
    exit
  fi
fi
