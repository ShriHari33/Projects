/* Problem Statement
   Building an algorithm for Package Delivery Companies which have a Priority Customer System, which let's know the delivery people as to which packages should delivered first 
   starting from their main office obeying the Priority Scheduling of the Customers. Path to be taken to deliver each package MUST also be given as the output.
*/


/*  Approach to solving this problem
    Considerations:
    1) The graph formed is a Complete Graph because we have considered a logical assumption that every house is connected to other house via some path.
    2) The distance between two houses (u, v) is the same as distance between (v, u). Again, a reasonable assumption to be made.
    
    Logic:
    Now, we discarded Floyd Warshall because printing the path in it was not very obvious to us. We saw that we can use a version of Dijkstra's algorithm, as it demanded a shortest
    path at every instant, GIVEN the destination using the Priority Scheduling. So, we constructed the graph using Adjacency List representation, wherein the 0th node represents the
    location from where we start. 
    
    Now, we fill in the customers with their customerID and Priority Card. We then arrange the deliveries to be given in the Order "Platinum, Gold and Silver". All Platinum's are to
    be delivered first, then gold and then the silver.
    
    We apply Dijkstra's algorithm to reach the first Platinum customer from the location of start delivery. We use a Heap Data Structure to maintain efficiency. This finds the short-
    est way to the Platinum customer (1st) from the store. After that, we print the path with the cost and then again apply Dijkstra's algorithm from this Platinum customer (1st) to
    the next Platinum customer (2nd). This goes on until we have finally finished all the deliveries.
*/

#include "stdio.h"
#include "stdlib.h"
#include "limits.h"
#include "string.h"
#include "time.h"


#define MAX 100

typedef struct
{
    int customerID; // customer ID begins from 1, and 0 is reserved for the store's address
    char cardType[11];
    double purchaseAmount;
}customer;

typedef struct node
{
    int vertex;
    int distanceInKms;
    struct node* next;
}node;

typedef struct
{
    int vertex;
    int distance;
}heapVertex;

typedef struct
{
    int itemNumber;
    double priceOfItem;
    int itemName;
}item;

typedef struct
{
    int customerID;
    char cardType[11];
}customerDetailsOfCard;

typedef struct
{
    heapVertex heapArray[MAX];
    int size;
}heap;

typedef struct
{
    int arr[MAX];
    int top;
}stack;

// global declarations needed for Graph Functions
item storeProducts[MAX];
customer customerDetails[MAX];
customerDetailsOfCard array[MAX];
int customerCount = 0;
int verticesCount = 15;
int priorityCustomers[MAX];
int k = 0;

// global declarations needed for Heap Functions
int availableInHeap[MAX];
int indexInHeap[MAX];
int path[MAX];
int distance[MAX];

// prototypes for Graph functions
void initGraph(node** graph);
void addCustomerToDelivery(node** graph, int id, double purchasePrice);
void connectCustomers(node** graph);
void displayGraph(node** graph);
void fillCustomerCardTypes();
void printCustomers();
void optimalPathOrderFiller(node** graph);
void optimalPathFinder(node** graph);

//prototypes for Heap functions
void initHeap(heap* h);
void decreaseKey(heap* h, int vertex, int wt);
heapVertex extractMin(heap* h);
void heapify(heap* h, int i);
int isEmpty(heap* h);
void initPath(int src);
void initDistance();

// prototypes for Stack Functions
void push(stack* s, int p);
int pop(stack* s);
void printPath(int src);

int main()
{
    srand(time(NULL));
    node* graph[MAX];
    initGraph(graph);
    int id;
    double purchasePrice;

    fillCustomerCardTypes();
    printCustomers();

    while(customerCount <= verticesCount - 1)
    {
        printf("\n");

        printf("Welcome to QuickMart! Please enter the below details\n");
        printf("Customer ID\n");
        scanf("%d", &id);

        printf("\n");

        printf("Please enter purchase Price below\n");
        scanf("%lf", &purchasePrice);
        addCustomerToDelivery(graph, id, purchasePrice);
    }
    connectCustomers(graph);
    displayGraph(graph);

    optimalPathFinder(graph);

}

void optimalPathFinder(node** graph)
{
    heap h;
    initHeap(&h);
    optimalPathOrderFiller(graph);

    heapVertex tempNode;
    int ver, d, count;
    node* e;

    int i, j;

    for(count = 1; count<=verticesCount; ++count)
    {
        for(i = 0; i<=verticesCount; ++i)
        {
            h.heapArray[i].vertex = i;
            h.heapArray[i].distance = INT_MAX;
            availableInHeap[i] = 1;
            indexInHeap[i] = i;
            h.size++;
        }

        initPath(priorityCustomers[count-1]);
        initDistance();

        decreaseKey(&h, indexInHeap[priorityCustomers[count-1]], 0);
        while(!isEmpty(&h))
        {
            tempNode = extractMin(&h);
            ver = tempNode.vertex;
            d = tempNode.distance;
            distance[ver] = d;

            if(ver == priorityCustomers[count])
            {
                h.size = 0;
                break;
            }

            node* p = graph[ver];
            while(p)
            {
                e = p;
                if( availableInHeap[e->vertex] && (e->distanceInKms+ d < h.heapArray[indexInHeap[e->vertex]].distance) )
                {
                    decreaseKey(&h, indexInHeap[e->vertex], e->distanceInKms + d);
                    path[e->vertex] = ver;
                }

                p = p->next;
            }
        }
        // exit after each iteration and print the respective path
        printPath(priorityCustomers[count]);
    }
}


// displaying the graph after initialization
void printCustomers()
{
    int i;
    for(i = 1; i<=verticesCount; ++i)
        printf("%d, %s\n", customerDetails[i].customerID, customerDetails[i].cardType);

}


// randomly assign every customer with a membership type
void fillCustomerCardTypes()
{
    int i, randomFiller;
    char differentMemberships[3][11] = {"Silver", "Gold", "Platinum"};

    for(i = 1; i<=verticesCount; ++i)
    {
        randomFiller = rand()%3;
        customerDetails[i].customerID = i;
        strcpy(customerDetails[i].cardType, differentMemberships[randomFiller]);
    }
}


// after the order has been received, add the customer to the graph
void addCustomerToDelivery(node** graph, int id, double purchasePrice)
{
    customer temp;
    temp.customerID = id;
    temp.purchaseAmount = purchasePrice;
    strcpy(temp.cardType, customerDetails[id].cardType);

    customerDetails[id] = temp;
    customerCount++;

    // connect store to this particular customer
    node* p = NULL, *q = NULL;
    p = (node* ) malloc(sizeof(node));

    // populate the data
    p->next = NULL;
    p->vertex = id;
    p->distanceInKms = rand()%20;

    if(graph[0] == NULL)  // 0th Vertex represents our store's location
        graph[0] = p;
    else
    {
        q = graph[0];
        while(q->next)
            q = q->next;

        q->next = p;
    }

}


// connecting each customer to other customer
void connectCustomers(node** graph)
{
    int i = 1;
    int randomKm;
    int matrix[MAX][MAX];  // this DS is to ensure the dist(u, v) = dist(v, u)
    int m, n, prevDistance;

    for(m = 0; m<MAX; m++)
    {
        for(n = 0; n<MAX; ++n)
            matrix[m][n] = INT_MAX;
    }

    node* q = NULL, *p = NULL;

    while(i <= verticesCount)
    {
        for(m = 1; m <= verticesCount; m++)
        {
            if(i == m)
                continue;

            p = (node* ) malloc(sizeof(node));
            p->vertex = m;
            p->next = NULL;

            if(matrix[m][i] != INT_MAX)
                p->distanceInKms = matrix[m][i];
            else
            {
                prevDistance = rand()%20;
                matrix[i][m] = prevDistance;
                p->distanceInKms = prevDistance;
            }

            if(graph[i] == NULL)
                graph[i] = p;
            else
            {
                q = graph[i];
                while(q->next)
                    q = q->next;

                q->next = p;
            }

        }
        i++;
    }
    customerDetails[0].customerID = 0;
}


// initializing the graph
void initGraph(node** graph)
{
    int i;
    for(i = 0; i<MAX; ++i)
        graph[i] = NULL;
}


//displaying the whole graph
void displayGraph(node** graph)
{
    int i, j;
    node* p = NULL;

    for(i = 0; i<=verticesCount; ++i)
    {
        p = graph[i];
        printf("%d connects these: ", i);
        while(p)
        {
            printf("(%d, %d, %s)", p->vertex, p->distanceInKms, customerDetails[p->vertex].cardType);
            p = p->next;
        }
        printf("\n\n");
    }
}


void optimalPathOrderFiller(node** graph)
{
    int i;
    priorityCustomers[k++] = 0;

    for(i = 1; i<=verticesCount; ++i)
    {
        if(strcasecmp(customerDetails[i].cardType, "Platinum") == 0)
            priorityCustomers[k++] = i;
    }

    for(i = 1; i<=verticesCount; ++i)
    {
        if(strcasecmp(customerDetails[i].cardType, "Gold") == 0)
            priorityCustomers[k++] = i;
    }

    for(i = 1; i<=verticesCount; ++i)
    {
        if(strcasecmp(customerDetails[i].cardType, "Silver") == 0)
            priorityCustomers[k++] = i;
    }
}


// Functions Related to Heap
void initHeap(heap* h)
{
    h->size = 0;
}


void initPath(int src)
{
    int i = 0;
    for(i = 0; i<MAX; ++i)
        path[i] = src;
}


void initDistance()
{
    int i;
    for(i = 0; i<MAX; ++i)
        distance[i] = INT_MAX;
}


heapVertex extractMin(heap* h)
{
    heapVertex e = h->heapArray[0];
    availableInHeap[h->heapArray[0].vertex] = 0;
    h->heapArray[0] = h->heapArray[h->size - 1];
    indexInHeap[h->heapArray[0].vertex] = 0;

    h->size --;

    heapify(h, 0);
    return e;
}


void heapify(heap* h, int i)
{
    int left = 2*i + 1, right = 2*i + 2;
    int min = i;
    heapVertex e;

    heapVertex* arr = h->heapArray;
    if(left < h->size && arr[left].distance < arr[min].distance)
        min = left;

    if(right < h->size && arr[right].distance < arr[min].distance)
        min = right;

    if(min != i)
    {
        indexInHeap[arr[min].vertex] = i;
        indexInHeap[arr[i].vertex] = min;
        e = arr[min];
        arr[min] = arr[i];
        arr[i] = e;

        heapify(h, min);
    }

}


int isEmpty(heap* h)
{
    if(h->size == 0)
        return 1;
    return 0;
}


void decreaseKey(heap* h, int vertex, int wt)
{
    h->heapArray[vertex].distance = wt;
    int i = vertex;

    heapVertex temp;
    temp = h->heapArray[vertex];


    while(i > 0 && temp.distance < h->heapArray[(i-1)/2].distance)
    {
        h->heapArray[i] = h->heapArray[(i-1)/2];
        indexInHeap[h->heapArray[(i-1)/2].vertex] = i;
        i = (i-1)/2;
    }
    h->heapArray[i] = temp;
    indexInHeap[temp.vertex] = i;
}



// stack related Functions
void printPath(int src)
{
    stack s;
    s.top = -1;

    int i = src, j = 0;
    while(i != path[i])
    {
        push(&s, path[i]);
        i = path[i];
    }

    printf("Deliver to %d by this path:  ", src);

    while(s.top != -1)
        printf("%d->", pop(&s));

    printf("%d\nIt will cost you %d units\n\n", src, distance[src]);

}


int pop(stack* s)
{
    int t = s->arr[s->top];
    s->top--;
    return t;
}


void push(stack* s, int p)
{
    s->top++;
    s->arr[s->top] = p;
}
