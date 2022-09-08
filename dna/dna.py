import csv
import sys


def main():

    # CHECK FOR CLI USAGE
    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py DATABASE.CSV SEQUENCE.TXT")
    # READ DATABASE FILE INTO VARIABLE
    # and also the list of names in a separate variable
    name = []
    table = []
    with open(sys.argv[1]) as database:
        data = csv.DictReader(database)
        i = 0
        # ADD EACH PERSON'S DATA INTO TABLE
        for person in data:
            person_data = []
            person_data.append(int(person["AGATC"]))
            person_data.append(int(person["AATG"]))
            person_data.append(int(person["TATC"]))
            table.append(person_data)
            name.append(person["name"])
            i += 1

    # DNA SEQUENCE
    seq_dna = ""
    # READ DNA SEQUENCE INTO VARIABLE
    with open(sys.argv[2]) as sequence:
        dna = csv.reader(sequence)
        # Note: I couldn't find out how to just directly read it into a string
        # so I read it into an array and assigned the first index
        for seq in dna:
            seq_dna = seq
        seq_dna = seq_dna[0]

    # STR - SHORT TANDEM REPEATS
    subseq = ["AGATC", "AATG", "TATC"]
    # STR COUNTS
    seq_count_list = []
    # FIND LONGEST MATCH
    for str in subseq:
        seq_count = longest_match(seq_dna, str)
        seq_count_list.append(seq_count)

    # CHECK FOR MATCH
    # IF ANY PERSON IN THE TABLE == STR COUNT -> PRINT PERSON'S NAME
    # i = match count
    i = 0
    for row in range(len(table)):
        if seq_count_list == table[row]:
            print(name[row])
            i += 1
    # ELSE, PRINT NO MATCH
    if (i == 0):
        print("No match.")
    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()