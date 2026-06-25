# VM Translator

Implementação em C do tradutor da linguagem Virtual Machine (VM) da plataforma Hack, desenvolvida como parte dos Projetos 7 e 8 do curso Nand2Tetris.

O objetivo do projeto é converter programas escritos na linguagem VM para código Assembly Hack (.asm), permitindo sua execução no computador Hack.

Desenvolvido por Thales Souza como parte da disciplina de Compiladores.

## Funcionalidades

### Comandos de Memória

* `push`
* `pop`

### Operações Aritméticas e Lógicas

* `add`
* `sub`
* `neg`
* `eq`
* `gt`
* `lt`
* `and`
* `or`
* `not`

### Controle de Fluxo

* `label`
* `goto`
* `if-goto`

### Chamadas de Função

* `function`
* `call`
* `return`

### Segmentos de Memória

* `constant`
* `local`
* `argument`
* `this`
* `that`
* `temp`
* `pointer`
* `static`

### Recursos Adicionais

* Remoção de comentários (`//`)
* Tradução de um único arquivo `.vm`
* Tradução de diretórios contendo múltiplos arquivos `.vm`

---

## Estrutura do Projeto


### parser

Responsável por analisar os comandos VM.


### codewriter

Responsável por gerar o código Assembly Hack correspondente aos comandos VM.


### translator

Responsável pelo fluxo de tradução.


### main

Controla a execução do programa, identificando se a entrada é um arquivo ou um diretório.

---

## Compilação

### Linux

```bash
gcc main.c parser.c codewriter.c translator.c -o VMTranslator
```

### Windows (MinGW)

```bash
gcc main.c parser.c codewriter.c translator.c -o VMTranslator.exe
```

---

## Utilização

### Traduzir um único arquivo

```bash
./VMTranslator SimpleAdd.vm
```

Saída:

```text
SimpleAdd.asm
```

---

### Traduzir um diretório

```bash
./VMTranslator FunctionCalls
```

Saída:

```text
FunctionCalls/
└── FunctionCalls.asm
```

O arquivo gerado conterá a tradução de todos os arquivos `.vm` presentes no diretório.

---

## Exemplos

### Entrada

```vm
push constant 7
push constant 8
add
```

### Saída

```asm
@7
D=A
@SP
A=M
M=D
@SP
M=M+1

@8
D=A
@SP
A=M
M=D
@SP
M=M+1

@SP
M=M-1
A=M
D=M
A=A-1
M=D+M
```

---

## Bootstrap

Quando a entrada é um diretório, o tradutor gera automaticamente:

```asm
@256
D=A
@SP
M=D
```

seguido de:

```vm
call Sys.init 0
```

conforme especificado pelo Projeto 8 do Nand2Tetris.

---

## Testes

O tradutor foi desenvolvido para ser compatível com os testes oficiais dos Projetos 7 e 8 do Nand2Tetris.

Exemplos de testes suportados:

* SimpleAdd
* StackTest
* BasicTest
* PointerTest
* StaticTest
* BasicLoop
* FibonacciSeries
* SimpleFunction
* NestedCall
* FibonacciElement
* StaticsTest

---
