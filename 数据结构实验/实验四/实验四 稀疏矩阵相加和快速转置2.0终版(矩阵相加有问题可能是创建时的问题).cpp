#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW -2

#define MAXSIZE 400							//�������Ԫ���������ֵΪ400

/* ��Ԫ��ϡ��������Ͷ��� */
typedef int MElemType_TSq;
typedef int Status;

typedef struct
{
	int i, j;								//�÷���Ԫ�����±�����±�
	MElemType_TSq e;
} Triple;

typedef struct
{
	Triple data[MAXSIZE+1];					//����Ԫ��Ԫ���data[0]δ��
	int mu, nu, tu;							//����������������ͷ���Ԫ����
} TSMatrix;

Status CreateSMatrix_T(int row, int col, TSMatrix &M);
void DestroySMatrix_T(TSMatrix &M);
void PrintSMatrix_T(TSMatrix M);
void CopySMatrix_T(TSMatrix M, TSMatrix &T);
Status AddSMatri_T(TSMatrix M, TSMatrix N, TSMatrix &Q);
void FastTransposeSMatrix_T(TSMatrix M, TSMatrix &T);


int main()
{
	srand((int) time(0));

	TSMatrix A, B, C, D;
	int col,row;

	printf("��������ϡ����� A��B...\n");
	printf ("����������������������\n");
	scanf ("%d %d",&row,&col);

	CreateSMatrix_T(row, col, A);
	CreateSMatrix_T(row, col, B);
	printf("A = \n");
	PrintSMatrix_T(A);
	printf("B = \n");
	PrintSMatrix_T(B);
	printf("\n");

	AddSMatri_T(A, B, C);
	printf("C = A + B = \n");
	PrintSMatrix_T(C);
	printf("\n");
	
	printf ("����C����ת��Ϊ����D��\n");
	FastTransposeSMatrix_T(C,D);
	PrintSMatrix_T(D);
	printf("\n");

	return 0;
}


Status CreateSMatrix_T(int row, int col, TSMatrix &M)
{
	M.mu = row;
	M.nu = col;
	M.tu = 0;
	int i, j = 1, k = 1, count, a[M.mu*M.nu+1];

	for (count = 1; count < M.mu*M.nu; count++)
	{
		a[count] = rand() % (1 - (0) + 1) + (0);
	}

	for (i=1; i<M.mu*M.nu; i++)
	{
		if ((i % col) == 0)
		{
			j++;
			k = 1;
		}
		
		if (a[i] == 0)
		{
			M.data[i].e = a[i];
			M.data[i].i = j;
			M.data[i].j = k++;
		}
		else
		{
			M.tu++;
			M.data[i].e = a[i];
			M.data[i].i = j;
			M.data[i].j = k++;
		}
	}

	return OK;
}


void DestroySMatrix_T(TSMatrix &M)
{
	M.mu = 0;
	M.nu = 0;
	M.tu = 0;
}

void PrintSMatrix_T(TSMatrix M)
{
	int r, c;
	int k = 1;

	for(r=1; r<=M.mu; r++)
	{
		for(c=1; c<=M.nu; c++)
		{
			if(r==M.data[k].i && c==M.data[k].j)
			{
				printf("%3d ", M.data[k].e);
				k++;
			}
			else
				printf("  0 ");
		}
		printf("\n");
	}
}

void CopySMatrix_T(TSMatrix M, TSMatrix &T)
{
	T = M;								//�ṹ����ֱ�Ӹ���
}

Status AddSMatri_T(TSMatrix M, TSMatrix N, TSMatrix &Q)
{
	int m, n, k;

	if(M.mu!=N.mu || M.nu!=N.nu)
	{
		printf("����������ӣ���\n");
		return ERROR;
	}

	Q.mu = M.mu;
	Q.nu = M.nu;
	Q.tu = 0;
	m = n = k = 1;

	while(m<=M.tu && n<=N.tu)				//���α���M��N����Ԫ��
	{
		if(M.data[m].i<N.data[n].i)
		{
			Q.data[k] = M.data[m];
			m++;
		}
		else if(M.data[m].i>N.data[n].i)
		{
			Q.data[k] = N.data[n];
			n++;
		}
		else								//M.data[m].i==N.data[n].i
		{
			if(M.data[m].j<N.data[n].j)
			{
				Q.data[k] = M.data[m];
				m++;
			}
			else if(M.data[m].j>N.data[n].j)
			{
				Q.data[k] = N.data[n];
				n++;
			}
			else							//M.data[m].j==N.data[n].j
			{
				if(M.data[m].e+N.data[n].e)
				{
					Q.data[k].i = M.data[m].i;
					Q.data[k].j = M.data[m].j;
					Q.data[k].e = M.data[m].e + N.data[n].e;
					m++;
					n++;
				}
				else
				{
					m++;
					n++;
					continue;
				}
			}
		}

		k++;
		Q.tu++;
	}

	while(m<=M.tu)
	{
		Q.data[k] = M.data[m];
		m++;
		k++;
		Q.tu++;
	}

	while(n<=N.tu)
	{
		Q.data[k] = N.data[n];
		n++;
		k++;
		Q.tu++;
	}

	return OK;
}

void FastTransposeSMatrix_T(TSMatrix M, TSMatrix &T)
{
	int col, t, p, q;
	int num[M.nu];							//num[col]��ʾM��col���з���Ԫ�ĸ���
	int copt[M.nu];							//copt[col]��ʾM��col�е�һ������Ԫ��T->data��ǡ����λ��

	T.mu = M.nu;
	T.nu = M.mu;
	T.tu = M.tu;

	if(T.tu)
	{
		for(col=1; col<=M.nu; ++col)
			num[col] = 0;					//��ʼ������num

		for(t=1; t<=M.tu; ++t)				//t����M����Ԫ��
			num[M.data[t].j]++;				//ͳ��M��ÿ�з���Ԫ����

		copt[1] = 1;
		for(col=2; col<=M.nu; ++col)
			copt[col] = copt[col-1] + num[col-1];

		for(p=1; p<=M.tu; ++p)				//����ɨ��M�е���Ԫ��
		{
			col = M.data[p].j;				//colΪM�е�p����Ԫ����Ԫ�ص���
			q = copt[col];					//qΪ��ǰ��Ԫ��Ԫ����T��Ӧ���õ�λ��
			T.data[q].i = M.data[p].j;
			T.data[q].j = M.data[p].i;
			T.data[q].e = M.data[p].e;
			++copt[col];					//����������Ԫ��ʱλ����һ
		}
	}
}

