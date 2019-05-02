#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW -2

typedef int Status;          /* Status�Ǻ���������,��ֵ�Ǻ������״̬���룬��OK�� */
typedef int ElemType;        /* ElemType���͸���ʵ������������������Ϊint */

//***���Ա�Ķ�̬����˳��洢�ṹ
#define LIST_INIT_SIZE 5
#define LISTINCREMENT 10
#define N 10	//���������ܸ��� 

typedef struct
{
	ElemType *elem;	//�洢�ռ��ַ
	int length;		//��ǰ����
	int listsize;	//��ǰ����Ĵ洢��������sizeof��ElemType��Ϊ��λ��
} SqList;

Status InitList_Sq(SqList &L);  //����˳���
Status CreateList(SqList &L); 	//��ʼ��˳���
Status BubbleSort(SqList &L);	//ð������
Status ListTraverse(SqList L);	//���ζ�L��ÿ������Ԫ�����
Status Visit(ElemType c);		//����˳���
Status DeleteSame(SqList *L);	//ȥ���ظ�Ԫ��
Status Swap(ElemType *a,ElemType *b); //����������
Status Reverse(SqList L); 		//����˳���

int main()
{
	int i;
	SqList L;
	i = InitList_Sq(L);
	if (i)
		printf("����%d��������\n",N);
	else
		exit (0);

	CreateList(L);

	BubbleSort(L);
	printf ("˳���ǵ��������\n");
	ListTraverse(L);

	printf("ȥ���ظ�Ԫ�أ�\n");
	DeleteSame(&L);
	ListTraverse(L);

	Reverse(L);
	printf("���ã�\n");
	ListTraverse(L);

	return 0;
}

Status InitList_Sq(SqList &L)  //����˳���
{
	L.elem=(ElemType*)malloc(LIST_INIT_SIZE*sizeof(ElemType)); //����ռ�
	if (! L.elem) exit (OVERFLOW);
	L.length = 0;
	L.listsize = LIST_INIT_SIZE;
	return OK;
}

Status CreateList(SqList &L) //��ʼ��˳���
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


Status BubbleSort(SqList &L)	//ð������
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
	printf("%d ",c);
	return OK;
}

Status DeleteSame(SqList *L)		//ȥ���ظ�Ԫ��
{
	//��һ��ʱ�临�Ӷ�ΪO��n��
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

	/*������ʱ�临�Ӷ�ΪO��n��
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
	/*������ʱ�临�Ӷ�ΪO��n^3��
	int i,j,k;
	for(i=0;i<L.length-1;i++)       //i����˳���
	{
	    for(j=i+1;j<L.length;j++)   //j����i֮���˳���Ԫ�أ���ÿ����iλ���бȽ�
	        if(L.elem[i]==L.elem[j])
	    	{
	        	for(k=j;k<L.length-1;k++)  //�����ظ�Ԫ�أ�����λ�����Ԫ��ȫ��һ������
	            	L.elem[k]=L.elem[k+1];
	        	L.length--;         //ɾ��Ԫ�غ�˳����ȼ�1
	    	}
	}*/
	return OK;
}

Status Reverse(SqList L) //����˳���
{
	int i;
	for(i=0; i<L.length/2; i++)
	{
		Swap(L.elem+i,L.elem+L.length-1-i);
	}
	return OK;
}

Status Swap(ElemType *a,ElemType *b) //����������
{
	ElemType temp;
	temp=*a;
	*a=*b;
	*b=temp;
	return OK;
}

/*���н��
����10��������
1 5 5 6 3 8 9 9 7 2
˳���ǵ��������
9 9 8 7 6 5 5 3 2 1
ȥ���ظ�Ԫ�أ�
9 8 7 6 5 3 2 1
���ã�
1 2 3 5 6 7 8 9
*/
