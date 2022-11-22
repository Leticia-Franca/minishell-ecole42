![(https://github.com/42Paris/minilibx-linux/actions/workflows/ci.yml/badge.svg)](https://img.shields.io/badge/norminette-100%25-green)
#### Desenvolvido por lfranca- e cleticia

## Minishell

#### As beautiful as a shell 
Minishell é um simples interpretador de comandos escrito em C e que usa o bash como modelo.
Ao ser executado, exibe um prompt no terminal e aguarda o usuário entrar uma linha de comando.
Diante de um comando inexistente, emite uma mensagem de erro e exibe novo prompt.  
<div align="left">
  <img width="720" src="./gifs_doc/minishell_working_1.gif"/>
</div>
O minishell localiza e executa corretamente builtins e executáveis com base na variável de ambiente `PATH`.

### Info <!-- explicar o projeto incluir diagramas -->
Dividimos o minishell em 3 fases : <br>
`Inicialização` faz o tratamento das variáveis de ambiente e inicia o loop principal que mantém o minishell aberto. <br>
`Parse e lexer` a comandline é dividida em subcomandos que por sua vez são divididos em tokens onde são analisados e validados. <br>
`Execução` executa comandos. <br>

As funções que definem as fases : <br>
`sm_handler()` modifica sinais e prepara a linha de comando <br>
`mm_list_environ()` copia as variáveis de ambiente <br>
`pc_management_parse()` analisa e trata a comandline e cria a lista de comandos <br>
`ec_exec_process()` executa comandos <br>

Para implementar o minishell, criamos três loops principais : <br>
`main_loop()` mantém o interpretador de comandos aberto <br>
`create_list()` cria a lista de comandos <br>
`exec_process()` executa a lista de comandos <br>

 ### Conhecimentos Prévios
[`FD`](https://www.computerhope.com/jargon/f/file-descriptor.htm)<br> 
[`fork`](https://www.section.io/engineering-education/fork-in-c-programming-language/)<br>
[`token`](https://gcc.gnu.org/onlinedocs/cpp/Tokenization.html)<br>
[`built-in`](https://www.tutorialspoint.com/ansi_c/c_function_references.htm)<br>
[`pipe`](https://www.programacaoprogressiva.net/2014/09/Pipes-em-C-Comunicao-entre-Processos-IPC-Interprocess-Communication.html)<br>
[`exec`](https://www.geeksforgeeks.org/exec-family-of-functions-in-c/)<br>
[`dup`](https://www.geeksforgeeks.org/dup-dup2-linux-system-call/)<br>
[`heredoc`](https://linuxize.com/post/bash-heredoc/)<br>
[`hashtable`](https://www.geeksforgeeks.org/hashing-data-structure/)<br>
[`environ`](https://opensource.com/article/19/8/what-are-environment-variables)<br>

### Outline

#### Análise
Inicializa o projeto modificando o padrão de interrupção `SIGINT` e `SIGQUIT`, preparando para receber as linhas de comando. <br>
Então duplicamos a matriz original `environ` vinda de fora do programa. <br>
Ao mesmo tempo que divide a linha em duas strings por chave e valor. <br>
Depois a lista de variáveis de ambiente é criada. <br>
Em seguinda liberamos a matriz criada com o conteúdo separado, para posteriormente refazer esse processo. <br>
Então o programa acessa a `main_loop()` abrindo o mini interpretador. <br>
O loop principal é inicializado para que os inputs sejam lidos e enviados para a função de gerenciamento. <br>

![main loop](https://github.com/carlarfranca/minishell_private/blob/56647a5a4f126e6d9f6b966030823021a7f20017/transparent_mainloop.png)


#### Lexer e Parse
Nessa parte a função para gerenciamento e tratamento da linha de comando `management_parse` é inicializada. <br>
Para fazer o parse  dividimos a linha de comando em subcomandos, criando uma matriz de subcomandos. <br>
Em seguida na `create_list()` iniciamos o loop para ler a matriz e criar a lista. <br>

![loop_2](https://github.com/carlarfranca/minishell_private/blob/56647a5a4f126e6d9f6b966030823021a7f20017/transparent_loop2.png)

Cada nodo equivalerá a um subcomando, ou seja, cada nodo terá a matriz de tokens de determinado subcomando.

![matriz de tokens dentro nodo de subcomandos](https://github.com/carlarfranca/minishell_private/blob/56647a5a4f126e6d9f6b966030823021a7f20017/transparent_cmdlist.png)

###### Consideramos um subcomando o que está entre pipes e se não houver pipes o comando terá a mesma tratativa que o subcomando.<!-- ###### p.s: O loop lê um item da matriz e cria um nodo e assim sucessivamente.-->
A medida que criamos nodos fazemos a análise e ajustes para receber os tokens. <br>
Desenvolvemos funções para: <br>
Validar redirecionamentos <br>
Verificar aspas <br>
Verificar a presença de variáveis <br>
Expandir variáveis (com atenção para aspas). <br>
Reconstrução de string (para os casos que precisam inserir espaço entre as redireções). <br>
Ordenação da `comandline` (para os casos em que o comando não encontra-se no início). <br>
Então dividimos os subcomandos em partículas que fazem sentido para o interpretador. <br>
E guardamos na matriz de tokens que estará dentro do nodo com demais informações como: <br>
- Um par de file descriptors (para o caso de pipe) <br>
- A informação se termina com pipe, para abrir os FD (leitura e escrita - para ser usado no fork). <br>
Por fim, remover aspas <br>

#### Execução
O programa inicializa `exec_process()`. <br>

![loop_3](https://github.com/carlarfranca/minishell_private/blob/56647a5a4f126e6d9f6b966030823021a7f20017/transparent_loop3.png) <br>
Os comandos são executados a medida que a lista_de_comandos é iterada. <br>
Os FD's de entrada e saída padrão (apontados para o terminal) são "salvos". <br>
Dessa forma, garantimos no final da execução do comando, a restauração dos FD's no mini interpretador(quando houver pipe ou redirecionamento para/de algum arquivo). <br>
Uma versão em matriz das variáveis de ambiente é criada no loop de execução (dentro das funcoes de cada caso - se tiver redirecionamento, ou se nao tiver). Assim, podem ser usadas caso o comando seja um executável ou se precisar dessas variáveis posteriormente. <br>
Ao final de cada repetição, liberaremos a matriz (recriada a cada iteração para não perder a informação). <br>

#### De forma resumida o loop é constituído de três partes: 

 - `Primeiro if:` Confere se o comando tem declaração de variável. Se não, a declaração é feita;
 - `Segundo if:` Examina se o comando tem redireção, se tiver, a `ec_execute_redires()` é acionada.  <br>
 Essa função diferencia quando são apenas redireções, portanto sem comando. Exemplo: < infile1.txt > outfile1.txt.
 Ou quando for comando, verifica e executa se houver pipe `execute_redirs_piped()` e se não houver pipe `execute_redirs_no_pipe()`. 
 Por sua vez essas duas funções analisam se tem ou não redirecionamento de `heredoc` e executará de acordo.
 - `Terceiro if:` (na verdade, o else): é a condição por não entrar no fluxo do `Segundo if`, ou seja, significa que não há redirecionamentos.
 A `ec_no_redir_exec()` segue o fluxo em condição específica para o caso do comando ser um `builtin` sem pipe (que será executado no próprio
 processo parent).  <br>
 Caso não seja, há um "else" pra isso, onde executará o comando para criar um novo processo fork(), e que atende casos em que pode ser builtin, mas está numa
 sequência de pipe, ou se é um executável (e daí independente de ter pipe ou não, precisa ser executado num processo filho). 
 - Uma vez que o loop de comandos é terminado, chamamos a função de finalização `finalize_exec_process()` que atribuirá o valor correto
 à variável global `exit_status` e fará a liberação da lista de comandos. <br>

  
## Compilação  
**Clone esse repo**  
> git clone [url do repo] [nome do diretorio]  
  
**Entre no diretorio**  
> cd [nome do diretório]  
  
**Gere o executável**    
> make  
  
## Execução    
> ./minishell  

### Depuração 
[`analysis tool`](https://valgrind.org/docs/manual/manual-core-adv.html) <br>
**Lista as ocorrências de erros**  
> valgrind --leak-check=full ./minishell  
  
**Lista erros com mais detalhes**
> valgrind --leak-check=full --show-leak-kinds=all ./minishell <br>

