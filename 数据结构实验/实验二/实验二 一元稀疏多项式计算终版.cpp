#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW -2

#define N 7	//产生的多项式的个数 
#define M 20	//产生的随机数的范围 

typedef int ElemType;
typedef int Status;

typedef struct    //项的表示，数据类型
{
	float coef; // 系数
	int expn;   // 指数
} LElemType_E;

typedef struct PNode     // 结点类型，单项
{
	LElemType_E data;
	struct PNode * next;
}*PositionPtr;

typedef struct       //链表类型, 多项式表示
{
	PositionPtr head, tail;
	int len;
} Polynomial;

void CreatePolyn(Polynomial &P, int m); //输入m项的系数和指数，建立表示一元多项式的有序链表P
//void DestroyPolyn(Polynomial &P);	//销毁一元多项式P
void PrintPolyn(Polynomial P);	//打印输出一元多项式P
//int PolyLength(Polynomial P);	//返回一元多项式P中的项数
void AddPolyn(Polynomial &Pa, Polynomial &Pb);	//完成多项式相加运算，即：Pa=Pa+Pb，并销毁一元多项式Pb
void SubtractPolyn(Polynomial &Pa, Polynomial &Pb);	//完成多项式相减运算，即：Pa=Pa-Pb，并销毁一元多项式Pb
int Cmp(LElemType_E c1, LElemType_E c2);//依a的指数值<（或=）（或>）b的指数值，分别返回-1、0和+1
Status DelFirst(Polynomial &L, PositionPtr h, PositionPtr q);
void FreeNode(PositionPtr p);	//释放空间
LElemType_E GetCurElem(PositionPtr p);	//得到当前结点的元素
PositionPtr GetHead(Polynomial L);	//得到头结点
void InsFirst(Polynomial &L, PositionPtr h, PositionPtr s);
Status MakeNode(PositionPtr &p, LElemType_E e);	//创建结点
PositionPtr NextPos(PositionPtr p);	//下一结点
void SetCurElem(PositionPtr p, LElemType_E e);	//对p所指向的结点赋值
Status InitList(Polynomial &L);	//链表初始化
void Append(Polynomial &L, PositionPtr s);	//追加链表中剩余的结点到另一链表中
int Srand(int m,int t,int *a);	//产生不重复的随机数
void Combine(Polynomial &Pa);	//合并Pa中的同类项
void RverList(Polynomial &Pa); //逆置链表

int main()
{
	srand((int) time(0));
	Polynomial P,Q, A,B;
	int m;
	printf ("已生成%d项多项式\n",N);
	CreatePolyn(P,N);
	printf ("\n一元稀疏多项式P为：\n");
	PrintPolyn(P);
	CreatePolyn(Q,N);
	printf ("\n一元稀疏多项式Q为：\n");
	PrintPolyn(Q);

	AddPolyn(P, Q);
	printf ("\n一元稀疏多项式P+Q的和为：\n");
	Combine(P);
	PrintPolyn(P);
	printf ("\n");

	printf ("\n已生成%d项多项式\n",N);
	CreatePolyn(A,N);
	printf ("\n一元稀疏多项式A为：\n");
	PrintPolyn(A);
	CreatePolyn(B,N);
	printf ("\n一元稀疏多项式B为：\n");
	PrintPolyn(B);

	SubtractPolyn(A, B);
	printf ("\n一元稀疏多项式A-B的差为：\n");
	Combine(A);
	PrintPolyn(A);
	printf ("\n");
	
	printf ("\n逆置A-B的差：\n");
	RverList(A);
	PrintPolyn(A);

	return 0;
}


int Cmp(LElemType_E c1, LElemType_E c2)
{
	int i = c1.expn - c2.expn;

	if(i<0)
		return -1;
	else if(i==0)
		return 0;
	else
		return 1;
}

//输入m项的系数和指数，建立表示一元多项式的有序链表P
void CreatePolyn(Polynomial &P, int m)
{
	PositionPtr h,p;
	LElemType_E e;
	int i,a[N+1];

	e.expn = rand() % M+1;

	InitList(P);		//初始化一个扩展的单链表存放多项式
	h = GetHead(P);		//获取多项式头指针

	//e.coef = 0.0;
	//e.expn = -1;

	SetCurElem(h, e);	//设置头结点的数据元素
	h->next = NULL;

	for(i=1; i<=m; i++)	//依次录入n个非0项
	{
		//scanf ("%f %d",&e.coef,&e.expn);

		//e.coef = rand() % M+1;
		//e.expn = rand() % M+1;

		e.coef = rand() % M+1;
		e.expn = Srand(i,e.expn,a);

		MakeNode(p, e);
		p->next = NULL;
		InsFirst(P, h, p);
		h = h->next;
	}
}


int Srand(int m,int t,int *a)
{
	int i,j;
	i = rand() % M+1;
	for (j = 0; j<m; j++)
	{
		while (i == a[j])
		{
			i = rand() % M+1;
		}
	}
	a[j] = i;
	return i;
}


//多项式加法：Pa=Pa+Pb，利用两个多项式的结点构成“和多项式”
void AddPolyn(Polynomial &Pa, Polynomial &Pb)
{
	PositionPtr ha, hb, qa, qb, head;
	LElemType_E a, b;
	float sum;

	ha = GetHead(Pa);			//ha、hb分别指向Pa、Pb头结点
	hb = GetHead(Pb);
	head = qa = NextPos(ha);			//qa、qb分别指向Pa、Pb的当前结点
	qb = NextPos(hb);

	while(qa && qb)				//qa、qb均非空
	{
		a = GetCurElem(qa);		//a和b为两表中当前比较元素
		b = GetCurElem(qb);

		switch(Cmp(a,b))				//比较当前元素的指数大小
		{
			case -1:						//多项式Pa中当前结点的指数值较小
				ha = qa;
				qa = NextPos(ha);
				break;
			case 0:							//两者数值相等
				sum = a.coef + b.coef;
				if(sum != 0.0)				//相加不能抵消时更新Pa结点的系数值
				{
					qa->data.coef = sum;
					SetCurElem(qa, qa->data);
					ha = qa;				//此时ha需后移
				}
				else						//相加抵消时，删除Pa中当前结点
				{
					DelFirst(Pa, ha, qa);
					FreeNode(qa);
				}
				DelFirst(Pb, hb, qb);	//删除Pb中扫描过的结点
				FreeNode(qb);
				qb = NextPos(hb);			//qa、qb均后移
				qa = NextPos(ha);
				break;
			case 1:							//多项式Pb中当前结点的指数值较小
				DelFirst(Pb, hb, qb);	//摘下Pb当前结点
				InsFirst(Pa, ha, qb); 	//将摘下结点链入Pa中
				qb = NextPos(hb);
				ha = NextPos(ha);
				break;
		}
	}

	if(qb)									//若Pb还未扫描完，将剩余项链接到Pa后
		Append(Pa, qb);

	FreeNode(hb);						//释放Pb头结点
	Pb.head = Pb.tail = NULL;			//设置Pb为销毁状态
	Pb.len = 0;

}

void Combine(Polynomial &Pa)	//合并Pa中的同类项
{
	PositionPtr ha,qa, qb, head;
	ha = GetHead(Pa);			//ha分别指向Pa头结点
	head = qa = NextPos(ha);	//qa分别指向Pa的当前结点

	for (; qa; qa = qa->next)
	{
		for (ha = qa->next; ha; ha = ha->next)
		{
			if (qa->data.expn == ha->data.expn)
			{
				qb = head;
				while(qb->next != ha)
				{
					qb = qb->next;
				}
				qa->data.coef += ha->data.coef;
				qb->next = ha->next;
				FreeNode(ha);
				ha = qb;
				Pa.len--;
			}
		}
	}
}

void SubtractPolyn(Polynomial &Pa, Polynomial &Pb)
{
	PositionPtr ha, hb, qa, qb, r;
	LElemType_E a, b;
	float sum;

	ha = GetHead(Pa);					//ha、hb分别指向Pa、Pb头结点
	hb = GetHead(Pb);
	qa = NextPos(ha);						//qa、qb分别指向Pa、Pb的当前结点
	qb = NextPos(hb);

	while(qa && qb)							//qa、qb均非空
	{
		a = GetCurElem(qa);				//a和b为两表中当前比较元素
		b = GetCurElem(qb);

		switch(Cmp(a,b))				//比较当前元素的指数大小
		{
			case -1:						//多项式Pa中当前结点的指数值较小
				ha = qa;
				qa = NextPos(ha);
				break;
			case 0:							//两者数值相等
				sum = a.coef - b.coef;
				if(sum != 0.0)				//相减不能抵消时更新Pa结点的系数值
				{
					qa->data.coef = sum;
					SetCurElem(qa, qa->data);
					ha = qa;				//此时ha需后移
				}
				else						//相加抵消时，删除Pa中当前结点
				{
					DelFirst(Pa, ha, qa);
					FreeNode(qa);
				}
				DelFirst(Pb, hb, qb);	//删除Pb中扫描过的结点
				FreeNode(qb);
				qb = NextPos(hb);			//qa、qb均后移
				qa = NextPos(ha);
				break;
			case 1:							//多项式Pb中当前结点的指数值较小
				DelFirst(Pb, hb, qb);	//摘下Pb当前结点
				qb->data.coef = - qb->data.coef;	//改变当前结点符号
				InsFirst(Pa, ha, qb); 	//将摘下结点链入Pa中
				qb = NextPos(hb);
				ha = NextPos(ha);
				break;
		}
	}

	if(qb)									//Pb还未扫描完
	{
		r = qb;
		while(r)
		{
			r->data.coef = - r->data.coef;	//改变剩余结点符号
			r = r->next;
		}
		Append(Pa, qb);
	}

	FreeNode(hb);						//释放Pb头结点
	Pb.head = Pb.tail = NULL;
	Pb.len = 0;
}

void RverList(Polynomial &Pa) //逆置链表
{
	PositionPtr p=Pa.head->next;
	PositionPtr q=p->next;
	Pa.head->next=NULL;
	while(p)
	{
		q=p->next;
		p->next=Pa.head->next;
		Pa.head->next=p;
		p=q;
	}
}

void Append(Polynomial &L, PositionPtr s)
{
	int count = 0;
	L.tail->next = s;
	while(s)							//考虑s为空的情况
	{
		L.tail = s;
		s = s->next;
		count++;
	}
	L.len += count;
}

PositionPtr GetHead(Polynomial L)
{
	return L.head;
}

PositionPtr NextPos(PositionPtr p)
{
	return p->next;
}

LElemType_E GetCurElem(PositionPtr p)
{
	return p->data;
}

void SetCurElem(PositionPtr p, LElemType_E e)
{
	p->data = e;
}

Status DelFirst(Polynomial &L, PositionPtr h, PositionPtr q)
{
	q = h->next;
	if(q)								//链表非空
	{
		h->next = q->next;
		if(!h->next)					//h后只有一个结点
			L.tail = h;
		L.len--;						//并不释放被删结点所占空间
		return OK;
	}
	return ERROR;
}

void FreeNode(PositionPtr p)					//释放空间
{
	free(p);
	p = NULL;
}

void InsFirst(Polynomial &L, PositionPtr h, PositionPtr s)
{
	s->next = h->next;
	h->next = s;
	if(h==L.tail)					//若h为尾结点
		L.tail = h->next;
	L.len++;
}

Status MakeNode(PositionPtr &p, LElemType_E e)
{
	p = (PositionPtr)malloc(sizeof(PNode));		//申请空间
	if(!p)
		exit(OVERFLOW);
	p->data = e;
	p->next = NULL;
	return OK;
}

Status InitList(Polynomial &L)
{
	PositionPtr p;
	p = (PositionPtr)malloc(sizeof(PNode));
	if(!p)
		exit(OVERFLOW);
	p->next = NULL;
	L.head =L.tail = p;
	L.len = 0;
	return OK;
}

void PrintPolyn(Polynomial P)
{
	int i;
	PositionPtr p;
	p = P.head->next;
	for(i=1; i<=P.len; i++)
	{
		if(i==1)
			printf("%2.0f", p->data.coef);
		else
		{
			if(p->data.coef>0)
			{
				printf(" + ");
				printf("%2.0f", p->data.coef);
			}
			else
			{
				printf(" - ");
				printf("%2.0f", -p->data.coef);
			}
		}
		if(p->data.expn)
		{
			printf("x");

			if(p->data.expn!=1)
				printf("^%d", p->data.expn);
		}
		p = p->next;
	}
}
