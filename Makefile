CC=gcc

CFLAGS=-Wextra -Werror -g -std=c11
LDFLAGS=-lm

all: professor aluno exec grade clean

professor: raidfs.c test.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o raidfs raidfs.c test.c -lm

aluno: raidfs.c test_aluno.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o aluno raidfs.c test_aluno.c -lm

exec: aluno
	./aluno

grade: raidfs
	./raidfs

clean:
	rm -rf *.o aluno
