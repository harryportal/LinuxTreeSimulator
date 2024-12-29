all:
	gcc -g -o unix_tree src/main.c src/file_system.c src/commands.c

clean:
	rm -f unix_tree