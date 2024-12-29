all:
	gcc -g -o unix_tree main.c file_system.c commands.c

clean:
	rm -f unix_tree