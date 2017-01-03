from cs50 import get_string
import sys

def rotate_letter(letter, n):  #  letter= y, n = 3
    #Rotates a letter by n places
    if letter.isupper():
        start = ord('A')        #start = 65
    elif letter.islower():
        start = ord('a')        #start = 97
    else:
        return letter

    c = ord(letter) - start       #'y' - 'a' = 121 - 97 = 24
    i = (c + n) % 26 + start      #i = (24+ 3) % 26 = 1   +  97 = 98 = 'b'
    return chr(i)


def rotate_word(word,n):    #word = 'Hello there'
    new= ''
    for letter in word:
        new = new + rotate_letter(letter,n)
    return new


if(len(sys.argv) != 2):
    print('Error! Usage: caesar.py num')
    sys.exit()

num = int(sys.argv[1])
p = get_string()   

print(rotate_word(p, num))     
