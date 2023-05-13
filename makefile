PORT=8000
IP_ADDRESS := $(shell hostname -I | cut -d' ' -f1)

# prints the ip address of this machine and the server port to 
# connect to.
log:
	@echo "The IP address of this computer is: $(IP_ADDRESS)"
	@echo "The Server port is: $(PORT)"

# creates a bin folder which is used to store all of the
# build c compiled files.
build: 
	@mkdir bin

compile:
	@gcc src/client.c src/functions.c -o bin/client
	@gcc src/server.c src/functions.c -o bin/server

# for demonstration in using 2 separate machines.
run-server: compile
	@./bin/server $(PORT)

run-client:
	@clear
	@./bin/client $(IP_ADDRESS) $(PORT)

# for testing only. run this if we're running this on a single machine.
# uses a temp address.
run-client-1: 
	@clear
	@./bin/client 127.0.0.1 $(PORT)

run-client-2: 
	@clear
	@./bin/client 127.0.0.2 $(PORT)