#include<stdio.h>
#include<stdlib.h>
/*图的建立*/
typedef struct enode* Edge;
typedef struct node* Node;
typedef struct lgraph* LGraph;
struct enode {
	int v1, v2, weight;
};
struct node {
	int adjl;
	int weight;
	Node Next;
};
typedef struct snode {
	Node Fristnode;
}adjlist[1000];
struct lgraph {
	int nv, ne;
	adjlist G;
 };
/*图的建立结束*/
void swap(struct enode *a,struct enode *b) {
	struct enode t = *a;
	*a= *b;
	*b = t;
}

/*并查集的建立*/
typedef int setType[1000];
void setv(setType s,int n){
	int x;
	for (x = 0; x < n; x++) {
		s[x] = -1;
	}
}
int  find(setType s, int x) {
	if (s[x] < 0)
		return x;
	else return s[x] = find(s, s[x]);
}
void Union(setType s,int root1,int root2) {
	if (s[root2] < s[root1])/*root2集合大小比较大*/
	{
		s[root2] += s[root1];
		s[root1] = root2;
	}
	else {
		s[root1] += s[root2];
		s[root2] = root1;
	}
}
bool checkis(setType s,int x1,int x2) {/*检查是否同一个集合*/
	int root1 = find(s, x1);
	int root2 = find(s, x2);
	if (root1 == root2) {
		return false;
	}
	else {
		Union(s, root1, root2);
		return true;
	}
}/*并查集定义结束*/
LGraph creatgraph(int n) {
	LGraph Graph = (LGraph)malloc(sizeof(struct lgraph));
	Graph->nv = n;
	Graph->ne = 0;
	for (int i = 0; i < Graph->nv; i++) {
		Graph->G[i].Fristnode = NULL;
	}
	return Graph;
}
void insert(LGraph Graph,Edge E) {
	Node newnode = (Node)malloc(sizeof(struct node));
	newnode->adjl = E->v2;
	newnode->weight = E->weight;
	newnode->Next = Graph->G[E->v1].Fristnode;
	Graph->G[E->v1].Fristnode = newnode;
}
LGraph buildgraph() {
	int n,e;
	LGraph Graph;
	scanf_s("%d %d", &n, &e);
	Graph = creatgraph(n);
	if (e != 0) {
		Graph->ne = e;
		for (int i = 0; i < Graph->ne; i++) {
			Edge E = (Edge)malloc(sizeof(struct enode));
			scanf_s("%d %d %d", &E->v1, &E->v2, &E->weight);
			insert(Graph, E);
		}


	}
	return Graph;

}
/*最小堆定义*/
void percdown(Edge Eset,int p,int n) {
	int parent, child;
	struct enode x= Eset[p];
	for(parent=p;parent*2+1<n;parent=child){
		child = parent * 2 + 1;
		if (child != n-1 && Eset[child].weight > Eset[child + 1].weight) {
			child++;
		}
		if (x.weight > Eset[child].weight) {
			Eset[parent] = Eset[child];
		}
		else break;
	}
	Eset[parent] = x;
}
/*初始化边数组*/
void sete(Edge Eset, LGraph Graph) {
	int ecount = 0;
	for (int i = 0; i < Graph->nv; i++) {
		for (Node w = Graph->G[i].Fristnode; w; w = w->Next) {
			if (i < w->adjl) {
				Eset[ecount].v1 = i;
				Eset[ecount].v2 = w->adjl;
				Eset[ecount++].weight = w->weight;
			}
		}
	}
	for (ecount = Graph->nv/ 2 - 1; ecount >= 0; ecount--)
		percdown(Eset, ecount, Graph->ne);

}
int getedge(Edge Eset,int n) {
	swap(&Eset[0], &Eset[n - 1]);
	percdown(Eset, 0, n-1);
	return n - 1;
}
int Kruskal(LGraph Graph,LGraph MST) {
	setType Vset;/*顶点集合*/
	Edge Eset=(Edge)malloc(sizeof(struct enode)*Graph->ne);/*编辑边数组*/
	int ecount, totalweight;
	 setv(Vset,Graph->nv);
	 sete(Eset,Graph);
	 MST = creatgraph(Graph->nv);
	 ecount = 0; totalweight = 0;
	 int nextedge = Graph->ne;
	 while (ecount < Graph->nv - 1) {
		 nextedge = getedge(Eset, nextedge);
		 if (nextedge < 0)break;
		 if (checkis(Vset, Eset[nextedge].v1, Eset[nextedge].v2)) {
			 insert(MST, Eset + nextedge);
			 totalweight += Eset[nextedge].weight;
			 // printf("$%d @%d\n", Eset[nextedge].weight,nextedge);
			 ecount++;
		 }
	 }
	 if (ecount < Graph->nv-1)
		 totalweight = -1;
	 return totalweight;

}
int main() {
	LGraph Graph = buildgraph();
	LGraph MST=(LGraph)malloc(sizeof(struct lgraph));
	int totalweight;
	totalweight = Kruskal(Graph,MST);
	printf("%d\n", totalweight);
}
