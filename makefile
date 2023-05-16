PORT=8000
IP_ADDRESS := $(shell hostname -I | cut -d' ' -f1)
SERVER_IP := 0

# prints the ip address of this machine and the server port to
# connect to.
log:
	@echo "The IP address of this computer is: $(IP_ADDRESS)"

# creates a bin folder which is used to store all of the
# build c compiled files.
build:
	@mkdir bin

compile:
	@gcc src/client.c src/functions.c -o bin/client
	@gcc src/server.c src/functions.c -o bin/server

# for demonstration in using 2 separate machines.
run-server: compile
	@echo "\033[33m Server IP Address: $(IP_ADDRESS)\033[0m"
	@echo "\033[33m Server Port: $(PORT)\033[0m"
	@./bin/server $(PORT)

# make SERVER_IP= PORT= run-client
run-client:
	@clear
	@./bin/client $(SERVER_IP) $(PORT)
