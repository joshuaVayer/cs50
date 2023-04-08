from cs50 import get_int

def main():
    # Promp the user for the height
    height = get_int("Height:")

    # Checks if the input is valid
    if (height > 8 or height < 1):
        height = get_int("Height:")
    # Call the draw function if the input is correct
    draw(height)


def draw(height):
    # Calculates the spaces required to complete the line and draw the pyramid
    for i in range(height):
        if (i + 1) != height:
            print(" " * (height - i - 2), "#" * (i + 1))
        else:
            print("#" * height)
            
main()