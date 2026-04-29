#include "triangle.h"
#include <bits/stdc++.h>
using namespace std;
#define MAXn 1007
int poziciq[MAXn];
map<vector<int>,bool> queries;
bool vypros(int prv, int vtr, int tr)
{
    if (prv==vtr && vtr==tr && tr==prv) return true;
    vector<int> vc(3);
    vc[0]=prv;
    vc[1]=vtr;
    vc[2]=tr;
    sort(vc.begin(),vc.end());
    if (queries.find(vc)!=queries.end()) return queries[vc];
    bool otg=query(prv-1,vtr-1,tr-1);
    queries[vc]=otg;
    return otg;
}

pair<vector<int>,vector<int>> razdeli(vector<int> pos, int lGol, int rGol, int mGol)
{
    ///iskame vlqvo lGol do mGol-1, a vdqsno mGol do rGol
    int prv,vtr;
    if (mGol%2==0)
    {
        prv=poziciq[mGol/2];
        vtr=poziciq[mGol/2];
    }
    else
    {
        prv=poziciq[mGol/2];
        vtr=poziciq[mGol/2+1];
    }
    vector<int> levi,desni;
    int exL=mGol-lGol,exR=rGol-mGol+1;
    for (int q=0;q<pos.size();q++)
    {
        if (levi.size()==exL) desni.push_back(pos[q]);
        else if (desni.size()==exR) levi.push_back(pos[q]);
        else if (vypros(prv,vtr,pos[q])) levi.push_back(pos[q]);
        else desni.push_back(pos[q]);
    }
    return {levi,desni};
}

void dnc(vector<int> pos, int lR, int rR)
{
    if (pos.size()==0) return;
    if (pos.size()==1)
    {
        poziciq[lR]=pos[0];
        return;
    }
    int mid=(lR+rR)/2;
    pair<vector<int>,vector<int>> razd=razdeli(pos,lR,rR,mid+1);
    dnc(razd.first,lR,mid);
    dnc(razd.second,mid+1,rR);
}

void reshi(int gol, vector<int> pos, vector<int> pogolemi, vector<int> otgore)
{
    //cout<<"pochvam gol "<<gol<<"\n";
    ///namira posiciite na vsichki ot 1 do gol
    if (gol==1)
    {
        poziciq[1]=pos[0];
        return;
    }
    if (gol==2)
    {
        if (vypros(pos[0],pos[0],pos[1]))
        {
            poziciq[2]=pos[0];
            poziciq[1]=pos[1];
        }
        else
        {
            poziciq[1]=pos[0];
            poziciq[2]=pos[1];
        }
        return;
    }
    if (gol==3)
    {
        if (!vypros(pos[0],pos[0],pos[1]) && !vypros(pos[0],pos[0],pos[2]))
        {
            poziciq[1]=pos[0];
            int pos1=0;
            for (int q=0;q<pogolemi.size();q++)
            {
                if (vypros(pos[1],pos[1],pogolemi[q])) pos1++;
            }
            int pos2=0;
            for (int q=0;q<pogolemi.size();q++)
            {
                if (pos2>pos1) break;
                if (vypros(pos[2],pos[2],pogolemi[q])) pos2++;
            }
            if (pos2>pos1)
            {
                poziciq[3]=pos[2];
                poziciq[2]=pos[1];
            }
            else
            {
                poziciq[3]=pos[1];
                poziciq[2]=pos[2];
            }
        }
        else if (!vypros(pos[1],pos[1],pos[2]) && !vypros(pos[1],pos[1],pos[0]))
        {
            poziciq[1]=pos[1];
            int pos1=0;
            for (int q=0;q<pogolemi.size();q++)
            {
                if (vypros(pos[0],pos[0],pogolemi[q])) pos1++;
            }
            int pos2=0;
            for (int q=0;q<pogolemi.size();q++)
            {
                if (pos2>pos1) break;
                if (vypros(pos[2],pos[2],pogolemi[q])) pos2++;
            }
            if (pos2>pos1)
            {
                poziciq[3]=pos[2];
                poziciq[2]=pos[0];
            }
            else
            {
                poziciq[3]=pos[0];
                poziciq[2]=pos[2];
            }
        }
        else if (!vypros(pos[2],pos[2],pos[1]) && !vypros(pos[2],pos[0],pos[2]))
        {
            poziciq[1]=pos[2];
            int pos1=0;
            for (int q=0;q<pogolemi.size();q++)
            {
                if (vypros(pos[0],pos[0],pogolemi[q])) pos1++;
            }
            int pos2=0;
            for (int q=0;q<pogolemi.size();q++)
            {
                if (pos2>pos1) break;
                if (vypros(pos[1],pos[1],pogolemi[q])) pos2++;
            }
            if (pos2>pos1)
            {
                poziciq[3]=pos[1];
                poziciq[2]=pos[0];
            }
            else
            {
                poziciq[3]=pos[0];
                poziciq[2]=pos[1];
            }
        }
        return;
    }
    vector<int> kandidati=pos;
    vector<int> malki,golemi;
    vector<int> zadolu(0);
    if (otgore.size()==0)
    {
        while (true)
        {
            random_shuffle(kandidati.begin(),kandidati.end());
            int kogo=kandidati[0];
            int kkk=8;
            if (gol==1000) kkk=30;
            for (int q=0;q<min(kkk,(int)kandidati.size());q++)
            {
                if (!vypros(kogo,kogo,kandidati[q])) kogo=kandidati[q];
            }
            golemi.clear();
            //cout<<kogo<<" "<<kandidati[kogo]<<"\n";
            for (int q=0;q<kandidati.size();q++)
            {
                if (vypros(kogo,kandidati[q],kandidati[q])) golemi.push_back(kandidati[q]);
                else malki.push_back(kandidati[q]);
            }
            //cout<<kogo<<" "<<kandidati[kogo]<<"\n";
            //cout<<malki.size()<<" "<<golemi.size()<<" krai\n";
            if (!malki.empty() && ((malki.size()*2-1)*2)>=gol) break;
            zadolu=malki;
            kandidati=golemi;
        }
    }
    else
    {
        malki=otgore;
        golemi=pos;
        for (int q=0;q<malki.size();q++)
        {
            for (int w=0;w<golemi.size();w++)
            {
                if (golemi[w]==malki[q])
                {
                    swap(golemi[w],golemi[golemi.size()-1]);
                    golemi.pop_back();
                    break;
                }
            }
        }
        while (true)
        {
            if (!malki.empty() && ((malki.size()*2-1)*2)>=gol) break;
            zadolu=malki;
            kandidati=golemi;
            random_shuffle(kandidati.begin(),kandidati.end());
            int kogo=kandidati[0];
            for (int q=0;q<min(8,(int)kandidati.size());q++)
            {
                if (!vypros(kogo,kogo,kandidati[q])) kogo=kandidati[q];
            }
            golemi.clear();
            //cout<<kogo<<" "<<kandidati[kogo]<<"\n";
            for (int q=0;q<kandidati.size();q++)
            {
                if (vypros(kogo,kandidati[q],kandidati[q])) golemi.push_back(kandidati[q]);
                else malki.push_back(kandidati[q]);
            }
        }
    }
    reshi(malki.size(),malki,golemi,zadolu);
    ///imame resheno vsichko v malki
    int super=(gol+malki.size())/2; ///super e kyde se razdelq na dve golemi
    if ((malki.size()*2-1)>=super)
    {
        //cout<<malki.size()<<" "<<super<<" "<<gol<<"\n";
        ///iskame da razdelim sys super
        pair<vector<int>,vector<int>> razdelenie=razdeli(golemi,malki.size()+1,gol,super+1);
        //cout<<"splitnah na "<<razdelenie.first.size()<<" "<<razdelenie.second.size()<<"\n";
        dnc(razdelenie.first,malki.size()+1,super);
        dnc(razdelenie.second,super+1,gol);
    }
    else
    {
        ///iskame da razdelim sys (malki.size())*2
        pair<vector<int>,vector<int>> razdelenie=razdeli(golemi,malki.size()+1,gol,malki.size()*2);
        //cout<<"splitnah na "<<razdelenie.first.size()<<" "<<razdelenie.second.size()<<"\n";
        dnc(razdelenie.first,malki.size()+1,malki.size()*2-1);
        dnc(razdelenie.second,malki.size()*2,gol);
    }
    //cout<<"svyrshvam gol "<<gol<<"\n";
}

vector <int> solve(int n)
{
    queries.clear();
    vector<int> posicii(n),golemi(0);
    for (int q=0;q<n;q++) posicii[q]=q+1;
    reshi(n,posicii,golemi,golemi);

    vector <int> answer(n);
    for (int q=1;q<=n;q++) answer[ poziciq[q]-1 ]=q;
    return answer;
}