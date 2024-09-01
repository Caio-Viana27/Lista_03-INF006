#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAX_SIZE_OF_NUMBER 12
#define SIZE_OF_BUFFER 1000

typedef struct _Node
{
    int num;
    int height;
    struct _Node *left;
    struct _Node *parent;
    struct _Node *right;
} Node;

Node *createNode(int num);
Node *addNode(Node *root, int num);
Node *getSucessor(Node *node);
bool searchNode(Node *root, int key);
Node *searchAndRemoveNode(Node *root, int key);
void mainFuntion(FILE *input, FILE *output);
Node *generateTree(Node *root, char *buffer);
int getNumberFromString(char *buffer, int *pos);
int getleafHeight(Node *leaf);
Node *getMaxElement(Node *root);
void inOrder(FILE *output, Node *root, Node *maxElement);
void freeTree(Node *root);

int main(void)
{
    FILE *input = fopen("teste_03.in", "r"); // L2Q3.in || teste_03.in
    if (input == NULL)
        return 1;
    FILE *output = fopen("teste_03.out", "w+"); // L2Q3.out || teste_03.out
    if (output == NULL)
        return 1;

    mainFuntion(input, output);

    fclose(input);
    fclose(output);
    return 0;
}

// função principal onde a magia acontece :)
void mainFuntion(FILE *input, FILE *output)
{
    char buffer[SIZE_OF_BUFFER];
    while (fgets(buffer, sizeof(buffer), input) != NULL)
    {
        Node *root = NULL;
        root = generateTree(root, buffer);
        inOrder(output, root, getMaxElement(root));
        fprintf(output, "\n");
        freeTree(root);
    }
}

// cria a arvore completa
// insere todos os nodes an arvore
Node *generateTree(Node *root, char *buffer)
{
    int i = 0;
    while (buffer[i] != '\0')
    {
        if (buffer[i] == 'a')
        {
            i += 2;
            int num = getNumberFromString(buffer, &i);
            root = addNode(root, num);
        }
        else if (buffer[i] == 'r')
        {
            i += 2;
            int num = getNumberFromString(buffer, &i);
            if (searchNode(root, num))
                root = searchAndRemoveNode(root, num);
            else
                root = addNode(root, num);
        }
        else
            i++;
    }
    return root;
}

// recebe uma string e a posição do primeiro numero dentro da string
// retorna o numero apos a transformação de ascii para int
int getNumberFromString(char *buffer, int *pos)
{
    int j = 0;
    char number[MAX_SIZE_OF_NUMBER];
    while (buffer[*pos] != ' ' && buffer[*pos] != '\n' &&
           buffer[*pos] != '\r' && buffer[*pos] != '\0')
    {
        number[j] = buffer[*pos];
        j++;
        number[j] = '\0';
        *pos += 1;
    }
    return atoi(number);
}

// retorna o sucessor de um node
// recebe o root->right como parametro
Node *getSucessor(Node *node)
{
    if (node == NULL)
        return NULL;

    if (node->left == NULL)
        return node;
    else
        return getSucessor(node->left);
}

// retorna true se o node com a chave existe, false se não existir
bool searchNode(Node *root, int key)
{
    if (root == NULL)
        return false;

    if (key < root->num)
        return searchNode(root->left, key);
    else if (key > root->num)
        return searchNode(root->right, key);
    else
        return true;
}

// casos da remoção
// 1: node não tem filhos
// 2: node só tem 1 filho (esquerda ou direita)
// 3: node tam 2 filhos (encontrar seu sucessor)
// procura e remove um node a partir de uma chave
// favor não mexer (alto risco de bugar de novo)
Node *searchAndRemoveNode(Node *root, int key)
{
    if (root == NULL)
        return NULL;

    if (key < root->num)
    {
        Node *nodeLeft = searchAndRemoveNode(root->left, key);
        if (nodeLeft == NULL)
            root->left = NULL;
        else
            root->left = nodeLeft;
    }
    else if (key > root->num)
    {
        Node *nodeRight = searchAndRemoveNode(root->right, key);
        if (nodeRight == NULL)
            root->right = NULL;
        else
            root->right = nodeRight;
    }
    else
    {
        if (root->left == NULL)
        {
            Node *temp = root->right;
            if (temp != NULL)
            {
                temp->parent = root->parent;
                temp->height = root->height;
            }
            free(root);
            return temp;
        }
        if (root->right == NULL)
        {
            Node *temp = root->left;
            if (temp != NULL)
            {
                temp->parent = root->parent;
                temp->height = root->height;
            }
            free(root);
            return temp;
        }
        Node *sucessor = getSucessor(root->right);
        root->num = sucessor->num;
        root->right = searchAndRemoveNode(root->right, sucessor->num);
    }
    return root;
}

// cria e retorna um node
Node *createNode(int num)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->num = num;
    newNode->height = 0;
    newNode->left = newNode->right = newNode->parent = NULL;
    return newNode;
}

// adiciona o node e calcula sua altura
Node *addNode(Node *root, int num)
{
    if (root == NULL)
        return createNode(num);

    if (num < root->num)
    {
        Node *nodeLeft = addNode(root->left, num);
        nodeLeft->parent = root;
        root->left = nodeLeft;
        nodeLeft->height = getleafHeight(nodeLeft);
        return root;
    }
    else
    {
        Node *nodeRight = addNode(root->right, num);
        nodeRight->parent = root;
        root->right = nodeRight;
        nodeRight->height = getleafHeight(nodeRight);
        return root;
    }
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

// printa em ordem crecente os valores de todos os nodes
void inOrder(FILE *output, Node *root, Node *maxElement)
{
    if (root == NULL)
        return;
    inOrder(output, root->left, maxElement);

    // sem o if o ultimo print vai ter um espaço
    if (root->num == maxElement->num && root->height == maxElement->height)
        fprintf(output, "%d (%d)", root->num, root->height);
    else
        fprintf(output, "%d (%d) ", root->num, root->height);

    inOrder(output, root->right, maxElement);
}

// libera toda memoria que foi alocada para a arvore
void freeTree(Node *root)
{
    if (root == NULL)
        return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}