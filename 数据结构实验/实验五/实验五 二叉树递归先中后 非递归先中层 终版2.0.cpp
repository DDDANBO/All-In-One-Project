#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW -2

typedef int Status;
typedef char TElemType;							//���������Ԫ�ؾ�Ϊ�ַ�

typedef struct BiTNode
{
	TElemType data;								//���Ԫ��
	struct BiTNode* lchild;						//����ָ��
	struct BiTNode* rchild;						//�Һ���ָ��
} BiTNode;										//���������
typedef BiTNode* BiTree;						//ָ�����������ָ��

/* ջԪ������ */
typedef BiTree SElemType_Sq;					//�ض���ջԪ������
#include "E:\���ݽṹ���㷨\���ݽṹ��ε��\Data-Structure-master\���α��㷨ʵ��\��03 ջ�Ͷ���\01 SequenceStack\SequenceStack.c"	//**��03 ջ�Ͷ���**// 
#include "E:\���ݽṹ���㷨\���ݽṹ��ε��\Data-Structure-master\���α��㷨ʵ��\��03 ջ�Ͷ���\07 LinkQueue\LinkQueue.c" 

/* �洢��ǰ�����Ϣ */
/* �ڰ����ṹ��ӡ����ʱ��ʹ�� */ 
typedef struct									
{ 
	BiTree left;								//��ǰ������ָ�� 
	BiTree right;								//��ǰ������ָ�� 
	int n;										//��ǰ���Ĵ��� 
}Node;

void InitBiTree(BiTree &T); 
Status CreateBiTree(BiTree &T);
void ClearBiTree(BiTree T);
Status PreOrderTraverse_1(BiTree T, Status (*Visit)(TElemType e));
Status InOrderTraverse_1(BiTree T, Status (*Visit)(TElemType e));
Status PostOrderTraverse(BiTree T, Status (*Visit)(TElemType e));
Status PreOrderTraverse_2(BiTree T, Status (*Visit)(TElemType e));
Status InOrderTraverse_2(BiTree T, Status (*Visit)(TElemType e));
Status LevelOrderTraverse_2(BiTree T, Status (*Visit)(TElemType e));
int BiTreeDepth(BiTree T);
void PrintTree(BiTree T);
Status Visit(TElemType e);

int main()
{
	BiTree T;
	
	Status(*visit)(TElemType e)=Visit;
	
	printf("����Ҫ�������������������У�'^'��ʾ�ս�㣨�磺T�� ABDG^^^EH^^I^^CF^J^^^��\n");
	//����������ABDH^^I^^EJ^^K^^CFL^^M^^GN^^O^^
	InitBiTree(T);
	CreateBiTree(T);
	printf("\n");

	printf("������TΪ��\n");
	PrintTree(T);
	printf("\n");

	printf("�ݹ������������Ϊ��");
	PreOrderTraverse_1(T,visit); 
	printf("\n\n");

	printf("�ݹ������������Ϊ��");
	InOrderTraverse_1(T,visit);
	printf("\n\n");
	
	printf("�ݹ�����������Ϊ��");
	PostOrderTraverse(T,visit);
	printf("\n\n");
	
	printf("�ǵݹ������������Ϊ��");
	PreOrderTraverse_2(T,visit);
	printf("\n\n");
	
	printf("�ǵݹ������������Ϊ��");
	InOrderTraverse_2(T,visit);
	printf("\n\n");

	printf("�ǵݹ��α�������Ϊ��");
	LevelOrderTraverse_2(T,visit);
	printf("\n\n");
	
	return 0;
}

void InitBiTree(BiTree &T)
{
	T = NULL;
} 

void ClearBiTree(BiTree T)
{
	if(T)												 		//��������Ϊ��
	{
		if(T->lchild)										//������� 
			ClearBiTree(T->lchild);							
		if(T->rchild)										//������� 
			ClearBiTree(T->rchild);

		free(T);												//�ͷŸ���� 
		
		T = NULL;												//�ÿ���ָ�� 
	} 
}

void DestroyBiTree(BiTree &T)
{
	//�������޷����� 
}

Status BiTreeEmpty(BiTree T)
{
	return T==NULL ? TRUE : ERROR;
} 


Status CreateBiTree(BiTree &T)
{

	char ch;//"ABDG^^^EH^^I^^CF^J^^^"
	scanf ("%c",&ch);

	if(ch == '^')
		T = NULL;
		
	else
	{
		T = (BiTree)malloc(sizeof(BiTNode));
		if(!T)
			exit(OVERFLOW);
		T->data = ch;
		CreateBiTree(T->lchild);
		CreateBiTree(T->rchild);
	}

	return OK;
}

//�ݹ�������� 
Status PreOrderTraverse_1(BiTree T, Status (*Visit)(TElemType e))
{
	if(T)
	{
		Visit(T->data);
		PreOrderTraverse_1(T->lchild, Visit);
		PreOrderTraverse_1(T->rchild, Visit);
	}
	return OK;
}

//�ݹ�������� 
Status InOrderTraverse_1(BiTree T, Status (*Visit)(TElemType e))
{
	if(T)
	{
		InOrderTraverse_1(T->lchild, Visit);
		Visit(T->data);
		InOrderTraverse_1(T->rchild, Visit);
	}
	return OK;
}

//�ݹ������� 
Status PostOrderTraverse(BiTree T, Status (*Visit)(TElemType e))
{
	if(T)
	{
		PostOrderTraverse(T->lchild, Visit);
		PostOrderTraverse(T->rchild, Visit);
		Visit(T->data);
	}
	return OK;
}

//�ǵݹ��������
Status PreOrderTraverse_2(BiTree T, Status (*Visit)(TElemType e))
{
	SqStack S;
	SElemType_Sq e;

	if(!BiTreeEmpty(T))
	{
		InitStack_Sq(&S);
		Push_Sq(&S, T);

		while(!StackEmpty_Sq(S))
		{
			GetTop_Sq(S, &e);
			Visit(e->data);

			if(e->lchild)
				Push_Sq(&S, e->lchild);
			else
			{
				while(!StackEmpty_Sq(S))
				{
					Pop_Sq(&S, &e);

					if(e->rchild)
					{
						Push_Sq(&S, e->rchild);
						break;
					}
				}
			}
		}
	}
	return OK;
}

//�ǵݹ��������
Status InOrderTraverse_2(BiTree T, Status (*Visit)(TElemType e))
{
	SqStack S;
	SElemType_Sq p;
	
	InitStack_Sq(&S);
	Push_Sq(&S, T);
	
	while(!StackEmpty_Sq(S))
	{
		while(GetTop_Sq(S, &p) && p)				//ջ��Ԫ�ز�Ϊ��
			Push_Sq(&S, p->lchild);					//�����ߵ���ͷ
			
		Pop_Sq(&S, &p);								//��ָ����ջ
		
		if(!StackEmpty_Sq(S))						//���ʽ�㣬����һ�� 
		{
			Pop_Sq(&S, &p);
			if(!Visit(p->data))
				return ERROR;
			Push_Sq(&S, p->rchild);
		}  
	}
	return OK;	 
}

//�������������_2�����ö���ʵ�֣�
Status LevelOrderTraverse_2(BiTree T, Status (*Visit)(TElemType e))
{
	BiTree p;
	LinkQueue Q;
	InitQueue_L(&Q);
	p = T;
	while (p || !QueueEmpty_L(Q))
	{
		if (p)
		{
			Visit(p->data);
			if (p->lchild)
				EnQueue_L(&Q, p->lchild);
			if (p->rchild)
				EnQueue_L(&Q, p->rchild);
			if (!QueueEmpty_L(Q))
				DeQueue_L(&Q, &p);
			else		//����Ϊ��ʱ���˳�whileѭ��
				break;
		}
	}
	return OK;
}

void PrintTree(BiTree T)
{
	int row, col;
	int i, j, m, l, r;
	BiTNode a[100][100] = {};					//��һ���㹻��ľ���ԭ���洢�� 
	
	if(T)
	{
		row = BiTreeDepth(T);					//������
		col = pow(2, row) - 1;					//������
		
		for(i=1; i<=row-1; i++)
		{
			for(j=1; j<=pow(2, i-1); j++)
			{
				m = (2*j-1)*pow(2, row-i);		//��ǰ�н�����λ�� 
				l = (4*j-3)*pow(2, row-i-1);	//��һ�н�����λ�� 
				r = (4*j-1)*pow(2, row-i-1);
				
				if(i==1)						//��ʼ�� 
					a[i][m] = *T;
				
				if(a[i][m].lchild)				//��һ�� 
					a[i+1][l] = *(a[i][m].lchild);
					
				if(a[i][m].rchild)				//��һ�� 
					a[i+1][r] = *(a[i][m].rchild);
			} 
		}
		
		for(i=1; i<=row; i++)
		{
			for(j=1; j<=col; j++)
			{
				if(a[i][j].data)
					printf("%c", a[i][j].data);
				else
					printf(" ");
			}
			printf("\n");
		}
	}
}

int BiTreeDepth(BiTree T)
{
	int LD, RD;
	
	if(T==NULL)
		return 0;								//�������Ϊ0 
	else
	{
		LD = BiTreeDepth(T->lchild);			//����������� 
		RD = BiTreeDepth(T->rchild);			//����������� 
	
		return (LD>=RD?LD:RD)+1;
	}
}

Status Visit(TElemType e)
{
	// �Զ������е�����Ԫ�ط���
	if(e=='\0')
	{
		return ERROR;
	}
	else
	{
		printf("%c",e);
	}
	return OK;
}
