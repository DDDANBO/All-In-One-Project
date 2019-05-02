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
typedef char TElemType;							//假设二叉树元素均为字符

typedef struct BiTNode
{
	TElemType data;								//结点元素
	struct BiTNode* lchild;						//左孩子指针
	struct BiTNode* rchild;						//右孩子指针
} BiTNode;										//二叉树结点
typedef BiTNode* BiTree;						//指向二叉树结点的指针

/* 栈元素类型 */
typedef BiTree SElemType_Sq;					//重定义栈元素类型
#include "E:\数据结构和算法\数据结构严蔚敏\Data-Structure-master\▲课本算法实现\▲03 栈和队列\01 SequenceStack\SequenceStack.c"	//**▲03 栈和队列**// 
#include "E:\数据结构和算法\数据结构严蔚敏\Data-Structure-master\▲课本算法实现\▲03 栈和队列\07 LinkQueue\LinkQueue.c" 

/* 存储当前结点信息 */
/* 在按树结构打印树的时候使用 */ 
typedef struct									
{ 
	BiTree left;								//当前结点的左指针 
	BiTree right;								//当前结点的右指针 
	int n;										//当前结点的次序 
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
	
	printf("输入要创建二叉树的先序序列，'^'表示空结点（如：T→ ABDG^^^EH^^I^^CF^J^^^）\n");
	//满二叉树：ABDH^^I^^EJ^^K^^CFL^^M^^GN^^O^^
	InitBiTree(T);
	CreateBiTree(T);
	printf("\n");

	printf("二叉树T为：\n");
	PrintTree(T);
	printf("\n");

	printf("递归先序遍历序列为：");
	PreOrderTraverse_1(T,visit); 
	printf("\n\n");

	printf("递归中序遍历序列为：");
	InOrderTraverse_1(T,visit);
	printf("\n\n");
	
	printf("递归后序遍历序列为：");
	PostOrderTraverse(T,visit);
	printf("\n\n");
	
	printf("非递归先序遍历序列为：");
	PreOrderTraverse_2(T,visit);
	printf("\n\n");
	
	printf("非递归中序遍历序列为：");
	InOrderTraverse_2(T,visit);
	printf("\n\n");

	printf("非递归层次遍历序列为：");
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
	if(T)												 		//二叉树不为空
	{
		if(T->lchild)										//清空左树 
			ClearBiTree(T->lchild);							
		if(T->rchild)										//清空右树 
			ClearBiTree(T->rchild);

		free(T);												//释放根结点 
		
		T = NULL;												//置空树指针 
	} 
}

void DestroyBiTree(BiTree &T)
{
	//二叉树无法销毁 
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

//递归先序遍历 
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

//递归中序遍历 
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

//递归后序遍历 
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

//非递归先序遍历
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

//非递归中序遍历
Status InOrderTraverse_2(BiTree T, Status (*Visit)(TElemType e))
{
	SqStack S;
	SElemType_Sq p;
	
	InitStack_Sq(&S);
	Push_Sq(&S, T);
	
	while(!StackEmpty_Sq(S))
	{
		while(GetTop_Sq(S, &p) && p)				//栈顶元素不为空
			Push_Sq(&S, p->lchild);					//向左走到尽头
			
		Pop_Sq(&S, &p);								//空指针退栈
		
		if(!StackEmpty_Sq(S))						//访问结点，向右一步 
		{
			Pop_Sq(&S, &p);
			if(!Visit(p->data))
				return ERROR;
			Push_Sq(&S, p->rchild);
		}  
	}
	return OK;	 
}

//层序遍历二叉树_2（利用队列实现）
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
			else		//队列为空时，退出while循环
				break;
		}
	}
	return OK;
}

void PrintTree(BiTree T)
{
	int row, col;
	int i, j, m, l, r;
	BiTNode a[100][100] = {};					//用一个足够大的矩阵按原样存储树 
	
	if(T)
	{
		row = BiTreeDepth(T);					//总行数
		col = pow(2, row) - 1;					//总列数
		
		for(i=1; i<=row-1; i++)
		{
			for(j=1; j<=pow(2, i-1); j++)
			{
				m = (2*j-1)*pow(2, row-i);		//当前行结点相对位序 
				l = (4*j-3)*pow(2, row-i-1);	//下一行结点相对位序 
				r = (4*j-1)*pow(2, row-i-1);
				
				if(i==1)						//初始化 
					a[i][m] = *T;
				
				if(a[i][m].lchild)				//下一行 
					a[i+1][l] = *(a[i][m].lchild);
					
				if(a[i][m].rchild)				//下一行 
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
		return 0;								//空树深度为0 
	else
	{
		LD = BiTreeDepth(T->lchild);			//求左子树深度 
		RD = BiTreeDepth(T->rchild);			//求右子树深度 
	
		return (LD>=RD?LD:RD)+1;
	}
}

Status Visit(TElemType e)
{
	// 对二叉树中的数据元素访问
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
