# TODO

# Mario.py


def main():
    pyr_height = 0
    # Prompt user for pyramid height
    # User input out of range -> prompt again
    while (pyr_height < 1) or (pyr_height > 8):
        try:
            pyr_height = int(input("Enter pyramid height (1-8): "))
            create_pyramid(pyr_height)
        # Non-numeric input -> Print error message
        except:
            print("Error: Non-numeric input")


def create_pyramid(height):
    # i iterates until == pyramid height
    i = 1
    while (i <= height):
        # Add a space * i to centre pyramid
        print(" " * (height - i), end="")
        # Print a block i times
        print("#" * i, end="")
        # Print 2 spaces in the middle
        print("  ", end="")
        # Print a block i times
        print("#" * i, end="\n")
        # Increment i
        i += 1


if __name__ == "__main__":
    main()
