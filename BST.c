#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _CRT_SECURE_NO_WARNINGS

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
            printf("There not exist %d\n", item);
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
        if(item < curr -> key) curr = curr -> left;
        else if (item > curr -> key) curr = curr -> right;
        else break;
    }
    return curr;
}

node_ptr* findLeftChild_ptr(tree_ptr* tree, node_ptr* target)
{
    return (target == NULL) ? NULL : target -> left;
}
node_ptr* findRightChild_ptr(tree_ptr* tree, node_ptr* target)
{
    return (target == NULL) ? NULL : target -> right;
}
node_ptr* findParent_ptr(tree_ptr* tree, node_ptr* target)
{
    return (target == NULL) ? NULL : target -> parent;
}


void traverseNode_ptr(node_ptr* root)
{
    if(root == NULL) return;
    traverseNode_ptr(root -> left);
    if(root -> left == NULL) printf("[NULL]");
    else printf("[ %d ]", root -> left -> key);
    printf("\t<-\t");
    printf("[ %d ] ", root -> key);
    printf("\t->\t");
    if(root -> right == NULL) printf("[NULL]\n");
    else printf("[ %d ]\n", root -> right -> key);
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
            tree -> arr = realloc(tree -> arr, (tree -> size) * 2 * sizeof(element));
            for(size_t i = tree -> size; i < (tree -> size) * 2; i++)
                tree -> arr[i] = null;
            tree -> size = (tree -> size) * 2;
        }
        if(tree -> arr[location] == null){
            tree -> arr[location] = item;
            return;
        }
        if(item < tree -> arr[location]){
            location = location * 2;
        }
        else if (item > tree -> arr[location]){
            location = location * 2 + 1;
        }
        else{ // there is already same key
            printf("There is already '%d'!!\n", item);
            return;
        }
    }
}

void moveNode(tree_arr* tree, int fromNode, int toNode)
{
    tree -> arr[toNode] = tree -> arr[fromNode];
    tree -> arr[fromNode] = null;
    return;
}

void moveTree(tree_arr* tree, int fromRoot, int toRoot)
{
    // firstly, move (fromRoot) -> (toRoot)
    // then, move childs by BFS
    // for BFS, we need two queues with size 2^(subtree's depth) = size / fromRoot
    element* fromQueue = (element*)malloc(sizeof(element) * (tree -> size) / fromRoot);
    element* toQueue = (element*)malloc(sizeof(element) * (tree -> size) / fromRoot);
    int front = -1, rear = -1;

    moveNode(tree, fromRoot, toRoot);
    // enqueue
    if(tree -> arr[findLeftChild_arr(tree, fromRoot)] != null){
        fromQueue[++rear] = findLeftChild_arr(tree, fromRoot);
        toQueue[rear] = findLeftChild_arr(tree, toRoot);
    }
    if(tree -> arr[findRightChild_arr(tree, fromRoot)] != null){
        fromQueue[++rear] = findRightChild_arr(tree, fromRoot);
        toQueue[rear] = findRightChild_arr(tree, toRoot);
    }
    
    while(front < rear){ // !isEmpty(queue)
        // dequeue
        int from = fromQueue[++front];
        int to = toQueue[front];
        moveNode(tree, from, to);

        if(tree -> arr[findLeftChild_arr(tree, from)] != null){
            fromQueue[++rear] = findLeftChild_arr(tree, from);
            toQueue[rear] = findLeftChild_arr(tree, to);
        }
        if(tree -> arr[findRightChild_arr(tree, from)] != null){
            fromQueue[++rear] = findRightChild_arr(tree, from);
            toQueue[rear] = findRightChild_arr(tree, to);
        }
    }

    free(fromQueue);
    free(toQueue); // to avoid memory leak

}

// insert 15, 10, 25, 22, 21, 20, 24, 23 한 뒤 delete 25하면 24랑 23이 사라짐
void delete_arr(tree_arr* tree, element item)
{
    if(item == null) return;
    int target = 1;
    while(tree -> arr[target] != item){
        if(tree -> arr[target] == null){
            printf("There not exist '%d'", item);
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
    int temp;

    if (tree -> arr[left] == null && tree -> arr[right] == null){
        tree -> arr[target] = null;
        return;
    }
    else if (tree -> arr[left] == null){
        temp = right;
        moveTree(tree, temp, target);
    }
    else if (tree -> arr[right] == null){
        temp = left;
        moveTree(tree, temp, target);
    }
    else{
        temp = right;
        while(tree -> arr[findLeftChild_arr(tree, temp)] != null){
            temp = findLeftChild_arr(tree, temp);
        }
        int val = tree -> arr[temp];
        delete_arr(tree, val);
        tree -> arr[target] = val;
    }
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

    if(tree -> arr[findLeftChild_arr(tree, index)] == null) printf("[NULL]");
    else printf("[ %d ]", tree -> arr[findLeftChild_arr(tree, index)]);
    printf("\t<-\t");
    printf("[ %d ] ", tree -> arr[index]);
    printf("\t->\t");
    if(tree -> arr[findRightChild_arr(tree, index)] == null) printf("[NULL]\n");
    else printf("[ %d ]\n", tree -> arr[findRightChild_arr(tree, index)]);

    inorderTraverseNode_arr(tree, findRightChild_arr(tree, index));
}

void inorderTraverse_arr(tree_arr* tree)
{
    inorderTraverseNode_arr(tree, 1);
    printf("\n");
}



void printCommands()
{
    printf("\ncommands :\n");
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
    tree_arr* arrTree = (tree_arr*)malloc(sizeof(tree_arr));
    arrTree -> arr = NULL;
    tree_ptr* ptrTree = (tree_ptr*)malloc(sizeof(tree_ptr));
    ptrTree -> root = NULL;

    do{
        printf("1. Binary Search Tree using Array.\n");
        printf("2. Binary Search Tree using Pointer.\n");
        printf("3. Exit Program.\n");
        scanf_s(" %c", &treeCmd);

        if(treeCmd == '1'){
            char cmd[11] = "default";
            while(strcmp(cmd, "quit") != 0){
                printCommands();
                scanf_s(" %s", cmd, 11);
                if(strcmp(cmd, "insert") == 0){
                    int key;
                    scanf_s(" %d", &key);
                    insert_arr(arrTree, key);
                }
                else if (strcmp(cmd, "delete") == 0){
                    int key;
                    scanf_s(" %d", &key);
                    delete_arr(arrTree, key);
                }
                else if (strcmp(cmd, "leftChild") == 0){
                    int key;
                    scanf_s(" %d", &key);
                    int left = arrTree -> arr[findLeftChild_arr(arrTree, findNode_arr(arrTree, key))];
                    if(left == null) printf("null\n");
                    else printf("%d\n", left);
                }
                else if (strcmp(cmd, "rightChild") == 0){
                    int key;
                    scanf_s(" %d", &key);
                    int right = arrTree -> arr[findRightChild_arr(arrTree, findNode_arr(arrTree, key))];
                    if(right == null) printf("null\n");
                    else printf("%d\n", right);
                }
                else if (strcmp(cmd, "parent") == 0){
                    int key;
                    scanf_s(" %d", &key);
                    int parent = arrTree -> arr[findParent_arr(arrTree, findNode_arr(arrTree, key))];
                    if(parent == null) printf("null\n");
                    else printf(" %d", parent);
                }
                else if (strcmp(cmd, "traverse") == 0){
                    inorderTraverse_arr(arrTree);
                }
            }
        }
        else if (treeCmd == '2'){
            char cmd[10];
            do{
                printCommands();
                scanf_s(" %s", cmd, 11);
                if(strcmp(cmd, "insert") == 0){
                    int key;
                    scanf_s(" %d", &key);
                    insert_ptr(ptrTree, key);
                }
                else if (strcmp(cmd, "delete") == 0){
                    int key;
                    scanf_s(" %d", &key);
                    delete_ptr(ptrTree, key);
                }
                else if (strcmp(cmd, "leftChild") == 0){
                    int key;
                    scanf_s(" %d", &key);
                    node_ptr* left = findLeftChild_ptr(ptrTree, findNode_ptr(ptrTree, key));
                    if(left == NULL) printf("null\n");
                    else printf("%d\n", left -> key);
                }
                else if (strcmp(cmd, "rightChild") == 0){
                    int key;
                    scanf_s(" %d", &key);
                    node_ptr* right = findRightChild_ptr(ptrTree, findNode_ptr(ptrTree, key));
                    if(right == NULL) printf("null\n");
                    else printf("%d\n", right -> key);
                }
                else if (strcmp(cmd, "parent") == 0){
                    int key;
                    scanf_s(" %d", &key);
                    node_ptr* parent = findParent_ptr(ptrTree, findNode_ptr(ptrTree, key));
                    if(parent == NULL) printf("null\n");
                    else printf("%d\n", parent -> key);
                }
                else if (strcmp(cmd, "traverse") == 0){
                    inorderTraverse_ptr(ptrTree);
                }
            }while(strcmp(cmd, "quit") != 0);
        }
    }while(treeCmd != '3');

    return 0;

}
