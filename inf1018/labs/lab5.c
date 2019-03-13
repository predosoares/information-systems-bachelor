/* LAB5 - Representação de arrays e structs
  Nome: Pedro Henrique Aragão Soares
  Matrícula: 1713186

  Questão#1

int b[2];       2*4(tam int) = 8 bytes      
Como todo vetor ele é representado de maneira contígua na memória

short a[2][3];  2*3*2(tam short) = 12 bytes 
Mesmo sendo arrays multidimensionais a representação na memória se mantém contígua


  Questão#2
  
As estruturas heterogêneas(structs) armazenam seus dados de forma sequencial entretanto nem 
sempre contíguo já que ele respeita um alinhamento que considera o tamanho do maior tipo contido na
estrutura(alinhamento a partir dos múltiplos do maior tipo).

  Questão#3
  
a. sizeof(struct X1) = 12 bytes;
Na memória -> |c1|0|0|0|i|i|i|i|c2|0|0|0|

b. sizeof(struct X2) = 16 bytes;
Na memória -> |l|l|l|l|l|l|l|l|c|0|0|0|0|0|0|0|

c. sizeof(struct X3) = 8 bytes;
Na memória -> |i|i|i|i|c1|c2|0|0|

d. sizeof(struct X4) = 24 bytes;
Na memória -> |l|l|l|l|l|l|l|l|c1|0|0|0|0|0|0|0|c2|0|0|0|0|0|0|0|

e. sizeof(struct X5) = 3 bytes;
Na memória -> |c1|c2|c3|

f. sizeof(struct X6) = 16 bytes;
Na memória -> |s1|0|0|0|i|i|i|i|c[0]|c[1]|c[2]|0|s2|s2|0|0|

g. sizeof(union U1) = 8 bytes;
Na memória(int) -> |i|i|i|i|0|0|0|0|
Na memória(char) -> |c[0]|c[1]|c[2]|c[3]|c[4]|0|0|0|

h. sizeof(union U2) = 6 bytes;
Na memória(short) -> |s|s|0|0|0|0|
Na memória(char) -> |c[0]|c[1]|c[2]|c[3]|c[4]|0|

*/
