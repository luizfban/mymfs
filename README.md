# Trabalho Prático de SO mymfs 

a) **Contexto do trabalho:** Pontifícia Universidade Católica de Minas Gerais. 
**Curso:** Engenharia de Software/Noturno. 
**Disciplina:** Sistemas Operacionais. **Professor:** Lesandro Ponciano 
**Alunos:** Luiz Felipe de Castro Baia Antunes & Guilherme Henrique Ladislau Biagini

---------------------------------------------------------------------------------------------------------------------------------

b) **Desenvolvedores:** Luiz Felipe de Castro Baia Antunes & Guilherme Henrique Ladislau Biagini

---------------------------------------------------------------------------------------------------------------------------------

c) **Dependências:** O  trabalho foi feito na linguagem C++ na IDE NetBeans implementando as funções config, import, listall, export, remove, removeall, head100, tail100, e grep, com a difilculdade de entender algumas partes e extender o código, passado pelo professor Lesando decidimos fazer um novo código utilizando como exemplo algumas lógicas e funções do código do git Haddadson (https://github.com/Haddadson/mymfs) o mesmo passado pelo professor.

---------------------------------------------------------------------------------------------------------------------------------

d) **Licenciamento:GPLv3**

---------------------------------------------------------------------------------------------------------------------------------

**Sobre o programa:**

Foi formulado um novo código, não resultante do antigo projeto do respositório de haddadson.

**mymfs.cpp é o arquivo principal**, que recebe os argumentos e chama as ações correspondentes.

As funcionalidades estão nos arquivos:
*  mymfsKernel.cpp (implementação) 
*  mymfskernel.h (interface)

--------------------------------------------------------------------------------------------------------------------------------------

**Para compliar:**

```g++ mymfsKernel.cpp mymfs.cpp -o mymfs```

--------------------------------------------------------------------------------------------------------------------------------------

**Caso o usuário digite o comando errado, o programa retorna:**  
```
Command not found

USAGE
mymfs.exe X config D E F
mymfs.exe X import file.txt
mymfs.exe X listall
mymfs.exe X export file.txt C:/file.txt
```
-----------------------------------------------------------------------------------------------------------------------------------

Compreendido RaidX com um folder formado por pen drives em RAID 5. Dentro desse folder, tem o arquivo de configuração, .config, como também os demais arquivos gravados.

---------------------------------------------------------------------------------------------------------------------------------------

**Macros:**

```#define CONFIG_FILE "/mymfs.config"```

Caso o diretório X não exista, irá ocorrer um erro no .config
```./mymfs Z config D E F
Running config command.
Error creating config file. Maybe de directory Z does not exist.
```
-----------------------------------------------------------------------------------------------------------------------------------
Comando config cria uma string com os dispositivos que farão parte do raid ex:
"D E F"

No config cada arquivo vai ter a configuração nesta forma:

``` FILE NOME
    FILE-PART NOME.1 D
    FILE-PART o nome.parte e a unidade
```

-------------------------------------------------------------------------------------------------------------------------------------

Sempre que tiver uma linha com 2 strings, é indicado um arquivo no mymfs



