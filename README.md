<div align="center">

<img src="./.github/assets/tml.svg" width="100" height="100"><h1>Turing Machine Language</h1>

[![License][badge-license]](#license)
</div>

## Resumo
TML (Turing Machine Language) é uma linguagem projetada para descrever máquinas de Turing determinísticas e não determinísticas de forma clara e eficiente. Escrita em C e composta por um interpretador construído com Flex e Yacc, a linguagem visa auxiliar estudantes de Teoria da Computação e desenvolvedores na modelagem e depuração de máquinas de Turing. Este artigo apresenta sua estrutura, funcionalidades, implementação e possíveis extensões futuras, demonstrando sua utilidade tanto como ferramenta educacional quanto como recurso para experimentação computacional.

## 1. Introdução
As máquinas de Turing são fundamentais na teoria da computação, sendo amplamente utilizadas para estudar decidibilidade, complexidade e computabilidade. Entretanto, sua descrição formal pode ser complexa para iniciantes. Com esse contexto, propomos a TML, uma linguagem dedicada à descrição de máquinas de Turing, com foco em clareza sintática, desempenho na execução e suporte a mecanismos não determinísticos. O projeto tem como principal objetivo apoiar o ensino e a prática da computação teórica.

## 2. Trabalhos relacionados
Diversas ferramentas simulam máquinas de Turing, mas poucas oferecem uma linguagem textual de baixo nível com foco em desempenho, otimização e uso educacional. A TML se diferencia ao adotar uma abordagem compilada, com alocação dinâmica, estruturas de dados otimizadas e suporte à definição de profundidade máxima de busca.

## 3. Metodologia
### 3.1 Estrutura da linguagem
#### A TML é composta por:
- Diretivas: identificam estados, alfabetos, estado inicial e final, e profundidade máxima.

- Transições: expressas formalmente, indicam o comportamento da máquina para cada par (estado, símbolo).

#### Diretivas:
- `@states`: lista os estados utilizados na máquina. Ex: `@states: q0,q1,q2,h`

- `@tape_alphabet`: símbolos possíveis na fita, incluindo espaço (`_`) e marcador inicial (`>`). Ex: `@tape_alphabet: a,b,_,>`

- `@input_alphabet`: símbolos válidos como entrada. Ex: `@input_alphabet: a,b`

- `@initial_state`: estado inicial da máquina. Ex: `@initial_state: q0`

- `@final_states`: estados de aceitação. Ex: `@final_states: h`

- `@max_depth`: profundidade máxima de execução (opcional, padrão = `100`). Ex: `@max_depth: 100`

#### Transições:
São descritas no formato:
`(estado_atual, símbolo_lido) = (novo_estado, ação)`

Exemplo:
`(q0, _) = (q0, R)`

#### Símbolos reservados:
- `R`: mover para a direita
- `L`: mover para a esquerda
- `_`: espaço em branco
- `>`: marcador de início de fita

#### Regras léxicas:
- Estados: devem começar com letra, podendo conter letras e números.
- Símbolos: letras ou números.

### 3.2 Estrutura do interpretador
O interpretador da TML é dividido em:
- `lexer` (análise léxica): reconhece tokens e símbolos da linguagem.

- `parser` (análise sintática): valida a estrutura das diretivas e transições.

- `semantic` (análise semântica): verifica coerência, como existência de símbolos e estados.

- `core`: implementa estruturas de dados (árvore, lista, fila) e lógica de execução (busca em largura).

- `main`: gerencia entrada, argumentos e execução do interpretador.

## 4. Resultados
A TML permite simular máquinas de Turing determinísticas e não determinísticas, com ótima performance e segurança de memória. A busca em largura garante que todas as ramificações possíveis são exploradas até a profundidade definida.

Para executar a máquina sobre uma entrada, rode:
```
./tml caminho/arquivo.tml entrada
```

O argumento `--verbose` oferece detalhamento completo:

```
@states: q0,q1,q2,h
@tape_alphabet: a,b,_,>
@input_alphabet: a,b
@initial_state: q0
@final_states (1): h
@max_depth: 100

Transitions:
Hash Table (7 elements / size 16):
  [0]:
    (q2, a) = (q1, R)
    (q0, _) = (q1, R)
  [1]:
    (q2, b) = (q1, R)
    (q1, a) = (q2, b)
  [2]:
    (q1, b) = (q2, a)
  [15]:
    (q1, _) = (h, _)
    (q0, >) = (q0, R)
```

## 5. Conclusão
A TML é uma linguagem prática e robusta para descrição de máquinas de Turing. Sua implementação eficiente, combinada com clareza sintática e suporte a não determinismo, a torna valiosa tanto no meio acadêmico quanto para entusiastas da teoria da computação. A linguagem também contribui para a compreensão e visualização do funcionamento interno dessas máquinas.

## 6. Trabalhos futuros
Pretende-se expandir a linguagem com:
- Suporte a múltiplas máquinas por arquivo.
- Importação e reutilização de máquinas entre arquivos.
- Ambiente gráfico de simulação integrado.

[badge-license]: https://img.shields.io/badge/license-MIT-green
