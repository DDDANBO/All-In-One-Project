#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW -2

#define MAXSIZE 400							//假设非零元个数的最大值为400

/* 三元组稀疏矩阵类型定义 */
typedef int MElemType_TSq;
typedef int Status;

typedef struct
{
	int i, j;								//该非零元的行下标和列下标
	MElemType_TSq e;
} Triple;

typedef struct
{
	Triple data[MAXSIZE+1];					//非零元三元组表，data[0]未用
	int mu, nu, tu;							//矩阵的行数、列数和非零元个数
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

	printf("创建两个稀疏矩阵 A、B...\n");
	printf ("请输入矩阵的行数和列数：\n");
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
	
	printf ("矩阵C快速转置为矩阵D：\n");
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
	T = M;								//结构可以直接复制
}

Status AddSMatri_T(TSMatrix M, TSMatrix N, TSMatrix &Q)
{
	int m, n, k;

	if(M.mu!=N.mu || M.nu!=N.nu)
	{
		printf("两矩阵不能相加！！\n");
		return ERROR;
	}

	Q.mu = M.mu;
	Q.nu = M.nu;
	Q.tu = 0;
	m = n = k = 1;

	while(m<=M.tu && n<=N.tu)				//依次遍历M与N的三元组
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
	int num[M.nu];							//num[col]表示M第col列中非零元的个数
	int copt[M.nu];							//copt[col]表示M第col列第一个非零元在T->data中恰当的位置

	T.mu = M.nu;
	T.nu = M.mu;
	T.tu = M.tu;

	if(T.tu)
	{
		for(col=1; col<=M.nu; ++col)
			num[col] = 0;					//初始化数组num

		for(t=1; t<=M.tu; ++t)				//t遍历M中三元组
			num[M.data[t].j]++;				//统计M中每列非零元个数

		copt[1] = 1;
		for(col=2; col<=M.nu; ++col)
			copt[col] = copt[col-1] + num[col-1];

		for(p=1; p<=M.tu; ++p)				//依次扫描M中的三元组
		{
			col = M.data[p].j;				//col为M中第p个三元组中元素的列
			q = copt[col];					//q为当前三元组元素在T中应放置的位置
			T.data[q].i = M.data[p].j;
			T.data[q].j = M.data[p].i;
			T.data[q].e = M.data[p].e;
			++copt[col];					//再遇到此列元素时位置增一
		}
	}
}

