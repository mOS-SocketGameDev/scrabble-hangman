PORT=8000

build: 
	@mkdir bin

compile:
	@gcc -o bin/client src/client.c
	@gcc -o bin/server src/server.c

run-server: compile
	@echo "Compiling and running the program..."
	@./bin/server $(PORT)