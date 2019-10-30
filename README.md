# Trabalho Prático de SO mymfs 

a) contexto do trabalho: PUC MINAS, Curso: Engenharia de Software/Noturno, Disciplina: Sistemas Operacionais, Professor: Lesandro Ponciano, Aluno: Luiz Felipe Antunes

b) desenvolvedor: Luiz Felipe Antunes

c)dependências:

d)licenciamento:GPLv3

---------------------------------------------------------------------------------------------------------------------------------

**Sobre o programa:**

Foi formulado um novo código, não resultante do antigo projedo do respositório de haddadson.

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
