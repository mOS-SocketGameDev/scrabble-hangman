# Scrabble of Death: A Crossover of Hangman and Scrabble

### Game Description:
A socket-based client-server game built in C. Scrabble of Death is a two-player word game that combines the elements of Hangman and Scrabble. One player thinks of a word, and the other player tries to guess it by suggesting letters. The game is played in rounds (4 rounds total: 2 rounds for guessing, 2 rounds for providing a word), where each round starts with a new word that is connected to the previous word by its first letter. The guessing player has 7 attempts to guess the word before the hangman figure is completed, and they lose. A round ends when one player runs out of attempts or guesses all the words correctly. Scrabble of Death is a game of strategy, vocabulary, and risk-taking.

### Prerequisites:

#### Make sure you are using Linux or WSL for compiling (since the project uses `MAKE`):
- First is to make directories such as the bin folders. `make build`
- In running the game, make sure that the server is up and running. To do this use `make run-server`
- Run the client using `make SERVER_IP=<ip_addr> PORT=<port> run-client`.
- After 2 clients joined the game, the game will start.

### Configurations:
```bash
PORT=8000
SERVER_IP=0

# Sample manual run of program...
./bin/server <PORT>
./bin/client <SERVER_IP_ADDRESS> <SERVER_PORT>
```