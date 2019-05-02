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

typedef int Status;          /* Status�Ǻ���������,��ֵ�Ǻ������״̬���룬��OK�� */
typedef int ElemType;        /* ElemType���͸���ʵ������������������Ϊint */

typedef struct
{
	ElemType *elem;  //�洢�ռ��ַ
	int length;    // ��ǰ����
	int listsize;  //��ǰ����Ĵ洢����
} SqList;

Status InitList_Sq(SqList &L);//����һ���յ����Ա�L
Status ListInsert_Sq(SqList &L,int i, ElemType e);//�ڵ�i��λ��֮ǰ��������Ԫ��e, L�ĳ��ȼ�1
void MergeList_Sq(SqList La,SqList Lb,SqList &Lc);//��֪���Ա�La��Lb������Ԫ�ذ�ֵ�ǵݼ�����,�鲢La��Lb�õ��µ����Ա�Lc,Lc������Ԫ��Ҳ��ֵ�ǵݼ�����
Status ListLength_Sq(SqList L);//����L������Ԫ�ظ���
Status GetElem_Sq(SqList L, int i, ElemType &e);//e����L�е�i������Ԫ�ص�ֵ
Status ListTraverse(SqList L);	//���ζ�L��ÿ������Ԫ�����
Status Visit(ElemType c);
Status BubbleSortIncrease(SqList &L);	//ð�ݵ�����������
Status BubbleSortDecrease(SqList &L);	//ð�ݵ����ݼ�����

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
		SL1.elem[i] = rand() % 20;	//����0~99֮�������� 
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

Status InitList_Sq(SqList &L)//����һ���յ����Ա�L
{
	L.elem = (ElemType *)malloc(LIST_INIT_SIZE*sizeof(SqList));
	if(!L.elem)
		exit (OVERFLOW);			//�洢����ʧ��
	L.length = 0;					// �ձ���Ϊ0
	L.listsize = LIST_INIT_SIZE;	// ��ʼ�洢����
	return OK;
} 

Status ListInsert_Sq(SqList &L,int i, ElemType e)//�ڵ�i��λ��֮ǰ��������Ԫ��e, L�ĳ��ȼ�1
{
	int j;
	ElemType * newbase;
	if(L.length+1 > L.listsize)
	{
		newbase = (ElemType *)realloc(L.elem, (L.listsize+LISTINCREMENT)*sizeof(ElemType));
		if(!newbase)
			exit(OVERFLOW);   //�洢����ʧ��
		L.elem = newbase;  // �»�ַ
		L.listsize += LISTINCREMENT;  // ���Ӵ洢����
	}
	for(j=L.length; j>=i; j--)
	{
		*(L.elem+j) = *(L.elem+j-1);
	}
	*(L.elem+i-1) = e;
	L.length++;   // ����1
	return OK;
}

Status BubbleSortIncrease(SqList &L)	//ð�ݵ�����������
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

Status BubbleSortDecrease(SqList &L)	//ð�ݵ����ݼ�����
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


//��֪���Ա�La��Lb������Ԫ�ذ�ֵ�ǵݼ�����
//�鲢La��Lb�õ��µ����Ա�Lc,Lc������Ԫ��Ҳ��ֵ�ǵݼ�����
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
		//La, LbΪ�ǿձ�
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

Status ListLength_Sq(SqList L)//����L������Ԫ�ظ���
{
	return L.length;
}

Status GetElem_Sq(SqList L, int i, ElemType &e)//e����L�е�i������Ԫ�ص�ֵ
{
	e = *(L.elem + i - 1);
	return OK;
}

Status ListTraverse(SqList L)	//���ζ�L��ÿ������Ԫ�����
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
