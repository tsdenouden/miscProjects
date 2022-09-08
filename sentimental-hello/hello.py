# Hello.py


def main():
    # Get string from user as input
    try:
        name = str(input("What is your name? "))
        # Print greeting
        print("Hello, " + name)
    # If error -> error msg
    except:
        print("Error!")


if __name__ == "__main__":
    main()