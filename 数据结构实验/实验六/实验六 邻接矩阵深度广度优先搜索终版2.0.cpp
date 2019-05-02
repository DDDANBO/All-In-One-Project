#include <stdio.h>
#include <limits.h>											//�ṩ��INT_MAX
#include <stdlib.h> 										//�ṩrand��srandԭ�� 
#include <stdarg.h>											//�ṩ��va_list��va_start��va_arg��va_end

#include "E:\���ݽṹ���㷨\���ݽṹ��ε��\Data-Structure-master\���α��㷨ʵ��\��03 ջ�Ͷ���\07 LinkQueue\LinkQueue.c"	 //**��03 ջ�Ͷ���**//
#include "E:\���ݽṹ���㷨\���ݽṹ��ε��\Data-Structure-master\���α��㷨ʵ��\��01 ����\Scanf.c"	//**��01 ����**//

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW -2

/* �궨�� */
#define INFINITY INT_MAX						//���ֵ��
#define MAX_VERTEX_NUM 20						//��󶥵����
 
typedef int Status;

/* ͼ��������飨�ڽӾ��󣩱�ʾ�����Ͷ��� */
typedef enum{ DG, DN, UDG, UDN }GraphKind;		//0-����ͼ��1-����������Ȩֵ����2-����ͼ��3-����������Ȩֵ��
typedef struct{ }InfoType;						//�������Ϣ						
typedef int VRType;								//ͼ�������ϵ����
typedef struct ArcCell
{
	VRType adj;									//��Ȩͼ�б�ʾȨֵ����Ȩͼ����0��1��ʾ�Ƿ�����
	InfoType info;								//�û������Ϣ�����Ժ���
} ArcCell;
typedef ArcCell AdjMatrix[MAX_VERTEX_NUM+1][MAX_VERTEX_NUM+1]; //�ڽӾ���

/* ͼ���������ʾ�����Ĵ洢��ʾ */
typedef char VertexType_M;				//ͼ����������
typedef struct
{
	VertexType_M vexs[MAX_VERTEX_NUM+1];//��������
	AdjMatrix arcs;						//�ڽӾ���
	int vexnum,	arcnum;					//ͼ����ĵ�ǰ�������ͻ���
	int IncInfo;						//IncInfoΪ0���������������Ϣ
	GraphKind kind;						//ͼ����������־
} MGraph;

/* ȫ�ֱ��� */
Status visited[MAX_VERTEX_NUM+1];		//��־����
void (*VisitFunc)(VertexType_M e);		//����ָ�����
 

/* �������� */ 
void PrintElem(char c);
Status CreateGraph_M(FILE *fp, MGraph *G); 
Status CreateDG_M(FILE *fp, MGraph *G);
Status CreateUDG_M(FILE *fp, MGraph *G);
void ClearGraph_M(MGraph *G);
void DFSTraverse_M(MGraph G, void(Visit)(VertexType_M));
void DFS_M(MGraph G, int v);
void BFSTraverse_M(MGraph G, void(Visit)(VertexType_M));
int LocateVex_M(MGraph G, VertexType_M u);
int FirstAdjVex_M(MGraph G, VertexType_M v);
int NextAdjVex_M(MGraph G, VertexType_M v, VertexType_M w);
int NextAdjVex_M(MGraph G, VertexType_M v, VertexType_M w);
void OutputMGraph(MGraph G);
void Input(FILE *fp, InfoType *info);

int main()
{
	MGraph G;
	FILE *fp;
	
	printf ("**********�ڽӾ����ʾ*********\n");
	printf("��ʼ������ͼ G ...\n");
	fp = fopen("TestData_UDG_M.txt", "r");
	CreateGraph_M(fp, &G);
	fclose(fp);
	printf("\n");

	printf("���ͼ���ڽӾ��� G = \n");
	OutputMGraph(G);
	printf("\n");
	
	printf("������ȱ���ͼ����G = ");
	DFSTraverse_M(G, PrintElem);
	printf("\n\n");
	
	printf("��ձ�ͼ��G ...\n");
	ClearGraph_M(&G);
	OutputMGraph(G);
	printf("\n");

	printf("��ʼ������ͼ G ...\n");
	fp = fopen("TestData_DG_M.txt", "r");
	CreateGraph_M(fp, &G);
	fclose(fp);
	printf("\n");
	
	printf("���ͼ���ڽӾ��� G = \n");
	OutputMGraph(G);
	printf("\n");

	printf("������ȱ���ͼ����G = ");
	BFSTraverse_M(G, PrintElem);
	printf("\n");

	return 0;
}

void PrintElem(char c)
{
	printf("%c ", c);
}

//����ͼ���� 
Status CreateGraph_M(FILE *fp, MGraph *G)
{	
	Scanf(fp, "%d", &((*G).kind));

	switch((*G).kind)								//�����������ͼ����������ͼ����һ�֡� 
	{
		case DG:
			return CreateDG_M(fp, G);
		//case DN:
		//	return CreateDN_M(fp, G);
		case UDG:
			return CreateUDG_M(fp, G);
		//case UDN:
		//	return CreateUDN_M(fp, G);
		default:
			return ERROR;
	}
}

//��������ͼ
Status CreateDG_M(FILE *fp, MGraph *G)
{
	int i, j, k;
	VertexType_M v1, v2;
	char tmp;

	Scanf(fp, "%d%d%d", &((*G).vexnum), &((*G).arcnum), &((*G).IncInfo));
	Scanf(fp, "%c", &tmp);						//�������з�

	for(i=1; i<=(*G).vexnum; i++)
		Scanf(fp, "%c", &((*G).vexs[i]));
	Scanf(fp, "%c", &tmp);						//�������з�

	for(i=1; i<=(*G).vexnum; i++)				//��ʼ���ڽӾ���
	{
		for(j=1; j<=(*G).vexnum; j++)
			(*G).arcs[i][j].adj = 0;
	}

	for(k=1; k<=(*G).arcnum; k++)
	{
		Scanf(fp, "%c%c", &v1, &v2);

		i = LocateVex_M(*G, v1);
		j = LocateVex_M(*G, v2);

		if(!i || !j)
			return ERROR;

		(*G).arcs[i][j].adj = 1;
		//if((*G).IncInfo==1)						//���л���������Ϣ����¼��
		//	Input(fp, &((*G).arcs[i][j].info));
	}

	return OK;
}

//��������ͼ
Status CreateUDG_M(FILE *fp, MGraph *G)
{
	int i, j, k;
	VertexType_M v1, v2;
	char tmp;

	Scanf(fp, "%d%d%d", &((*G).vexnum), &((*G).arcnum), &((*G).IncInfo));
	Scanf(fp, "%c", &tmp);							//�������з�

	for(i=1; i<=(*G).vexnum; i++)
		Scanf(fp, "%c", &((*G).vexs[i]));
	Scanf(fp, "%c", &tmp);							//�������з�

	for(i=1; i<=(*G).vexnum; i++)					//��ʼ���ڽӾ���
	{
		for(j=1; j<=(*G).vexnum; j++)
			(*G).arcs[i][j].adj = 0;
	}

	for(k=1; k<=(*G).arcnum; k++)
	{
		Scanf(fp, "%c%c", &v1, &v2);

		i = LocateVex_M(*G, v1);
		j = LocateVex_M(*G, v2);

		if(!i || !j)
			return ERROR;

		(*G).arcs[i][j].adj = 1;
		//if((*G).IncInfo==1)							//���л���������Ϣ����¼��
		//	Input(fp, &((*G).arcs[i][j].info));

		(*G).arcs[j][i] = (*G).arcs[i][j];			//���ԳƵ�
	}

	return OK;
}

//Ѱ�Ҷ���u��λ��
int LocateVex_M(MGraph G, VertexType_M u)
{
	int i;
	
	for(i=1; i<=G.vexnum; i++)
	{
		if(G.vexs[i]==u)
			return i;
	}
	
	return 0;
}

//���ͼ������
void ClearGraph_M(MGraph *G)
{
	(*G).vexnum = 0;
	(*G).arcnum = 0;
	(*G).IncInfo = 0;
}

//������ȱ���
void DFSTraverse_M(MGraph G, void(Visit)(VertexType_M))
{
	int v;

	VisitFunc = Visit;

	for(v=1; v<=G.vexnum; v++)
		visited[v] = FALSE;					//��ʼ��Ϊδ����

	for(v=1; v<=G.vexnum; v++)
	{
		if(!visited[v])						//δ����
			DFS_M(G, v);
	}
}

//������ȱ������ĺ���
void DFS_M(MGraph G, int v)
{
	int w;

	visited[v] = TRUE;

	VisitFunc(G.vexs[v]);

	for(w=FirstAdjVex_M(G, G.vexs[v]); w; w=NextAdjVex_M(G, G.vexs[v], G.vexs[w]))
	{
		if(!visited[w])
			DFS_M(G, w);
	}
}

//������ȱ���
void BFSTraverse_M(MGraph G, void(Visit)(VertexType_M))
{
	int v, w;
	LinkQueue Q;
	QElemType_L e;

	for(v=1; v<=G.vexnum; v++)
		visited[v] = FALSE;					//��ʼ��Ϊδ����

	InitQueue_L(&Q);

	for(v=1; v<=G.vexnum; v++)
	{
		if(!visited[v])
		{
			visited[v] = TRUE;
			Visit(G.vexs[v]);
			EnQueue_L(&Q, v);
			while(!QueueEmpty_L(Q))
			{
				DeQueue_L(&Q, &e);
				for(w=FirstAdjVex_M(G, G.vexs[e]); w; w=NextAdjVex_M(G, G.vexs[e], G.vexs[w]))
				{
					if(!visited[w])
					{
						visited[w] = TRUE;
						Visit(G.vexs[w]);
						EnQueue_L(&Q, w);
					}
				}
			}
		}
	}
}

//���ͼ����
void OutputMGraph(MGraph G)
{
	int i, j;

	if(!G.vexnum && !G.arcnum)
		printf("��ͼ������\n");
	else
	{
		printf("  ");
		for(i=1; i<=G.vexnum; i++)
			printf("%2c ", G.vexs[i]);
		printf("\n");

		for(i=1; i<=G.vexnum; i++)
		{
			printf("%c ", G.vexs[i]);
			for(j=1; j<=G.vexnum; j++)
			{
				if(G.arcs[i][j].adj==INFINITY)
					printf("�� ");
				else
					printf("%2d ", G.arcs[i][j]);
			}

			printf("\n");
		}
	}
}

//����v�ĵ�һ���ڽӶ������
int FirstAdjVex_M(MGraph G, VertexType_M v)
{
	int k, j, t;
	
	k = LocateVex_M(G, v);
	
	if(k)
	{							
		t = 0;//ͼ 
		for(j=1; j<=G.vexnum; j++)
		{
			if(G.arcs[k][j].adj!=t)
				return j;
		}
	}
	
	return 0;
} 

//����v�����w����һ���ڽӶ������
int NextAdjVex_M(MGraph G, VertexType_M v, VertexType_M w)
{
	int k1, k2, j, t;
	
	k1 = LocateVex_M(G, v);
	k2 = LocateVex_M(G, w);
	
	if(k1 && k2)
	{		
		t = 0;			//ͼ 

		for(j=k2+1; j<=G.vexnum; j++)
		{
			if(G.arcs[k1][j].adj!=t)
				return j;
		}
	}
	
	return 0;
} 

void Input(FILE *fp, InfoType *info)
{
	//¼�뻡����Ϣ�����ĵ��漰���Ļ�Ĭ����������Ϣ
#if defined CRITICALPATH_H || \
    defined Question_7
	Scanf(fp, "%d", &(info->in));
#endif
}

