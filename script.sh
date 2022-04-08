gcc -g main.c -o executable -lcurl -ljson-c `mysql_config --cflags --libs`
./executable
