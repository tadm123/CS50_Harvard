from cs50 import get_float

# cents: 25, 10, 5 , 1

while True:
    print('How much change is owed?')
    n= get_float()
    if n>=0:
        break

money_coins = round(n*100)
count = 0

while money_coins>=25:
    money_coins -= 25
    count+=1;

while money_coins>=10:
    money_coins -= 10
    count+=1;

while money_coins>=5:
    money_coins -= 5
    count+=1;

while money_coins>=1:
    money_coins -= 1
    count+=1;

print('%d' %count)
        
