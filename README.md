# Pé de Galinha (Shisima)

Implementação do jogo pé de galinha para a disciplina de Introdução a Programação de Computadores da Universidade de São Paulo, campus São Carlos. A implementação foi realizada no primeiro semestre de 2025 como parte das atividades de monitoria.

# Licensa

Este programa foi implementado sob a [licensa MIT](LICENSE).

# Regras

O jogo consiste em 2 jogadores que jogam alternadamente em um tabuleiro 3x3. Cada jogador conta com 3 peças. Utiliza-se o símbolo "X" para o jogador que inicia o jogo e "O" para o outro jogador. O símbolo "+" é usado para casas (ou posições) vazias e os símbolos "---", "|", "\\" e "/" são usados para representar as ligações entre as casas.

A posição inicial do jogo é como no exemplo abaixo:

            O---O---O
            | \ | / |
            +---+---+
            | / | \ |
            X---X---X

Cada peça pode se mover apenas para uma casa vizinha (ligada por uma linha).

O vencedor do jogo é àquele que posicionar suas peças em de forma a se ter uma linha reta (vertical, horizontal ou diagonal). Contudo, essa linha não deve ser na posição inicial do tabuleiro. Abaixo está um exemplo de vitória do jogador "X":

            O---O---O
            | \ | / |
            X---X---X
            | / | \ |
            +---+---+

# Requisitos

O programa foi implementado na linguagem C, sendo necessário compilá-lo. O programa foi testado apenas em sistemas Linux com o compilador gcc versão 11.4.0.

# Compilação e Execução (Linux)

Para compilar e executar o programa utilize o comando abaixo no terminal:

```gcc pe_de_galinha.c -o pe_de_galinha.out```

```./pe_de_galinha.out```


# Autor
[Arthur H. S. Cruz](https://github.com/thuzax)


