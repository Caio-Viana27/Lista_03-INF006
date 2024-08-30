#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

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

int main(void)
{
    MyTree *T = (MyTree *)malloc(sizeof(MyTree));
    T->root = NULL;

    char string[] = {"5 8 -1 4 6 9 11 15"};

    return 0;
}

// func get_ArraySize que calcula o tamanho do array
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

// func get_Integers separa os numeros e coloca no array
int *getIntegers(char *buffer, int ArraySize)
{
    int *array = (int *)malloc(sizeof(int) * ArraySize);

    int i = 0;
    int k = 0;
    while (buffer[i] != '\0')
    {
        int j = 0;
        char number[12];
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
    return array;
}

void mainFunction(char *buffer)
{
    while (buffer != NULL)
    {
        int *array = getIntegers(buffer, getArraySize(buffer));
    }
}

// func criar node
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

// Retorna a altura de um node especifico
int getNodeHeight(Node *leaf)
{
    if (leaf == NULL)
        return -1;
    return getNodeHeight(leaf->parent) + 1;
}

// func para inserir um node na arvore
Node *insertElement(Node *root, int num)
{
    if (root == NULL)
        return createNode(num);

    if (num < root->num)
    {
        Node *leftChild = insertElement(root->left, num);
        leftChild->parent = root;
        leftChild->node_Height = getNodeHeight(leftChild);
        return leftChild;
    }
    else
    {
        Node *rightChild = insertElement(root->right, num);
        rightChild->parent = root;
        rightChild->node_Height = getNodeHeight(rightChild);
        return rightChild;
    }
}

// func para calcular altura da arvore para cada node inserido
// func para encontrar o maior elemento
// func para achar o predecessor do maior elemento