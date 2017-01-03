from cs50 import get_int

while True:
    print('Height: ', end='')  #n=4
    n= get_int()
    if(n>=0 and n<=23):
        break
    
for i in range(n):  #0,1,2,3
    print(('##' + '#'*i).rjust(n+1))


    
