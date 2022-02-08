# Wordle Clone

## Specifications

* Requres ncurses.
* `make` to compile.
* `./server` to run a server. Optional parameter `-show` will print the daily word when the server is run.
* `./client` to run a client. Optional first parameter is the IP that it'll connect to. By default it connects to localhost.
* Make sure that you run everything in the same folder or that you have the required files in the folder that you run everything in.
  * Server requires `answer_words.txt` and will create files named `server_stats.bin` and `daily_word.txt`
  * Client requires `all_words.txt` and will create a file named `client_stats.bin`.

## Project proposal

* Name: Willow Veytsman
* Period: 5
* Description: A clone of the hit game [wordle](https://www.powerlanguage.co.uk/wordle/) in which players attempt to guess a five-letter word using mastermind-like game mechanics.
* User interface: Probably in the terminal with ncurses or a similar library. Possibly using graphics (SDL or whatever)
* Technical design:
	* A server will host the game and keep track of statistics, sending the same word out to every client every day. The previous information and the statistics will be stored in a file.
	* Clients will connect to the server to fetch the word of the day. Once the player finishes, they will then send the statistics. The client may save a file with the most recent word, to make sure the same game isn't played multiple times. This information may also be saved on the server.
	* This uses: Sockets, processes (forking for server), working with files, and allocating memory
* Timeline (very approximate):
	* 1 day - create code to choose a daily word, save it in a file, and refuse to choose a new word until the next day
	* 1 day - create server and client code for sending info back and forth
	* 2 days - user interface for client, and code to store statistics
	* 1 day - finishing touches

