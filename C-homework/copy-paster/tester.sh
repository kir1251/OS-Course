
if [[ ! -d ./tmp ]]; then
  mkdir ./tmp
fi
head /dev/urandom -c256 > ./tmp/testin.txt

exec 6<> ./tmp/testin.txt 
exec 7<> ./tmp/testout.txt

./$1 6 7;

cat ./tmp/testin.txt
echo "\n"
cat ./tmp/testout.txt
