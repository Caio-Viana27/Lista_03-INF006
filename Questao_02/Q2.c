#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE_OF_BUFFER 1000

typedef struct _Node
{
  int num;
  int height;
  int difference;
  struct _Node *left;
  struct _Node *parent;
  struct _Node *right;
} Node;

void mainFuntion(FILE *input, FILE *output);
Node *generateTree(Node *root, char *buffer);
Node *createNode(int num);
void freeTree(Node *root);
bool searchNode(Node *root, int key);
Node *addNode(Node *root, int num);
int getleafHeight(Node *leaf);
int sumOfNodes(Node *root);
void orderDifference(Node *root, FILE *output, int *first);

int main(void)
{
  FILE *input = fopen("L2Q2.in", "r"); // L2Q2.in || teste_02.in
  if (input == NULL)
    return 1;
  FILE *output = fopen("L2Q2.out", "w+"); // L2Q2.out || teste_02.out
  if (output == NULL)
    return 1;

  mainFuntion(input, output);

  fclose(input);
  fclose(output);
  return 0;
}

void mainFuntion(FILE *input, FILE *output)
{
  char buffer[SIZE_OF_BUFFER];
  while (fgets(buffer, sizeof(buffer), input) != NULL)
  {
    int first = 1;
    Node *root = NULL;
    root = generateTree(root, buffer);
    orderDifference(root, output, &first);
    fprintf(output, "\n");
    freeTree(root);
  }
}

Node *generateTree(Node *root, char *buffer)
{
  char *token = strtok(buffer, " \n");
  while (token != NULL)
  {
    int num = atoi(token);
    root = addNode(root, num);
    token = strtok(NULL, " \n");
  }
  return root;
}

Node *createNode(int num)
{
  Node *newNode = (Node *)malloc(sizeof(Node));
  newNode->num = num;
  newNode->height = 0;
  newNode->left = newNode->right = newNode->parent = NULL;
  return newNode;
}

void freeTree(Node *root)
{
  if (root == NULL)
    return;
  freeTree(root->left);
  freeTree(root->right);
  free(root);
}

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

Node *addNode(Node *root, int num)
{
  if (root == NULL)
    return createNode(num);

  if (searchNode(root, num))
  {
    return root;
  }

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

int getleafHeight(Node *leaf)
{
  if (leaf == NULL)
    return -1;
  return getleafHeight(leaf->parent) + 1;
}

int sumOfNodes(Node *root)
{
  if (root == NULL)
    return 0;
  return root->num + sumOfNodes(root->left) + sumOfNodes(root->right);
}

void orderDifference(Node *root, FILE *output, int *first)
{
  if (root == NULL)
    return;

  orderDifference(root->left, output, first);

  int leftSum = sumOfNodes(root->left);
  int rightSum = sumOfNodes(root->right);
  int diff = rightSum - leftSum;

  if (*first)
  {
    fprintf(output, "%d (%d)", root->num, diff);
    *first = 0;
  }
  else
  {
    fprintf(output, " %d (%d)", root->num, diff);
  }
  orderDifference(root->right, output, first);
}
