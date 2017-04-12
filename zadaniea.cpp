 #include<iostream>
#include<queue>
#include <list>
#include<fstream>
#include<climits>
using namespace std;
struct lista{
	int x;
	int f;
	struct lista *next;
};
struct graf{   
 int drogi; 
 int odl;
 bool kolejka;
  lista* polaczenia; 
};
lista* ostatni(lista *pierwszy)
{
	lista *pom=pierwszy;
	while(pom->next!=NULL)pom=pom->next;
	return pom;
}
lista* dodajElement(lista *pierwszy, int v, int *ilosci, int pier, int ujscie, int typ)
{
	
	lista* pom2;
	lista* pom;
	int a; 
	if(pierwszy==NULL)
	{
		pom2=new lista;
		pom2->x=v;
		pom2->next=NULL;
		pierwszy=pom2;
	}
	else
	{
		pom=ostatni(pierwszy);
		pom2=new lista;
		pom2->x=v;
		pom2->next=NULL;
		pom->next=pom2;
	}
	//tworzenie przeplywosci
	if(typ==1)
	{
		if(pom2->x==ujscie)
		{
				pom2->f=1;
		}
		else
		{
				pom2->f=ilosci[pier];
		}
	}
	else
	{
				pom2->f=0;
	}
	return pierwszy;
}
void augment(graf *r ,int rozmiar, int *pred, int *f)
{
	(*f)++;
	int pom=rozmiar;
	int a;
	lista *pom2;
	lista *pom3;
	while(pom!=1)
	{
		pom3=r[pred[pom]].polaczenia;
		while(pom3->x!=pom)pom3=pom3->next;
		pom3->f=pom3->f-1;
		pom3=r[pom].polaczenia;
		while(pom3->x!=pred[pom])pom3=pom3->next;
		pom3->f=pom3->f+1;
	//	cout<<pom<<"->"<<pred[pom]<<" "<<pom3->f<<endl;
		pom=pred[pom];
	}
}
int edmons_karp(graf *r, int zrodlo, int ujscie)
{
	int* et= new int[ujscie+1];
	int* pred = new int[ujscie+1];
	int f=0;
	for(int i=1; i<=ujscie;i++)et[i]=0;
	queue<int> bfs;
	r[ujscie].odl=0;
	et[0]=1;
	r[ujscie].kolejka=true;
	int c;
	bfs.push(ujscie);
	int licznik=0;
	while(!bfs.empty())
	{
		c=bfs.front();
		bfs.pop();
		lista *pom=r[c].polaczenia;
		while(pom){
			if(r[pom->x].kolejka!=true)
			{
				bfs.push(pom->x);
				r[pom->x].odl=r[c].odl+1;
				et[r[pom->x].odl]++;
				r[pom->x].kolejka=true;
			
			}
				pom=pom->next;
		}
	}
	int a=1;
	queue<int> kolejka;
	lista * pom;
	pom=r[a].polaczenia;
	int min;
	while(1)
	{
		if(r[a].odl==r[pom->x].odl+1 && pom->f>0)
		{
			pred[pom->x]=a;
			a=pom->x;
			pom=r[a].polaczenia;
			if(a==ujscie)
			{
				augment(r,ujscie,pred,&f);
				a=1;
				pom=r[a].polaczenia;
			}
		}
		else if(pom->next)pom=pom->next;
		else
		{
			pom=r[a].polaczenia;		
			min=99999;;
			while(pom)
			{
				if(min>r[pom->x].odl && pom->f>0)
				{
					min=r[pom->x].odl;					
				}
				pom=pom->next;
			}
			if(min==99999)
				min=r[a].odl;
			et[r[a].odl]=et[r[a].odl]-1;
			if(et[r[a].odl]==0)
				break;
			r[a].odl=min+1;
			et[r[a].odl]=et[r[a].odl]+1;
			if(a!=1)
			{
				a=pred[a];
				pom=r[a].polaczenia;
			}
			else
			{
				pom=r[1].polaczenia;
				a=1;
			}
		}
	}
	return f;
}

int main(){
	
	ifstream wej;
	wej.open("zadanie.in");
	int rozmiar, i=1, next, ilosc, drogidoKonca=0;
	wej>>rozmiar;
	graf*w = new graf[rozmiar+1];
	int *ilosci = new int[rozmiar+1];
	for(int i=1; i<=rozmiar;i++)
	{
		ilosci[i]=0;		
		w[i].polaczenia=NULL;
	}
	ilosci[1]=1;
	while(!wej.eof()){
		wej>>ilosc;
		w[i].drogi=ilosc;
		for(int j=0; j<ilosc;j++)
		{
			wej>>next;
			ilosci[next]=ilosci[next]+ilosci[i];
			w[i].polaczenia=dodajElement(w[i].polaczenia,next, ilosci,i,rozmiar,1);
			w[next].polaczenia=dodajElement(w[next].polaczenia,i,ilosci,i,rozmiar,2);
		}	
		i++;
	}
	int a=edmons_karp(w,1,rozmiar);
	cout<<a;
	return 0;
	
}
