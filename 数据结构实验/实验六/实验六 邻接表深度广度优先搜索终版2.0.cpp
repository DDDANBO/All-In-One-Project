#include <stdio.h>
#include <limits.h>											//提供宏INT_MAX
#include <stdlib.h> 										//提供rand、srand原型 
#include <stdarg.h>											//提供宏va_list、va_start、va_arg、va_end

#include "E:\数据结构和算法\数据结构严蔚敏\Data-Structure-master\▲课本算法实现\▲03 栈和队列\07 LinkQueue\LinkQueue.c"	 //**▲03 栈和队列**//
#include "E:\数据结构和算法\数据结构严蔚敏\Data-Structure-master\▲课本算法实现\▲01 绪论\Scanf.c"	//**▲01 绪论**//

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW -2

/* 宏定义 */
#define INFINITY INT_MAX						//最大值∞
#define MAX_VERTEX_NUM 20						//最大顶点个数
 
typedef int Status;

/* 图的类型定义 */
typedef enum{ DG, UDG }GraphKind;				//0-有向图，1-无向图
typedef struct
{
	int in;
}InfoType;

/* 表结点 */
typedef struct ArcNode
{
	int adjvex;							//顶点序号 
	struct ArcNode *nextarc;			//指向下一条弧的指针 
	InfoType info;						//该弧相关信息
}ArcNode;

/* 头结点 */
typedef char VertexType_AL;				//图顶点类型 
typedef struct VNode
{
	VertexType_AL data;					//顶点信息
	ArcNode *firstarc;
}VNode;
typedef VNode AdjList[MAX_VERTEX_NUM+1];

/* 图（邻接表）的存储表示 */
typedef struct
{
	AdjList vertices;					//邻接表
	int vexnum,	arcnum;					//图的当前顶点数和弧数
	int IncInfo;						//IncInfo为0则各弧不含其它信息 
	GraphKind kind;						//图的种类标志 
}ALGraph;

/* 全局变量 */
Status visited[MAX_VERTEX_NUM+1];		//标志数组
void (*VisitFunc)(VertexType_AL e);		//函数指针变量

/* 函数声明 */ 
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
	
	printf ("**********邻接表表示*********\n");
	printf("初始化无向图 G...\n");
	fp = fopen("TestData_UDG_AL.txt", "r");
	CreateGraph_AL(fp, &G); 
	fclose(fp);
	printf("\n");

	printf("输出图的邻接表 G = \n");		
	OutputALGraph(G);
	printf("\n");
	
	printf("广度优先遍历图 G = ");
	BFSTraverse_AL(G, PrintElem);	
	printf("\n\n");

	printf("清空图...\n");
	ClearGraph_AL(&G);
	OutputALGraph(G);	
	printf("\n");

	printf("初始化有向图 G...\n");
	fp = fopen("TestData_DG_AL.txt", "r");
	CreateGraph_AL(fp, &G); 
	fclose(fp);
	printf("\n");
	
	printf("输出图的邻接表 G = \n");		
	OutputALGraph(G);
	printf("\n");
	
	printf("深度优先遍历图 G = ");
	DFSTraverse_AL(G, PrintElem);	
	printf("\n");
	
	return 0;
}

//创建图
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

//构造有向图 
Status CreateDG_AL(FILE *fp, ALGraph *G)
{
	int i, j, k;
	VertexType_AL v1, v2;
	char tmp;
	ArcNode *p;
	ArcNode *r[MAX_VERTEX_NUM+1];					//作为访问标记，用作定位 
	
	Scanf(fp, "%d%d%d", &((*G).vexnum), &((*G).arcnum), &((*G).IncInfo));
	Scanf(fp, "%c", &tmp);							//跳过换行符 
	
	for(i=1; i<=(*G).vexnum; i++)
	{
		Scanf(fp, "%c", &((*G).vertices[i].data));
		(*G).vertices[i].firstarc = NULL;
		r[i] = NULL;	
	}
	Scanf(fp, "%c", &tmp);							//跳过换行符
	
	for(k=1; k<=(*G).arcnum; k++)					//读取各边，制作邻接表 
	{
		Scanf(fp, "%c%c", &v1, &v2);
		
		i = LocateVex_AL(*G, v1);
		j = LocateVex_AL(*G, v2);
		
		if(!i || !j)								//保证获取的顶点存在 
			return ERROR;
		
		p = (ArcNode *)malloc(sizeof(ArcNode));
		if(!p)
			exit(OVERFLOW);
		p->adjvex = j;
		p->nextarc = NULL;
		if((*G).IncInfo==1)							//表示有弧的其他信息，录入 
			Input(fp, &(p->info));
		
		if(r[i]==NULL)								//邻接表中第一个结点 
			(*G).vertices[i].firstarc = p;			//加入到邻接表 
		else
			r[i]->nextarc = p;
			
		r[i] = p;
	}
	
	return OK;	
}

//构造无向图
Status CreateUDG_AL(FILE *fp, ALGraph *G)
{
	int i, j, k;
	VertexType_AL v1, v2;
	char tmp;
	ArcNode *p, *q;
	ArcNode *r[MAX_VERTEX_NUM+1];					//作为访问标记 
	
	Scanf(fp, "%d%d%d", &((*G).vexnum), &((*G).arcnum), &((*G).IncInfo));
	Scanf(fp, "%c", &tmp);							//跳过换行符 
	
	for(i=1; i<=(*G).vexnum; i++)
	{
		Scanf(fp, "%c", &((*G).vertices[i].data));
		(*G).vertices[i].firstarc = NULL;	
		r[i] = NULL;
	}
	Scanf(fp, "%c", &tmp);							//跳过换行符
	
	for(k=1; k<=(*G).arcnum; k++)
	{
		Scanf(fp, "%c%c", &v1, &v2);
		
		i = LocateVex_AL(*G, v1);
		j = LocateVex_AL(*G, v2);
		
		if(!i || !j)								//保证获取的顶点存在
			return ERROR;
		
		p = (ArcNode *)malloc(sizeof(ArcNode));
		if(!p)
			exit(OVERFLOW);
		p->adjvex = j;
		p->nextarc = NULL;
		if((*G).IncInfo==1)							//表示有弧的其他信息，录入 
			Input(fp, &(p->info));
			
		if(r[i]==NULL)								//邻接表中第一个结点 
			(*G).vertices[i].firstarc = p;			//加入到邻接表 
		else
			r[i]->nextarc = p;			
		r[i] = p;		

		q = (ArcNode *)malloc(sizeof(ArcNode));
		if(!q)
			exit(OVERFLOW);
		q->adjvex = i;
		q->nextarc = NULL;
		if((*G).IncInfo==1)							//表示有弧的其他信息，录入 
			q->info = p->info;		

		if(r[j]==NULL)								//邻接表中第一个结点 
			(*G).vertices[j].firstarc = q;			//加入到邻接表 
		else
			r[j]->nextarc = q;			
		r[j] = q;
	}
	
	return OK;
}

//清空图 
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

//深度优先遍历 
void DFSTraverse_AL(ALGraph G, void (Visit)(VertexType_AL))
{
	int v;
	
	VisitFunc = Visit;
	
	for(v=1; v<=G.vexnum; v++)
		visited[v] = FALSE;					//初始化为未访问
	
	for(v=1; v<=G.vexnum; v++)
	{
		if(!visited[v])						//未访问
			DFS_AL(G, v); 
	} 
}

//深度优先遍历核心函数 
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

//广度优先遍历 
void BFSTraverse_AL(ALGraph G, void (Visit)(VertexType_AL))
{
	int v, w;
	LinkQueue Q;
	QElemType_L e;
	
	for(v=1; v<=G.vexnum; v++)
		visited[v] = FALSE;					//初始化为未访问
	
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

//输出图 
void OutputALGraph(ALGraph G)
{
	int i, j;
	ArcNode *p;
	
	if(!G.vexnum && !G.arcnum)
		printf("空图（表）！\n");
	else
	{
		for(i=1; i<=G.vexnum; i++)
		{
			printf("%c→", G.vertices[i].data);
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

//寻找顶点u的位置
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

//返回v的第一个邻接顶点序号
int FirstAdjVex_AL(ALGraph G, VertexType_AL v)
{
	int k;
	ArcNode *p;
	
	k = LocateVex_AL(G, v);
	
	if(k && G.vertices[k].firstarc)
		return G.vertices[k].firstarc->adjvex;
	
	return 0;
}

//返回v相对于w的下一个邻接顶点序号
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
 
	return 0;									//结点v和结点w并不连通 
}

void PrintElem(char c)
{
	printf("%c ", c);
}

void Input(FILE *fp, InfoType *info)
{
	//录入弧的信息，本文档涉及到的弧默认无其他信息
#if defined CRITICALPATH_H || \
    defined Question_7
	Scanf(fp, "%d", &(info->in));
#endif
}
