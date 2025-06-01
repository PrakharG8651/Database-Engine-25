#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define degree 4



int level=0;
typedef struct NODE {
    struct NODE* children[degree+2];
    int keys[degree+1];
    int count_keys;
}Node;

Node* constructNode(){
    Node* nd=(Node*)malloc(sizeof(Node));
    nd->count_keys=0;
    for(int i=0;i<degree+1;i++){
        nd->children[i]=NULL;
        if(i<degree){
            nd->keys[i]=INT_MIN;
        }
    }
    return nd;
}

int search(Node* root,int val){
    if(root==NULL)return 0;
    int i=0;
    for(;i<root->count_keys;i++){
        if(root->keys[i]==val){
            return 1;
        }
        if(root->keys[i]>val){
            return search(root->children[i],val);
        }
    }
    return search(root->children[i],val);
}

Node* sisterNode(Node* root){
  //  printf("here\n");
    Node* sisterNode=constructNode();
    int i=0,j=degree/2+1;
    for(;j<=degree;j++,i++){
        sisterNode->children[i]=root->children[j-1];
        sisterNode->keys[i]=root->keys[j];
        root->children[j-1]=NULL;
        root->keys[j]=INT_MIN;
        //printf(" filling %d\n",sisterNode->children[i]);
    }
    sisterNode->children[i]=root->children[j-1];
    sisterNode->count_keys=degree-degree/2;
    root->count_keys=degree/2;
    return sisterNode;
}

Node* check(Node* root,int curr_level){
    if(level!=curr_level)return root;
    if(root->count_keys==degree+1){
        Node* newNode=constructNode();
        newNode->keys[newNode->count_keys++]=root->keys[degree/2];
        newNode->children[0]=root;
        newNode->children[1]=sisterNode(root);
        level++;
        return newNode;
    }
    return root;
}

void printer();

Node* insertNode(Node* root,int val,int curr_level){
    if(root==NULL){
        root=constructNode();
        root->keys[root->count_keys++]=val;
        return root;
    }
    if(curr_level==0){
        for(int i=0;i<root->count_keys;i++){
            if(root->keys[i]>val){
                for(int j=root->count_keys;j>i;j--){
                    root->keys[j]=root->keys[j-1];
                }
                root->keys[i]=val;
                root->count_keys++;
                return check(root,curr_level);
            }
        }
        root->keys[root->count_keys++]=val;
        return check(root,curr_level);
    }
    for(int i=0;i<root->count_keys;i++){
        if(root->keys[i]>val){
            root->children[i]=insertNode(root->children[i],val,curr_level-1);
            if(root->children[i]->count_keys==degree+1){
                for(int j=root->count_keys;j>i;j--){
                    root->keys[j]=root->keys[j-1];
                    root->children[j+1]=root->children[j];
                }
                root->keys[i]=root->children[i]->keys[degree/2];
                root->children[i+1]=sisterNode(root->children[i]);
            }
            return check(root,curr_level);
        }
    }
    root->children[root->count_keys]=insertNode(root->children[root->count_keys],val,curr_level-1);
    if(root->children[root->count_keys]->count_keys==degree+1){
        root->keys[root->count_keys]=root->children[root->count_keys]->keys[degree/2];
        root->children[root->count_keys+1]=sisterNode(root->children[root->count_keys]);
       root->count_keys++;
    }
    return check(root,curr_level);
}


// ...existing code...

typedef struct QNode {
    Node* node;
    struct QNode* next;
} QNode;

typedef struct Queue {
    QNode* front;
    QNode* rear;
} Queue;

void enqueue(Queue* q, Node* node) {
    QNode* temp = (QNode*)malloc(sizeof(QNode));
    temp->node = node;
    temp->next = NULL;
    if (q->rear == NULL) {
        q->front = q->rear = temp;
        return;
    }
    q->rear->next = temp;
    q->rear = temp;
}

Node* dequeue(Queue* q) {
    if (q->front == NULL) return NULL;
    QNode* temp = q->front;
    Node* node = temp->node;
    q->front = q->front->next;
    if (q->front == NULL) q->rear = NULL;
    free(temp);
    return node;
}

int isEmpty(Queue* q) {
    return q->front == NULL;
}

void printer(Node* root) {
    if (root == NULL) return;
    Queue q = {NULL, NULL};
    enqueue(&q, root);
    int nodesInCurrentLevel = 1;
    int nodesInNextLevel = 0;
    while (!isEmpty(&q)) {
        Node* node = dequeue(&q);
        nodesInCurrentLevel--;
        // Print all keys in this node
        for (int i = 0; i < node->count_keys; i++) {
            printf("%d ", node->keys[i]);
        }
        printf("| ");
        // Enqueue all children for the next level
        for (int i = 0; i <= node->count_keys; i++) {
            if (node->children[i] != NULL) {
                enqueue(&q, node->children[i]);
                nodesInNextLevel++;
            }
        }
        if (nodesInCurrentLevel == 0) {
            printf("\n");
            nodesInCurrentLevel = nodesInNextLevel;
            nodesInNextLevel = 0;
        }
    }
}
// ...existing code...
int main(){
    Node* head=insertNode(NULL,10,level);
    for(int i=-9;i<=91;i+=5){
        head=insertNode(head,i,level);
       printer(head);
        printf("\n\n");
       //printf("level is %d\n",level);
    }
   printer(head);
}