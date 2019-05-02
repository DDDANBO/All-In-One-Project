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

/* ͼ�����Ͷ��� */
typedef enum{ DG, UDG }GraphKind;				//0-����ͼ��1-����ͼ
typedef struct
{
	int in;
}InfoType;

/* ���� */
typedef struct ArcNode
{
	int adjvex;							//������� 
	struct ArcNode *nextarc;			//ָ����һ������ָ�� 
	InfoType info;						//�û������Ϣ
}ArcNode;

/* ͷ��� */
typedef char VertexType_AL;				//ͼ�������� 
typedef struct VNode
{
	VertexType_AL data;					//������Ϣ
	ArcNode *firstarc;
}VNode;
typedef VNode AdjList[MAX_VERTEX_NUM+1];

/* ͼ���ڽӱ��Ĵ洢��ʾ */
typedef struct
{
	AdjList vertices;					//�ڽӱ�
	int vexnum,	arcnum;					//ͼ�ĵ�ǰ�������ͻ���
	int IncInfo;						//IncInfoΪ0���������������Ϣ 
	GraphKind kind;						//ͼ�������־ 
}ALGraph;

/* ȫ�ֱ��� */
Status visited[MAX_VERTEX_NUM+1];		//��־����
void (*VisitFunc)(VertexType_AL e);		//����ָ�����

/* �������� */ 
void PrintElem(char c);
Status CreateGraph_AL(FILE *fp, ALGraph *G);
Status CreateDG_AL(FILE *fp, ALGraph *G);
Status CreateUDG_AL(FILE *fp, ALGraph *G);
void ClearGraph_AL(ALGraph *G);
void DFSTraverse_AL(ALGraph G, void (Visit)(VertexType_AL));
void DFS_AL(ALGraph G, int v);
void BFSTraverse_AL(ALGraph G, void (Visit)(VertexType_AL));
void OutputALGraph(ALGraph G);
void Input(FILE *fp, InfoType *info);
int LocateVex_AL(ALGraph G, VertexType_AL u);
int FirstAdjVex_AL(ALGraph G, VertexType_AL v);
int NextAdjVex_AL(ALGraph G, VertexType_AL v, VertexType_AL w);
void Input(FILE *fp, InfoType *info);

int main()
{
	ALGraph G;
	FILE *fp;
	
	printf ("**********�ڽӱ��ʾ*********\n");
	printf("��ʼ������ͼ G...\n");
	fp = fopen("TestData_UDG_AL.txt", "r");
	CreateGraph_AL(fp, &G); 
	fclose(fp);
	printf("\n");

	printf("���ͼ���ڽӱ� G = \n");		
	OutputALGraph(G);
	printf("\n");
	
	printf("������ȱ���ͼ G = ");
	BFSTraverse_AL(G, PrintElem);	
	printf("\n\n");

	printf("���ͼ...\n");
	ClearGraph_AL(&G);
	OutputALGraph(G);	
	printf("\n");

	printf("��ʼ������ͼ G...\n");
	fp = fopen("TestData_DG_AL.txt", "r");
	CreateGraph_AL(fp, &G); 
	fclose(fp);
	printf("\n");
	
	printf("���ͼ���ڽӱ� G = \n");		
	OutputALGraph(G);
	printf("\n");
	
	printf("������ȱ���ͼ G = ");
	DFSTraverse_AL(G, PrintElem);	
	printf("\n");
	
	return 0;
}

//����ͼ
Status CreateGraph_AL(FILE *fp, ALGraph *G)
{	
	Scanf(fp, "%d", &((*G).kind));

	switch((*G).kind)
	{
		case DG:
			return CreateDG_AL(fp, G);
		case UDG:
			return CreateUDG_AL(fp, G);
		default:
			return ERROR;
	}
}

//��������ͼ 
Status CreateDG_AL(FILE *fp, ALGraph *G)
{
	int i, j, k;
	VertexType_AL v1, v2;
	char tmp;
	ArcNode *p;
	ArcNode *r[MAX_VERTEX_NUM+1];					//��Ϊ���ʱ�ǣ�������λ 
	
	Scanf(fp, "%d%d%d", &((*G).vexnum), &((*G).arcnum), &((*G).IncInfo));
	Scanf(fp, "%c", &tmp);							//�������з� 
	
	for(i=1; i<=(*G).vexnum; i++)
	{
		Scanf(fp, "%c", &((*G).vertices[i].data));
		(*G).vertices[i].firstarc = NULL;
		r[i] = NULL;	
	}
	Scanf(fp, "%c", &tmp);							//�������з�
	
	for(k=1; k<=(*G).arcnum; k++)					//��ȡ���ߣ������ڽӱ� 
	{
		Scanf(fp, "%c%c", &v1, &v2);
		
		i = LocateVex_AL(*G, v1);
		j = LocateVex_AL(*G, v2);
		
		if(!i || !j)								//��֤��ȡ�Ķ������ 
			return ERROR;
		
		p = (ArcNode *)malloc(sizeof(ArcNode));
		if(!p)
			exit(OVERFLOW);
		p->adjvex = j;
		p->nextarc = NULL;
		if((*G).IncInfo==1)							//��ʾ�л���������Ϣ��¼�� 
			Input(fp, &(p->info));
		
		if(r[i]==NULL)								//�ڽӱ��е�һ����� 
			(*G).vertices[i].firstarc = p;			//���뵽�ڽӱ� 
		else
			r[i]->nextarc = p;
			
		r[i] = p;
	}
	
	return OK;	
}

//��������ͼ
Status CreateUDG_AL(FILE *fp, ALGraph *G)
{
	int i, j, k;
	VertexType_AL v1, v2;
	char tmp;
	ArcNode *p, *q;
	ArcNode *r[MAX_VERTEX_NUM+1];					//��Ϊ���ʱ�� 
	
	Scanf(fp, "%d%d%d", &((*G).vexnum), &((*G).arcnum), &((*G).IncInfo));
	Scanf(fp, "%c", &tmp);							//�������з� 
	
	for(i=1; i<=(*G).vexnum; i++)
	{
		Scanf(fp, "%c", &((*G).vertices[i].data));
		(*G).vertices[i].firstarc = NULL;	
		r[i] = NULL;
	}
	Scanf(fp, "%c", &tmp);							//�������з�
	
	for(k=1; k<=(*G).arcnum; k++)
	{
		Scanf(fp, "%c%c", &v1, &v2);
		
		i = LocateVex_AL(*G, v1);
		j = LocateVex_AL(*G, v2);
		
		if(!i || !j)								//��֤��ȡ�Ķ������
			return ERROR;
		
		p = (ArcNode *)malloc(sizeof(ArcNode));
		if(!p)
			exit(OVERFLOW);
		p->adjvex = j;
		p->nextarc = NULL;
		if((*G).IncInfo==1)							//��ʾ�л���������Ϣ��¼�� 
			Input(fp, &(p->info));
			
		if(r[i]==NULL)								//�ڽӱ��е�һ����� 
			(*G).vertices[i].firstarc = p;			//���뵽�ڽӱ� 
		else
			r[i]->nextarc = p;			
		r[i] = p;		

		q = (ArcNode *)malloc(sizeof(ArcNode));
		if(!q)
			exit(OVERFLOW);
		q->adjvex = i;
		q->nextarc = NULL;
		if((*G).IncInfo==1)							//��ʾ�л���������Ϣ��¼�� 
			q->info = p->info;		

		if(r[j]==NULL)								//�ڽӱ��е�һ����� 
			(*G).vertices[j].firstarc = q;			//���뵽�ڽӱ� 
		else
			r[j]->nextarc = q;			
		r[j] = q;
	}
	
	return OK;
}

//���ͼ 
void ClearGraph_AL(ALGraph *G)
{
	int i;
	ArcNode *p, *q;
	
	for(i=1; i<=(*G).vexnum; i++)
	{
		p = (*G).vertices[i].firstarc;
		while(p)
		{
			q = p;
			p = p->nextarc;
			free(q);
		}
	}

	(*G).vexnum = 0;
	(*G).arcnum = 0;
}

//������ȱ��� 
void DFSTraverse_AL(ALGraph G, void (Visit)(VertexType_AL))
{
	int v;
	
	VisitFunc = Visit;
	
	for(v=1; v<=G.vexnum; v++)
		visited[v] = FALSE;					//��ʼ��Ϊδ����
	
	for(v=1; v<=G.vexnum; v++)
	{
		if(!visited[v])						//δ����
			DFS_AL(G, v); 
	} 
}

//������ȱ������ĺ��� 
void DFS_AL(ALGraph G, int v)
{
	int w;

	visited[v] = TRUE;

	VisitFunc(G.vertices[v].data);

	for(w=FirstAdjVex_AL(G, G.vertices[v].data); w; w=NextAdjVex_AL(G, G.vertices[v].data, G.vertices[w].data))
	{
		if(!visited[w])
			DFS_AL(G, w);
	}
}

//������ȱ��� 
void BFSTraverse_AL(ALGraph G, void (Visit)(VertexType_AL))
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
			Visit(G.vertices[v].data);
			EnQueue_L(&Q, v);
			while(!QueueEmpty_L(Q))
			{
				DeQueue_L(&Q, &e);
				for(w=FirstAdjVex_AL(G, G.vertices[e].data); w; w=NextAdjVex_AL(G, G.vertices[e].data, G.vertices[w].data))
				{
					if(!visited[w])
					{
						visited[w] = TRUE;
						Visit(G.vertices[w].data);
						EnQueue_L(&Q, w);
					}
				}
			}
		}
	}
}

//���ͼ 
void OutputALGraph(ALGraph G)
{
	int i, j;
	ArcNode *p;
	
	if(!G.vexnum && !G.arcnum)
		printf("��ͼ������\n");
	else
	{
		for(i=1; i<=G.vexnum; i++)
		{
			printf("%c��", G.vertices[i].data);
			p = G.vertices[i].firstarc;
			while(p)
			{
				printf(" %c", G.vertices[p->adjvex].data);
				p = p->nextarc;
			}
			printf("\n");	
		}
	}
}

//Ѱ�Ҷ���u��λ��
int LocateVex_AL(ALGraph G, VertexType_AL u)
{
	int i;
	
	for(i=1; i<=G.vexnum; i++)
	{
		if(G.vertices[i].data==u)
			return i;
	}
	
	return 0;
}

//����v�ĵ�һ���ڽӶ������
int FirstAdjVex_AL(ALGraph G, VertexType_AL v)
{
	int k;
	ArcNode *p;
	
	k = LocateVex_AL(G, v);
	
	if(k && G.vertices[k].firstarc)
		return G.vertices[k].firstarc->adjvex;
	
	return 0;
}

//����v�����w����һ���ڽӶ������
int NextAdjVex_AL(ALGraph G, VertexType_AL v, VertexType_AL w)
{
	int k1, k2;
	ArcNode *p, *q;
	
	k1 = LocateVex_AL(G, v);
	k2 = LocateVex_AL(G, w);
	
	if(k1 && k2)
	{
		for(p=G.vertices[k1].firstarc; p; p=p->nextarc)
		{
			if(p->adjvex==k2)
			{
				if(p->nextarc)
					return p->nextarc->adjvex;
				else
					return 0;	
			}
		}	
	}
 
	return 0;									//���v�ͽ��w������ͨ 
}

void PrintElem(char c)
{
	printf("%c ", c);
}

void Input(FILE *fp, InfoType *info)
{
	//¼�뻡����Ϣ�����ĵ��漰���Ļ�Ĭ����������Ϣ
#if defined CRITICALPATH_H || \
    defined Question_7
	Scanf(fp, "%d", &(info->in));
#endif
}
