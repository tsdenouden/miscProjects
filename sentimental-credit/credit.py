# credit.py

def main():
    # Get credit card from user as input
    try:
        card = int(input("Enter card number: "))
        # Print luhn sum & Check card validity
        print(luhn(card))
        check_card(card)
    # If error -> Print error msg
    except:
        print("Error!")


def luhn(card):
    # Card split into individual digits
    credit_card = split(card)
    # Two operations: 1 == Add, -1 == Multiply by 2 & Split
    operation = 1
    # List of numbers to get the final sum
    listsum = []
    # Read credit card digits in reverse
    for digit in reversed(credit_card):
        # Operation 1: Add
        if (operation == 1):
            listsum.append(digit)
        # Operation 2: Multiply by 2 & Split
        if (operation == -1):
            # Split numbers into list
            num_split = (split(digit*2))
            # Append each number to main list
            for num in num_split:
                listsum.append(num)
        # Change operation
        operation *= -1
    # Add all numbers and return the sum
    final_sum = 0
    for i in range(len(listsum)):
        final_sum += listsum[i]
    return final_sum


def check_card(card):
    # Split card into individual digits
    card_list = split(card)
    # Get first number of the card
    firstNumber = card_list[0]
    # Get sum from luhn function
    valid_check = luhn(card)
    # If last digit of luhn sum is 0 -> Valid
    if (valid_check % 10 == 0):
        print("VALID CARD")
        # Print corresponding card brand to the first number
        match firstNumber:
            case 3: print("AMEX")
            case 4: print("VISA")
            case _: print("MASTERCARD")
    # Else print invalid
    else:
        print("INVALID")


def split(num):
    # Split a number into individual digits
    # Convert to a string
    num_string = str(num)
    num_split = []
    # Convert each character to an int and append to num_split
    for i in range(len(num_string)):
        num_split.append(int(num_string[i]))
    return num_split


if __name__ == "__main__":
    main()