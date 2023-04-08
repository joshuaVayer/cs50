from cs50 import get_float


def main():

    # Promp the user for a number and check it
    change = 0
    while change <= 0:
        change = get_float("Change owed:")

    # Convert to cents
    cents = change * 100

    #  Call the function and print the result to the user
    print(get_change(cents))


# LIB
# -----------
def get_change(change):

    coins = 0

    # Loop until there is no change left and increments coins on each call
    while change != 0:
        coins += 1

        if change >= 25: change -= 25
        elif change >= 10: change -= 10
        elif change >= 5: change -= 5
        else: change -= 1

    # Return the result
    return coins


main()