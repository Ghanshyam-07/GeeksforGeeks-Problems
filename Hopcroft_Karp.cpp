#include <bits/stdc++.h>
using namespace std;

const int INF = 1e9;

struct BipartiteGraph {
    int n, m; // n = #left vertices, m = #right vertices
    vector<vector<int>> adj;
    vector<int> pairU, pairV, dist;

    BipartiteGraph(int _n, int _m) : n(_n), m(_m) {
        adj.assign(n+1, vector<int>());
        pairU.assign(n+1, 0);
        pairV.assign(m+1, 0);
        dist.assign(n+1, 0);
    }

    void addEdge(int u, int v) { // u in left, v in right
        adj[u].push_back(v);
    }

    bool bfs() {
        queue<int> q;
        for(int u=1; u<=n; u++) {
            if(pairU[u]==0) { dist[u]=0; q.push(u); }
            else dist[u]=INF;
        }
        dist[0]=INF;
        while(!q.empty()) {
            int u=q.front(); q.pop();
            if(dist[u]<dist[0]) {
                for(int v: adj[u]) {
                    if(dist[pairV[v]]==INF) {
                        dist[pairV[v]] = dist[u]+1;
                        q.push(pairV[v]);
                    }
                }
            }
        }
        return dist[0]!=INF;
    }

    bool dfs(int u) {
        if(u==0) return true;
        for(int v: adj[u]) {
            if(dist[pairV[v]]==dist[u]+1) {
                if(dfs(pairV[v])) {
                    pairU[u]=v;
                    pairV[v]=u;
                    return true;
                }
            }
        }
        dist[u]=INF;
        return false;
    }

    int maxMatching() {
        int matching=0;
        while(bfs()) {
            for(int u=1; u<=n; u++) {
                if(pairU[u]==0 && dfs(u)) matching++;
            }
        }
        return matching;
    }

    void printMatching() {
        for(int u=1; u<=n; u++) {
            if(pairU[u]) cout << u << " - " << pairU[u] << "\n";
        }
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, e;
    cin >> n >> m >> e; // n=left vertices, m=right vertices, e=edges
    BipartiteGraph g(n, m);
    for(int i=0; i<e; i++){
        int u,v; cin >> u >> v;
        g.addEdge(u,v);
    }

    int matchSize = g.maxMatching();
    cout << "Maximum Matching Size: " << matchSize << "\n";
    cout << "Matched Pairs:\n";
    g.printMatching();
}
