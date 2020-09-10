#include<stdio.h>
#include<stdlib.h>
typedef struct node{
        int b;
        struct node* next;
    }Node;
void main()
{
    int a;
    Node* head=NULL;
    Node* operate;
    Node* record;
    while(1){
        scanf("%d",&a);
        if(a!=1){
            operate=(Node*)malloc(sizeof(Node));
        if(head==NULL){
            head=operate;
        }
        else{
            record->next=operate;
        }
        operate->b=a;
        operate->next=NULL;
        record=operate;
    }
        else{
            break;
        }
    }
    operate=head;
    while(operate!=NULL){
        printf("%d",operate->b);
        operate=operate->next;
    }
    operate=head;
    while(operate!=NULL){
        record=operate;
        free(operate);
        operate=record->next;
    }
}
