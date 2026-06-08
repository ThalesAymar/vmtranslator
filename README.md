# VM Translator

Implementação em C de um tradutor da linguagem Virtual Machine (VM) da plataforma Hack.

O objetivo do projeto é converter comandos VM em código Assembly Hack (.asm), permitindo a execução dos programas na arquitetura Hack.

Desenvolvido por Thales Souza como parte da disciplina de Compiladores.

## Funcionalidades Implementadas

Atualmente o tradutor suporta:

* Comandos de memória:

  * `push`
  * `pop` 

* Operações aritméticas e lógicas:

  * `add`
  * `sub`
  * `neg`
  * `eq`
  * `gt`
  * `lt`
  * `and`
  * `or`
  * `not`

* Segmentos de memória:

  * `constant`
  * `local`
  * `argument`
  * `this`
  * `that`
  * `temp`
  * `pointer`
  * `static`

* Remoção de comentários (`//`)

* Leitura de arquivos `.vm`

* Geração de arquivos `.asm`


## Estrutura do Projeto


### parser

Responsável pela análise léxica dos comandos VM.


### codewriter

Responsável por gerar o código Assembly Hack correspondente aos comandos VM.


### main

Controla o fluxo principal do tradutor:

1. Abre o arquivo VM.
2. Lê cada linha.
3. Remove comentários.
4. Identifica o tipo do comando.
5. Chama o método apropriado do CodeWriter.
6. Gera o arquivo Assembly de saída.

## Compilação

Linux:

```bash
gcc main.c parser.c codewriter.c -o VMtranslator
```

Windows (MinGW):

```bash
gcc main.c parser.c codewriter.c -o VMTranslator.exe
```

## Uso

Executar o tradutor informando um arquivo `.vm`:

```bash
./VMtranslator SimpleAdd.vm
```

ou

```bash
VMTranslator.exe SimpleAdd.vm
```

O programa criará automaticamente:

```text
SimpleAdd.vm
↓
SimpleAdd.asm
```

## Exemplo

Entrada (`SimpleAdd.vm`):

```vm
push constant 7
push constant 8
add
```

Saída (`SimpleAdd.asm`):

```asm
@7
D=A
@SP
A=M
M=D
@SP
M=M+1
...
```

## Testes

O tradutor foi desenvolvido e validado utilizando os testes do curso Nand2Tetris.
