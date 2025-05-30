#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_V 100   // 최대 정점 수 (필요시 조정)
#define INF 100000000

typedef struct Edge {
    int to;
    int w1;
    int w2;
    struct Edge* next;
} Edge;

typedef struct {
    Edge* head[MAX_V];
    int nV;     // 정점 수
} Graph;

// 우선순위 큐 대체 구조
typedef struct {
    int vertex;
    double cost;
} Node;

void initGraph(Graph* g, int nV) {
    g->nV = nV;
    for (int i = 0; i < nV; i++) g->head[i] = NULL;
}

void addEdge(Graph* g, int from, int to, int w1, int w2) {
    Edge* e = (Edge*)malloc(sizeof(Edge));
    e->to = to;
    e->w1 = w1;
    e->w2 = w2;
    e->next = g->head[from];
    g->head[from] = e;
}

void dijkstra(Graph* g, int start, int end, double alpha, double beta, int* path, double* total_cost) {
    double dist[MAX_V];
    int prev[MAX_V];
    int visited[MAX_V] = {0};
    for (int i = 0; i < g->nV; i++) {
        dist[i] = INF;
        prev[i] = -1;
    }
    dist[start] = 0.0;

    for (int cnt = 0; cnt < g->nV; cnt++) {
        // 최소 dist를 가진 정점 선택
        int u = -1;
        double minDist = INF;
        for (int i = 0; i < g->nV; i++) {
            if (!visited[i] && dist[i] < minDist) {
                minDist = dist[i];
                u = i;
            }
        }
        if (u == -1) break;
        visited[u] = 1;

        for (Edge* e = g->head[u]; e; e = e->next) {
            double cost = alpha * e->w1 + beta * e->w2;
            if (dist[e->to] > dist[u] + cost) {
                dist[e->to] = dist[u] + cost;
                prev[e->to] = u;
            }
        }
    }
    // 역추적으로 경로 복원
    int idx = 0;
    int stack[MAX_V];
    int cur = end;
    while (cur != -1) {
        stack[idx++] = cur;
        cur = prev[cur];
    }
    // path에 경로 넣기(정방향)
    for (int i = 0; i < idx; i++)
        path[i] = stack[idx - i - 1];
    path[idx] = -1; // 끝 표시
    *total_cost = dist[end];
}


int main() {
    FILE* fin = fopen("input.txt", "r");
    FILE* fout = fopen("output.txt", "w");
    if (!fin || !fout) {
        printf("파일을 열 수 없습니다.\n");
        return 1;
    }

    int nV, start, end;
    fscanf(fin, "%d,%d,%d", &nV, &start, &end);
    // 1-indexed 입력일 경우 0-index로 변환
    start--; end--;

    Graph g;
    initGraph(&g, nV);

    int from, to, w1, w2;
    char buf[100];
    while (fgets(buf, sizeof(buf), fin)) {
        if (sscanf(buf, " (%d,%d,%d,%d)", &from, &to, &w1, &w2) == 4) {
        addEdge(&g, from - 1, to - 1, w1, w2);
        }
    }

    // α = 0, 0.1, 0.2 ... 1.0
    for (int i = 0; i <= 10; i++) {
        double alpha = i / 10.0;
        double beta = 1.0 - alpha;
        int path[MAX_V + 1];
        double total_cost;
        dijkstra(&g, start, end, alpha, beta, path, &total_cost);

        fprintf(fout, "(%.1f, %.1f, %.0f)\n", alpha, beta, total_cost);
        fprintf(fout, "Path: ");
        for (int j = 0; path[j] != -1; j++) {
            if (j > 0) fprintf(fout, "-");
            fprintf(fout, "%d", path[j] + 1);  // 1-index 출력
        }
        fprintf(fout, "\n\n");
    }
    fclose(fin);
    fclose(fout);
    return 0;
}
