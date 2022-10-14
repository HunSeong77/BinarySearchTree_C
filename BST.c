#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CRT_SECURE_NO_WARNINGS

typedef int element;

// for BST made by pointer

typedef struct node_ptr{
    element key;
    struct node_ptr* parent;
    struct node_ptr* left;
    struct node_ptr* right;
} node_ptr;

typedef struct tree_ptr{
    node_ptr* root;
} tree_ptr;


void insert_ptr(tree_ptr* tree, element item);
void delete_ptr(tree_ptr* tree, element item);
node_ptr* findNode_ptr(tree_ptr* tree, element item);
node_ptr* findLeftChild_ptr(tree_ptr* tree, node_ptr* target);
node_ptr* findRightChild_ptr(tree_ptr* tree, node_ptr* target);
node_ptr* findParent_ptr(tree_ptr* tree, node_ptr* target);
void inorderTraverse_ptr(tree_ptr *tree);

void insert_ptr(tree_ptr* tree, element item)
{
    node_ptr* newNode = (node_ptr*)malloc(sizeof(node_ptr));
    newNode -> key = item;
    newNode -> parent = NULL;
    newNode -> left = newNode -> right = NULL;

    if(tree -> root == NULL){
        tree -> root = newNode;
        return;
    }

    node_ptr* temp = tree -> root;
    newNode -> parent = temp;
    while(1){
        if(item < temp -> key){
            if(temp -> left == NULL){
                temp -> left = newNode;
                return;
            }
            temp = temp -> left;
            newNode -> parent = temp;
        }
        else if (item > temp -> key){
            if(temp -> right == NULL){
                temp -> right = newNode;
                return;
            }
            temp = temp -> right;
            newNode -> parent = temp;
        }
        else{ // there is already same key
            free(newNode);
            printf("There is already '%d'!!\n", item);
            return;
        }
    }
}

void delete_ptr(tree_ptr* tree, element item)
{
    node_ptr* target = tree -> root;
    if(target == NULL){
        printf("Tree is empty.\n");
    }
    node_ptr* parent = NULL;
    while(target -> key != item){
        if(item < target -> key){
            parent = target;
            target = target -> left;
        }
        else if (item > target -> key){
            parent = target;
            target = target -> right;
        }
        if(target == NULL){
            printf("There is not exist %d\n", item);
            return;
        }
    }

    if(target -> left == NULL && target -> right == NULL){
        if(parent != NULL){
            if(parent -> left == target) parent -> left = NULL;
            else parent -> right = NULL;
        }
        else{
            tree -> root = NULL;
        }
    }
    else if (target -> left == NULL){
        if(parent != NULL){
            if(parent -> left == target) parent -> left = target -> right;
            else parent -> right = target -> right;
        }
        else{
            tree -> root = target -> right;
        }
        free(target);
    }
    else if (target -> right == NULL){
        if(parent != NULL){
            if(parent -> left == target) parent -> left = target -> left;
            else parent -> right = target -> left;
        }
        else{
            tree -> root = target -> left;
        }
        free(target);
    }
    else{
        node_ptr* temp = target -> right;
        while(temp -> left != NULL) temp = temp -> left;
        element tempKey = temp -> key;
        delete_ptr(tree, tempKey);
        target -> key = tempKey;
    }
}


node_ptr* findNode_ptr(tree_ptr* tree, element item)
{
    node_ptr* curr = tree -> root;
    while(1){
        if(curr == NULL) {
            printf("Find Failed.\n");
            break;
        }
        if(item < curr -> key) curr = curr -> right;
        else if (item > curr -> key) curr = curr -> left;
        else break;
    }
    return curr;
}

node_ptr* findLeftChild_ptr(tree_ptr* tree, node_ptr* target)
{
    return target -> left;
}
node_ptr* findRightChild_ptr(tree_ptr* tree, node_ptr* target)
{
    return target -> right;
}
node_ptr* findParent_ptr(tree_ptr* tree, node_ptr* target)
{
    return target -> parent;
}


void traverseNode_ptr(node_ptr* root)
{
    if(root == NULL) return;
    traverseNode_ptr(root -> left);
    printf("%d ", root -> key);
    traverseNode_ptr(root -> right);
}

void inorderTraverse_ptr(tree_ptr* tree)
{
    node_ptr* root = tree -> root;
    traverseNode_ptr(root);
    printf("\b\n");
}



// for BST made by array

#define null -210000000

typedef struct tree_arr{
    element* arr;
    int size;
} tree_arr;

void insert_arr(tree_arr* tree, element item);
void delete_arr(tree_arr* tree, element item);
int findNode_arr(tree_arr* tree, element item);
int findLeftChild_arr(tree_arr* tree, int index);
int findRightChild_arr(tree_arr* tree, int index);
int findParent_arr(tree_arr* tree, int index);
void inorderTraverse_arr(tree_arr* tree);


void insert_arr(tree_arr* tree, element item)
{
    if(tree -> arr == NULL){
        tree -> arr = (element*)malloc(8 * sizeof(element));
        tree -> size = 8;
        for(size_t i = 0; i < tree -> size; ++i)
            tree -> arr[i] = null;
        tree -> arr[1] = item;
        return;
    }
    int location = 1;
    while(1){
        if(location >= tree -> size){
            realloc(tree -> arr, tree -> size * 2);
            for(size_t i = tree -> size; i < (tree -> size) * 2; i++)
                tree -> arr[i] = null;
            tree -> size = (tree -> size) * 2;
        }
        if(tree -> arr[location] == null){
            tree -> arr[location] = item;
            return;
        }
        if(item < tree -> arr[location]){
            location = findLeftChild_arr(tree, location);
        }
        else if (item > tree -> arr[location]){
            location = findRightChild_arr(tree, location);
        }
        else{ // there is already same key
            printf("There is already '%d'!!\n", item);
            return;
        }
    }
}

void delete_arr(tree_arr* tree, element item)
{
    if(item == null) return;
    int target = 1;
    while(tree -> arr[target] == item){
        if(tree -> arr[target] == null){
            printf("There is not exist '%d'", item);
            return;
        }
        if(item < tree -> arr[target]){
            target = findLeftChild_arr(tree, target);
        }
        else if (item > tree -> arr[target]){
            target = findRightChild_arr(tree, target);
        }
    }

    int left = findLeftChild_arr(tree, target);
    int right = findRightChild_arr(tree, target);
    int temp = null;

    if (left == null){
        temp = tree -> arr[right];
    }
    else if (right == null){
        temp = tree -> arr[left];
    }
    else{
        int tempIdx = right;
        while(tree -> arr[findLeftChild_arr(tree, tempIdx)] != null){
            tempIdx = findLeftChild_arr(tree, tempIdx);
        }
        temp = tree -> arr[tempIdx];
    }
    delete_arr(tree, temp);
    tree -> arr[target] = temp;
}

int findNode_arr(tree_arr* tree, element item)
{
    int root = 1;
    while(1){
        if(tree -> arr[root] == null){
            printf("Find Failed.\n");
            return 0;
        }
        if(item < tree -> arr[root]) root = findLeftChild_arr(tree, root);
        else if (item > tree -> arr[root]) root = findRightChild_arr(tree, root);
        else return root;
    }
}

int findLeftChild_arr(tree_arr* tree, int index)
{
    int left = index * 2;
    return (left >= tree -> size) ? 0 : left;
}

int findRightChild_arr(tree_arr* tree, int index)
{
    int right = index * 2 + 1;
    return (right >= tree -> size) ? 0 : right;
}

int findParent_arr(tree_arr* tree, int index)
{
    return (index/2);
}

void inorderTraverseNode_arr(tree_arr* tree, int index)
{
    if(tree -> arr[index] == null) return;
    inorderTraverseNode_arr(tree, findLeftChild_arr(tree, index));
    printf("%d ", tree -> arr[index]);
    inorderTraverseNode_arr(tree, findRightChild_arr(tree, index));
}

void inorderTraverse_arr(tree_arr* tree)
{
    inorderTraverseNode_arr(tree, 1);
    printf("\n");
}



void printCommands()
{
    printf("commands :\n");
    printf("\tinsert (key)\n");
    printf("\tdelete (key)\n");
    printf("\tleftChild (key)\n");
    printf("\trightChild (key)\n");
    printf("\tparent (key)\n");
    printf("\ttraverse\n");
    printf("\tquit\n");
}


int main(void){
    char treeCmd = '0';
    tree_arr* arrayTree;
    tree_ptr* pointerTree;

    do{
        printf("1. Binary Search Tree using Array.\n");
        printf("2. Binary Search Tree using Pointer.\n");
        printf("3. Exit Program.\n");
        scanf(" %c", &treeCmd);

        if(treeCmd == '1'){
            char cmd[10];
            do{
                printCommands();
                scanf("%s", cmd);
                printf("input command is %s\n", cmd);
                if(strcmp(cmd, "insert") == 0){
                    int key;
                    scanf(" %d", key);
                    insert_arr(arrayTree, key);
                }
                else if (strcmp(cmd, "delete") == 0){
                    int key;
                    scanf(" %d", key);
                    delete_arr(arrayTree, key);
                }
                else if (strcmp(cmd, "leftChild") == 0){
                    int key;
                    scanf(" %d", key);
                    int left = arrayTree -> arr[findLeftChild_arr(arrayTree, key)];
                    if(left == null) printf("null\n");
                    else printf("%d", left);
                }
                else if (strcmp(cmd, "rightChild") == 0){
                    int key;
                    scanf(" %d", key);
                    int right = arrayTree -> arr[findRightChild_arr(arrayTree, key)];
                    if(right == null) printf("null\n");
                    else printf("%d", right);
                }
                else if (strcmp(cmd, "parent") == 0){
                    int key;
                    scanf("%d", key);
                    int parent = arrayTree -> arr[findParent_arr(arrayTree, key)];
                    if(parent == null) printf("null\n");
                    else printf("%d", parent);
                }
                else if (strcmp(cmd, "traverse") == 0){
                    inorderTraverse_arr(arrayTree);
                }
            }while(strcmp(cmd, "quit") != 0);
        }
        else if (treeCmd == '2'){
            char cmd[10];
            do{
                printCommands();
                scanf("%s", cmd);
                if(strcmp(cmd, "insert") == 0){
                    int key;
                    scanf(" %d", key);
                    insert_ptr(pointerTree, key);
                }
                else if (strcmp(cmd, "delete") == 0){
                    int key;
                    scanf(" %d", key);
                    delete_ptr(pointerTree, key);
                }
                else if (strcmp(cmd, "leftChild") == 0){
                    int key;
                    scanf(" %d", key);
                    node_ptr* left = findLeftChild_ptr(pointerTree, findNode_ptr(pointerTree, key));
                    if(left == NULL) printf("null\n");
                    else printf("%d", left -> key);
                }
                else if (strcmp(cmd, "rightChild") == 0){
                    int key;
                    scanf(" %d", key);
                    node_ptr* right = findRightChild_ptr(pointerTree, findNode_ptr(pointerTree, key));
                    if(right == NULL) printf("null\n");
                    else printf("%d", right -> key);
                }
                else if (strcmp(cmd, "parent") == 0){
                    int key;
                    scanf("%d", key);
                    node_ptr* parent = findParent_ptr(pointerTree, findNode_ptr(pointerTree, key));
                    if(parent == NULL) printf("null\n");
                    else printf("%d", parent -> key);
                }
                else if (strcmp(cmd, "traverse") == 0){
                    inorderTraverse_ptr(pointerTree);
                }
            }while(strcmp(cmd, "quit") != 0);
        }
    }while(treeCmd != '3');
}