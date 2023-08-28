# Projeto final da disciplina ACIEPE - Programação Paralela

Eduardo dos Santos Gualberto

eduardo.gualberto@estudante.ufscar.br 

769726

## Descrição

O problema consiste em ordenar um vetor com itens numéricos utilizando-se do algoritmo de divisão e conquista, chamado Merge Sort. Serão comparadas duas implementações: uma sem qualquer tipo de paralelismo e outra paralelizadas com o uso de threads POSIX.

## Estratégia de paralelização

Foi utilizada a estratégia de paralelização de dados, que consiste na ideia de separar o vetor de itens na mesma quantidade de threads selecionadas (os tests foram feitos utilizando 2, 3, 4 e 8 threads).

Desta forma os subvetores estarão todos ordenados ao fim das operações e será necessário fazer uma operação de merge de todos para garantir a corretude do Merge Sort.

![imagem da estrategia de paralelização](https://drive.google.com/uc?export=view&id=1v4iDUa_w1hddMlsFCU0Gj_KmSglO4vnn)

## Merge Sort sequencial

O algoritmo convencional e sequencial faz a divisão dos vetores em dois até que haja apenas 1 elemento no vetor. Na volta da recursão é realizada a operação de merge entre dois subvetores.

![imagem da implementação sequencial](https://drive.google.com/uc?export=view&id=1_aB5AiaCEByz8JPpkQ87JKEyfqTaspj4)


## Versão paralelizada

A versão paralelizada fará a mesma coisa que a sequencial, porém cada thread fará ação em cima de uma fração do vetor. Existe no argumento task um campo que permite a thread principal entender se a "worker-thread" terminou seu processamento ou namo, o campo se chama busy.

![imagem da implementação paralelizada](https://drive.google.com/uc?export=view&id=1Cw10bfnUiJZ0AQPwP4oQ2Y-SkB5Iqeu2)

## Analise da escalabilidade

![imagem grafico comparativo](https://drive.google.com/uc?export=view&id=1vkAa-ItKUkvkMTJMNNx-cYGRdSrGSzHP)

A partir do gráfico acima e da implementação realizada pode-se retirar algumas conclusões.
1. A implementação paralela adiciona um overhead no algoritmo: o de gerenciamento das threads
2. O merge que ocorre após o processamento das threads aumenta a quantidade de operações necessárias
3. O aumento de threads não garante o ganho de eficiência, pelo menos não no nível de escala de 500000 itens no vetor

## Discussão

Observa-se que com mais de 2 threads a eficiência do algoritmo cai, talvez por dois motivos principais: a quantidade de operações feitas cresce de forma proporcional a quantidade de threads criadas, ou seja, com N threads eu preciso repartir o vetor em N partes e então fazer o processo de merge entre essas N partes para chegar no resultado esperado.

O outro motivo pode ter a ver com a implementação das Threads POSIX em máquinas MacOS não implementam algumas das especificações, como o ```pthread_barrier_t``` e, portanto, exige métodos menos eficientes de gerenciamento das threads criadas pela thread principal.

## Conclusões

Algoritmos de ordenação oferecem amplas possibilidades de "tunning", porém é realmente desafiador fazer isso por meio da paralelização, uma vez que a natureza dos dados no problema faz com que cada item seja dependente um do outro, o que fecha portas quanto a estratégias de paralelização baseadas em dados.

Os dados alcançados indicam que o overhead que surge quando se paraleliza este algoritmo supera os ganhos que teoricamente viriam por conta de uma paralelização
