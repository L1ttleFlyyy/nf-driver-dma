#! /bin/sh
gcc -g   -c -o sendreq.o sendreq.c
gcc   sendreq.o ../common/nf2util.o   -o sendreq