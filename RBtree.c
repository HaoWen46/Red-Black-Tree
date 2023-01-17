#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    RED,
    BLACK
} Color;

typedef struct node {
    int data;
    Color color;
    struct node *left, *right;
}Node;

Node *_root = NULL;

Node *new_node(int data) {
    Node *node = malloc(sizeof(Node));
    node->data = data, node->color = RED;
    node->left = node->right = NULL;
    return node;
}

Node *left_rotate(Node *y) {
    Node *x = y->right;
    Node *tmp = x->left;
    x->left = y, y->right = tmp;
    if (y == _root) _root = x;
    return x;
}

Node *right_rotate(Node *y) {
    Node *x = y->left;
    Node *tmp = x->right;
    x->right = y, y->left = tmp;
    if (y == _root) _root = x;
    return x;
}

Node *insert(Node *root, int data) {
    if (root == NULL) {
        root = new_node(data);
        if (_root == NULL) root->color = BLACK;
        return root;
    }
    if (data < root->data) {
        root->left = insert(root->left, data);
    }
    else if (data > root->data) {
        root->right = insert(root->right, data);
    }
    if (root->left != NULL && root->left->color == RED) {
        Node *node = root->left;
        if (node->left != NULL && node->left->color == RED) {
            if (root->right == NULL || root->right->color == BLACK) {
                root = right_rotate(root);
                root->color = BLACK;
                root->right->color = RED;
            }
            else if (root->right->color == RED) {
                root->color = RED;
                root->left->color = BLACK;
                root->right->color = BLACK;
            }
        }
        else if (node->right != NULL && node->right->color == RED) {
            if (root->right == NULL || root->right->color == BLACK) {
                root->left = left_rotate(root->left);
                root = right_rotate(root);
                root->color = BLACK;
                root->right->color = RED;
            }
            else if (root->right->color == RED) {
                root->color = RED;
                root->left->color = BLACK;
                root->right->color = BLACK;
            }
        }
    }
    if (root->right != NULL && root->right->color == RED) {
        Node *node = root->right;
        if (node->left != NULL && node->left->color == RED) {
            if (root->left == NULL || root->left->color == BLACK) {
                root->right = right_rotate(root->right);
                root = left_rotate(root);
                root->color = BLACK;
                root->left->color = RED;
            }
            else if (root->left->color == RED) {
                root->color = RED;
                root->left->color = BLACK;
                root->right->color = BLACK;
            }
        }
        else if (node->right != NULL && node->right->color == RED) {
            if (root->left == NULL || root->left->color == BLACK) {
                root = left_rotate(root);
                root->color = BLACK;
                root->left->color = RED;
            }
            else if (root->left->color == RED) {
                root->color = RED;
                root->left->color = BLACK;
                root->right->color = BLACK;
            }
        }
    }
    if (root == _root && root->color == RED) {
        root->color = BLACK;
    }
    return root;
}

void preorder(Node *root) {
    if (root == NULL) return;
    printf("%d ", root->data);
    preorder(root->left);
    preorder(root->right);
}

void inorder(Node *root) {
    if (root == NULL) return;
    inorder(root->left);
    printf("%d ", root->data);
    inorder(root->right);
}

int main() {
    int data;
    char action[8];
    while (1) {
        printf("*****enter \"insert [data]\" or \"display\" or \"exit\" *****\n");
        scanf("%s", action);
        if (!strcmp(action, "insert")) {
            scanf("%d", &data);
            _root = insert(_root, data);
        }
        else if (!strcmp(action, "display")) {
            printf("The preorder traversal:\n");
            preorder(_root);
            printf("\nThe inorder traversal:\n");
            inorder(_root);
            printf("\n");
        }
        else if (!strcmp(action, "exit")) break;
        else printf("could not interpret action\n");
    }
    return 0;
}