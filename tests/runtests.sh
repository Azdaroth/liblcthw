set -e

bldred=${txtbld}$(tput setaf 1) #  red
bldred=${txtbld}$(tput setaf 2) #  green
bldblu=${txtbld}$(tput setaf 4) #  blue

echo "Running unit tests:"

for i in *_tests
do
  if $VALGRIND ./$i 2>> tests.log
  then
    echo $i PASS
  else
    echo "ERROR in test $i: here's tests.log"
    echo "------"
    tail tests.log
    exit 1 
  fi
done

echo ""