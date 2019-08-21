def corrige( gabarito , tquestoes):
    score = 0
    lista = []
    for idx , question in enumerate(tquestoes):
        print(question)
        answer = input("Digite a resposta da questão %d: "%(idx + 1))
        if answer == gabarito[idx]:
            score += 1
        lista.append(answer)
    return ( score , lista )

def exibeCorrecao( matr , nota , lista ):
	print("%d - Sua nota é: %d\n"%(matr , nota))
	print(lista)

arq = open("apoio/questoes.txt",'r')
texto = arq.read()
lquestoes = texto.split("#")
tquestoes = tuple(lquestoes)
gab = ('d','d','a','d','b','e','c','c','c','c')

while True:
	matr = int(input("Sua matricula? (Digite 0 para finalizar sessão)\n"))
	if matr == 0:
		break;
	( nota , lCorrecao ) = corrige( gab , tquestoes )
	exibeCorrecao( matr , nota , lCorrecao)