PORT=8001

build: 
	@mkdir bin

compile:
	@gcc src/client.c src/functions.c -o bin/client
	@gcc src/server.c src/functions.c -o bin/server

run-client-1: 
	@./bin/client 127.0.0.1 $(PORT)

run-client-2: 
	@./bin/client 127.0.0.2 $(PORT)

run-server: compile
	@./bin/server $(PORT)