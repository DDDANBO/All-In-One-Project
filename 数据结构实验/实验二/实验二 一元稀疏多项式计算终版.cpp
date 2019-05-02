#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW -2

#define N 7	//�����Ķ���ʽ�ĸ��� 
#define M 20	//������������ķ�Χ 

typedef int ElemType;
typedef int Status;

typedef struct    //��ı�ʾ����������
{
	float coef; // ϵ��
	int expn;   // ָ��
} LElemType_E;

typedef struct PNode     // ������ͣ�����
{
	LElemType_E data;
	struct PNode * next;
}*PositionPtr;

typedef struct       //��������, ����ʽ��ʾ
{
	PositionPtr head, tail;
	int len;
} Polynomial;

void CreatePolyn(Polynomial &P, int m); //����m���ϵ����ָ����������ʾһԪ����ʽ����������P
//void DestroyPolyn(Polynomial &P);	//����һԪ����ʽP
void PrintPolyn(Polynomial P);	//��ӡ���һԪ����ʽP
//int PolyLength(Polynomial P);	//����һԪ����ʽP�е�����
void AddPolyn(Polynomial &Pa, Polynomial &Pb);	//��ɶ���ʽ������㣬����Pa=Pa+Pb��������һԪ����ʽPb
void SubtractPolyn(Polynomial &Pa, Polynomial &Pb);	//��ɶ���ʽ������㣬����Pa=Pa-Pb��������һԪ����ʽPb
int Cmp(LElemType_E c1, LElemType_E c2);//��a��ָ��ֵ<����=������>��b��ָ��ֵ���ֱ𷵻�-1��0��+1
Status DelFirst(Polynomial &L, PositionPtr h, PositionPtr q);
void FreeNode(PositionPtr p);	//�ͷſռ�
LElemType_E GetCurElem(PositionPtr p);	//�õ���ǰ����Ԫ��
PositionPtr GetHead(Polynomial L);	//�õ�ͷ���
void InsFirst(Polynomial &L, PositionPtr h, PositionPtr s);
Status MakeNode(PositionPtr &p, LElemType_E e);	//�������
PositionPtr NextPos(PositionPtr p);	//��һ���
void SetCurElem(PositionPtr p, LElemType_E e);	//��p��ָ��Ľ�㸳ֵ
Status InitList(Polynomial &L);	//�����ʼ��
void Append(Polynomial &L, PositionPtr s);	//׷��������ʣ��Ľ�㵽��һ������
int Srand(int m,int t,int *a);	//�������ظ��������
void Combine(Polynomial &Pa);	//�ϲ�Pa�е�ͬ����
void RverList(Polynomial &Pa); //��������

int main()
{
	srand((int) time(0));
	Polynomial P,Q, A,B;
	int m;
	printf ("������%d�����ʽ\n",N);
	CreatePolyn(P,N);
	printf ("\nһԪϡ�����ʽPΪ��\n");
	PrintPolyn(P);
	CreatePolyn(Q,N);
	printf ("\nһԪϡ�����ʽQΪ��\n");
	PrintPolyn(Q);

	AddPolyn(P, Q);
	printf ("\nһԪϡ�����ʽP+Q�ĺ�Ϊ��\n");
	Combine(P);
	PrintPolyn(P);
	printf ("\n");

	printf ("\n������%d�����ʽ\n",N);
	CreatePolyn(A,N);
	printf ("\nһԪϡ�����ʽAΪ��\n");
	PrintPolyn(A);
	CreatePolyn(B,N);
	printf ("\nһԪϡ�����ʽBΪ��\n");
	PrintPolyn(B);

	SubtractPolyn(A, B);
	printf ("\nһԪϡ�����ʽA-B�Ĳ�Ϊ��\n");
	Combine(A);
	PrintPolyn(A);
	printf ("\n");
	
	printf ("\n����A-B�Ĳ\n");
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

//����m���ϵ����ָ����������ʾһԪ����ʽ����������P
void CreatePolyn(Polynomial &P, int m)
{
	PositionPtr h,p;
	LElemType_E e;
	int i,a[N+1];

	e.expn = rand() % M+1;

	InitList(P);		//��ʼ��һ����չ�ĵ������Ŷ���ʽ
	h = GetHead(P);		//��ȡ����ʽͷָ��

	//e.coef = 0.0;
	//e.expn = -1;

	SetCurElem(h, e);	//����ͷ��������Ԫ��
	h->next = NULL;

	for(i=1; i<=m; i++)	//����¼��n����0��
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


//����ʽ�ӷ���Pa=Pa+Pb��������������ʽ�Ľ�㹹�ɡ��Ͷ���ʽ��
void AddPolyn(Polynomial &Pa, Polynomial &Pb)
{
	PositionPtr ha, hb, qa, qb, head;
	LElemType_E a, b;
	float sum;

	ha = GetHead(Pa);			//ha��hb�ֱ�ָ��Pa��Pbͷ���
	hb = GetHead(Pb);
	head = qa = NextPos(ha);			//qa��qb�ֱ�ָ��Pa��Pb�ĵ�ǰ���
	qb = NextPos(hb);

	while(qa && qb)				//qa��qb���ǿ�
	{
		a = GetCurElem(qa);		//a��bΪ�����е�ǰ�Ƚ�Ԫ��
		b = GetCurElem(qb);

		switch(Cmp(a,b))				//�Ƚϵ�ǰԪ�ص�ָ����С
		{
			case -1:						//����ʽPa�е�ǰ����ָ��ֵ��С
				ha = qa;
				qa = NextPos(ha);
				break;
			case 0:							//������ֵ���
				sum = a.coef + b.coef;
				if(sum != 0.0)				//��Ӳ��ܵ���ʱ����Pa����ϵ��ֵ
				{
					qa->data.coef = sum;
					SetCurElem(qa, qa->data);
					ha = qa;				//��ʱha�����
				}
				else						//��ӵ���ʱ��ɾ��Pa�е�ǰ���
				{
					DelFirst(Pa, ha, qa);
					FreeNode(qa);
				}
				DelFirst(Pb, hb, qb);	//ɾ��Pb��ɨ����Ľ��
				FreeNode(qb);
				qb = NextPos(hb);			//qa��qb������
				qa = NextPos(ha);
				break;
			case 1:							//����ʽPb�е�ǰ����ָ��ֵ��С
				DelFirst(Pb, hb, qb);	//ժ��Pb��ǰ���
				InsFirst(Pa, ha, qb); 	//��ժ�½������Pa��
				qb = NextPos(hb);
				ha = NextPos(ha);
				break;
		}
	}

	if(qb)									//��Pb��δɨ���꣬��ʣ�������ӵ�Pa��
		Append(Pa, qb);

	FreeNode(hb);						//�ͷ�Pbͷ���
	Pb.head = Pb.tail = NULL;			//����PbΪ����״̬
	Pb.len = 0;

}

void Combine(Polynomial &Pa)	//�ϲ�Pa�е�ͬ����
{
	PositionPtr ha,qa, qb, head;
	ha = GetHead(Pa);			//ha�ֱ�ָ��Paͷ���
	head = qa = NextPos(ha);	//qa�ֱ�ָ��Pa�ĵ�ǰ���

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

	ha = GetHead(Pa);					//ha��hb�ֱ�ָ��Pa��Pbͷ���
	hb = GetHead(Pb);
	qa = NextPos(ha);						//qa��qb�ֱ�ָ��Pa��Pb�ĵ�ǰ���
	qb = NextPos(hb);

	while(qa && qb)							//qa��qb���ǿ�
	{
		a = GetCurElem(qa);				//a��bΪ�����е�ǰ�Ƚ�Ԫ��
		b = GetCurElem(qb);

		switch(Cmp(a,b))				//�Ƚϵ�ǰԪ�ص�ָ����С
		{
			case -1:						//����ʽPa�е�ǰ����ָ��ֵ��С
				ha = qa;
				qa = NextPos(ha);
				break;
			case 0:							//������ֵ���
				sum = a.coef - b.coef;
				if(sum != 0.0)				//������ܵ���ʱ����Pa����ϵ��ֵ
				{
					qa->data.coef = sum;
					SetCurElem(qa, qa->data);
					ha = qa;				//��ʱha�����
				}
				else						//��ӵ���ʱ��ɾ��Pa�е�ǰ���
				{
					DelFirst(Pa, ha, qa);
					FreeNode(qa);
				}
				DelFirst(Pb, hb, qb);	//ɾ��Pb��ɨ����Ľ��
				FreeNode(qb);
				qb = NextPos(hb);			//qa��qb������
				qa = NextPos(ha);
				break;
			case 1:							//����ʽPb�е�ǰ����ָ��ֵ��С
				DelFirst(Pb, hb, qb);	//ժ��Pb��ǰ���
				qb->data.coef = - qb->data.coef;	//�ı䵱ǰ������
				InsFirst(Pa, ha, qb); 	//��ժ�½������Pa��
				qb = NextPos(hb);
				ha = NextPos(ha);
				break;
		}
	}

	if(qb)									//Pb��δɨ����
	{
		r = qb;
		while(r)
		{
			r->data.coef = - r->data.coef;	//�ı�ʣ�������
			r = r->next;
		}
		Append(Pa, qb);
	}

	FreeNode(hb);						//�ͷ�Pbͷ���
	Pb.head = Pb.tail = NULL;
	Pb.len = 0;
}

void RverList(Polynomial &Pa) //��������
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
	while(s)							//����sΪ�յ����
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
	if(q)								//����ǿ�
	{
		h->next = q->next;
		if(!h->next)					//h��ֻ��һ�����
			L.tail = h;
		L.len--;						//�����ͷű�ɾ�����ռ�ռ�
		return OK;
	}
	return ERROR;
}

void FreeNode(PositionPtr p)					//�ͷſռ�
{
	free(p);
	p = NULL;
}

void InsFirst(Polynomial &L, PositionPtr h, PositionPtr s)
{
	s->next = h->next;
	h->next = s;
	if(h==L.tail)					//��hΪβ���
		L.tail = h->next;
	L.len++;
}

Status MakeNode(PositionPtr &p, LElemType_E e)
{
	p = (PositionPtr)malloc(sizeof(PNode));		//����ռ�
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
