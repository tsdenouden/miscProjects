# readability.py


def main():
    # Get a text from the user as input
    text = str(input("Text: "))
    # Calculate the avg num of letters per 100 words
    L = (letter_count(text) / word_count(text)) * 100
    # Calculate the avg num of sentences per 100 words
    S = (sentence_count(text) / word_count(text)) * 100
    # Coleman-Lieu index
    cmanlieu = round(0.0588 * L - 0.296 * S - 15.8)
   # Print Grade Level
    if cmanlieu >= 16:
        print("Grade 16+")
        return
    elif cmanlieu < 1:
        print("Before Grade 1")
    else:
        print("Grade " + str(cmanlieu))


def letter_count(text):
    # Letter count
    ltr_count = 0
    # For every character in text
    for i in range(len(text)):
        # Check if it is a letter
        if text[i].isalpha() == True:
            # If true -> Increment
            ltr_count += 1
    # Return
    return ltr_count


def word_count(text):
    # Word count
    word_count = 0
    # Symbols indicating end of a sentence
    end_symbol = [".", "!", "?"]
    # For every character in text
    for i in range(len(text)):
        # If character is a space -> increment word count
        if text[i] == " ":
            word_count += 1
    # If last character indicates end of a sentence -> increment word count
    if text[-1] in end_symbol:
        word_count += 1
    # Return
    return word_count


def sentence_count(text):
    # Sentence count
    snt_count = 0
    # Symbols indicating end of a sentence
    end_symbol = [".", "!", "?"]
    # For every character in text
    for i in range(len(text)):
        # If character is one of the symbols -> increment sentence count
        if text[i] in end_symbol:
            snt_count += 1
    # Return
    return snt_count


if __name__ == "__main__":
    main()