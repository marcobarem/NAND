#include <stdio.h>
#include <string.h>

// Definição da porta NAND no alfabeto binário
char NAND(char a, char b) {
    return !(a == '1' && b == '1') ? '1' : '0';
}

// Porta NOT usando NAND
char NOT(char a) {
    return NAND(a, a);
}

// Porta AND usando NAND
char AND(char a, char b) {
    return NOT(NAND(a, b));
}

// Porta OR usando NAND
char OR(char a, char b) {
    return NAND(NOT(a), NOT(b));
}

// Porta XOR usando NAND
char XOR(char a, char b) {
    char not_a = NOT(a);
    char not_b = NOT(b);
    return NAND(NAND(a, not_b), NAND(not_a, b));
}

// Função para ajustar tamanhos das strings binárias (completando com '0' à esquerda)
void adjustLength(char *a, char *b) {
    int len_a = strlen(a);
    int len_b = strlen(b);
    if (len_a < len_b) {
        memmove(a + (len_b - len_a), a, len_a + 1);
        memset(a, '0', len_b - len_a);
    } else if (len_b < len_a) {
        memmove(b + (len_a - len_b), b, len_b + 1);
        memset(b, '0', len_a - len_b);
    }
}

// Half-Adder: calcula soma e carry
void halfAdder(char a, char b, char *sum, char *carry) {
    *sum = XOR(a, b);      // Soma parcial
    *carry = AND(a, b);    // Carry
}

// Full-Adder: usa dois Half-Adders para incluir carry-in
void fullAdder(char a, char b, char carry_in, char *sum, char *carry_out) {
    char sum1, carry1, carry2;

    // Primeiro Half-Adder
    halfAdder(a, b, &sum1, &carry1);

    // Segundo Half-Adder (inclui carry_in)
    halfAdder(sum1, carry_in, sum, &carry2);

    // Carry final
    *carry_out = OR(carry1, carry2);
}

// Função para somar dois números binários representados como strings
void addBinary(char *a, char *b, char *result) {
    adjustLength(a, b); // Ajusta os tamanhos das strings

    int len = strlen(a);
    char carry = '0';
    result[len] = '\0'; // Garante que a string final será terminada

    for (int i = len - 1; i >= 0; i--) {
        fullAdder(a[i], b[i], carry, &result[i], &carry);
    }

    // Adiciona o carry final, se existir
    if (carry == '1') {
        memmove(result + 1, result, len + 1);
        result[0] = carry;
    }
}

// Função para subtrair dois números binários representados como strings
void subtractBinary(char *a, char *b, char *result) {
    char complement_b[33];
    char one[33] = "1";

    // Garante que os tamanhos de 'a' e 'b' sejam iguais
    adjustLength(a, b);

    int len = strlen(b);

    // Inverte os bits de b
    for (int i = 0; i < len; i++) {
        complement_b[i] = NOT(b[i]);
    }
    complement_b[len] = '\0';

    // Soma o complemento de b com 1 (complemento de dois)
    char temp_result[33];
    addBinary(complement_b, one, temp_result);

    // Soma a com o complemento de dois de b
    addBinary(a, temp_result, result);
}

// Função principal para entrada binária
int main() {
    char a[33], b[33], result[33];

    // Solicita entrada dos números em binário
    printf("Digite o primeiro número em binário (a): ");
    scanf("%s", a);

    printf("Digite o segundo número em binário (b): ");
    scanf("%s", b);

    // Calcula soma
    addBinary(a, b, result);
    printf("Soma de %s e %s: %s\n", a, b, result);

    // Calcula subtração
    subtractBinary(a, b, result);
    printf("Subtração de %s e %s: %s\n", a, b, result);

    return 0;
}
