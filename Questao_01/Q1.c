#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#define SIZE_OF_BUFFER 250
#define MAX_SIZE_OF_NUMBER 12

typedef struct _Node
{
    int num;
    int node_Height;
    struct _Node *parent;
    struct _Node *right;
    struct _Node *left;
} Node;

typedef struct
{
    Node *root;
} MyTree;

void mainFunction(FILE *input, FILE *output);
MyTree *createTree(MyTree *T, int *elements, int size, int *listOfHeight);
int getArraySize(char *buffer);
int *getIntegers(char *buffer, int ArraySize);
Node *createNode(int num);
int getleafHeight(Node *leaf);
Node *getPredecessor(Node *max);
Node *getMaxElement(Node *root);
Node *insertElement(Node *root, int num, int *listOfHeights);
void freeTree(Node *root);
void printArray(FILE *output, int *array, int size);
void printTreeInPreOrder(Node *root);

int main(void)
{
    FILE *input = fopen("L2Q1.in", "r"); // teste_1.in | L2Q1.in
    if (input == NULL)
        return 1;
    FILE *output = fopen("L2Q1.out", "w+"); // teste_1.txt | L2Q1.out
    if (output == NULL)
        return 1;

    mainFunction(input, output);

    fclose(input);
    fclose(output);
    return 0;
}
// Professor espero que o senhor não esteja lendo essa mensagem
// pois se estiver significa que alguma coisa deu muito errado
// se este for o caso, por favor tenha paciencia, obrigado!!!

// func principal, na qual todo o processamento de entrada e saída ocorre
void mainFunction(FILE *input, FILE *output)
{
    MyTree *T = (MyTree *)malloc(sizeof(MyTree));

    char buffer[SIZE_OF_BUFFER];
    while (fgets(buffer, sizeof(buffer), input) != NULL)
    {
        T->root = NULL;
        int size = getArraySize(buffer);
        int *array = getIntegers(buffer, size);
        int *listOfHeights = (int *)malloc(sizeof(int) * size);

        T = createTree(T, array, size, listOfHeights);

        printArray(output, listOfHeights, size);
        Node *max = getMaxElement(T->root);
        Node *pred = getPredecessor(max);
        fprintf(output, "max %d alt %d ", max->num, max->node_Height);
        if (pred == NULL)
            fprintf(output, "NaN");
        else
            fprintf(output, "pred %d\n", pred->num);

        free(array);
        free(listOfHeights);
        freeTree(T->root);
    }
    free(T);
}

// Cria a arvore com todos os nodes
// Preenche a lista de alturas dos nodes na ordem de inserção
// recebe um array de inteiros e seu tamanho, a lista de alturas(vazia) e a árvore
// retorna um ponteiro para uma arvore
MyTree *createTree(MyTree *T, int *numbers, int size, int *listOfHeights)
{
    for (int i = 0; i < size; i++)
    {
        T->root = insertElement(T->root, numbers[i], (listOfHeights + i));
    }
    return T;
}

// func get_ArraySize que calcula e retorna o tamanho do array
// a partir do número de espaços presentes na string
int getArraySize(char *buffer)
{
    int size = 1;
    for (int i = 0; buffer[i] != '\0'; i++)
    {
        if (buffer[i] == ' ')
        {
            size++;
        }
    }
    return size;
}

// func get_Integers separa os numeros da string e coloca em um array
// recebe uma string e o tamanho do array como parametros
// a func retorna um ponteiro para o array
int *getIntegers(char *buffer, int ArraySize)
{
    int *array = (int *)malloc(sizeof(int) * ArraySize);

    int i = 0;
    int k = 0;
    while (buffer[i] != '\0')
    {
        if (buffer[i] >= '0' && buffer[i] <= '9' || buffer[i] == '-')
        {
            int j = 0;
            char number[MAX_SIZE_OF_NUMBER];
            while (buffer[i] != ' ' && buffer[i] != '\n' && buffer[i] != '\r' && buffer[i] != '\0')
            {
                number[j] = buffer[i];
                j++;
                number[j] = '\0';
                i++;
            }
            array[k] = atoi(number);
            k++;
        }
        else
            i++;
    }
    return array;
}

// func criar e retorna o node criado
Node *createNode(int num)
{
    Node *new_Node = (Node *)malloc(sizeof(Node));
    new_Node->num = num;
    new_Node->node_Height = 0;
    new_Node->parent = NULL;
    new_Node->right = NULL;
    new_Node->left = NULL;
    return new_Node;
}

// Retorna a altura de um node especifico da arvore (sentido: folha até a raiz)
int getleafHeight(Node *leaf)
{
    if (leaf == NULL)
        return -1;
    return getleafHeight(leaf->parent) + 1;
}

// Retorna o elemeto com a maior chave da árvore
Node *getMaxElement(Node *root)
{
    if (root == NULL)
        return NULL;

    if (root->right == NULL)
        return root;
    else
        return getMaxElement(root->right);
}

// retorna o node com o predessesor de um node específico
// se não houver predessesor retorna NULL
Node *getPredecessor(Node *max)
{
    if (max == NULL)
        return NULL;

    if (max->left != NULL)
        return max->left;

    Node *temp = max;
    while (temp->parent != NULL && temp->num >= max->num)
        temp = temp->parent;

    if (temp == max)
        return NULL;
    else
        return temp;
}

// func para inserir um node na arvore
Node *insertElement(Node *root, int num, int *listOfHeights)
{
    if (root == NULL)
    {
        *listOfHeights = 0;
        return createNode(num);
    }

    if (num < root->num)
    {
        Node *leftChild = insertElement(root->left, num, listOfHeights);
        root->left = leftChild;
        leftChild->parent = root;
        leftChild->node_Height = getleafHeight(leftChild);

        // sem o if o listOfHeight vai pegar sempre a altura do ultimo pai antes da raiz (BUG-Recursão)
        if (*listOfHeights == 0)
            *listOfHeights = leftChild->node_Height;
        return root;
    }
    else
    {
        Node *rightChild = insertElement(root->right, num, listOfHeights);
        root->right = rightChild;
        rightChild->parent = root;
        rightChild->node_Height = getleafHeight(rightChild);

        // sem o if o listOfHeight vai pegar sempre a altura do ultimo pai antes da raiz (BUG-Recursão)
        if (*listOfHeights == 0)
            *listOfHeights = rightChild->node_Height;
        return root;
    }
}

// libera a memória de todos os nodes da arvore
void freeTree(Node *root)
{
    if (root == NULL)
        return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

void printArray(FILE *output, int *array, int size)
{
    for (int i = 0; i < size; i++)
    {
        fprintf(output, "%d ", array[i]);
    }
}

void printTreeInPreOrder(Node *root)
{
    if (root == NULL)
        return;
    printTreeInPreOrder(root->left);
    printTreeInPreOrder(root->right);
    printf("%d ", root->num);
}