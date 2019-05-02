#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW -2

typedef int Status;          /* Status是函数的类型,其值是函数结果状态代码，如OK等 */
typedef int ElemType;        /* ElemType类型根据实际情况而定，这里假设为int */

//***线性表的动态分配顺序存储结构
#define LIST_INIT_SIZE 5
#define LISTINCREMENT 10
#define N 10	//输入数据总个数 

typedef struct
{
	ElemType *elem;	//存储空间基址
	int length;		//当前长度
	int listsize;	//当前分配的存储容量（以sizeof（ElemType）为单位）
} SqList;

Status InitList_Sq(SqList &L);  //创建顺序表
Status CreateList(SqList &L); 	//初始化顺序表
Status BubbleSort(SqList &L);	//冒泡排序
Status ListTraverse(SqList L);	//依次对L的每个数据元素输出
Status Visit(ElemType c);		//遍历顺序表
Status DeleteSame(SqList *L);	//去除重复元素
Status Swap(ElemType *a,ElemType *b); //交换两个数
Status Reverse(SqList L); 		//逆置顺序表

int main()
{
	int i;
	SqList L;
	i = InitList_Sq(L);
	if (i)
		printf("输入%d个整数：\n",N);
	else
		exit (0);

	CreateList(L);

	BubbleSort(L);
	printf ("顺序表非递增排序后：\n");
	ListTraverse(L);

	printf("去除重复元素：\n");
	DeleteSame(&L);
	ListTraverse(L);

	Reverse(L);
	printf("逆置：\n");
	ListTraverse(L);

	return 0;
}

Status InitList_Sq(SqList &L)  //创建顺序表
{
	L.elem=(ElemType*)malloc(LIST_INIT_SIZE*sizeof(ElemType)); //分配空间
	if (! L.elem) exit (OVERFLOW);
	L.length = 0;
	L.listsize = LIST_INIT_SIZE;
	return OK;
}

Status CreateList(SqList &L) //初始化顺序表
{
	int data,i=0;
	while(i<N)
	{
		scanf("%d",&data);
		L.elem[i++]=data;
		L.length++;
	}
	return OK;
}


Status BubbleSort(SqList &L)	//冒泡排序
{
	ElemType temp;
	int i,j;
	for (i = 0; i < N; i++)
	{
		for (j = 0; j<N-1-i; j++)
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
	printf("%d ",c);
	return OK;
}

Status DeleteSame(SqList *L)		//去除重复元素
{
	//法一：时间复杂度为O（n）
	int k=0;
	ElemType temp = -1;
	for(int i=0; i<L->length; i++)
	{
		if(L->elem[i]!=temp)
		{
			temp=L->elem[i];
			L->elem[k++]=L->elem[i];
		}
	}
	L->length=k;

	/*法二：时间复杂度为O（n）
	int *p,*q,*r=L->length+L->elem;
	for(p=L->elem,q=p+1; q<r; q++)
	{
		if(*p!=*q)
		{
			*(p+1)=*q;
			p++;
		}
		else
		{
			L->length--;
		}
	}
	*/
	/*法三：时间复杂度为O（n^3）
	int i,j,k;
	for(i=0;i<L.length-1;i++)       //i遍历顺序表
	{
	    for(j=i+1;j<L.length;j++)   //j遍历i之后的顺序表元素，并每次与i位进行比较
	        if(L.elem[i]==L.elem[j])
	    	{
	        	for(k=j;k<L.length-1;k++)  //遇到重复元素，将该位后面的元素全部一次左移
	            	L.elem[k]=L.elem[k+1];
	        	L.length--;         //删除元素后，顺序表长度减1
	    	}
	}*/
	return OK;
}

Status Reverse(SqList L) //逆置顺序表
{
	int i;
	for(i=0; i<L.length/2; i++)
	{
		Swap(L.elem+i,L.elem+L.length-1-i);
	}
	return OK;
}

Status Swap(ElemType *a,ElemType *b) //交换两个数
{
	ElemType temp;
	temp=*a;
	*a=*b;
	*b=temp;
	return OK;
}

/*运行结果
输入10个整数：
1 5 5 6 3 8 9 9 7 2
顺序表非递增排序后：
9 9 8 7 6 5 5 3 2 1
去除重复元素：
9 8 7 6 5 3 2 1
逆置：
1 2 3 5 6 7 8 9
*/
