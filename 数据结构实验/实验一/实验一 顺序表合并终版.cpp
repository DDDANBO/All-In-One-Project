#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW -2

#define LIST_INIT_SIZE 20
#define LISTINCREMENT 10
#define N 5

typedef int Status;          /* Status是函数的类型,其值是函数结果状态代码，如OK等 */
typedef int ElemType;        /* ElemType类型根据实际情况而定，这里假设为int */

typedef struct
{
	ElemType *elem;  //存储空间基址
	int length;    // 当前长度
	int listsize;  //当前分配的存储容量
} SqList;

Status InitList_Sq(SqList &L);//构造一个空的线性表L
Status ListInsert_Sq(SqList &L,int i, ElemType e);//在第i个位置之前插入数据元素e, L的长度加1
void MergeList_Sq(SqList La,SqList Lb,SqList &Lc);//已知线性表La和Lb中数据元素按值非递减排列,归并La和Lb得到新的线性表Lc,Lc的数据元素也按值非递减排列
Status ListLength_Sq(SqList L);//返回L中数据元素个数
Status GetElem_Sq(SqList L, int i, ElemType &e);//e返回L中第i个数据元素的值
Status ListTraverse(SqList L);	//依次对L的每个数据元素输出
Status Visit(ElemType c);
Status BubbleSortIncrease(SqList &L);	//冒泡单调递增排序
Status BubbleSortDecrease(SqList &L);	//冒泡单调递减排序

int main()
{
	srand((int) time(0));
	
	int i;
	SqList SL1, SL2;
	InitList_Sq(SL1);
	InitList_Sq(SL2);
	
	SL1.length = N;
	SL2.length = N;
	for (i = 0; i < SL1.length; i++)
		SL1.elem[i] = rand() % 20;	//产生0~99之间的随机数 
	for (int i = 0; i < SL2.length; i++)
		SL2.elem[i] = rand() % 20;
	
	BubbleSortIncrease(SL1);
	BubbleSortIncrease(SL2);
	
	ListTraverse(SL1);
	ListTraverse(SL2);
	
	SqList SL3;
	MergeList_Sq(SL1, SL2, SL3);
	BubbleSortDecrease(SL3);
	ListTraverse(SL3);
	
	return 0;
}

Status InitList_Sq(SqList &L)//构造一个空的线性表L
{
	L.elem = (ElemType *)malloc(LIST_INIT_SIZE*sizeof(SqList));
	if(!L.elem)
		exit (OVERFLOW);			//存储分配失败
	L.length = 0;					// 空表长度为0
	L.listsize = LIST_INIT_SIZE;	// 初始存储容量
	return OK;
} 

Status ListInsert_Sq(SqList &L,int i, ElemType e)//在第i个位置之前插入数据元素e, L的长度加1
{
	int j;
	ElemType * newbase;
	if(L.length+1 > L.listsize)
	{
		newbase = (ElemType *)realloc(L.elem, (L.listsize+LISTINCREMENT)*sizeof(ElemType));
		if(!newbase)
			exit(OVERFLOW);   //存储分配失败
		L.elem = newbase;  // 新基址
		L.listsize += LISTINCREMENT;  // 增加存储容量
	}
	for(j=L.length; j>=i; j--)
	{
		*(L.elem+j) = *(L.elem+j-1);
	}
	*(L.elem+i-1) = e;
	L.length++;   // 表长加1
	return OK;
}

Status BubbleSortIncrease(SqList &L)	//冒泡单调递增排序
{
	ElemType temp;
	int i,j;
	for (i = 0; i < N; i++)
	{
		for (j = 0; j<N-1-i; j++)
		{
			if (L.elem[j] > L.elem[j+1])
			{
				temp = L.elem[j];
				L.elem[j] = L.elem[j+1];
				L.elem[j+1] = temp;
			}
		}
	}
	return OK;
}

Status BubbleSortDecrease(SqList &L)	//冒泡单调递减排序
{
	ElemType temp;
	int i,j;
	for (i = 0; i < 2*N; i++)
	{
		for (j = 0; j<2*N-1-i; j++)
		{
			if (L.elem[j] < L.elem[j+1])
			{
				temp = L.elem[j];
				L.elem[j] = L.elem[j+1];
				L.elem[j+1] = temp;
			}
		}
	}
	return OK;
}


//已知线性表La和Lb中数据元素按值非递减排列
//归并La和Lb得到新的线性表Lc,Lc的数据元素也按值非递减排列
void MergeList_Sq(SqList La,SqList Lb,SqList &Lc)
{
	
	int i, j, k, ai, bj;
	int La_len, Lb_len;
	ElemType e;
	InitList_Sq(Lc);
	i = j = k = 1;
	La_len = ListLength_Sq(La);
	Lb_len = ListLength_Sq(Lb);
	while(i<=La_len && j<=Lb_len)
	{
		//La, Lb为非空表
		GetElem_Sq(La, i, ai);
		GetElem_Sq(Lb, j, bj);
		if(ai <= bj)
		{
			ListInsert_Sq(Lc, k++, ai);
			i++;
		}
		else
		{
			ListInsert_Sq(Lc, k++, bj);
			j++;
		}
	}
	while(i <= La_len)
	{
		GetElem_Sq(La, i++, ai);
		ListInsert_Sq(Lc, k++, ai);
	}
	while(j <= Lb_len)
	{
		GetElem_Sq(Lb, j++, bj);
		ListInsert_Sq(Lc, k++, bj);
	}
}

Status ListLength_Sq(SqList L)//返回L中数据元素个数
{
	return L.length;
}

Status GetElem_Sq(SqList L, int i, ElemType &e)//e返回L中第i个数据元素的值
{
	e = *(L.elem + i - 1);
	return OK;
}

Status ListTraverse(SqList L)	//依次对L的每个数据元素输出
{
	int i;
	for(i=0; i<L.length; i++)
		Visit(L.elem[i]);
	printf("\n");
	return OK;
}

Status Visit(ElemType c)
{
	printf("%3d",c);
	return OK;
}
