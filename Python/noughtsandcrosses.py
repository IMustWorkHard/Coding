#Ugesh KC
#ID-NP03CS4S240088
import random  #generate random numbers for computer moves
import os.path #os.path checks if a file exists 
import json    #json module for working with JSON data included in leaderboard.txt
random.seed()

# Function to draw the Tic-Tac-Toe board
def draw_board(board):
    """Display the Tic-Tac-Toe board."""
    print("-------------")
    for row in board:
        print("| " + " | ".join(row) + " |")
        print("-------------")

# display welcome messge and board by calling draw_board(board)
def welcome(board):
    """Shows a welcome message and displays the board."""
    print('Welcome to the "Unbeatable Noughts and crosses" game. \nThe board layout is shown below:')
    draw_board(board)
    print('When prompted, enter the number corresponding to the square you want.')
    
# set all elements of the board to one space ' '
def initialise_board(board):
    """Initialise the board with empty spaces' '. """
    for row in range(3):
     for col in range(3):
         board[row][col]=' '
    return board
    
def get_player_move(board):
    """Ask the user for the cell to put the X in and return row and col."""
    while True:
        try:
            move = int(input('Choose your square \n 1 2 3 \n 4 5 6 \n 7 8 9:'))
            if 1 <= move <= 9:
              row = (move-1)//3
              col = (move-1)%3
              if board[row][col] == ' ':
                  board[row][col] = 'X'
                  # return row and col
                  return row,col
              else:
                  print('Sorry!Given spot is already taken.Try another')
            else:
                print('Invalid input. Please enter an integer between 1 and 9.')
        except ValueError:
            print('Invalid input. Your move should be ingteger.')
    
#let the computer chose a cell to put a nought in and return row and col
def choose_computer_move(board):
    while True:
        row = random.randint(0,2)
        col = random.randint(0,2)
        if board[row][col] == ' ':
            board[row][col] = 'O'
            return row, col


def check_for_win(board, mark):
    """Check if the specified mark has won the game."""
    # Check for a win in rows
    for row in range(3):
        if board[row][0] == board[row][1] == board[row][2] == mark:
            return True

    # Check for a win in columns
    for col in range(3):
        if board[0][col] ==  board[1][col] == board[2][col] == mark:
            return True

    # Check for a win in diagonals
    if board[0][0] ==  board[1][1] ==  board[2][2] == mark:
        return True
    if board[0][2] == board[1][1] == board[2][0] == mark:
        return True

    # No win found
    return False

def check_for_draw(board):
    """Check if the game is draw."""
    # develop cope to check if all cells are occupied
    for row in board:
        if any(cell == ' ' for cell in row):
            return False
    return True
        
def play_game(board):
    '''Play the noughts and crosses game.'''
    initialise_board(board)
    draw_board(board)

    for i in range(9):
        row, col = get_player_move(board)
       
        if check_for_win(board, 'X'):
            draw_board(board) 
            print("You won:)")
            return 1
        if check_for_draw(board):
            draw_board(board)  
            print("The game is a draw:|")
            return 0
        draw_board(board) 
       

        row, col = choose_computer_move(board)
        

        if check_for_win(board, 'O'):
            draw_board(board)  # Draw the board after the computer's move
            print("You lost:(")
            return -1
        if check_for_draw(board):
            draw_board(board)  # Draw the board after the computer's move
            print("The game is draw:|")
            return 0
        draw_board(board)
        

    return 0
       
                
def menu():
    """Display the menu and get the user's choice."""
    while True:
        try:
            choice = input("Enter one of the following options:\n1 - Play the game\n2 - Save score in leaderboard\n3 - Load and display the leaderboard\nq - End the program\n1, 2, 3, or q?: ")
            if choice in ['1', '2', '3', 'q']:
                return choice
            else:
                print("Invalid choice. Please enter 1, 2, 3, or q.")
        except Exception as e:
            print(f"An error occurred: {e}")

def load_scores():
    """Load the leaderboard from a file."""
    try:
        with open("leaderboard.txt", 'r') as file:
          leaders = json.load(file)
          return leaders
    except json.decoder.JSONDecodeError:
        print("Error: The file 'leaderboard.txt' does not contain valid JSON data.")
        return {}
    except Exception as e:
        print(f"An error occurred while loading the leaderboard: {e}")
        return {}

    
def save_score(score):
    """Save the player's score in the leaderboard."""
    name = input("Enter your name: ")
    leaders = load_scores()
    leaders[name] = score
    with open('leaderboard.txt', 'w') as f:
        json.dump(leaders, f)


def display_leaderboard(leaders):
    """Display the leaderboard."""
    if leaders:
        print("\nLeaderboard:")
        print("Name\t\tScore")
        for name, score in leaders.items():
            print(f"{name}\t\t{score}")
    else:
        print("Leaderboard is empty.")

