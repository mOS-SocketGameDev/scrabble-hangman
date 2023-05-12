# Scrabble of Death: A Crossover of Hangman and Scrabble

### Game Description:
A socket-based client-server game built in C. Scrabble of Death is a two-player word game that combines the elements of Hangman and Scrabble. One player thinks of a word, and the other player tries to guess it by suggesting letters. The game is played in rounds (4 rounds total: 2 rounds for guessing, 2 rounds for providing a word), where each round starts with a new word that is connected to the previous word by its first letter. The guessing player has 7 attempts to guess the word before the hangman figure is completed, and they lose. A round ends when one player runs out of attempts or guesses all the words correctly. Scrabble of Death is a game of strategy, vocabulary, and risk-taking.

### Prerequisites:

#### Make sure you are using Linux or WSL for compiling (since the project uses `MAKE`):
- First is to make directories such as the bin folders. `make build`
- In running the game, make sure that the server is up and running, waiting for connections from 2 clients. `make run-server`
- Run the client 1 `make run-client-1`
- Run the client 2 `make run-client-2`

### Configurations:
```bash
PORT=8000
CLIENT_1_ADDRESS=127.0.0.1
CLIENT_2_ADDRESS=127.0.0.2

# Manual run of program...
./bin/server 8000
./bin/client 127.0.0.n 8000
```

### Naming Convention:
```c
use lower_case

// sending
int s_category_res = send(client_sock, buff...
int s_category0_res = send(client_sock, buff...
if (s_category_res < 0)
{
    ...
}
```

```c
// recieving
int r_category_res = send(client_sock, buff...
int r_category0_res = send(client_sock, buff...
if (r_category_res < 0)
{
    ...
}
```