#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define DOUBLE_BLACK ((void*)0xFFFFFFFFFFFFFFFF)

typedef enum {
    RED,
    BLACK,
    D_BLACK
} Color;

typedef struct node {
    int data;
    Color color;
    struct node *left, *right;
} Node;

Node *_root = NULL;

bool is_black(Node *node) {
    return node == NULL || node->color == BLACK;
}

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

Node *min_value_node(Node *node) {
    while (node->left != NULL) node = node->left;
    return node;
}

Node *delete(Node* root, int data) {
    if (root == NULL) return NULL;
    if (data < root->data) {
        root->left = delete(root->left, data);
    }
    else if (data > root->data) {
        root->right = delete(root->right, data);
    }
    else {
        if (root->left != NULL && root->right != NULL) {
            Node *tmp = min_value_node(root->right);
            root->data = tmp->data;
            root->right = delete(root->right, tmp->data);
        }
        else if (root->left != NULL) {
            Node *tmp = root->left;
            free(root);
            tmp->color = BLACK;
            return tmp;
        }
        else if (root->right != NULL) {
            Node *tmp = root->right;
            free(root);
            tmp->color = BLACK;
            return tmp;
        }
        else {
            bool tmp = (root == _root) || (root->color == RED);
            free(root);
            return (tmp)? NULL: DOUBLE_BLACK;
        }
    }
    if (root->left == DOUBLE_BLACK || (root->left != NULL && root->left->color == D_BLACK)) {
        Node *node = root->right;
        if (root->color == RED) {
            // two nephews are both black
            if (is_black(node->left) && is_black(node->right)) {
                root->color = BLACK;
                root->right->color = RED;
                if (root->left == DOUBLE_BLACK) root->left = NULL;
                else root->left->color = BLACK;
            }
            else {
                if (is_black(node->right)) {
                    root->right = right_rotate(root->right);
                    root->right->color = BLACK;
                    root->right->right->color = RED;
                }
                root = left_rotate(root);
                root->color = RED;
                root->left->color = BLACK;
                root->right->color = BLACK;
                if (root->left->left == DOUBLE_BLACK) root->left->left = NULL;
                else root->left->left->color = BLACK;
            }
        }
        else if (node->color == RED) {
            root = left_rotate(root);
            root->color = BLACK;
            root->left->color = BLACK;
            root->left->right->color = RED;
            if (root->left->left == DOUBLE_BLACK) root->left->left = NULL;
            else root->left->left->color = BLACK;
        }
        else if (is_black(node->left) && is_black(node->right)) {
            root->color = D_BLACK;
            root->right->color = RED;
            if (root->left == DOUBLE_BLACK) root->left = NULL;
            else root->left->color = BLACK;
        }
        else {
            if (is_black(node->right)) {
                root->right = right_rotate(root->right);
                root->right->color = BLACK;
                root->right->right->color = RED;
            }
            root = left_rotate(root);
            root->right->color = BLACK;
            if (root->left->left == DOUBLE_BLACK) root->left->left = NULL;
            else root->left->left->color = BLACK;
        }
    }
    else if (root->right == DOUBLE_BLACK || (root->right != NULL && root->right->color == D_BLACK)) {
        Node *node = root->left;
        if (root->color == RED) {
            if (is_black(node->left) && is_black(node->right)) {
                root->color = BLACK;
                root->left->color = RED;
                if (root->right == DOUBLE_BLACK) root->right = NULL;
                else root->right->color = BLACK;
            }
            else {
                if (is_black(node->left)) {
                    root->left = left_rotate(root->left);
                    root->left->color = BLACK;
                    root->left->left->color = RED;
                }
                root = right_rotate(root);
                root->color = RED;
                root->left->color = BLACK;
                root->right->color = BLACK;
                if (root->right->right == DOUBLE_BLACK) root->right->right = NULL;
                else root->right->right->color = BLACK;
            }
        }
        else if (node->color == RED) {
            root = right_rotate(root);
            root->color = BLACK;
            root->right->color = BLACK;
            root->right->left->color = RED;
            if (root->right->right == DOUBLE_BLACK) root->right->right = NULL;
            else root->right->right->color = BLACK;
        }
        else if (is_black(node->left) && is_black(node->right)) {
            root->color = D_BLACK;
            root->left->color = RED;
            if (root->right == DOUBLE_BLACK) root->right = NULL;
            else root->right->color = BLACK;
        }
        else {
            if (is_black(node->left)) {
                root->left = left_rotate(root->left);
                root->left->color = BLACK;
                root->left->left->color = RED;
            }
            root = right_rotate(root);
            root->left->color = BLACK;
            if (root->right->right == DOUBLE_BLACK) root->right->right = NULL;
            else root->right->right->color = BLACK;
        }
    }
    if (root == _root) {
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
        printf("\n***** Enter \"insert [data]\" // \"delete [data] \" // \"display\" // \"exit\" *****\n");
        scanf("%8s", action);
        if (!strcmp(action, "insert")) {
            scanf("%d", &data);
            _root = insert(_root, data);
        }
        else if (!strcmp(action, "delete")) {
            scanf("%d", &data);
            _root = delete(_root, data);
        }
        else if (!strcmp(action, "display")) {
            printf("The preorder traversal:\n");
            preorder(_root);
            printf("\nThe inorder traversal:\n");
            inorder(_root);
            printf("\n");
        }
        else if (!strcmp(action, "exit")) break;
        else printf("\ncould not interpret the action\n\n");
    }
    return 0;
}
