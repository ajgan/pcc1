Ferramenta pmt de busca de padrões.

Implementada por Antonio Gadelha(ajgan) e Gabriel Melo(gvmgs) como Projeto 1 para a disciplina de Processamento de Cadeias de Caracteres(if767) do CIn-UFPE.

Antonio implementou os algoritmos Brute Force, KMP e Sellers. Fez também a parte da leitura das flags da linha de comando.
Gabriel implementou os algoritmos Aho-Corasick e Ukkonen.
Ambos contribuíram com a documentação e com os testes.

Para compilar o projeto, basta dar o comando:
make pmt
dentro da pasta src. Após isso um executável é gerado e obedece ao seguinte comando:
./pmt [options] pattern textfile
Um executável já foi previamente gerado pela equipa e se encontra dentro da pasta bin

Os seguintes algoritmos de busca foram implementados:
Para busca exata:
    - Brute Force
    - KMP
    - Aho-Corasick
Para busca por proximidade:
    - Sellers
    - Ukkonen

As flags de linha de comando implementadas foram:
    -c ou --count
        Flag para exibir contagem de ocorrências do padrão no texto(por default, não se exibe)
    -e ou --edit emax
        Flag para explicitar o valor de edit máximo em casos de algoritmos de busca aproximada(por default, é zero)
    -a ou --algorithm algorithmname
        Flag para explicitar qual algoritmo de busca o usuário deseja utilizar(por default, a escolha do algoritmo segue uma heurística que será explicada posteriormente)
        lista de algoritmos:
            - brt (Brute Force)
            - kmp (KMP)
            - aho (Aho-Corasick)
            - sel (Sellers)
            - ukk (Ukkonen)
    -p ou --pattern patternfile
        Flag para indicar que os padrões devem vir de um arquivo(por default, o padrão é um só e é digitado pelo usuário)
    -h ou --help
        Flag que exibe informações básicas do programa ao usuário
    -t ou --time
        Flag que exibe ao final o tempo gasto com a execução do programa(por default, não se exibe)

A flag -t foi uma opção que a equipe julgou como interessante, mas não estava no escopo das especificações do projeto

Como citado anteriormente, caso o usuário não informe qual algoritmo ele deseja executar, o programa segue uma heurística obtida através de experimentações.
A heurística se dá da seguinte forma:

Para busca exata:
 - Caso existam mais de 20 padrões para serem pesquisados, é selecionado o Aho-Corasick
 - Caso seja apenas um padrão de tamanho menor que 5, se usa o Brute Force
 - De resto, utiliza-se o KMP
Para busca aproximada:
 - Seleciona o Sellers(tivemos problemas com o Ukkonen)
