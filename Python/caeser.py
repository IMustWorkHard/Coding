def welcome():
    """
    Prints the welcome message to declare it is Caesar Cipher
    """
    print("Welcome to the Caesar Cipher")
    print("This program encrypts and decrypts text with the Caesar Cipher.")


def enter_message():
    """
    Asks the user for their desired mode, message, and shift number.
    It returns mode, message in uppercase for handling input error and shift number.
    """
    while True:
        mode = input("Would you like to encrypt (e) or decrypt (d)? : ").lower()
        if mode not in ['e', 'd']:
            print("Invalid Mode")
            continue
        if mode == 'e':
            message = input(f"What message would you like to {mode}ncrypt: ").upper()
        else:
            message = input(f"What message would you like to {mode}ecrypt: ").upper()
        while True:
            shift = input("What is the shift number: ")
            if not shift.isdigit() or not 0 <= int(shift) <= 25:
                print("Invalid Shift")
                continue
            shift = int(shift)
            break
        return mode, message, shift

    
def encrypt(text, shift):
    """
    Encrypts the text using a Caesar cipher.

    Args:
        text (str): The text to be encrypted.
        shift (int): The number of positions to shift each character.

    Returns:
        str: The encrypted text.
    """
    result = ''
    for char in text:
        if 'A' <= char <= 'Z':
            shifted = ord(char) + shift
            if shifted > ord('Z'):
                shifted -= 26
            result += chr(shifted)
        elif 'a' <= char <= 'z':
            shifted = ord(char) + shift
            if shifted > ord('z'):
                shifted -= 26
            result += chr(shifted)
        else:
            result += char
    return result


def decrypt(text, shift):
    """
    Decrypts the text using a Caesar cipher.

    Args:
        text (str): The text to be decrypted.
        shift (int): The number of positions each character was shifted.

    Returns decrypted text.
    """
    result = ''
    for char in text:
        if 'A' <= char <= 'Z':
            shifted = ord(char) - shift
            if shifted < ord('A'):
                shifted += 26
            result += chr(shifted)
        elif 'a' <= char <= 'z':
            shifted = ord(char) - shift
            if shifted < ord('a'):
                shifted += 26
            result += chr(shifted)
        else:
            result += char
    return result



def process_file(filename, mode, shift):
    """
    Processes lines in a file for encryption or decryption
    and handels if file is not found
    """
    processed_lines = []
    try:
        with open(filename, 'r') as file:
            for line in file:
                if mode == 'e':
                    processed_line = encrypt(line.strip().upper(), shift)
                else:
                    processed_line = decrypt(line.strip().upper(), shift)
                processed_lines.append(processed_line)
    except FileNotFoundError:
        print("File not found.")
    return processed_lines


def is_file(filename):
    """
    it checks if the given filename exists in the current folder or not
    """
    try:
        with open(filename, 'r'):
            return True
    except FileNotFoundError:
        return False


def write_messages(messages):
    """
    Writes the desired messages to a file called results.txt
    """
    with open('results.txt', 'w') as file:
        for message in messages:
            file.write(message + '\n')


def message_or_file():
    """
    Ask the user for mode, and whether to read from console or file
    handle if file name is invalid if valid ask for shift number
    and if shift number is invalid loop will continue
    """
    mode, message, filename = None, None, None
    while True:
        mode = input(" Would you like to encrypt (e) or decrypt (d): ").lower()
        if mode not in ['e', 'd']:
            print("Invalid Mode")
            continue
        source = input("Would you like to read from a file (f) or the console (c)?: ").lower()
        if source == 'f':
            while True:
                filename = input("Enter a filename: ")
                if is_file(filename):
                    break
                else:
                    print("Invalid Filename")
            shift = int(input("What is the shift number: "))
            break
        elif source == 'c':
            if mode == 'e':
                message = input(f"What message would you like to {mode}ncrypt: ").upper()
            else:
                message = input(f"What message would you like to {mode}ecrypt: ").upper()
            shift = int(input("What is the shift number: "))
            break
        else:
            print("Invalid option")
            continue
    return mode, message, filename, shift

def main():
    """
    This is main function to run the whole program
    """
    welcome()
    while True:
        mode, message, filename, shift = message_or_file()
        if filename:
            messages = process_file(filename, mode, shift)
            write_messages(messages)
            print(f"Output written to results.txt")
        else:
            if mode == 'e':
                encrypted = encrypt(message, shift)
                print(encrypted)
            else:
                decrypted = decrypt(message, shift)
                print(decrypted)
        again = input("Would you like to encrypt or decrypt another message? (y/n): ").lower()
        if again == 'n':
            break
    print("Thanks for using the program, goodbye!")


if __name__ == "__main__":
    main()
