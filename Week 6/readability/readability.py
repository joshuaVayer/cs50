# IMPORTS
from cs50 import get_string


# MAIN DEF
def main():

    string = ""
    while string == "":
        string = get_string("Text:")

    grade = get_grade(get_params(string))

    print(grade)


# SECONDARY FUNCTIONS
# Return an objects with the required parameters for the Coleman-Liau formula
def get_params(string):

    params = {"letters": 0, "words": 0, "sentences": 0}

    # Count the first word if the sentence do not start by a space
    if string[0] != " ": params["words"] += 1

    # Loops on every charc. and test it to increment the write counter
    for i in range(len(string)):

        if string[i] == " ": params["words"] += 1
        elif string[i] in ["?", "!", "."]: params["sentences"] += 1
        elif string[i].isalpha(): params["letters"] += 1

    # In case the string hasn't got an ending charc we count it as one sentence
    if params["sentences"] == 0:
        params["sentences"] = 1

    return params


# Calculate and return the grade
def get_grade(params):

    # Gets the parameters for the formula
    L = (params["letters"] * 100) / params["words"]
    S = (params["sentences"] * 100) / params["words"]

    # Calculates the grade
    grade = round((0.0588 * L - 0.296 * S - 15.8))

    # Checks if the grade if between 1 and 16 otherwise returns the correct output
    if grade < 1: return "Before Grade 1"
    if grade > 16: return "Grade 16+"

    return "Grade " + str(grade)


# EXEC
if __name__ == "__main__":
    main()