#include <bits/stdc++.h>
using namespace std;

struct Node {
    Node *ch[2], *p;
    bool rev;
    long long val, sum;
    Node(long long v = 0) : p(nullptr), rev(false), val(v), sum(v) {
        ch[0] = ch[1] = nullptr;
    }
};

bool isRoot(Node* x) { return !x->p || (x->p->ch[0] != x && x->p->ch[1] != x); }

void pushUp(Node* x) {
    x->sum = x->val;
    if (x->ch[0]) x->sum += x->ch[0]->sum;
    if (x->ch[1]) x->sum += x->ch[1]->sum;
}

void pushDown(Node* x) {
    if (!x->rev) return;
    swap(x->ch[0], x->ch[1]);
    if (x->ch[0]) x->ch[0]->rev ^= 1;
    if (x->ch[1]) x->ch[1]->rev ^= 1;
    x->rev = false;
}

void rotate(Node* x) {
    Node* p = x->p; Node* g = p->p;
    int d = (p->ch[1] == x);
    if (!isRoot(p)) (g->ch[0]==p?g->ch[0]:g->ch[1]) = x;
    x->p = g;
    p->ch[d] = x->ch[d^1];
    if (x->ch[d^1]) x->ch[d^1]->p = p;
    x->ch[d^1] = p; p->p = x;
    pushUp(p); pushUp(x);
}

void splay(Node* x) {
    static vector<Node*> stk;
    stk.clear();
    Node* y = x;
    while (!isRoot(y)) { stk.push_back(y->p); y = y->p; }
    for (int i=stk.size()-1;i>=0;i--) pushDown(stk[i]);
    pushDown(x);
    while (!isRoot(x)) {
        Node* p = x->p; Node* g = p->p;
        if (!isRoot(p)) {
            if ((p->ch[0]==x)^(g->ch[0]==p)) rotate(x);
            else rotate(p);
        }
        rotate(x);
    }
}

Node* access(Node* x) {
    Node* last = nullptr;
    for (Node* y = x; y; y = y->p) {
        splay(y); y->ch[1] = last; pushUp(y); last = y;
    }
    splay(x);
    return last;
}

void makeRoot(Node* x) { access(x); x->rev ^= 1; }

Node* findRoot(Node* x) {
    access(x); while (x->ch[0]) { pushDown(x); x=x->ch[0]; } splay(x); return x;
}

bool connected(Node* a, Node* b) { return findRoot(a)==findRoot(b); }

void link(Node* a, Node* b) { makeRoot(a); if (findRoot(b)!=a) a->p=b; }

void cut(Node* a, Node* b) { makeRoot(a); access(b); if(b->ch[0]==a){ b->ch[0]->p=nullptr; b->ch[0]=nullptr; pushUp(b); } }

void updateValue(Node* x, long long v) { access(x); x->val=v; pushUp(x); }

long long queryPath(Node* a, Node* b) { makeRoot(a); access(b); return b->sum; }

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n,q; cin>>n>>q;
    vector<Node*> nodes(n+1);
    for(int i=1;i<=n;i++){ long long v; cin>>v; nodes[i]=new Node(v); }

    while(q--){
        string cmd; cin>>cmd;
        if(cmd=="link"){ int u,v; cin>>u>>v; link(nodes[u],nodes[v]); }
        else if(cmd=="cut"){ int u,v; cin>>u>>v; cut(nodes[u],nodes[v]); }
        else if(cmd=="set"){ int u; long long val; cin>>u>>val; updateValue(nodes[u],val); }
        else if(cmd=="query"){ int u,v; cin>>u>>v; cout<< (connected(nodes[u],nodes[v]) ? queryPath(nodes[u],nodes[v]) : -1) << "\n"; }
    }
}
