def Bigger(numbers):
    bigger = numbers[0]
    for el in numbers:
        if el > bigger:
            bigger = el
    return bigger

def searchBigger(numbers):
    bigger = numbers[0]
    biggerIndex = 0
    for ( ind, val ) in enumerate(numbers):
        if val > bigger:
            bigger = val
            biggerIndex = ind
    return biggerIndex

def searchEachBigger(numbers):
    listOfIndexes = []
    bigger = Bigger(numbers)
    for ind in range(len(numbers)):
        if numbers[ind] == bigger:
            listOfIndexes.append(ind)
    return listOfIndexes

def sumAfterBigger(numbers):
    sumAfter = 0
    biggerIndex = searchBigger(numbers)
    for el in numbers[biggerIndex + 1::]:
        sumAfter += el
    return sumAfter

def corrigirProva(gab):
    acertos = 0
    for ( idx, val ) in enumerate(gab):
        answer = input("Qual sua resposta para a Q%d?",%idx+1)
        if val == answer:
            acertos+=1
    return acertos