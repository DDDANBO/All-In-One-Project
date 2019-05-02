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

/* 图、表的数组（邻接矩阵）表示法类型定义 */
typedef enum{ DG, DN, UDG, UDN }GraphKind;		//0-有向图，1-有向网（带权值），2-无向图，3-无向网（带权值）
typedef struct{ }InfoType;						//弧相关信息						
typedef int VRType;								//图、表顶点关系类型
typedef struct ArcCell
{
	VRType adj;									//带权图中表示权值，无权图中用0、1表示是否相邻
	InfoType info;								//该弧相关信息，可以忽略
} ArcCell;
typedef ArcCell AdjMatrix[MAX_VERTEX_NUM+1][MAX_VERTEX_NUM+1]; //邻接矩阵

/* 图、表（数组表示法）的存储表示 */
typedef char VertexType_M;				//图、表顶点类型
typedef struct
{
	VertexType_M vexs[MAX_VERTEX_NUM+1];//顶点向量
	AdjMatrix arcs;						//邻接矩阵
	int vexnum,	arcnum;					//图、表的当前顶点数和弧数
	int IncInfo;						//IncInfo为0则各弧不含其它信息
	GraphKind kind;						//图、表的种类标志
} MGraph;

/* 全局变量 */
Status visited[MAX_VERTEX_NUM+1];		//标志数组
void (*VisitFunc)(VertexType_M e);		//函数指针变量
 

/* 函数声明 */ 
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
	
	printf ("**********邻接矩阵表示*********\n");
	printf("初始化无向图 G ...\n");
	fp = fopen("TestData_UDG_M.txt", "r");
	CreateGraph_M(fp, &G);
	fclose(fp);
	printf("\n");

	printf("输出图的邻接矩阵 G = \n");
	OutputMGraph(G);
	printf("\n");
	
	printf("深度优先遍历图（表）G = ");
	DFSTraverse_M(G, PrintElem);
	printf("\n\n");
	
	printf("清空表（图）G ...\n");
	ClearGraph_M(&G);
	OutputMGraph(G);
	printf("\n");

	printf("初始化有向图 G ...\n");
	fp = fopen("TestData_DG_M.txt", "r");
	CreateGraph_M(fp, &G);
	fclose(fp);
	printf("\n");
	
	printf("输出图的邻接矩阵 G = \n");
	OutputMGraph(G);
	printf("\n");

	printf("广度优先遍历图（表）G = ");
	BFSTraverse_M(G, PrintElem);
	printf("\n");

	return 0;
}

void PrintElem(char c)
{
	printf("%c ", c);
}

//创建图、表 
Status CreateGraph_M(FILE *fp, MGraph *G)
{	
	Scanf(fp, "%d", &((*G).kind));

	switch((*G).kind)								//随机创建有向图、网或无向图网的一种。 
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

//构造有向图
Status CreateDG_M(FILE *fp, MGraph *G)
{
	int i, j, k;
	VertexType_M v1, v2;
	char tmp;

	Scanf(fp, "%d%d%d", &((*G).vexnum), &((*G).arcnum), &((*G).IncInfo));
	Scanf(fp, "%c", &tmp);						//跳过换行符

	for(i=1; i<=(*G).vexnum; i++)
		Scanf(fp, "%c", &((*G).vexs[i]));
	Scanf(fp, "%c", &tmp);						//跳过换行符

	for(i=1; i<=(*G).vexnum; i++)				//初始化邻接矩阵
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
		//if((*G).IncInfo==1)						//若有弧的其他信息，则录入
		//	Input(fp, &((*G).arcs[i][j].info));
	}

	return OK;
}

//构造无向图
Status CreateUDG_M(FILE *fp, MGraph *G)
{
	int i, j, k;
	VertexType_M v1, v2;
	char tmp;

	Scanf(fp, "%d%d%d", &((*G).vexnum), &((*G).arcnum), &((*G).IncInfo));
	Scanf(fp, "%c", &tmp);							//跳过换行符

	for(i=1; i<=(*G).vexnum; i++)
		Scanf(fp, "%c", &((*G).vexs[i]));
	Scanf(fp, "%c", &tmp);							//跳过换行符

	for(i=1; i<=(*G).vexnum; i++)					//初始化邻接矩阵
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
		//if((*G).IncInfo==1)							//若有弧的其他信息，则录入
		//	Input(fp, &((*G).arcs[i][j].info));

		(*G).arcs[j][i] = (*G).arcs[i][j];			//填充对称点
	}

	return OK;
}

//寻找顶点u的位置
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

//清空图（网）
void ClearGraph_M(MGraph *G)
{
	(*G).vexnum = 0;
	(*G).arcnum = 0;
	(*G).IncInfo = 0;
}

//深度优先遍历
void DFSTraverse_M(MGraph G, void(Visit)(VertexType_M))
{
	int v;

	VisitFunc = Visit;

	for(v=1; v<=G.vexnum; v++)
		visited[v] = FALSE;					//初始化为未访问

	for(v=1; v<=G.vexnum; v++)
	{
		if(!visited[v])						//未访问
			DFS_M(G, v);
	}
}

//深度优先遍历核心函数
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

//广度优先遍历
void BFSTraverse_M(MGraph G, void(Visit)(VertexType_M))
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

//输出图（表）
void OutputMGraph(MGraph G)
{
	int i, j;

	if(!G.vexnum && !G.arcnum)
		printf("空图（表）！\n");
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
					printf("∞ ");
				else
					printf("%2d ", G.arcs[i][j]);
			}

			printf("\n");
		}
	}
}

//返回v的第一个邻接顶点序号
int FirstAdjVex_M(MGraph G, VertexType_M v)
{
	int k, j, t;
	
	k = LocateVex_M(G, v);
	
	if(k)
	{							
		t = 0;//图 
		for(j=1; j<=G.vexnum; j++)
		{
			if(G.arcs[k][j].adj!=t)
				return j;
		}
	}
	
	return 0;
} 

//返回v相对于w的下一个邻接顶点序号
int NextAdjVex_M(MGraph G, VertexType_M v, VertexType_M w)
{
	int k1, k2, j, t;
	
	k1 = LocateVex_M(G, v);
	k2 = LocateVex_M(G, w);
	
	if(k1 && k2)
	{		
		t = 0;			//图 

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
	//录入弧的信息，本文档涉及到的弧默认无其他信息
#if defined CRITICALPATH_H || \
    defined Question_7
	Scanf(fp, "%d", &(info->in));
#endif
}

