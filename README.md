# mult-matrix-parallel

work description:

Olá pessoal,

Essa tarefa está relacionada ao desenvolvimento do projeto em grupo previsto nesta disciplina. Trata-se do estudo e do desenvolvimento de uma aplicação paralela relevante.

Os problemas a serem resolvidos podem ser escolhidos daqui: Maratona de Programação Paralela - SBAC/PAD

Os trabalhos deverão ser feitos em grupo de 4 alunos e devem conter:

Algoritmo e estratégias de decomposição adequadas para o problema.
Implementação paralela para o problema, usando threads (OpenMP) em um sistema com memória compartilhada.
Implementação paralela para o problema usando MPI, ou alguma outra plataforma para programação paralela com passagem de mensagem.
Implementação paralela para o problema usando GPUs.
Considerações sobre uma possível implementação híbrida para o problema, envolvendo múltiplos nós interligados em rede, cada um contendo vários processadores e núcleos e ao menos 1 GPU. 
Cada grupo deverá entregar a documentação de seu estudo na forma de um (ou mais) Notebook(s) Colab (Google), incluindo os textos descritivos, os algoritmos e as implementações executáveis no próprio documento, ou em notebooks relacionados, bem como resultados de speedup e eficiência obtidos com a implementação.

Além disso, deverá ser entregue um texto sobre o projeto, na forma de resumos expandidos com até 4 páginas, usando o formato da SBC. O modelo pode ser obtido em: https://www.sbc.org.br/documentos-da-sbc/summary/169-templates-para-artigos-e-capitulos-de-livros/878-modelosparapublicaodeartigos.

Os trabalhos serão apresentados no período da manhã dos dias 26 a 29 / 4 / 2022, em horários agendados.  Nas apresentações, deve ser explicada a implementação mais eficiente realizada para o problema e os resultados obtidos.

Boas pesquisas,

Hélio

# Interesting Links

https://gist.github.com/nadavrot/5b35d44e8ba3dd718e595e40184d03f0

# Running the code

##  Serial

gcc -O2 -Wall -fopenmp multDinMatrixSerial.c && ./a.out 

## Parallel openMP

gcc -O2 -Wall -fopenmp multMatrixParallel.c && ./a.out  