#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW -2

#define STACK_INIT_SIZE	100				//顺序栈存储空间的初始分配量
#define STACKINCREMENT	10 				//顺序栈存储空间的分配增量

typedef int SElemType_Sq;
typedef int QElemType_L;
typedef int Status;

typedef struct
{
	SElemType_Sq *base;				//在栈构造之前和销毁之后，base的值为NULL 
	SElemType_Sq *top;				//栈顶指针 
	int stacksize;					//当前已分配的存储空间，以元素为单位 
}SqStack;

typedef struct QNode				 
{
	QElemType_L data;
	struct QNode *next;
}QNode,* QueuePtr;

typedef struct
{
	QueuePtr front;					//头指针 
	QueuePtr rear;					//尾指针 
}LinkQueue;							//队列的链式存储表示 

Status InitStack_Sq(SqStack &S);
Status DestroyStack_Sq(SqStack &S);
Status ClearStack_Sq(SqStack &S);
Status StackEmpty_Sq(SqStack S);
int StackLength_Sq(SqStack S);
Status GetTop_Sq(SqStack S, SElemType_Sq &e);
Status Push_Sq(SqStack &S, SElemType_Sq e);
Status Pop_Sq(SqStack &S, SElemType_Sq &e);
Status StackTraverse_Sq(SqStack S, void(Visit)(SElemType_Sq));
Status InitQueue_L(LinkQueue &Q);
void ClearQueue_L(LinkQueue &Q);
void DestroyQueue_L(LinkQueue &Q);
Status QueueEmpty_L(LinkQueue Q);
int QueueLength_L(LinkQueue Q);
Status GetHead_L(LinkQueue Q, QElemType_L &e);
Status EnQueue_L(LinkQueue &Q, QElemType_L e);
Status DeQueue_L(LinkQueue &Q, QElemType_L &e);
void QueueTraverse_L(LinkQueue Q, void (Visit)(QElemType_L));
Status PhraseString(char c[]);

int main()
{
	char a[] = "abcdedcba@";
	char b[] = "ababab@";
	
	if(PhraseString(a))
		printf("%s（不包括@）是回文序列！\n", a);
	else
		printf("%s（不包括@）不是回文序列！！\n", a);
	printf("\n");
	
	if(PhraseString(b))
		printf("%s（不包括@）是回文序列！\n", b);
	else
		printf("%s（不包括@）不是回文序列！！\n", b);
	printf("\n");

	return 0;
}

Status PhraseString(char c[])
{
	SqStack S;
	LinkQueue Q;
	SElemType_Sq e1;
	QElemType_L e2;
	int i;
	
	InitStack_Sq(S);
	InitQueue_L(Q);
	
	for(i=0; c[i]!='@'; i++)		//将字符读入栈和队列
	{
		Push_Sq(S, c[i]);
		EnQueue_L(Q, c[i]);
	}
	
	while(!StackEmpty_Sq(S) && !QueueEmpty_L(Q))
	{
		Pop_Sq(S, e1);
		DeQueue_L(Q, e2);
		if(e1!=e2)
			return ERROR;
	}
	
	if(StackEmpty_Sq(S) && QueueEmpty_L(Q))
		return OK;
}

//栈*********************************
Status InitStack_Sq(SqStack &S)
{
 	S.base = (SElemType_Sq *)malloc(STACK_INIT_SIZE*sizeof(SElemType_Sq));
	if(!S.base)
		exit(OVERFLOW);
		
	S.top = S.base;
	S.stacksize = STACK_INIT_SIZE;
	
	return OK;
} 

Status DestroyStack_Sq(SqStack &S)
{
	free(S.base);
	
	S.base = NULL;
	S.top = NULL;
	S.stacksize = 0;
	
	return OK;
} 

Status ClearStack_Sq(SqStack &S)
{
	S.top = S.base;
	
	return OK;
} 

Status StackEmpty_Sq(SqStack S)
{
	if(S.top==S.base)
		return TRUE;
	else
		return FALSE;
} 

int StackLength_Sq(SqStack S)
{
	return S.top - S.base;
} 

Status GetTop_Sq(SqStack S, SElemType_Sq &e)
{
	if(S.top==S.base)
		return ERROR;
		
	e = *(S.top - 1);							//并不破坏栈 
	
	return OK;

} 

Status Push_Sq(SqStack &S, SElemType_Sq e)
{
	if(S.top-S.base>=S.stacksize)		//栈满，追加存储空间
	{
		S.base = (SElemType_Sq *)realloc(S.base, (S.stacksize+STACKINCREMENT)*sizeof(SElemType_Sq));
		if(!S.base)
			exit(OVERFLOW);						//存储分配失败
		S.top = S.base + S.stacksize;
		S.stacksize += STACKINCREMENT;
	}
	
	*(S.top) = e;								//进栈先赋值，栈顶指针再自增 
	(S.top)++;

	return OK;
} 

Status Pop_Sq(SqStack &S, SElemType_Sq &e)
{
	if(S.top==S.base)
		return ERROR;

	S.top--;									//出栈栈顶指针先递减，再赋值 
	e = *(S.top);

	return OK;
} 

Status StackTraverse_Sq(SqStack S, void(Visit)(SElemType_Sq))
{												//遍历不应该破坏栈 
	SElemType_Sq *p = S.base;
	
	while(p<S.top)
		Visit(*p++);
	
	return OK;
}

//队列*********************************
Status InitQueue_L(LinkQueue &Q)
{
	Q.front = Q.rear = (QueuePtr)malloc(sizeof(QNode));
	if(!Q.front)
		exit(OVERFLOW);

	Q.front->next = NULL;

	return OK;
}

void ClearQueue_L(LinkQueue &Q)
{
	Q.rear = Q.front->next;
	
	while(Q.rear)
	{
		Q.front->next = Q.rear->next;		
		free(Q.rear);		
		Q.rear = Q.front->next;
	}
	
	Q.rear = Q.front;
}

void DestroyQueue_L(LinkQueue &Q)
{
	while(Q.front)
	{
		Q.rear = Q.front->next;
		free(Q.front);
		Q.front = Q.rear;	
	}
}

Status QueueEmpty_L(LinkQueue Q)
{
	if(Q.front==Q.rear)
		return TRUE;
	else
		return FALSE;
} 

int QueueLength_L(LinkQueue Q)
{
	int count = 0;
	QueuePtr p = Q.front;
	
	while(p!=Q.rear)
	{
		count++;
		p = p->next;
	}
	
	return count;
} 

Status GetHead_L(LinkQueue Q, QElemType_L &e)
{
	QueuePtr p;
	
	if(Q.front==Q.rear)
		return ERROR;
		
	p = Q.front->next;
	e = p->data;
	
	return OK;
} 

Status EnQueue_L(LinkQueue &Q, QElemType_L e)
{
	QueuePtr p;
	
	p = (QueuePtr)malloc(sizeof(QNode));
	if(!p)
		exit(OVERFLOW);

	p->data = e;
	p->next = NULL;
	
	Q.rear->next = p;
	Q.rear=p;

	return OK;
} 

Status DeQueue_L(LinkQueue &Q, QElemType_L &e)
{
	QueuePtr p;
	
	if(Q.front==Q.rear)
		return ERROR;
		
	p = Q.front->next;
	e = p->data;
	
	Q.front->next = p->next;
	if(Q.rear==p)
		Q.rear = Q.front;
		
	free(p);
	
	return OK;
} 

void QueueTraverse_L(LinkQueue Q, void (Visit)(QElemType_L))
{
	QueuePtr p;
	
	p = Q.front->next;
	
	while(p)
	{
		Visit(p->data);
		p = p->next;
	}
}
